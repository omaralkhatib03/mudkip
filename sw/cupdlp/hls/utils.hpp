#pragma once

#include "types.hpp"

void makeRandomCsCMatrix(int **c_beg, int **r_idx, data_t **val, int m, int n, int nnz);
void printMatrix(int *beg, int *idx, data_t *val, int n);
void printVector(data_t * x, int n);
void csCToCsR(int *c_beg, int *r_idx, data_t *val, int m, int n, int **r_beg, int **c_idx, data_t **out, int nnz);
void spMvCsC(int *c_beg, int *r_idx, data_t *val, int n, data_t * x, data_t ** out);
void spMvCsR(int *r_beg, int *c_idx, data_t *c_val, int m, data_t * x, data_t * out);


