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
#include "xrt/xrt_graph.h"
#include "xrt_utility.hpp"
#include "utility.hpp"
#include "experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"

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
    std::vector<uint*> boRptrMaps; 
    std::vector<uint*> boCidxMaps; 
    std::vector<data_t*> boValuesMaps; 
    std::vector<xrtBufferHandle> boRptr;
    std::vector<xrtBufferHandle> boCidx;
    std::vector<xrtBufferHandle> boValues; 
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

// template<typename T>
// void retrieve_y(SpMvBoBuffers & boBuffers, DenseVector<T> & y, const CSRMatrixTiled<T> & tiles, BlockVectors & blockVectors, std::vector<std::vector<int>> & yPartRows)
// {

//     for (size_t i=0; i < tiles.size(); i++) {
//         boBuffers.boValues[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     }

//     int locRows = 0;
//     for (size_t i=0; i < tiles.size(); i++) {
//         auto bo_vals_map = boBuffers.boValues[i].map<T*>();
//         auto offset = blockVectors.nnzBlocksTot[i] + blockVectors.vecBlocksTot[i];
//         offset *= BLOCK_SIZE;
//         for (size_t j=0; j < tiles[i][0]->rows(); j++) { 
//             int index = yPartRows[i][j];
//             y[index] = bo_vals_map[j+offset];
//         }
//         locRows += tiles[i][0]->rows();
//     }

// }

template<typename T>
double time_kernel_csr( const CSRMatrixTiled<T> & tiles, SpMvKernel & kernel, SpMvBoBuffers & boBuffers, BlockVectors & blockVectors, std::vector<std::vector<int>> & yPartRows, DenseVector<T> & y) {

    // LOG_INIT_CERR();

    auto kernel_lambda = [&]() {
 
    };

    double kernel_time = time_op<>(kernel_lambda);
    // retrieve_y<T>(boBuffers, y, tiles, blockVectors, yPartRows);

    return kernel_time;
}

template<typename T>
bool validate(const DenseVector<T> &ref, const DenseVector<T> &test) {
    LOG_INIT_CERR();

    const float eps = 1e-6;

    for (int i = 0; i < ref.size(); ++i) {

        if (std::abs(ref[i] - test[i]) > eps) {
            std::cerr << "Mismatch at index " << i << ": " << ref[i] << " vs " << test[i] << "\n";
        }
    }
    return true;
}

template<typename T>
BlockVectors allocate_buffer(CSRMatrixTiled<T> & tiles, xrt::device & device, 
                    int verifiability, DenseVector<T> vecX, 
                    SpMvBoBuffers & boBuffers, SpMvKernel & kernel)
{

}

static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
  if (fnm.empty())
    throw std::runtime_error("No xclbin speified");

  std::ifstream stream(fnm);
  stream.seekg(0,stream.end);
  size_t size = stream.tellg();
  stream.seekg(0,stream.beg);

  std::vector<char> header(size);
  stream.read(header.data(),size);

  auto top = reinterpret_cast<const axlf*>(header.data());
  if (xrtDeviceLoadXclbin(device, top))
    throw std::runtime_error("Bitstream download failed");

  return header;
}


