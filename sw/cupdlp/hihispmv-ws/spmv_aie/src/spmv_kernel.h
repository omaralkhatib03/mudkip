#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

using namespace adf;

using idx_t = int32;
using data_t = float;

template<typename T> 
using ibuffer_t = input_buffer<T>;


template<typename T>
using obuffer_t = output_buffer<T>;
#define CHUNK_LENGTH 128
#define HW_SIDE_LEN 128
#define ELEMENETS_PER_CHUNK (CHUNK_LENGTH / (sizeof(data_t)*8))
#define SIDE_LEN_READS (HW_SIDE_LEN / CHUNK_LENGTH)

using aie_d_vec_t = aie::vector<data_t, ELEMENETS_PER_CHUNK>;
using aie_idx_vec_t = aie::vector<idx_t, ELEMENETS_PER_CHUNK>;

struct val_row_pair_t {
    idx_t row;
    data_t val;

    val_row_pair_t(idx_t r, data_t v)
        : row(r), val(v)
    {}

    val_row_pair_t()
        : row(-1), val(0)
    {}

};

void load_x(input_stream<data_t>* x, int vector_length);

void row_accumulate(input_buffer<idx_t> & r_ptr,
                    input_stream<data_t> * products,
                    output_stream<data_t> * out_vals,
                    int vector_length);


void mult_products(input_stream<idx_t> * c_idx,
                   input_stream<data_t> * vals,
                   input_buffer<data_t> & x, // read HW_SIDE_LEN
                   output_stream<data_t> * prod_out,
                   int vector_length);

// void mult_products(input_window<idx_t> * c_idx, 
//                    input_window<data_t> * vals,
//                    input_stream<data_t> * x, 
//                    output_window<data_t> * prod_out,
//                    int vector_length
//                    );
//
// void row_accumulate(input_window<idx_t> * r_ptr, 
//                     input_window<data_t> * products, 
//                     output_window<data_t> * out_vals,
//                     int vector_length
//                     );
//

// void mult_products(input_window<idx_t> * c_idx, 
//                    input_window<data_t> * vals,
//                    input_stream<data_t> * x, 
//                    output_window<data_t> * prod_out,
//                    input_stream<int> * vector_length
//                    );
//
// void row_accumulate(input_window<idx_t> * r_ptr, 
//                     input_window<data_t> * products, 
//                     output_window<data_t> * out_vals,
//                     input_stream<int> * vector_length
//                     );
