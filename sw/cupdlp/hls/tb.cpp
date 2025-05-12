#include <iostream>
#include <ostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.hpp"
#include "spmv.hpp"

int main(int argc, char *argv[])
{
    data_t m = 4;
    data_t n = 8;
    data_t nnz = 12;
    int ll = 1;
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

    data_t * x = (data_t * ) malloc(sizeof(data_t) * n);

    for (int i = 0; i < n; i++)
    {
        x[i] = rand()+ 1;
    }

    /*printf("Input Vector (x): \n");*/
    /*printVector(x, n);*/
    /*printf("\n");*/

    data_t * out = (data_t * ) malloc(sizeof(data_t) * m);
    memset(out, 0, sizeof(data_t) * m);

    data_t * hw_out = (data_t * ) malloc(sizeof(data_t) * m);
    memset(hw_out, 0, sizeof(data_t) * m);

    makeRandomCsCMatrix(&c_beg, &r_idx, &r_val, m, n, nnz);

    csCToCsR(c_beg, r_idx, r_val, m, n, &r_beg, &c_idx, &c_val, nnz);

    printf("Matrix A:\n");
    printMatrix(r_beg, c_idx, c_val, m);
    printf("\n");

    spMvCsR(r_beg, c_idx, c_val, m, x, out);
    spmv(r_beg, c_idx, c_val, m, x, hw_out);

    printf("Result (SW): \n");
    printVector(out, m);

    printf("Result (HW): \n");
    printVector(hw_out, m);
       
    bool passed = 1;

    for (int i = 0; i < m; i++)
    {
        passed &= out[i] == hw_out[i];    
    }
    
    
    if (passed)
        std::cout << "Passed !" << std::endl;
    else
        std::cerr << "Failed !" << std::endl;

    free(r_val);
    free(c_beg);
    free(c_idx);
    free(c_val);
    free(r_beg);
    free(out);
    free(hw_out);
}
