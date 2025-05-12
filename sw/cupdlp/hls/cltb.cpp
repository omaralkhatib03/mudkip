#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "utils.hpp"

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1

#include <CL/opencl.hpp>

using data_t = float;

// Parameters
const int m = 4;
const int n = 8;
const int nnz = 12;

struct MatrixCSR {
    std::vector<int> rowPtr;
    std::vector<int> colIdx;
    std::vector<data_t> values;
};

cl::Program loadXclbin(const std::string &binaryFile, cl::Context &context, cl::Device &device) {
    std::ifstream bin(binaryFile, std::ios::binary);
    bin.seekg(0, std::ios::end);
    size_t size = bin.tellg();
    bin.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    bin.read(buffer.data(), size);
    cl::Program::Binaries binaries{{buffer.data(), size}};
    cl::Program program(context, {device}, binaries);
    return program;
}

void initOpenCL(cl::Context &context, cl::Device &device, cl::CommandQueue &queue) {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (auto &platform : platforms) {
        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
        if (!devices.empty()) {
            device = devices.front();
            context = cl::Context(device);
            queue = cl::CommandQueue(context, device);
            return;
        }
    }
    throw std::runtime_error("No Xilinx OpenCL devices found.");
}

// Generate test input data
void prepareData(MatrixCSR &csr, std::vector<data_t> &x, std::vector<data_t> &ref_out) {
    int *c_beg, *r_idx;
    data_t *val;
    makeRandomCsCMatrix(&c_beg, &r_idx, &val, m, n, nnz);

    int *r_beg, *c_idx;
    data_t *csr_val;
    csCToCsR(c_beg, r_idx, val, m, n, &r_beg, &c_idx, &csr_val, nnz);

    csr.rowPtr.assign(r_beg, r_beg + m + 1);
    csr.colIdx.assign(c_idx, c_idx + nnz);
    csr.values.assign(csr_val, csr_val + nnz);

    x.resize(n);
    for (int i = 0; i < n; ++i)
        x[i] = rand() % 10 + 1;

    ref_out.resize(m, 0);
    spMvCsR(csr.rowPtr.data(), csr.colIdx.data(), csr.values.data(), m, x.data(), ref_out.data());

    free(c_beg); free(r_idx); free(val);
    free(r_beg); free(c_idx); free(csr_val);
}

void runKernel(const cl::Context &context, const cl::CommandQueue &queue, cl::Kernel &kernel,
               const MatrixCSR &csr, const std::vector<data_t> &x, std::vector<data_t> &hw_out) {
    hw_out.resize(m);

    cl::Buffer buf_r_ptr(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * (m + 1), (void*)csr.rowPtr.data());
    cl::Buffer buf_c_idx(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * nnz, (void*)csr.colIdx.data());
    cl::Buffer buf_vals(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(data_t) * nnz, (void*)csr.values.data());
    cl::Buffer buf_x(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(data_t) * n, (void*)x.data());
    cl::Buffer buf_y(context, CL_MEM_WRITE_ONLY, sizeof(data_t) * m);

    kernel.setArg(0, buf_r_ptr);
    kernel.setArg(1, buf_c_idx);
    kernel.setArg(2, buf_vals);
    kernel.setArg(3, m);  // rows
    kernel.setArg(4, buf_x);
    kernel.setArg(5, buf_y);

    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(buf_y, CL_TRUE, 0, sizeof(data_t) * m, hw_out.data());
}

bool validate(const std::vector<data_t> &ref, const std::vector<data_t> &test) {
    const float eps = 1e-3;
    for (size_t i = 0; i < ref.size(); ++i) {
        if (std::abs(ref[i] - test[i]) > eps) {
            std::cerr << "Mismatch at index " << i << ": " << ref[i] << " vs " << test[i] << "\n";
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./cltb <xclbin>\n";
        return EXIT_FAILURE;
    }

    try {
        cl::Context context;
        cl::Device device;
        cl::CommandQueue queue;
        initOpenCL(context, device, queue);

        cl::Program program = loadXclbin(argv[1], context, device);
        cl::Kernel kernel(program, "spmv");

        MatrixCSR csr;
        std::vector<data_t> x, ref_out, hw_out;
        prepareData(csr, x, ref_out);
        runKernel(context, queue, kernel, csr, x, hw_out);

        bool success = validate(ref_out, hw_out);
        std::cout << (success ? "PASSED" : "FAILED") << std::endl;
        return success ? EXIT_SUCCESS : EXIT_FAILURE;

    } catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

