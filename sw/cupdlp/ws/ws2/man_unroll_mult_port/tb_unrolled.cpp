#include <iostream>
#include <ostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.hpp"
#include "utils.hpp"
#include "spmv.hpp"
#include "BSLogger.hpp"

#define WORD_SIZE 16

void pack_int_array_to_uint512(const int* in, uint512_t* out, int len) {
    int num_words = (len + WORD_SIZE - 1) / WORD_SIZE;
    for (int i = 0; i < num_words; i++) {
        uint512_t word = 0;
        for (int j = 0; j < WORD_SIZE; j++) {
            int idx = i * WORD_SIZE + j;
            if (idx < len) {
                word.range(32 * j + 31, 32 * j) = in[idx];
            }
        }
        out[i] = word;
    }
}

void pack_float_array_to_uint512(const data_t* in, uint512_t* out, int len) {
    int num_words = (len + WORD_SIZE - 1) / WORD_SIZE;
    for (int i = 0; i < num_words; i++) {
        uint512_t word = 0;
        for (int j = 0; j < WORD_SIZE; j++) {
            int idx = i * WORD_SIZE + j;
            if (idx < len) {
                union { float f; uint32_t u; } conv;
                conv.f = in[idx];
                word.range(32 * j + 31, 32 * j) = conv.u;
            }
        }
        out[i] = word;
    }
}

void spmv_par(
    int* r_beg,
    int* c_idx,
    data_t* c_val,
    data_t* x,
    data_t* out,
    int m,
    int nnz
) {
    data_t x_loc[MAX_VECTOR];

    for (int i = 0; i < MAX_VECTOR; i++)
    {
        x_loc[i] = 1;
    }

    spmv_naive_pipe(r_beg, c_idx, c_val, x_loc, out, m, nnz);
}

int main(int argc, char *argv[])
{
    LOG_INIT_CERR();
    log.set_log_level(LOG_DEBUG);

    data_t m    = 32;
    data_t n    = 32;
    data_t nnz  = 32;
    int ll      = 1;

    data_t sparsity = nnz / (m * n);
    //   data_t sparsity = 0.0000000627;
    nnz = (unsigned) (sparsity * m * n);
    clock_t c0;
    clock_t c1;
    data_t runtime_diff_ms;

    printf("m x n: %i x %i\n",(int) m, (int) n);

    printf("sparsity: %e\n", sparsity);

    int * c_beg = NULL;
    int * r_idx = NULL;
    data_t * r_val = NULL;

    int * r_beg = NULL;
    int * c_idx = NULL;
    data_t * c_val = NULL;

    printf("nnz: %e\n", nnz);
    
    log(LOG_DEBUG) << "Mallocing Input Vector \n";
    data_t * x = (data_t * ) malloc(sizeof(data_t) * n);

    for (int i = 0; i < n; i++)
    {
        x[i] = 1;
    }

    printf("Input Vector (x): \n");
    printVector(x, n);
    printf("\n");

    log(LOG_DEBUG) << "Mallocing Output Vector ... \n";
    data_t * out = (data_t * ) malloc(sizeof(data_t) * m);
    memset(out, 0, sizeof(data_t) * m);

    log(LOG_DEBUG) << "Mallocing HW Output Vector ... \n";
    data_t * hw_out = (data_t * ) malloc(sizeof(data_t) * MAX_VECTOR);
    memset(hw_out, 0, sizeof(data_t) * MAX_VECTOR);

    log(LOG_DEBUG) << "Building Random CSC Matrix ... \n";
    makeRandomCsCMatrix(&c_beg, &r_idx, &r_val, m, n, nnz);

    log(LOG_DEBUG) << "Converting to CSR ... \n";
    csCToCsR(c_beg, r_idx, r_val, m, n, &r_beg, &c_idx, &c_val, nnz);

    printf("Matrix A:\n");
    printMatrix(r_beg, c_idx, c_val, m);
    printf("\n");

    log(LOG_DEBUG) << "Computing SW ... \n";
    spMvCsR(r_beg, c_idx, c_val, m, x, out);

    log(LOG_DEBUG) << "Computing HW ... \n";
    spmv_par(r_beg, c_idx, c_val, x, hw_out, m, nnz);
    spmv_par(r_beg, c_idx, c_val, x, hw_out, m, nnz);

    // printf("Result (SW): \n");
    // printVector(out, m);

    // printf("Result (HW): \n");
    // printVector(hw_out, m);
       
    bool passed = 1;
    double eps = 10e-5;

    for (int i = 0; i < m; i++)
    {
        passed &= (out[i] - hw_out[i]) < eps;
        if (!passed)
        {
            std::cout << "i: " << i << " ref: " << out[i] << " hw: " << hw_out[i] << std::endl;
            // break;
        }
    }
    
    if (passed)
        std::cout << "Passed !" << std::endl;
    else
        std::cerr << "Failed !" << std::endl;

    spmv_par(r_beg, c_idx, c_val, x, hw_out, m, nnz);

    for (int i = 0; i < m; i++)
    {
        passed &= (out[i] - hw_out[i]) < eps;
        if (!passed)
        {
            std::cout << "i: " << i << " ref: " << out[i] << " hw: " << hw_out[i] << std::endl;
            // break;
        }
    }

    if (passed)
        std::cout << "Passed !" << std::endl;
    else
        std::cerr << "Failed !" << std::endl;

    spmv_par(r_beg, c_idx, c_val, x, hw_out, m, nnz);

    free(r_val);
    free(c_beg);
    free(c_idx);
    free(c_val);
    free(r_beg);
    free(out);
    free(x);
    free(hw_out);
}
