#include <ap_int.h>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <sys/types.h>
#include "read_matrix.hpp"
#include <cstring>

template <typename T>
void s2mm(hls::stream<T>& in, T * out, int size) {
    #pragma HLS INTERFACE axis port=in depth=512
    #pragma HLS INTERFACE m_axi port=out depth=512

    for (int i = 0; i < size; i++) {
        #pragma HLS PIPELINE II = 1
        out[i] = in.read();
    }
}

template <typename T>
void mm2s(T * in, hls::stream<T>& out, int size) {
    #pragma HLS INTERFACE axis port=out depth=512
    #pragma HLS INTERFACE m_axi port=in depth=512

    for (int i = 0; i < size; i++) {
        #pragma HLS PIPELINE II = 1
        out.write(in[i]);
    }
}


ap_uint<32> float_to_bits(float val) {
    union {
        float f;
        int32_t u;
    } conv ;

    conv.f = val;
    return conv.u;
}

uintDDR_t packf_to_uintDDR(data_t vals[EL_PER_DDR]) {
    uintDDR_t out = 0;
    for (int i = 0; i < EL_PER_DDR; i++) {
        #pragma HLS UNROLL
        out.range((i + 1) * ELEMENT_WIDTH - 1, i * ELEMENT_WIDTH) = float_to_bits(vals[i]);
    }
    return out;
}

template<typename T> 
uintDDR_t pack_DDR(const T v[EL_PER_DDR]) {
    T local_arr[EL_PER_DDR];
    #pragma HLS ARRAY_PARTITION variable=local_arr complete

    for (int i = 0; i < EL_PER_DDR; i++) {
        #pragma HLS UNROLL
        local_arr[i] = v[i];
    }

    uintDDR_t result;
    
    std::memcpy(&result, local_arr, sizeof(uintDDR_t));
    return result;
}

template<typename T>
void ddr_decoder(uintDDR_t raw, T* out) {
    for (int i = 0; i < EL_PER_DDR; i++) {
        #pragma HLS UNROLL
        out[i] = raw.range((i + 1) * ELEMENT_WIDTH - 1, i * ELEMENT_WIDTH);
    }
}

template<typename T>
T * unpackDDR_pack(uintDDR_t val) {
    static T result[EL_PER_DDR];
#pragma HLS ARRAY_PARTITION variable=result complete

    for (int i = 0; i < EL_PER_DDR; ++i) {
#pragma HLS UNROLL
        ap_uint<ELEMENT_WIDTH> bits = val.range(ELEMENT_WIDTH * (i + 1) - 1, ELEMENT_WIDTH * i);

        union {
            uint32_t u;
            T f;
        } conv;

        conv.u = bits;
        result[i] = conv.f;
    }

    return result;
}

void read_matrix(ddr_stream_t & r_beg_s, ddr_stream_t & r_idx, int m)
{
    // #pragma HLS INLINE off

    #pragma HLS INTERFACE axis port=r_beg_s depth=512
    ddr_arr_internal_t<int> r_ptrs;
    int lens[EL_PER_DDR + 1];
    lens[0] = 0;

    ddr_arr_internal_t<int> r_idx_arr;

    #pragma HLS ARRAY_PARTITION variable=r_ptrs complete
    #pragma HLS ARRAY_PARTITION variable=lens complete 
    #pragma HLS ARRAY_PARTITION variable=r_idx_arr complete

    arr_ptr_t arr_offset = 0;
    int prev_rptr = 0;

    rows:
    for (int i = 0; i < (m + 1) / EL_PER_DDR; i++) 
    {
        #pragma HLS LOOP_TRIPCOUNT max=1000000 / EL_PER_DDR min=10000 / EL_PER_DDR

        ddr_decoder<>(r_beg_s.read(), r_ptrs);

        int current_row = i * EL_PER_DDR - 1;

        lens[0] = r_ptrs[0] - prev_rptr;

        len_calc: for (int j = 0; j < EL_PER_DDR; j++)
        {
            #pragma HLS UNROLL
            lens[j + 1] = r_ptrs[j + 1] - r_ptrs[j];
        }

        nnzs: for (int j = 0; j < EL_PER_DDR; j++)
        {
            
            // #pragma HLS UNROLL type=partial factor=4

            current_nnz: for (int k = 0; k < lens[j]; k++)
            {
                #pragma HLS PIPELINE II=1
                #pragma HLS LOOP_TRIPCOUNT max=1000000 min=0

                r_idx_arr[arr_offset++] = current_row;

                if (arr_offset == EL_PER_DDR)
                {
                    r_idx.write(pack_DDR<>(r_idx_arr));
                    arr_offset = 0;
                }
            }
            current_row++;
        }

        prev_rptr = r_ptrs[EL_PER_DDR - 1];
    }

    if (arr_offset > 0) {
        r_idx.write(pack_DDR(r_idx_arr));
    }

    lens[0] = 0;
    arr_offset = 0;
}

