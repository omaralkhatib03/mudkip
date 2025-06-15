#include <cstring>
#include <ctime>
#include <ratio>
#include <unistd.h>
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
#include "utility.hpp"
#include "utils.hpp"
#include "CsvWriter.hpp"

volatile int run    = 1;

#define MAX_NNZ 10e5
#define INCR 500
#define M_MAX 20000 
#define N_MAX 20000 
#define M_INCR 1000 
#define N_INCR 1000 

struct MatrixCSR {
    std::vector<int> rowPtr;
    std::vector<int> colIdx;
    std::vector<data_t> values;
};

void generate_csr_matrix(MatrixCSR &csr, int m, int n, int nnz) {
    // csr.values.resize(nnz); 
    // csr.colIdx.resize(nnz); 
    // csr.rowPtr.resize(m + 1, 0);

    srand(time(0));

    std::vector<int> row_nnz(m, 0);
    for (int i = 0; i < nnz; ++i) {
        int r = rand() % m;
        row_nnz[r]++;
    }

    csr.rowPtr[0] = 0;
    for (int i = 0; i < m; ++i) {
        csr.rowPtr[i + 1] = csr.rowPtr[i] + row_nnz[i];
    }

    assert(csr.rowPtr[m] == nnz);

    for (int i = 0; i < m; ++i) {
        int row_start = csr.rowPtr[i];
        int row_end = csr.rowPtr[i + 1];
        for (int j = row_start; j < row_end; ++j) {
            csr.colIdx[j] = rand() % n;
            csr.values[j] = rand() % 100 + 1;
        }
    }
}

void prepareData(MatrixCSR &csr, std::vector<data_t> &x, std::vector<data_t> &ref_out, int nnz, int m, int n) {
    LOG_INIT_CERR();
    log(LOG_DEBUG) << "Building Random CSR ... \n";
    generate_csr_matrix(csr, m, n, nnz);    
    log(LOG_DEBUG) << "Done With Building CSR ... \n";

    // x.resize(n);

    for (int i = 0; i < n; ++i)
    {
        x[i] = rand() % 10 + 1;
    }

    log(LOG_DEBUG) << "Out Size: " << ref_out.size() << "\n";
    log(LOG_DEBUG) << "X Size: " << x.size() << "\n"; 
    log(LOG_DEBUG) << "Computing Reference ... \n";

    spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), ref_out.data());

}

template <typename T>
double time_op(T aFunction) {
    auto t1 = std::chrono::high_resolution_clock::now();
    aFunction();
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(t2 - t1).count();
}

double time_cpu_csr(MatrixCSR &csr, std::vector<data_t> &x, int m, std::vector<data_t> & out) {
    auto cpu_lambda = [&]() {
        spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), out.data());
    };
    return time_op<>(cpu_lambda);
}