int main(int argc, char *argv[]) {
    LOG_INIT_CERR();

    log.set_log_level(LOG_INFO);

    log(LOG_DEBUG) << "Arguments: " << argc << "\n";

    if (argc != 2) {
        std::cerr << "Usage: ./xrt_spmv <xclbin>\n";
        return EXIT_FAILURE;
    }

    int nnz = 128;
    int m   = 128;
    int n   = 128;

    double sparsity = 0.01;
    int compute_units = 1;
    int hw_side_len = 128;
    int verbosity = 0;
    int verify = 0;

    std::string xclbin_file = argv[1];

    DenseVector<data_t> x(n), ref_out(m), hw_out(m), out(m);
    std::vector<std::vector<int>> yPartRows;

    log(LOG_INFO) << "Preparing Test Matrix ... \n";
    CSRMatrix<data_t> csr = prepareData(x, ref_out, nnz, m, n);
    // CSRMatrixTiled<data_t> csr_tiled;
    // tile_matrix<data_t>(csr, compute_units, hw_side_len, yPartRows, csr_tiled);

    // xuid_t uuid;
    // xrtDeviceHandle dhdl = xrtDeviceOpen(0);
    xrt::device device = xrt::device(0);
    auto uuid = device.load_xclbin(xclbin_file);
    // auto xclbin = load_xclbin(dhdl, xclbin_file);//xrtDeviceLoadXclbinFile(dhdl, xclbinFilename);
    // xrtDeviceGetXclbinUUID(dhdl, uuid);

    size_t vals_size = csr.nnz() * sizeof(data_t);
    size_t idx_size  = csr.nnz() * sizeof(int);
    size_t rptr_size = (csr.rows() + 1) * sizeof(int);
    size_t x_size    = n * sizeof(data_t);
    size_t y_size    = m * sizeof(data_t);

    xrt::kernel mm2s_vals = xrt::kernel(device, uuid, "mm2s");
    xrt::kernel mm2s_c_idx = xrt::kernel(device, uuid, "mm2s");
    xrt::kernel mm2s_rptr = xrt::kernel(device, uuid, "mm2s");
    xrt::kernel mm2s_x = xrt::kernel(device, uuid, "mm2s");
    xrt::kernel s2mm_y = xrt::kernel(device, uuid, "s2mm");
    xrt::graph ghdl = xrt::graph(device, uuid, "spmv_I");

    // Allocate buffers
    // auto bo_vals    = xrtBOAlloc(dhdl, vals_size, 0, 0);
    // auto bo_idx     = xrtBOAlloc(dhdl, idx_size, 0, 0);
    // auto bo_rptr    = xrtBOAlloc(dhdl, rptr_size, 0, 0);
    // auto bo_x       = xrtBOAlloc(dhdl, x_size, 0, 0);
    // auto bo_y       = xrtBOAlloc(dhdl, y_size, 0, 0);

    xrt::bo bo_vals    = xrt::bo(device, vals_size, xrt::bo::flags::normal, mm2s_vals.group_id(0));
    xrt::bo bo_idx     = xrt::bo(device, idx_size, xrt::bo::flags::normal, mm2s_c_idx.group_id(0));
    xrt::bo bo_rptr    = xrt::bo(device, rptr_size + 1, xrt::bo::flags::normal, mm2s_rptr.group_id(0));
    xrt::bo bo_x       = xrt::bo(device, x_size, xrt::bo::flags::normal, mm2s_x.group_id(0));
    xrt::bo bo_y       = xrt::bo(device, y_size, xrt::bo::flags::normal, s2mm_y.group_id(0));
    
    auto bo_vals_map    = bo_vals.map();
    auto bo_idx_map     = bo_idx.map();
    auto bo_rptr_map    = bo_rptr.map();
    auto bo_x_map       = bo_x.map();
    auto bo_y_map       = bo_y.map();

    std::memcpy(bo_vals_map, csr.data.get(), vals_size);
    std::memcpy(bo_idx_map, csr.colIndex.get(), idx_size);
    std::memcpy(bo_rptr_map, csr.rowPointer.get(), rptr_size);
    std::memcpy(bo_x_map, x.elements.get(), x_size);

    bo_vals.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_idx.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_rptr.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_x.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    
    // xrtBOSync(bo_vals, XCL_BO_SYNC_BO_TO_DEVICE, vals_size, 0);
    // xrtBOSync(bo_idx,  XCL_BO_SYNC_BO_TO_DEVICE, idx_size, 0);
    // xrtBOSync(bo_rptr, XCL_BO_SYNC_BO_TO_DEVICE, rptr_size, 0);
    // xrtBOSync(bo_x,    XCL_BO_SYNC_BO_TO_DEVICE, x_size, 0);

    // auto mm2s_vals  = xrtPLKernelOpen(dhdl, uuid, "mm2s:mm2s_vals");
    // auto mm2s_idx   = xrtPLKernelOpen(dhdl, uuid, "mm2s_c_idx");
    // auto mm2s_rptr  = xrtPLKernelOpen(dhdl, uuid, "mm2s_rptr");
    // auto mm2s_x     = xrtPLKernelOpen(dhdl, uuid, "mm2s_x");

    // auto s2mm_y     = xrtPLKernelOpen(dhdl, uuid, "s2mm_y_out");

    // auto run_vals  = xrtRunOpen(mm2s_vals);
    // auto run_idx   = xrtRunOpen(mm2s_idx);
    // auto run_rptr  = xrtRunOpen(mm2s_rptr);
    // auto run_x     = xrtRunOpen(mm2s_x);
    // auto run_s2mm  = xrtRunOpen(s2mm_y);

    auto run_vals  = xrt::run(mm2s_vals);
    auto run_idx   = xrt::run(mm2s_c_idx);
    auto run_rptr  = xrt::run(mm2s_rptr);
    auto run_x     = xrt::run(mm2s_x);
    auto run_s2mm  = xrt::run(s2mm_y);

    run_vals.set_arg(0, bo_vals);
    run_vals.set_arg(1, nnz);
    run_idx.set_arg(0, bo_idx);
    run_idx.set_arg(1, nnz);
    run_rptr.set_arg(0, bo_rptr);
    run_rptr.set_arg(1, (m+ 1));
    run_x.set_arg(0, bo_x);
    run_x.set_arg(1, n);
    run_s2mm.set_arg(0, bo_y);
    run_s2mm.set_arg(1, m);

    // xrtRunSetArg(run_vals, 0, bo_vals);
    // xrtRunSetArg(run_vals, 2, nnz);

    // xrtRunSetArg(run_idx, 0, bo_idx);
    // xrtRunSetArg(run_idx, 2, nnz);

    // xrtRunSetArg(run_rptr, 0, bo_rptr);
    // xrtRunSetArg(run_rptr, 2, (m + 1));

    // xrtRunSetArg(run_x, 0, bo_x);
    // xrtRunSetArg(run_x, 2, n);

    // xrtRunSetArg(run_s2mm, 0, bo_y);
    // xrtRunSetArg(run_s2mm, 2, m);

    // xrtRunStart(run_vals);
    // xrtRunStart(run_idx);
    // xrtRunStart(run_rptr);
    // xrtRunStart(run_x);
    // xrtRunStart(run_s2mm);

    run_vals.start();
    run_idx.start();
    run_rptr.start();
    run_x.start();
    run_s2mm.start();

    

    // auto ghdl = xrtGraphOpen(dhdl, uuid, "spmv_I");
    int m_plus1 = m + 1;
    int nnz_val = nnz;
    ghdl.update("spmv_I.k_acc.in[2]", m_plus1);
    ghdl.update("spmv_I.k_mult.in[3]", nnz_val);
    // xrtGraphUpdateRTP(ghdl, "spmv_I.k_acc.in[2]", reinterpret_cast<const char*>(&m_plus1), sizeof(int));
    // xrtGraphUpdateRTP(ghdl, "spmv_I.k_mult.in[3]", reinterpret_cast<const char*>(&nnz_val), sizeof(int));
    ghdl.run(1);
    // xrtGraphRun(ghdl, 1 / compute_units);
    run_vals.wait();
    run_idx.wait();
    run_rptr.wait();
    run_x.wait();
    run_s2mm.wait();

    ghdl.wait();
    ghdl.end(0);
    // xrtRunWait(run_vals);
    // xrtRunWait(run_idx);
    // xrtRunWait(run_rptr);
    // xrtRunWait(run_x);
    // xrtRunWait(run_s2mm);

    // xrtGraphEnd(ghdl, 0);
    // xrtGraphClose(ghdl);

    // xrtBOSync(bo_y, XCL_BO_SYNC_BO_FROM_DEVICE, y_size, 0);
    // float* y_ptr = reinterpret_cast<float*>(xrtBOMap(bo_y));
    // for (int i = 0; i < m; ++i)
    //     hw_out[i] = y_ptr[i];

    // bool v = validate(ref_out, hw_out);

    // if (v)
    //     log(LOG_INFO) << "Passed ! \n";
    // else
    //     log(LOG_INFO) << "Failed ! \n";        

}
