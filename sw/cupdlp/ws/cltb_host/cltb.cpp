#include <cstddef>
#include <cstring>
#include <ctime>
#include <memory>
#include <ratio>
#include <stdexcept>
#include <xrt/xrt_device.h>
#include <xrt/xrt_bo.h>
#include <xrt/xrt_kernel.h>
#include <experimental/xrt_xclbin.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "BSLogger.hpp"
#include "deprecated/xrt.h"
#include "include/csr_matrix.hpp"
#include "include/dense_vector.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "CsvWriter.hpp"
#include <cstdio>
#include <stdlib.h>
#include "partitioning_utility.hpp"
#include "xrt_utility.hpp"

constexpr int run           = 1;
constexpr int iterations    = 1;

#define MAX_NNZ 1e6 
#define INCR 1000
#define M_MAX 49152 
#define N_MAX 49152  
#define M_INCR 1000 
#define N_INCR 1000 

template <typename T>
using CSRMatrixTiled = std::vector<std::vector<CSRMatrix<T>*>>;


struct SpMvKernel
{
    std::vector<xrt::kernel> krnl1;
    std::vector<xrt::kernel> krnl2;
    std::vector<xrt::kernel> krnl3;
    std::vector<xrt::kernel> krnl4;
};

struct SpMvBoBuffers
{
    std::vector<uint*> boIndicesMaps; 
    std::vector<data_t*> boValuesMaps; 
    std::vector<xrt::bo> boIndices;
    std::vector<xrt::bo> boValues; 
    std::vector<uint> validTiles;
};

struct BlockVectors
{
    std::vector<uint> nnzBlocksTot; 
    std::vector<uint> rowBlocksTot; 
    std::vector<uint> vecBlocksTot; 
    uint vecBlocks;
    uint rowBlocks;
};

struct KernelRuns
{
    std::vector<xrt::run> runKrnl1; 
    std::vector<xrt::run> runKrnl2; 
    std::vector<xrt::run> runKrnl3; 
    std::vector<xrt::run> runKrnl4; 
};

template<typename T>
void sort_by_rowsum(CSRMatrix<T> &csr) {
    int m = csr.rows();
    int* row_ptr = csr.rowPointer.get();
    int* col_idx = csr.colIndex.get();
    T* data = csr.data.get();

    std::vector<std::pair<int, T>> row_sums(m);
    for (int i = 0; i < m; ++i) {
        T sum = 0;
        for (int j = row_ptr[i]; j < row_ptr[i + 1]; ++j) {
            sum += data[j];
        }
        row_sums[i] = {i, sum};
    }

    std::sort(row_sums.begin(), row_sums.end(),
              [](const std::pair<int, T>& a, const std::pair<int, T>& b) {
                  return a.second > b.second;
              });

    std::unique_ptr<int[]> new_row_ptr(new int[m + 1]);
    std::unique_ptr<int[]> new_col_idx(new int[csr.nnz()]);
    std::unique_ptr<T[]> new_data(new T[csr.nnz()]);

    new_row_ptr[0] = 0;
    int nnz_index = 0;

    for (int i = 0; i < m; ++i) {
        int orig_row = row_sums[i].first;
        int row_start = row_ptr[orig_row];
        int row_end = row_ptr[orig_row + 1];
        int row_len = row_end - row_start;

        for (int j = 0; j < row_len; ++j) {
            new_col_idx[nnz_index] = col_idx[row_start + j];
            new_data[nnz_index] = data[row_start + j];
            ++nnz_index;
        }

        new_row_ptr[i + 1] = nnz_index;
    }

    for (int i = 0; i <= m; ++i)
        csr.setRowPointer(i, new_row_ptr[i]);

    for (int i = 0; i < csr.nnz(); ++i) {
        csr.setColIndex(i, new_col_idx[i]);
        csr.setData(i, new_data[i]);
    }
}