void csr_to_2d(
    uintDDR_t   * r_beg,  
    uintDDR_t   * c_idx,
    uintDDR_t   * c_val,
    data_t      * x,
    uintDDR_t   * r_idx, 
    uintDDR_t   * c_ids, 
    uintDDR_t   * vals,    
    int m,
    int nnz
) {

    #pragma HLS INTERFACE m_axi port=r_beg  bundle=gmem_r  depth=128
    #pragma HLS INTERFACE m_axi port=c_idx  bundle=gmem_c  depth=128
    #pragma HLS INTERFACE m_axi port=c_val  bundle=gmem_cv depth=128
    #pragma HLS INTERFACE m_axi port=x      bundle=gmem_x  depth=128

    #pragma HLS INTERFACE m_axi port=r_idx  bundle=gmem_ridx depth=128
    #pragma HLS INTERFACE m_axi port=c_ids  bundle=gmem_cidx depth=128
    #pragma HLS INTERFACE m_axi port=vals   bundle=gmem_vals depth=128

    #pragma HLS INTERFACE s_axilite port=m
    #pragma HLS INTERFACE s_axilite port=nnz
    #pragma HLS INTERFACE s_axilite port=return

    // #pragma HLS DATAFLOW
    hls::stream<uintDDR_t> r_beg_s;
    hls::stream<uintDDR_t> c_idx_s;
    hls::stream<uintDDR_t> c_val_s;
    hls::stream<uintDDR_t> r_idx_s;
    hls::stream<uintDDR_t> c_idx_out_s;
    hls::stream<uintDDR_t> c_val_out_s;

    #pragma HLS STREAM variable=r_beg_s depth=64
    #pragma HLS STREAM variable=c_idx_s depth=64
    #pragma HLS STREAM variable=c_val_s depth=64
    #pragma HLS STREAM variable=r_idx_s depth=64
    #pragma HLS STREAM variable=c_idx_out_s depth=64
    #pragma HLS STREAM variable=c_val_out_s depth=64

    mm2s(r_beg, r_beg_s, (m + 1) / EL_PER_DDR);
    mm2s(c_idx, c_idx_s, nnz / EL_PER_DDR);
    mm2s(c_val, c_val_s, nnz / EL_PER_DDR);

    read_matrix(r_beg_s, r_idx_s, m);

    s2mm(r_idx_s, r_idx, nnz / EL_PER_DDR);
    s2mm(c_idx_s, c_ids, nnz / EL_PER_DDR);
    s2mm(c_val_s, vals, nnz / EL_PER_DDR);
}

void coo_spmv_kernel(ddr_stream_t & r_idx, ddr_stream_t & col_idx, ddr_stream_t & val, data_t * x, ddr_stream_t & out, int nnz)
{
    ddr_arr_internal_t<int> r_idx_arr;
    ddr_arr_internal_t<int> col_idx_arr;
    ddr_arr_internal_t<data_t> val_arr;
    data_t out_arr[2*EL_PER_DDR]; // only need to store two rows

    #pragma HLS ARRAY_PARTITION variable=col_idx_arr complete
    #pragma HLS ARRAY_PARTITION variable=val_arr complete 
    #pragma HLS ARRAY_PARTITION variable=r_idx_arr complete

    int max_row = 0;
    bool ping = 0;
    int offset = 0;

    for (int i = 0; i < nnz; i+=EL_PER_DDR)
    {
        ddr_decoder<>(r_idx.read(), r_idx_arr);    
        ddr_decoder<>(col_idx.read(), col_idx_arr);
        ddr_decoder<data_t>(val.read(), val_arr);

        offset = r_idx_arr[EL_PER_DDR - 1] - max_row;
        max_row = r_idx_arr[EL_PER_DDR - 1]; // The maximum row to process
        
        // if its less than EL_PER_DDR continue, becuase we still have space  
        // if its larger then EL_PER_DDR what do we do ? 
        // We might have values which we have not written yet
        // We need to complete what we have, and shift 
        //

        // Loop to write / compute value
        for (int j = 0; j < EL_PER_DDR; j++)
        {
            out_arr[(j % EL_PER_DDR) + offset] += x[col_idx_arr[j]] * val_arr[j];
        }


    }

}
