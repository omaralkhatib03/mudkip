// #include <cstring>
// #include <ctime>
// #include <ratio>
// #include <unistd.h>
// #include <xrt/xrt_device.h>
// #include <xrt/xrt_bo.h>
// #include <xrt/xrt_kernel.h>
// #include <experimental/xrt_xclbin.h>
// #include <chrono>
// #include <cmath>
// #include <cstdlib>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <cassert>
// #include "BSLogger.hpp"
// #include "deprecated/xrt.h"
// #include "ert.h"
// #include "include/csr_matrix.hpp"
// #include "include/dense_vector.hpp"
// #include "types.hpp"
// #include "utility.hpp"
// #include "utils.hpp"
// #include "CsvWriter.hpp"
//
// volatile int run    = 1;
//
// #define MAX_NNZ 10e5
// #define INCR 500
// #define M_MAX 20000 
// #define N_MAX 20000 
// #define M_INCR 1000 
// #define N_INCR 1000 
//
// struct MatrixCSR {
//     std::vector<int> rowPtr;
//     std::vector<int> colIdx;
//     std::vector<data_t> values;
// };
//
// void generate_csr_matrix(MatrixCSR &csr, int m, int n, int nnz) {
//     csr.values.resize(nnz); 
//     csr.colIdx.resize(nnz); 
//     csr.rowPtr.resize(m + 1, 0);
//
//     srand(time(0));
//
//     std::vector<int> row_nnz(m, 0);
//     for (int i = 0; i < nnz; ++i) {
//         int r = rand() % m;
//         row_nnz[r]++;
//     }
//
//     csr.rowPtr[0] = 0;
//     for (int i = 0; i < m; ++i) {
//         csr.rowPtr[i + 1] = csr.rowPtr[i] + row_nnz[i];
//     }
//
//     assert(csr.rowPtr[m] == nnz);
//
//     for (int i = 0; i < m; ++i) {
//         int row_start = csr.rowPtr[i];
//         int row_end = csr.rowPtr[i + 1];
//         for (int j = row_start; j < row_end; ++j) {
//             csr.colIdx[j] = rand() % n;
//             csr.values[j] = rand() % 100 + 1;
//         }
//     }
// }
//
// void prepareData(MatrixCSR &csr, std::vector<data_t> &x, std::vector<data_t> &ref_out, int nnz, int m, int n) {
//     LOG_INIT_CERR();
//     log(LOG_DEBUG) << "Building Random CSR ... \n";
//     generate_csr_matrix(csr, m, n, nnz);    
//     log(LOG_DEBUG) << "Done With Building CSR ... \n";
//
//     x.resize(n);
//
//     for (int i = 0; i < n; ++i)
//     {
//         x[i] = rand() % 10 + 1;
//     }
//
//     log(LOG_DEBUG) << "Out Size: " << ref_out.size() << "\n";
//     log(LOG_DEBUG) << "X Size: " << x.size() << "\n"; 
//     log(LOG_DEBUG) << "Computing Reference ... \n";
//
//     spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), ref_out.data());
//
// }
//
// template <typename T>
// double time_op(T aFunction) {
//     auto t1 = std::chrono::high_resolution_clock::now();
//     for (int i = 0; i < run; i++) {
//         aFunction();
//     }
//     auto t2 = std::chrono::high_resolution_clock::now();
//     return std::chrono::duration<double, std::micro>(t2 - t1).count();
// }
//
// double time_cpu_csr(MatrixCSR &csr, std::vector<data_t> &x, int m, std::vector<data_t> & out) {
//     auto cpu_lambda = [&]() {
//         spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), out.data());
//     };
//     return time_op<>(cpu_lambda);
// }
//
// double time_kernel_csr(const xrt::device &device, xrt::kernel &kernel, const std::unique_ptr<CSRMatrix<data_t>> & csr,
//                        const DenseVector<data_t> &x, DenseVector<data_t> &hw_out) {
//
//     LOG_INIT_CERR();
//     log(LOG_DEBUG) << " Allocating XRT Buffers... \n";
//
//     for (int i = 0; i < 5; ++i) {
//         log(LOG_INFO) << "kernel.group_id(" << i << ") = " << kernel.group_id(i) << "\n";
//     }
//
//     int m = csr->rows();
//     int n = csr->cols();
//     int nnz = csr->nnz();
//
//     log(LOG_DEBUG) << " Allocating Row Pointer ... \n";
//     xrt::bo rowPtr_bo = xrt::bo(device, (m + 1) * sizeof(int), kernel.group_id(0));
//
//     log(LOG_DEBUG) << " Allocating Column index ... \n";
//     auto colIdx_bo = xrt::bo(device, nnz * sizeof(int), kernel.group_id(2));
//
//     log(LOG_DEBUG) << " Allocating Values ... \n";
//     auto vals_bo = xrt::bo(device, nnz * sizeof(data_t), kernel.group_id(1));
//
//     log(LOG_DEBUG) << " Allocating Input X ... \n";
//     auto x_bo = xrt::bo(device, n * sizeof(data_t), kernel.group_id(3));
//
//     log(LOG_DEBUG) << " Allocating Output Y ... \n";
//     auto y_bo = xrt::bo(device, m * sizeof(data_t), kernel.group_id(4));
//
//     std::memcpy(rowPtr_bo.map(), csr->rowPointer.get(), (m + 1) * sizeof(int));
//     std::memcpy(colIdx_bo.map(), csr->colIndex.get(), nnz * sizeof(int));
//     std::memcpy(vals_bo.map(), csr->data.get(), nnz * sizeof(data_t));
//     std::memcpy(x_bo.map(), x.elements.get(), n * sizeof(data_t));
//
//     log(LOG_DEBUG) << " Syncing Buffers .... \n";
//
//     rowPtr_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     colIdx_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     vals_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     x_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     y_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//
//     log(LOG_DEBUG) << " Creating Operation .... \n";
//
//     // void spmv(
//     // int* r_beg, 0
//     // int* c_idx, 1
//     // data_t* c_val, 2
//     // data_t* x, 3
//     // data_t* out, 4
//     // int m, 5
//     // int nnz 6
//     // );
//
//     xrt::run run(kernel);
//
//     run.set_arg(0, rowPtr_bo);
//     run.set_arg(1, colIdx_bo);
//     run.set_arg(2, vals_bo);
//     run.set_arg(3, x_bo);
//     run.set_arg(4, y_bo);
//     run.set_arg(5, m);
//     run.set_arg(6, nnz);
//
//     auto op = [&]() {
//         run.start();
//         run.wait();
//     };
//
//     log(LOG_DEBUG) << " Running Timing ... \n";
//     auto diff = time_op<>(op);
//
//     log(LOG_DEBUG) << " Syncing Output ... \n";
//     y_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
//
//     log(LOG_DEBUG) << " Reading Device Buffer ... \n";
//     std::memcpy(hw_out.elements.get(), y_bo.map(), m * sizeof(data_t));
//
//     return diff;
// }
//
// bool validate(const DenseVector<data_t> &ref, const DenseVector<data_t> &test) {
//     LOG_INIT_CERR();
//     const float eps = 1e-3;
//     for (size_t i = 0; i < ref.size(); ++i) {
//         log(LOG_INFO) << "i: " << i << " Ref: " << ref[i] << " test: " << test[i] << "\n";  
//
//         if (std::abs(ref[i] - test[i]) > eps) {
//             log(LOG_ERR) << "Mismatch at index " << i << ": " << ref[i] << " vs " << test[i] << "\n";
//             // return false;
//         }
//     }
//     return true;
// }
//
// void nnz_run(xrt::device & device, xrt::kernel & kernel, int nnz, int m, int n)
// {
//     LOG_INIT_CERR();
//
//     log(LOG_INFO) << "Beginning Data Collection\n";
//
//     log(LOG_INFO) << "Creating CsvWriter \n";
//
//     auto filename = "cltb_" + std::to_string(std::time(0)) + ".csv";
//     auto csvfile = csv::CsvWriter(filename, "nnz", "cpu", "kenerl");
//
//     log(LOG_INFO) << "FileName: " << filename << "\n";
//     log(LOG_INFO) << "Columns: " << "nnz " << " cpu " << " kenerl " << "\n";
//
//     log(LOG_INFO) << "Max NNZ: " << MAX_NNZ << "\n";
//     log(LOG_INFO) << "Increment: " << INCR << "\n";
//     log(LOG_INFO) << "Start NNZ: " << nnz << "\n";
//
//     for (; nnz < MAX_NNZ; nnz+=INCR)
//     {
//         MatrixCSR csr;
//         std::vector<data_t> x, ref_out, hw_out, out;
//
//         out.resize(m);
//         hw_out.resize(m);
//         ref_out.resize(m);
//
//         prepareData(csr, x, ref_out, nnz, m, n);
//
//         // auto diff_kernel = time_kernel_csr(device, kernel, csr, x, hw_out, nnz, m, n);
//         // log(LOG_INFO) << "NNZ: " << nnz << ", Kernel Time Test: " << diff_kernel << " ms\n";
//
//         auto diff_cpu = time_cpu_csr(csr, x, m, out);
//         log(LOG_INFO) << "NNZ: " << nnz << ", CPU Time Test: " << diff_cpu << " ms\n";
//
//         // if (!validate(out, hw_out))
//         //     throw std::runtime_error("What ? Bad compute at NNZ: " + std::to_string(nnz));
//
//         // csvfile.write(nnz, diff_cpu, diff_kernel);
//     }
// }
//
// void m_n_run(xrt::device & device, xrt::kernel & kernel, double spsty, int m, int n)
// {
//     LOG_INIT_CERR();
//
//     log(LOG_INFO) << "Beginning Grid Search over M and N\n";
//
//     auto filename = "m_n_grid_" + std::to_string(std::time(0)) + ".csv";
//     auto csvfile = csv::CsvWriter(filename, "m", "n", "cpu", "kernel");
//
//     int nnz  = (n * m) * spsty;
//
//     for (int m_curr = m; m_curr < M_MAX; m_curr += M_INCR)
//     {
//         for (int n_curr = n; n_curr < N_MAX; n_curr += N_INCR)
//         {
//             MatrixCSR csr;
//             std::vector<data_t> x, ref_out, hw_out, out;
//
//             out.resize(m_curr);
//             hw_out.resize(m_curr);
//             ref_out.resize(m_curr);
//
//             prepareData(csr, x, ref_out, nnz, m_curr, n_curr);
//
//             // auto diff_kernel = time_kernel_csr(device, kernel, csr, x, hw_out, nnz, m_curr, n_curr);
//             // log(LOG_INFO) << "M: " << m_curr << ", N: " << n_curr << ", Kernel Time Test: " << diff_kernel << " ms\n";
//
//             auto diff_cpu = time_cpu_csr(csr, x, m_curr, out);
//             log(LOG_INFO) << "M: " << m_curr << ", N: " << n_curr << ", CPU Time Test: " << diff_cpu << " ms\n";
//
//             // if (!validate(out, hw_out))
//             //     throw std::runtime_error("Bad compute at M: " + std::to_string(m_curr) + " N: " + std::to_string(n_curr));
//
//             // csvfile.write(m_curr, n_curr, diff_cpu, diff_kernel);
//         }
//     }
// }
//
// void spmv_wrapper(std::unique_ptr<CSRMatrix<data_t>> & csr, DenseVector<data_t> & x, DenseVector<data_t> & out)
// {
//     spMvCsR(csr->rowPointer.get(), csr->colIndex.get(), csr->data.get(), csr->rows(), x.elements.get(), out.elements.get());
// }
//
// int main2(int argc, char *argv[]) {
//     LOG_INIT_CERR();
//
//     log.set_log_level(LOG_DEBUG);
//
//     log(LOG_DEBUG) << "Arguments: " << argc << "\n";
//
//     if (argc != 3) {
//         std::cerr << "Usage: ./xrt_spmv <xclbin> <problem>\n";
//         return EXIT_FAILURE;
//     }
//
//     std::string xclbin_file = argv[1];
//     std::string problem = argv[2];
//
//     auto device = xrt::device(0);
//     auto uuid = device.load_xclbin(xclbin_file);
//     auto kernel = xrt::kernel(device, uuid, "spmv");
//
//
//     auto filename = "solved.csv";
//     auto csvfile = csv::CsvWriter(filename, "problem", "time_us");
//     bool read;
//     auto mtrxPtr = ReadMatrixCSR<data_t>(problem, read);
//
//     if (!read)
//     {
//        return EXIT_FAILURE; 
//     }
//
//     log(LOG_INFO) << "Opened Matrix... \n";
//
//     DenseVector<data_t> x(MAX_VECTOR, 1);
//     DenseVector<data_t> ref_out(mtrxPtr->rows(), -1); 
//     DenseVector<data_t> hw_out(mtrxPtr->rows(), -1);
//
//     log(LOG_INFO) << "Obtaining Golden... \n";
//
//     spmv_wrapper(mtrxPtr, x, ref_out);
//
//     log(LOG_INFO) << "Obtaining Golden Got... \n";
//
//     log(LOG_INFO) << "Starting Kernel Time Test ... \n";
//     auto diff_kernel = time_kernel_csr(device, kernel, mtrxPtr, x, hw_out);
//     log(LOG_INFO) << "Finished Kernel Time Test: " << diff_kernel << " µs\n";
//
//     bool success = validate(ref_out, hw_out);
//     std::cout << (success ? "PASSED" : "FAILED") << std::endl;   
//
//     csvfile.write(problem, diff_kernel);
//
//     return 0;
// }
//
// static std::vector<char>
// load_xclbin(xrtDeviceHandle device, const std::string& fnm)
// {
//   if (fnm.empty())
//     throw std::runtime_error("No xclbin speified");
//
//   std::ifstream stream(fnm);
//   stream.seekg(0,stream.end);
//   size_t size = stream.tellg();
//   stream.seekg(0,stream.beg);
//
//   std::vector<char> header(size);
//   stream.read(header.data(),size);
//
//   auto top = reinterpret_cast<const axlf*>(header.data());
//   if (xrtDeviceLoadXclbin(device, top))
//     throw std::runtime_error("Bitstream download failed");
//
//   return header;
// }
//
// int main(int argc, char *argv[]) 
// {
//     LOG_INIT_CERR();
//     log.set_log_level(LOG_INFO);
//
//     if (argc != 2) {
//         std::cerr << "Usage: ./xrt_spmv <xclbin>\n";
//         return EXIT_FAILURE;
//     }
//
//     int nnz = 32;
//     int m   = 32;
//     int n   = 32;
//     double sparsity = 0.01;
//
//     std::string xclbin_file = argv[1];
//
//     xrt::device device = xrt::device(0);
//
//     log(LOG_INFO) << "Opened Device... \n";
//     auto uuid = device.load_xclbin(xclbin_file);
//     auto kernel = xrt::kernel(device, uuid, "spmv");
//
//     log(LOG_INFO) << "Loeaded XCLbin... \n";
//     MatrixCSR csr;
//     std::vector<data_t> x, ref_out, hw_out, out;
//
//     out.resize(m);
//     hw_out.resize(m);
//     ref_out.resize(m);
//
//     for (int i = 0; i < m; i++)
//     {
//         hw_out[i] = -1;
//     }
//
//     log(LOG_INFO) << "Preparing Test Matrix ... \n";
//     prepareData(csr, x, ref_out, nnz, m, n);
//
//     log(LOG_INFO) << "Allocating Buffers ... \n";
//     // Allocate buffers
//     auto bo_rptr    = xrt::bo(device, (m + 1) * sizeof(int),xrt::bo::flags::normal, kernel.group_id(0));
//     auto bo_idx     = xrt::bo(device, nnz * sizeof(int),  xrt::bo::flags::normal, kernel.group_id(1));
//     auto bo_vals    = xrt::bo(device, nnz * sizeof(data_t),  xrt::bo::flags::normal, kernel.group_id(2));
//     auto bo_x       = xrt::bo(device, n * sizeof(data_t),    xrt::bo::flags::normal, kernel.group_id(3));
//     auto bo_y       = xrt::bo(device, m * sizeof(data_t),    xrt::bo::flags::normal, kernel.group_id(4));
//
//     log(LOG_INFO) << "Copying Data ... \n";
//
//     std::memcpy(bo_rptr.map<int*>(), csr.rowPtr.data(), (m+1) * sizeof(int));
//     std::memcpy(bo_vals.map<data_t*>(), csr.values.data(), nnz * sizeof(data_t));
//     std::memcpy(bo_idx.map<int*>(), csr.colIdx.data(), nnz * sizeof(data_t));
//     std::memcpy(bo_x.map<data_t*>(), x.data(), n * sizeof(data_t));
//
//     log(LOG_INFO) << "Syncing ... \n";
//
//     bo_vals.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     bo_idx.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     bo_rptr.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     bo_x.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//
//     log(LOG_INFO) << "Opening Kernel ... \n";
//
//     auto spmv   = xrt::run(kernel);
//
//     log(LOG_INFO) << "Setting RPTR ... \n";
//     spmv.set_arg(0, bo_rptr);
//     log(LOG_INFO) << "Setting IDX ... \n";
//     spmv.set_arg(1, bo_idx);
//     log(LOG_INFO) << "Setting VALS ... \n";
//     spmv.set_arg(2, bo_vals);
//     log(LOG_INFO) << "Setting X ... \n";
//     spmv.set_arg(3, bo_x);
//     log(LOG_INFO) << "Setting Y ... \n";
//     spmv.set_arg(4, bo_y);
//     log(LOG_INFO) << "Setting M ... \n";
//     spmv.set_arg(5, m);
//     log(LOG_INFO) << "Setting NNZ ... \n";
//     spmv.set_arg(6, nnz);
//
//     log(LOG_INFO) << "Starting Kernel ... \n";
//
//     log(LOG_INFO) << "Kernel Code: " << spmv.return_code() << "\n";
//
//     auto t1 = std::chrono::high_resolution_clock::now();
//     spmv.start();
//     spmv.wait();
//     auto t2 = std::chrono::high_resolution_clock::now();
//     log(LOG_INFO) << "Kernel Code: " << spmv.return_code() << "\n";
//     auto t = std::chrono::duration<double, std::micro>(t2 - t1).count();
//
//     log(LOG_INFO) << "Kernel Time: " << t << "\n";
//
//     bo_y.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
//     std::memcpy(hw_out.data(), bo_y.map<data_t*>(), m * sizeof(data_t));
//
//     // for (int i = 0; i < m; i++)
//     // {
//     //     std::cout << "v[" << i << "] = " << hw_out[i] << std::endl;
//     // }
//
//     if (spmv.state() != ERT_CMD_STATE_COMPLETED) {
//         std::cerr << "Kernel failed to complete. State: " << static_cast<int>(spmv.state()) << "\n";
//         std::cerr << "Error Message: " << spmv.get_ert_packet() << "\n";
//     }
//
//     return 1;
// }