double time_kernel_csr(const xrt::device &device, xrt::kernel &kernel, const MatrixCSR csr,
                       const std::vector<data_t> &x, std::vector<data_t> &hw_out, 
                       int m, int n, int nnz) {

    LOG_INIT_CERR();

    log(LOG_DEBUG) << " Allocating XRT Buffers... \n";

    for (int i = 0; i < 5; ++i) {
        log(LOG_INFO) << "kernel.group_id(" << i << ") = " << kernel.group_id(i) << "\n";
    }

    // Allocate buffers
    auto bo_rptr    = xrt::bo(device, (m + 1) * sizeof(int),xrt::bo::flags::normal, kernel.group_id(0));
    log(LOG_INFO) << "Allocated RPTR ... \n";
    auto bo_idx     = xrt::bo(device, nnz * sizeof(int),  xrt::bo::flags::normal, kernel.group_id(1));
    log(LOG_INFO) << "Allocated C_IDX ... \n";
    auto bo_vals    = xrt::bo(device, nnz * sizeof(data_t),  xrt::bo::flags::normal, kernel.group_id(2));
    log(LOG_INFO) << "Allocated C_VALS ... \n";
    auto bo_x       = xrt::bo(device, n * sizeof(data_t),    xrt::bo::flags::normal, kernel.group_id(3));
    log(LOG_INFO) << "Allocated X ... \n";
    auto bo_y       = xrt::bo(device, m * sizeof(data_t),    xrt::bo::flags::normal, kernel.group_id(4));
    log(LOG_INFO) << "Allocated Y ... \n";

    log(LOG_INFO) << "Copying Data ... \n";

    std::memcpy(bo_rptr.map<int*>(), csr.rowPtr.data(), (m+1) * sizeof(int));
    std::memcpy(bo_vals.map<data_t*>(), csr.values.data(), nnz * sizeof(data_t));
    std::memcpy(bo_idx.map<int*>(), csr.colIdx.data(), nnz * sizeof(data_t));
    std::memcpy(bo_x.map<data_t*>(), x.data(), n * sizeof(data_t));

    log(LOG_INFO) << "Syncing ... \n";

    bo_vals.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_idx.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_rptr.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo_x.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    log(LOG_INFO) << "Opening Kernel ... \n";

    auto spmv   = xrt::run(kernel);

    log(LOG_INFO) << "Setting RPTR ... \n";
    spmv.set_arg(0, bo_rptr);
    log(LOG_INFO) << "Setting IDX ... \n";
    spmv.set_arg(1, bo_idx);
    log(LOG_INFO) << "Setting VALS ... \n";
    spmv.set_arg(2, bo_vals);
    log(LOG_INFO) << "Setting X ... \n";
    spmv.set_arg(3, bo_x);
    log(LOG_INFO) << "Setting Y ... \n";
    spmv.set_arg(4, bo_y);
    log(LOG_INFO) << "Setting M ... \n";
    spmv.set_arg(5, m);
    log(LOG_INFO) << "Setting NNZ ... \n";
    spmv.set_arg(6, nnz);

    log(LOG_INFO) << "Starting Kernel ... \n";

    auto op = [&]() {
        spmv.start();
        spmv.wait();
    };

    log(LOG_DEBUG) << " Running Timing ... \n";
    auto diff = time_op<>(op);

    log(LOG_DEBUG) << " Syncing Output ... \n";
    bo_y.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    log(LOG_DEBUG) << " Reading Device Buffer ... \n";
    std::memcpy(hw_out.data(), bo_y.map<data_t*>(), m * sizeof(data_t));

    return diff;
}

bool validate(const DenseVector<data_t> &ref, const DenseVector<data_t> &test) {
    LOG_INIT_CERR();
    const float eps = 1e-3;
    for (int i = 0; i < ref.size(); ++i) {
        auto res = test[i] - 1;
        // log(LOG_INFO) << "i: " << i << " Ref: " << ref[i] << " test: " << res << "\n";  

        if (std::abs(ref[i] - res) > eps) {
            log(LOG_ERR) << "Mismatch at index " << i << ": " << ref[i] << " vs " << res << "\n";
            // return false;
        }
    }
    return true;
}

void nnz_run(xrt::device & device, xrt::kernel & kernel, int nnz, int m, int n)
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
        MatrixCSR csr;
        std::vector<data_t> x, ref_out, hw_out, out;

        out.resize(m);
        hw_out.resize(m);
        ref_out.resize(m);

        prepareData(csr, x, ref_out, nnz, m, n);

        // auto diff_kernel = time_kernel_csr(device, kernel, csr, x, hw_out, nnz, m, n);
        // log(LOG_INFO) << "NNZ: " << nnz << ", Kernel Time Test: " << diff_kernel << " ms\n";

        auto diff_cpu = time_cpu_csr(csr, x, m, out);
        log(LOG_INFO) << "NNZ: " << nnz << ", CPU Time Test: " << diff_cpu << " ms\n";

        // if (!validate(out, hw_out))
        //     throw std::runtime_error("What ? Bad compute at NNZ: " + std::to_string(nnz));

        // csvfile.write(nnz, diff_cpu, diff_kernel);
    }
}

void m_n_run(xrt::device & device, xrt::kernel & kernel, double spsty, int m, int n)
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
            MatrixCSR csr;
            std::vector<data_t> x, ref_out, hw_out, out;

            out.resize(m_curr);
            hw_out.resize(m_curr);
            ref_out.resize(m_curr);

            prepareData(csr, x, ref_out, nnz, m_curr, n_curr);

            // auto diff_kernel = time_kernel_csr(device, kernel, csr, x, hw_out, nnz, m_curr, n_curr);
            // log(LOG_INFO) << "M: " << m_curr << ", N: " << n_curr << ", Kernel Time Test: " << diff_kernel << " ms\n";

            auto diff_cpu = time_cpu_csr(csr, x, m_curr, out);
            log(LOG_INFO) << "M: " << m_curr << ", N: " << n_curr << ", CPU Time Test: " << diff_cpu << " ms\n";

            // if (!validate(out, hw_out))
            //     throw std::runtime_error("Bad compute at M: " + std::to_string(m_curr) + " N: " + std::to_string(n_curr));

            // csvfile.write(m_curr, n_curr, diff_cpu, diff_kernel);
        }
    }
}

