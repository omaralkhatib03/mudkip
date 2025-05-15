#pragma once

#include "types.hpp"

// void spmv(int *r_beg, int *c_idx, data_t *c_val, int m, data_t * x, data_t * out);
void spmv(
    int* r_beg,
    int* c_idx,
    data_t* c_val,
    data_t* x,
    data_t* out,
    int m,
    int nnz
);