#include <cstring>
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
#include "utils.hpp"

using data_t = float;
static constexpr int m = 32;
static constexpr int n = 32;
static constexpr int nnz = 100;
static constexpr int run = 1;

struct MatrixCSR {
    std::vector<int> rowPtr;
    std::vector<int> colIdx;
    std::vector<data_t> values;
};

void generate_csr_matrix(MatrixCSR &csr, int m, int n, int nnz) {
    csr.values.resize(nnz); 
    csr.rowPtr.resize(m + 1);
    csr.colIdx.resize(nnz);

    srand(1);

    for (int i = 0; i < nnz; ++i) {
        csr.values[i] = rand() % 100 + 1;
        csr.colIdx[i] = rand() % n;
    }
    for (int i = 0; i < nnz; ++i) {
        csr.rowPtr[rand() % m + 1]++;
    }
    for (int i = 1; i <= m; ++i) {
        csr.rowPtr[i] += csr.rowPtr[i - 1];
    }
}

void prepareData(MatrixCSR &csr, std::vector<data_t> &x, std::vector<data_t> &ref_out) {
    LOG_INIT_CERR();
    log(LOG_INFO) << "Building Random CSR ... \n";
    generate_csr_matrix(csr, m, n, nnz);    
    log(LOG_INFO) << "Done With Building CSR ... \n";

    x.resize(n);
    ref_out.resize(n);

    for (int i = 0; i < n; ++i)
    {
        x[i] = rand() % 10 + 1;
    }

    log(LOG_INFO) << "Out Size: " << ref_out.size() << "\n";

    log(LOG_INFO) << "X Size: " << x.size() << "\n"; 

    // for (int i = 0; i < n; i++)
    //     log(LOG_DEBUG) << "X[" << i << "]: " << x[i] << "\n";

    log(LOG_INFO) << "Computing Reference ... \n";

    spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), ref_out.data());

    ref_out.resize(n, 0);
}

