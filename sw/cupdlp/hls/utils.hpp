#pragma once

#include "types.hpp"

void makeRandomCsCMatrix(int **c_beg, int **r_idx, data_t **val, int m, int n, int nnz);
void printMatrix(int *beg, int *idx, data_t *val, int n);
void printVector(data_t * x, int n);
void csCToCsR(int *c_beg, int *r_idx, data_t *val, int m, int n, int **r_beg, int **c_idx, data_t **out, int nnz);
void spMvCsC(int *c_beg, int *r_idx, data_t *val, int n, data_t * x, data_t ** out);
void spMvCsR(int *r_beg, int *c_idx, data_t *c_val, int m, data_t * x, data_t * out);

void makeRandomRowPtrs(int *r_idx, int m, int nnz);
void makeRandomCidx(int *c_idx, int nnz, int n);
void makeRandomValues(data_t *vals, int nnz);
void makeRandomCsr(int *r_idx, int *c_idx, data_t *vals, int m, int n, int nnz);
