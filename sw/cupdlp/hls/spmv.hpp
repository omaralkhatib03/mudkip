#pragma once

#include "types.hpp"
#include <ap_int.h>

#define NUM_KERNELS 8

typedef ap_uint<512> uint512_t;
typedef ap_uint<1024> uint1024_t;

void spmv(
    int* r_beg, // 0
    int* c_idx, // 1
    data_t* c_val, // 2
    data_t* x, // 3
    data_t* out, // 4
    int m, // 5
    int nnz //  6
);
  
void spmv_man_unroll(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
);

void spmv_naive(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
);

void spmv_naive_pipe(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
);

void spmv_stream_manu_loc(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
);

void spmv_steam_local_unrolled(
    int* r_beg, // 0
    int* c_idx, // 1
    data_t* c_val, // 2
    data_t* x, // 3
    data_t* out, // 4
    int m, // 5
    int nnz // 6
);

void spmv_jin_mori(
    int* r_beg, // 0
    int* c_idx, // 1
    data_t* c_val, // 2
    data_t* x, // 3
    data_t* out, // 4
    int m, // 5
    int nnz // 6
);