template<typename T>
CSRMatrix<T> generate_csr_matrix(int m, int n, int nnz) {
    
    CSRMatrix<T> csr(nnz, m, n);

    srand(time(0));

    std::vector<int> row_nnz(m, 0);
    for (int i = 0; i < nnz; ++i) {
        int r = rand() % m;
        row_nnz[r]++;
    }

    csr.setRowPointer(0, 0);

    for (int i = 0; i < m; ++i) {
        csr.setRowPointer(i + 1, csr.getRowPointer(i) + row_nnz[i]);
    }

    assert(csr.getRowPointer(m) == nnz);

    for (int i = 0; i < m; ++i) {
        int row_start = csr.getRowPointer(i);
        int row_end = csr.getRowPointer(i + 1);
        for (int j = row_start; j < row_end; ++j) {
            csr.setColIndex(j, rand() % n);
            csr.setData(j, rand() % 100 + 1);
        }
    }
    
    // sort_by_rowsum(csr);

    return csr;
}


template<typename T> 
void tile_matrix(CSRMatrix<T> & csr, int computeUnits, int hwSideLen, std::vector<std::vector<int>> & yPartRows, CSRMatrixTiled<T> & csr_tiled)
{
    LOG_INIT_CERR();

    int yParts = computeUnits;
    int xParts = std::ceil(csr.cols()/(double)hwSideLen);/*tiles_in_part*/;

    log(LOG_DEBUG) << "tilesInYPart: " << xParts <<  "\n";

    if (hwSideLen < std::ceil(csr.rows()/(double)yParts)) {
        log(LOG_ERROR) << "The hardware size: " << hwSideLen 
            <<  " can not accomodate y_partition size: " << csr.rows()/yParts << "\n";
        throw std::runtime_error("what ?");
    }

    if (hwSideLen < std::ceil(csr.cols()/xParts)) {
        log(LOG_ERROR) << "The hardware size: " << hwSideLen 
            <<  " can not accomodate x_partition size: " << csr.cols()/xParts << "\n";
        throw std::runtime_error("what ?");
    }

    auto start = std::chrono::high_resolution_clock::now();

    csr_tiled.resize(yParts);

    for (int i=0; i<yParts; i++) {
        csr_tiled[i].reserve(xParts);
    }

    yPartRows.resize(yParts);

    PartitionMatrixIntoNnzBalancedYPartitionTiles<>(csr, csr.rows(), csr.cols(), yParts, xParts, csr_tiled, yPartRows);


   auto end = std::chrono::high_resolution_clock::now();

    log(LOG_DEBUG) << "partitioning_matrix_time (sec): " << std::chrono::duration<double, std::milli>(end - start).count() << "\n";

}

template <typename T>
CSRMatrix<T> prepareData(DenseVector<T> & x, DenseVector<T> & ref_out, int nnz, int m, int n) {
    LOG_INIT_CERR();
    log(LOG_DEBUG) << "Building Random CSR ... \n";
    auto csr = generate_csr_matrix<data_t>(m, n, nnz);    
    log(LOG_DEBUG) << "Done With Building CSR ... \n";

    for (int i = 0; i < n; ++i)
    {
        x[i] = rand() % 10 + 1;
    }

    log(LOG_DEBUG) << "Out Size: " << ref_out.size() << "\n";
    log(LOG_DEBUG) << "X Size: " << x.size() << "\n"; 
    log(LOG_DEBUG) << "Computing Reference ... \n";

    // Perfect use of encapsulation ofc (jk) 
    spMvCsR(csr.rowPointer.get(), csr.colIndex.get(), csr.data.get(), m, x.elements.get(), ref_out.elements.get());

    return csr;
}

template <typename T>
double time_op(T aFunction) {
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < run; i++) 
    {
        aFunction();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::micro>(t2 - t1).count();
}

template<typename T>
double time_cpu_csr(CSRMatrix<T> &csr, const DenseVector<T> &x, int m, const DenseVector<T> & out) {
    auto cpu_lambda = [&]() {
        spMvCsR(csr.rowPointer.get(), csr.colIndex.get(), csr.data.get(), m, x.elements.get(), out.elements.get());
    };
    return time_op<>(cpu_lambda);
}