double spmv_wrapper(std::unique_ptr<CSRMatrix<data_t>> & csr, DenseVector<data_t> & x, DenseVector<data_t> & out)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    spMvCsR(csr->rowPointer.get(), csr->colIndex.get(), csr->data.get(), csr->rows(), x.elements.get(), out.elements.get());
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(t2 - t1).count();
}

int main2(int argc, char *argv[]) 
{
    LOG_INIT_CERR();
    log.set_log_level(LOG_INFO);

    if (argc != 2) {
        std::cerr << "Usage: ./xrt_spmv <xclbin>\n";
        return EXIT_FAILURE;
    }

    std::string xclbin_file = argv[1];
    // std::string mat_file = argv[2];

    xrt::device device = xrt::device(0);

    log(LOG_INFO) << "Opened Device... \n";
    auto uuid = device.load_xclbin(xclbin_file);
    auto kernel = xrt::kernel(device, uuid, "spmv_stream_manu_loc");

    auto csvfile = csv::CsvWriter("solved.csv", "problem", "time_ms");
    log(LOG_INFO) << "Loeaded XCLbin... \n";
    MatrixCSR csr;

    // bool read;
    // auto mtx = ReadMatrixCSR<data_t>(mat_file, read); 

    // if (!read)
    // {
    //     log(LOG_ERROR) << "Where your file at ? \n";
    //     return EXIT_FAILURE;
    // }


    // int nnz = mtx->nnz();
    // int m   = mtx->rows();
    // int n   = mtx->cols();

    int nnz = 128;
    int m   = 100;
    int n   = 100;

    std::vector<data_t> x, ref_out, hw_out, out;

    csr.values.resize(nnz); 
    csr.colIdx.resize(nnz); 
    csr.rowPtr.resize(m + 1, 0);

    log(LOG_INFO) << "Resizing Vectors \n";

    out.resize(m);
    hw_out.resize(m);
    ref_out.resize(m);
    x.resize(n);

    for (int i = 0; i < n; i++) x[i] = 1;

    log(LOG_INFO) << "Preparing Test Matrix ... \n";
    prepareData(csr, x, ref_out, nnz, m, n);

    log(LOG_INFO) << "Computing REF \n";

    auto c0 = std::chrono::high_resolution_clock::now();
    spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), ref_out.data());
    auto c1 = std::chrono::high_resolution_clock::now();
    auto c = std::chrono::duration<double, std::milli>(c1 - c0).count();

    log(LOG_INFO) << "CPU Time: " << c << "ms \n";


    for (int i = 0; i < m; i++)
    {
        hw_out[i] = -1;
    }

    log(LOG_INFO) << "Allocating Buffers ... \n";
        
    auto t = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    // auto t1 = std::chrono::high_resolution_clock::now();
    // spmv.start();
    // spmv.wait();
    // auto t2 = std::chrono::high_resolution_clock::now();
    // log(LOG_INFO) << "Kernel Code: " << spmv.return_code() << "\n";
    // auto t = std::chrono::duration<double, std::milli>(t2 - t1).count();

    csvfile.write(argv[2], t);

    log(LOG_INFO) << "Kernel Time: " << t << "ms \n";

    DenseVector<data_t> ref(m, -1), dhw(m, -1);

    for (int i = 0 ; i < m; i ++)
    {
        ref[i] = ref_out[i];
        dhw[i] = hw_out[i];
    }

    return validate(ref, dhw);
}

