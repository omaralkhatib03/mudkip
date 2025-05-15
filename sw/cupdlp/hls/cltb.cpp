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
static constexpr int m = 100;
static constexpr int n = 100;
static constexpr int nnz = 320;
static constexpr int run = 10;

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
    for (int i = 0; i < run; i++) {
        aFunction();
    }
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
    
    xrt::run run;

    auto op = [&]() {
        run = kernel(rowPtr_bo, colIdx_bo, vals_bo, x_bo, y_bo, m, nnz);
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