template <typename T>
double time_op(T aFunction) {
    auto t1 = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < run; i++) {
    aFunction();
    // }
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(t2 - t1).count();
}

double time_cpu_csr(MatrixCSR &csr, std::vector<data_t> &x) {
    std::vector<data_t> out(n);
    auto cpu_lambda = [&]() {
        spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), out.data());
    };
    return time_op<>(cpu_lambda);
}

double time_kernel_csr(const xrt::device &device, xrt::kernel &kernel, const MatrixCSR &csr,
                       const std::vector<data_t> &x, std::vector<data_t> &hw_out) {

    LOG_INIT_CERR();
    log(LOG_INFO) << " Allocating XRT Buffers... \n";

    hw_out.resize(n);

    // for (int i = 0; i < 6; ++i) {
    //     log(LOG_INFO) << "kernel.group_id(" << i << ") = " << kernel.group_id(i) << "\n";
    // }
    
    auto rowPtr_bo = xrt::bo(device, (m + 1) * sizeof(int), kernel.group_id(0));
    auto colIdx_bo = xrt::bo(device, nnz * sizeof(int), kernel.group_id(1));
    auto vals_bo = xrt::bo(device, nnz * sizeof(data_t), kernel.group_id(2));
    auto x_bo = xrt::bo(device, n * sizeof(data_t), kernel.group_id(3));
    auto y_bo = xrt::bo(device, n * sizeof(data_t), kernel.group_id(4));
    
    std::memcpy(rowPtr_bo.map(), csr.rowPtr.data(), (m + 1) * sizeof(int));
    std::memcpy(colIdx_bo.map(), csr.colIdx.data(), nnz * sizeof(int));
    std::memcpy(vals_bo.map(), csr.values.data(), nnz * sizeof(data_t));
    std::memcpy(x_bo.map(), x.data(), n * sizeof(data_t));

    log(LOG_INFO) << " Syncing Buffers .... \n";

    rowPtr_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    colIdx_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    vals_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    x_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    log(LOG_INFO) << " Creating Operation .... \n";
    
    xrt::run run(kernel);

    auto op = [&]() 
    {
        run.set_arg(0, rowPtr_bo);
        run.set_arg(1, colIdx_bo);
        run.set_arg(2, vals_bo);
        run.set_arg(3, x_bo);
        run.set_arg(4, y_bo);
        run.set_arg(5, m);
        run.set_arg(6, nnz);
        run.start();
        run.wait();
    };

    log(LOG_INFO) << " Running Timing ... \n";
    auto diff = time_op<>(op);

    log(LOG_INFO) << " Syncing Output ... \n";
    y_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    log(LOG_INFO) << " Reading Device Buffer ... \n";
    std::memcpy(hw_out.data(), y_bo.map(), n * sizeof(data_t));
    
    return diff;
}

