#include <adf.h>
#include "spmv_kernel.h"
#include <aie_api/aie.hpp>

// alignas(32) __attribute__((section(".data"))) data_t x_local[HW_SIDE_LEN];

// void load_x(input_stream<data_t>* x, int vector_length) {
//     for (int i = 0; i < vector_length / ELEMENETS_PER_CHUNK; i++)
//     chess_loop_range(1, SIDE_LEN_READS) {
//         aie_d_vec_t tmp = readincr_v<ELEMENETS_PER_CHUNK>(x);
//         for (int j = 0; j < ELEMENETS_PER_CHUNK; j++)
//         chess_unroll_loop(ELEMENETS_PER_CHUNK) {
//             x_local[i * ELEMENETS_PER_CHUNK + j] = tmp[j];
//         }
//     }
// }

void mult_products(input_stream<idx_t> * c_idx,
                   input_stream<data_t> * vals,
                   input_buffer<data_t> & x,
                   output_stream<data_t> * prod_out,
                   int vector_length)
{
    data_t * x_local = x.data();

    int total_chunks = std::ceil(((double) vector_length) / ELEMENETS_PER_CHUNK);
    printf("[mult_products] vector_length = %d, chunks = %d\n", vector_length, total_chunks);

    for (int i = 0; i < total_chunks; i++) {
        printf("[mult_products] reading chunk %d/%d\n", i, total_chunks);

        aie::vector<idx_t, ELEMENETS_PER_CHUNK> idxs = readincr_v<ELEMENETS_PER_CHUNK>(c_idx);
        aie::vector<data_t, ELEMENETS_PER_CHUNK> vals_vec = readincr_v<ELEMENETS_PER_CHUNK>(vals);
        aie::vector<data_t, ELEMENETS_PER_CHUNK> result;

        for (int j = 0; j < ELEMENETS_PER_CHUNK; j++) {
            result[j] = vals_vec[j] * x_local[idxs[j]];
            printf("[mult_products] i=%d, j=%d -> idx=%d, val=%.6f, x=%.6f, product=%.6f\n",
                   i, j, idxs[j].get(), vals_vec[j].get(), x_local[idxs[j]], result[j].get());
        }

        writeincr(prod_out, result);
    }

    printf("[mult_products] completed\n");
}

void row_accumulate(input_buffer<idx_t> & r_ptr,
                    input_stream<data_t> * products,
                    output_stream<data_t> * out_vals,
                    int vector_length)
{
    const idx_t* row_ptr = r_ptr.data();

    printf("[row_accumulate] vector_length = %d\n", vector_length);

    for (int i = 0; i < vector_length; i++) {
        idx_t start = row_ptr[i];
        idx_t end   = row_ptr[i + 1];

        printf("[row_accumulate] row %d: start = %d, end = %d\n", i, start, end);

        data_t sum = 0;

        for (int j = start; j < end; j++) {
            data_t val = readincr(products);
            sum += val;
            printf("[row_accumulate] row=%d, idx=%d -> val=%.6f, sum=%.6f\n", i, j, val, sum);
        }

        writeincr(out_vals, sum);
        printf("[row_accumulate] row %d result = %.6f\n", i, sum);
    }

    printf("[row_accumulate] completed\n");
}

// void load_x(input_stream<data_t> * x, int vector_length)
// {
//     for (int i = 0; i < vector_length; i++)
//     chess_loop_range(SIDE_LEN_READS, SIDE_LEN_READS)
//     {
//         aie_d_vec_t tmp = readincr_v4( x);  
//         for (int j = 0; j < ELEMENETS_PER_CHUNK; j++)
//         chess_unroll_loop(ELEMENETS_PER_CHUNK)
//         {
//             x_local[i*ELEMENETS_PER_CHUNK + j] = tmp[j];
//         }
//     }
// }

// void mult_products( input_window<idx_t> *  c_idx, 
//                     input_window<data_t> *  vals,
//                     input_stream<data_t> * x, 
//                     output_window<data_t> *  prod_out,
//                     int vector_length
//                     )
// {
    
//     load_x(x, vector_length);

//     int32 delay_by_1_ptr = 0;

//     for (int i = 0; i < vector_length; i++) // Loop through the number of rows
//     chess_prepare_for_pipelining
//     chess_loop_range(1, )
//     {
//         aie_d_vec_t matrix_vals = window_readincr_v4( vals);
//         aie_idx_vec_t c_idxs      = window_readincr_v4( c_idx);
//         aie_d_vec_t products{};

//         for (int j = 0; j < ELEMENETS_PER_CHUNK; j++)
//         chess_unroll_loop(ELEMENETS_PER_CHUNK)
//         {
//             products[j] = matrix_vals[j] * x_local[c_idxs[j]];
//         }

//         window_writeincr(prod_out, products);
//     }
// }

// void row_accumulate(input_window<idx_t>* r_ptr, 
//                     input_window<data_t>* products, 
//                     output_window<data_t>* out_vals,
//                     int vector_length
//                     )
// {

//     idx_t row_start = window_readincr(r_ptr);

//     for (int i = 0; i < vector_length; i++)
//     chess_prepare_for_pipelining
//     chess_loop_range(1,)
//     {
//         idx_t row_end = window_readincr(r_ptr);
//         data_t sum = 0;

//         for (int j = row_start; j < row_end; j++)
//         {
//             data_t val = window_readincr(products);
//             sum += val;
//         }

//         row_start = row_end;
//         window_writeincr(out_vals, sum); 
//     }
// }