template<typename T>
void setArgs(SpMvKernel & kernel, SpMvBoBuffers & boBuffers, BlockVectors & blockVectors, KernelRuns & krnlRuns, const CSRMatrixTiled<T> & tiles)
{
    for (size_t j = 0; j < tiles.size(); j++) {
        krnlRuns.runKrnl1[j] = xrt::run(kernel.krnl1[j]);
        krnlRuns.runKrnl1[j].set_arg(3, boBuffers.boValues[j]); 
        krnlRuns.runKrnl1[j].set_arg(4, boBuffers.boIndices[j]);
        krnlRuns.runKrnl1[j].set_arg(5, blockVectors.vecBlocksTot[j]); // TODO: do the total calculation above
        krnlRuns.runKrnl1[j].set_arg(6, blockVectors.rowBlocksTot[j]); // TODO: do the total calculation above
        krnlRuns.runKrnl1[j].set_arg(7, blockVectors.vecBlocks); // vecBlock constant across all the tiles
        krnlRuns.runKrnl1[j].set_arg(8, blockVectors.nnzBlocksTot[j]); // TODO: do the total calculation above
        krnlRuns.runKrnl1[j].set_arg(9, iterations); 

        krnlRuns.runKrnl2[j] = xrt::run(kernel.krnl2[j]);
        krnlRuns.runKrnl2[j].set_arg(4, blockVectors.vecBlocks); // vecBlock constant across all the tiles
        krnlRuns.runKrnl2[j].set_arg(5, blockVectors.rowBlocks);
        krnlRuns.runKrnl2[j].set_arg(6, tiles[j][0]->rows());
        krnlRuns.runKrnl2[j].set_arg(7, boBuffers.validTiles[j]); //
        krnlRuns.runKrnl2[j].set_arg(8, blockVectors.nnzBlocksTot[j]); // TODO: do the total calculation above
        krnlRuns.runKrnl2[j].set_arg(9, iterations);
       
        krnlRuns.runKrnl3[j] = xrt::run(kernel.krnl3[j]);
        krnlRuns.runKrnl3[j].set_arg(3, boBuffers.validTiles[j]); 
        krnlRuns.runKrnl3[j].set_arg(4, blockVectors.nnzBlocksTot[j]); // TODO: do the total calculation above
        krnlRuns.runKrnl3[j].set_arg(5, iterations); 
       
        krnlRuns.runKrnl4[j] = xrt::run(kernel.krnl4[j]);
        krnlRuns.runKrnl4[j].set_arg(2, blockVectors.vecBlocks); // vecBlock constant across all the tiles
        krnlRuns.runKrnl4[j].set_arg(3, boBuffers.validTiles[j]);
        krnlRuns.runKrnl4[j].set_arg(4, iterations); 
    }
}