bool validate(const std::vector<data_t> &ref, const std::vector<data_t> &test) {
    LOG_INIT_CERR();
    const float eps = 1e-3;
    for (size_t i = 0; i < ref.size(); ++i) {
        // log(LOG_INFO) << "i: " << i << " Ref: " << ref[i] << " test: " << test[i] << "\n";  

        if (std::abs(ref[i] - test[i]) > eps) {
            std::cerr << "Mismatch at index " << i << ": " << ref[i] << " vs " << test[i] << "\n";
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    LOG_INIT_CERR();
    log.set_log_level(LOG_DEBUG);

    if (argc != 2) {
        std::cerr << "Usage: ./xrt_spmv <xclbin>\n";
        return EXIT_FAILURE;
    }

    try {
        std::string xclbin_file = argv[1];
        xrt::device device(0);
        auto uuid = device.load_xclbin(xclbin_file);
        auto kernel = xrt::kernel(device, uuid, "spmv");

        MatrixCSR csr;
        std::vector<data_t> x, ref_out, hw_out;

        log(LOG_INFO) << "Preparing Test Matrix ... \n";
        prepareData(csr, x, ref_out);

        log(LOG_INFO) << "Starting Kernel Time Test ... \n";
        auto diff_kernel = time_kernel_csr(device, kernel, csr, x, hw_out);
        log(LOG_INFO) << "Finished Kernel Time Test: " << diff_kernel << " ms\n";

        log(LOG_INFO) << "Starting CPU Time Test ... \n";
        auto diff_cpu = time_cpu_csr(csr, x);
        log(LOG_INFO) << "Finished CPU Time Test: " << diff_cpu << " ms\n";

        bool success = validate(ref_out, hw_out);
        std::cout << (success ? "PASSED" : "FAILED") << std::endl;
        return success ? EXIT_SUCCESS : EXIT_FAILURE;

    } catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}