int main(int argc, char *argv[]) {
    LOG_INIT_CERR();

    log.set_log_level(LOG_DEBUG);

    log(LOG_DEBUG) << "Arguments: " << argc << "\n";

    if (argc != 4) {
        std::cerr << "Usage: ./xrt_spmv <xclbin> <problem>\n";
        return EXIT_FAILURE;
    }

    std::string xclbin_file = argv[1];
    std::string problem = argv[2];

    auto device = xrt::device(0);
    auto uuid = device.load_xclbin(xclbin_file);
    auto kernel = xrt::kernel(device, uuid, argv[3]);

    log(LOG_INFO) << "Kernel Name: " << argv[3] << " \n";

    auto filename = "solved.csv";
    auto csvfile = csv::CsvWriter(filename, "problem", argv[3], "spmv_jin_mori", "spmv_man_unroll", "spmv_naive", "spmv_steam_local_unrolled", "spmv_stream_manu_loc", "spmv_naive_pipe", "cpu_time");
    bool read;
    auto mtrxPtr = ReadMatrixCSR<data_t>(problem, read);

    if (!read)
    {
       return EXIT_FAILURE; 
    }
    
    MatrixCSR csr;

    std::vector<data_t> x, ref_out, hw_out, out;
    log(LOG_INFO) << "Resizing \n";
    
    int m = mtrxPtr->rows();
    int n = mtrxPtr->cols();
    int nnz = mtrxPtr->nnz();

    csr.values.resize(nnz); 
    csr.colIdx.resize(nnz); 
    csr.rowPtr.resize(m + 1, 0);
    hw_out.resize(m);
    x.resize(n);

    for (int i = 0; i < n; i++) x[i] = 1;

    log(LOG_INFO) << "Copying NNZ \n";

    for (int i = 0; i < nnz; i++)
    {
        csr.colIdx[i] = mtrxPtr->getColIndex(i);
        csr.values[i] = mtrxPtr->getData(i);
    }

    log(LOG_INFO) << "Copying Rows \n";

    for (int i = 0; i < m + 1; i++)
    {
        csr.rowPtr[i] = mtrxPtr->getRowPointer(i);
    }

    log(LOG_INFO) << "Opened Matrix... \n";

    DenseVector<data_t> x2(MAX_VECTOR, 1);
    DenseVector<data_t> ref_out2(mtrxPtr->rows(), -1); 
    DenseVector<data_t> hw_out2(mtrxPtr->rows(), -1);

    log(LOG_INFO) << "Obtaining Golden... \n";

    auto cpu_time = spmv_wrapper(mtrxPtr, x2, ref_out2);

    log(LOG_INFO) << "Obtaining Golden Got... \n";

    auto diff_kernel = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    log(LOG_INFO) << "Finished Kernel Time Test: " << argv[3] << ": " << diff_kernel << " ms\n";

    auto kernel_name =   "spmv_jin_mori";
    kernel = xrt::kernel(device, uuid, kernel_name);

    auto diff_kernel1 = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    log(LOG_INFO) << "Finished Kernel Time Test: " << kernel_name << ": " << diff_kernel1 << " ms\n";

    kernel_name =   "spmv_man_unroll";
    kernel = xrt::kernel(device, uuid, kernel_name);

    auto diff_kernel2 = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    log(LOG_INFO) << "Finished Kernel Time Test: " << kernel_name << ": " << diff_kernel2 << " ms\n";

    kernel_name =   "spmv_naive";
    kernel = xrt::kernel(device, uuid, kernel_name);

    auto diff_kernel3 = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    log(LOG_INFO) << "Finished Kernel Time Test: " << kernel_name << ": " << diff_kernel3 << " ms\n";

    kernel_name =   "spmv_steam_local_unrolled";
    kernel = xrt::kernel(device, uuid, kernel_name);

    auto diff_kernel4 = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    log(LOG_INFO) << "Finished Kernel Time Test: " << kernel_name << ": " << diff_kernel4 << " ms\n";

    kernel_name =   "spmv_stream_manu_loc";
    kernel = xrt::kernel(device, uuid, kernel_name);

    auto diff_kernel5 = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    log(LOG_INFO) << "Finished Kernel Time Test: " << kernel_name << ": " << diff_kernel5 << " ms\n";

    kernel_name =   "spmv_naive_pipe";
    kernel = xrt::kernel(device, uuid, kernel_name);

    auto diff_kernel6 = time_kernel_csr(device, kernel, csr, x, hw_out, m, n, nnz);
    log(LOG_INFO) << "Finished Kernel Time Test: " << kernel_name << ": " << diff_kernel6 << " ms\n";

    DenseVector<data_t> ref(m, -1), dhw(m, -1);

    for (int i = 0 ; i < m; i ++)
    {
        ref[i] = ref_out2[i];
        dhw[i] = hw_out[i];
    }

    bool success = validate(ref, dhw);
    std::cout << (success ? "PASSED" : "FAILED") << std::endl;   

    csvfile.write(problem, diff_kernel, diff_kernel1, diff_kernel2, diff_kernel3, diff_kernel4, diff_kernel5, diff_kernel6, cpu_time);

    return 0;
}