template<typename T>
void retrieve_y(SpMvBoBuffers & boBuffers, DenseVector<T> & y, const CSRMatrixTiled<T> & tiles, BlockVectors & blockVectors, std::vector<std::vector<int>> & yPartRows)
{

    for (size_t i=0; i < tiles.size(); i++) {
        boBuffers.boValues[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
    }

    int locRows = 0;
    for (size_t i=0; i < tiles.size(); i++) {
        auto bo_vals_map = boBuffers.boValues[i].map<T*>();
        auto offset = blockVectors.nnzBlocksTot[i] + blockVectors.vecBlocksTot[i];
        offset *= BLOCK_SIZE;
        for (size_t j=0; j < tiles[i][0]->rows(); j++) { 
            int index = yPartRows[i][j];
            y[index] = bo_vals_map[j+offset];
        }
        locRows += tiles[i][0]->rows();
    }

}

template<typename T>
double time_kernel_csr( const CSRMatrixTiled<T> & tiles, SpMvKernel & kernel, SpMvBoBuffers & boBuffers, BlockVectors & blockVectors, 
                        std::vector<std::vector<int>> & yPartRows, DenseVector<T> & y) {

    // LOG_INIT_CERR();

    KernelRuns krnlRuns;

    krnlRuns.runKrnl1.resize(tiles.size());
    krnlRuns.runKrnl2.resize(tiles.size());
    krnlRuns.runKrnl3.resize(tiles.size());
    krnlRuns.runKrnl4.resize(tiles.size());

    // log(LOG_DEBUG) << "Setting Arguments .... \n";

    setArgs(kernel, boBuffers, blockVectors, krnlRuns, tiles);

    // log(LOG_DEBUG) << "Starting Runs .... \n";


    auto kernel_lambda = [&]() {
        for (size_t j=0; j<tiles.size(); j++) {
           krnlRuns.runKrnl2[j].start();
           krnlRuns.runKrnl3[j].start();
           krnlRuns.runKrnl4[j].start();    
        }


        for (size_t j=0; j<tiles.size(); j++) {
            krnlRuns.runKrnl1[j].start();
        }

        for (size_t j=0; j<tiles.size(); j++) {
            krnlRuns.runKrnl1[j].wait();
        }


        for (size_t j=0; j<tiles.size(); j++) {
            krnlRuns.runKrnl2[j].wait();
            krnlRuns.runKrnl3[j].wait();
            krnlRuns.runKrnl4[j].wait();
        }
 
    };

    // retrieve_y(boBuffers, y, tiles, blockVectors, yPartRows);
    double kernel_time = time_op<>(kernel_lambda);
    retrieve_y<T>(boBuffers, y, tiles, blockVectors, yPartRows);

    return kernel_time;
}

template<typename T>
bool validate(const DenseVector<T> &ref, const DenseVector<T> &test) {
    LOG_INIT_CERR();

    const float eps = 1e-6;

    for (int i = 0; i < ref.size(); ++i) {

        if (std::abs(ref[i] - test[i]) > eps) {
            std::cerr << "Mismatch at index " << i << ": " << ref[i] << " vs " << test[i] << "\n";
            // return false;
        }
    }
    return true;
}

template<typename T>
BlockVectors allocate_buffer(CSRMatrixTiled<T> & tiles, xrt::device & device, 
                    int verifiability, DenseVector<T> vecX, 
                    SpMvBoBuffers & boBuffers, SpMvKernel & kernel)
{

    LOG_INIT_CERR();
    // Start: Device buffer creation and assignment
    boBuffers.boIndicesMaps.resize(tiles.size());
    boBuffers.boValuesMaps.resize(tiles.size());
    boBuffers.boIndices.resize(tiles.size()); 
    boBuffers.boValues.resize(tiles.size()); 
    boBuffers.validTiles.resize(tiles.size());

    log(LOG_DEBUG) << "Allocating Buffers \n";

    AllocateBuffers(device, kernel.krnl1, boBuffers.boIndices, boBuffers.boValues, tiles, boBuffers.validTiles, BLOCK_SIZE);
    
    BlockVectors blockVectors;
    blockVectors.nnzBlocksTot.reserve(tiles.size());
    blockVectors.rowBlocksTot.reserve(tiles.size());
    blockVectors.vecBlocksTot.reserve(tiles.size());

    uint vecBlocks = ((tiles[0][0]->cols()-1)/BLOCK_SIZE)+1;
    uint rowBlocks = ((tiles[0][0]->rows())/BLOCK_SIZE)+1; // |row_ptr|=|x|+1

    for (size_t i=0; i<tiles.size(); i++) {
        uint locVecBlocks = ((tiles[i][0]->cols()-1)/BLOCK_SIZE)+1;
        uint locRowBlocks = ((tiles[i][0]->rows())/BLOCK_SIZE)+1; // |row_ptr|=|x|+1
        vecBlocks = locVecBlocks > vecBlocks ? locVecBlocks : vecBlocks;
        rowBlocks = locRowBlocks > rowBlocks ? locRowBlocks : rowBlocks;
    }   
    
    log(LOG_DEBUG) << "Packing Tiles Into buffers";

    PackTilesIntoBuffers(boBuffers.boIndices, boBuffers.boValues, tiles, vecX,
        blockVectors.nnzBlocksTot, blockVectors.rowBlocksTot, blockVectors.vecBlocksTot, 
        boBuffers.validTiles, vecBlocks, rowBlocks, BLOCK_SIZE);


    if (verifiability&2) {
        log(LOG_DEBUG) << "Verifying Tiling \n";
        VerifyTilesPacking(boBuffers.boIndices, boBuffers.boValues, tiles, boBuffers.validTiles, rowBlocks, 
        vecBlocks, BLOCK_SIZE);
    }

    log(LOG_DEBUG) << "Syncking Kernels \n";

    for (size_t i=0; i < tiles.size(); i++) {
        boBuffers.boValues[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
        boBuffers.boIndices[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
    }

    blockVectors.rowBlocks = rowBlocks;
    blockVectors.vecBlocks = vecBlocks;

    return blockVectors;
}

void nnz_run(int nnz, int m, int n, int compute_units, int hw_side_len, 
            SpMvKernel & spmvKernel, xrt::device & device)
{
    LOG_INIT_CERR();

    log(LOG_INFO) << "Beginning Data Collection\n";

    log(LOG_INFO) << "Creating CsvWriter \n";

    auto filename = "cltb_" + std::to_string(std::time(0)) + ".csv";
    auto csvfile = csv::CsvWriter(filename, "nnz", "cpu", "kenerl");

    log(LOG_INFO) << "FileName: " << filename << "\n";
    log(LOG_INFO) << "Columns: " << "nnz " << " cpu " << " kenerl " << "\n";
    
    log(LOG_INFO) << "Max NNZ: " << MAX_NNZ << "\n";
    log(LOG_INFO) << "Increment: " << INCR << "\n";
    log(LOG_INFO) << "Start NNZ: " << nnz << "\n";

    for (; nnz < MAX_NNZ; nnz+=INCR)
    {

        DenseVector<data_t> x(n);
        DenseVector<data_t> ref_out(m); 
        DenseVector<data_t> hw_out(m);
        DenseVector<data_t> out(m);

        CSRMatrix<data_t> csr = prepareData(x, ref_out, nnz, m, n);
        std::vector<std::vector<int>>  yPartRows;

        CSRMatrixTiled<data_t> csr_tiled;
        tile_matrix<data_t>(csr, compute_units, hw_side_len, yPartRows, csr_tiled);

        SpMvBoBuffers boBuffers;
        auto blockVectors  = allocate_buffer(csr_tiled, device, 0, x, boBuffers, spmvKernel);

        auto diff_kernel = time_kernel_csr(csr_tiled, spmvKernel, boBuffers, blockVectors, yPartRows, hw_out);
        log(LOG_INFO) << "NNZ: " << nnz << ", Kernel Time Test: " << diff_kernel << " ms\n";

        auto diff_cpu = time_cpu_csr(csr, x, m, out);
        log(LOG_INFO) << "NNZ: " << nnz << ", CPU Time Test: " << diff_cpu << " ms\n";

        if (!validate(out, hw_out))
            throw std::runtime_error("What ? Bad compute at NNZ: " + std::to_string(nnz));

        csvfile.write(nnz, diff_cpu, diff_kernel);
    }

}

void m_n_run(xrt::device & device, SpMvKernel spmvKernel, double spsty, int m, int n, int compute_units, int hw_side_len)
{
    LOG_INIT_CERR();

    log(LOG_INFO) << "Beginning Grid Search over M and N\n";

    auto filename = "m_n_grid_" + std::to_string(std::time(0)) + ".csv";
    auto csvfile = csv::CsvWriter(filename, "m", "n", "cpu", "kernel");

    int nnz  = (n * m) * spsty;

    for (int m_curr = m; m_curr < M_MAX; m_curr += M_INCR)
    {
        for (int n_curr = n; n_curr < N_MAX; n_curr += N_INCR)
        {

            DenseVector<data_t> x(n);
            DenseVector<data_t> ref_out(m); 
            DenseVector<data_t> hw_out(m);
            DenseVector<data_t> out(m);

            CSRMatrix<data_t> csr = prepareData(x, ref_out, nnz, m, n);
            std::vector<std::vector<int>>  yPartRows;

            CSRMatrixTiled<data_t> csr_tiled;
            tile_matrix<data_t>(csr, compute_units, hw_side_len, yPartRows, csr_tiled);

            SpMvBoBuffers boBuffers;
            auto blockVectors  = allocate_buffer(csr_tiled, device, 0, x, boBuffers, spmvKernel);

            auto diff_kernel = time_kernel_csr(csr_tiled, spmvKernel, boBuffers, blockVectors, yPartRows, hw_out);
            log(LOG_INFO) << "M: " << m_curr << ", N: " << n_curr << ", Kernel Time Test: " << diff_kernel << " ms\n";

            auto diff_cpu = time_cpu_csr(csr, x, m, out);
            log(LOG_INFO) << "M: " << m_curr << ", N: " << n_curr << ", CPU Time Test: " << diff_cpu << " ms\n";

            if (!validate(out, hw_out))
                throw std::runtime_error("Bad compute at M: " + std::to_string(m_curr) + " N: " + std::to_string(n_curr));

            csvfile.write(m_curr, n_curr, diff_cpu, diff_kernel);
        }
    }
}

int main(int argc, char *argv[]) {
    LOG_INIT_CERR();

    log.set_log_level(LOG_INFO);

    log(LOG_DEBUG) << "Arguments: " << argc << "\n";

    if (argc != 2) {
        std::cerr << "Usage: ./xrt_spmv <xclbin>\n";
        return EXIT_FAILURE;
    }

    int nnz = 1000;
    int m   = 49152;
    int n   = 49152;

    // int nnz = 10;
    // int m   = 49;
    // int n   = 49;

    // double sparsity = 0.01;
    int compute_units = 12;
    int hw_side_len = 4096;
    int verbosity = 0;
    int verify = 0;

    std::string xclbin_file = argv[1];

    DenseVector<data_t> x(n), ref_out(m), hw_out(m), out(m);
    std::vector<std::vector<int>> yPartRows;

    log(LOG_INFO) << "Preparing Test Matrix ... \n";
    CSRMatrix<data_t> csr = prepareData(x, ref_out, nnz, m, n);
    CSRMatrixTiled<data_t> csr_tiled;
    tile_matrix<data_t>(csr, compute_units, hw_side_len, yPartRows, csr_tiled);
   

    auto device = xrt::device(0);
    auto uuid = device.load_xclbin(xclbin_file);

    SpMvKernel spmvKernel;
    

    spmvKernel.krnl1.resize(csr_tiled.size());
    spmvKernel.krnl2.resize(csr_tiled.size());
    spmvKernel.krnl3.resize(csr_tiled.size());
    spmvKernel.krnl4.resize(csr_tiled.size());

    log(LOG_INFO) << "Creating Kernels \n";

    CreateKernels(spmvKernel.krnl1, spmvKernel.krnl2, spmvKernel.krnl3, spmvKernel.krnl4, 
        device, uuid, xclbin_file, csr_tiled.size(), verbosity);

    log(LOG_INFO) << "Calling Allocator Kernels \n";

    SpMvBoBuffers boBuffers;
    auto blockVectors  = allocate_buffer(csr_tiled, device, verify, x, boBuffers, spmvKernel);

    log(LOG_INFO) << "Starting CPU Time Test ... \n";
    auto diff_cpu = time_cpu_csr(csr, x, m, out);
    log(LOG_INFO) << "Finished CPU Time Test: " << diff_cpu << " \u03BCs\n";

    log(LOG_INFO) << "Starting Kernel Time Test ... \n";
    auto diff_kernel = time_kernel_csr(csr_tiled, spmvKernel, boBuffers, blockVectors, yPartRows, hw_out);
    log(LOG_INFO) << "Finished Kernel Time Test: " << diff_kernel << " \u03BCs\n";

    bool success = validate(ref_out, hw_out);

    std::cout << (success ? "PASSED" : "FAILED") << std::endl;
    // return success ? EXIT_SUCCESS : EXIT_FAILURE;

    nnz_run(nnz, m, n, compute_units, hw_side_len, spmvKernel, device);
    
}

