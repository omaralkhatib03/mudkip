#include "spmv.hpp"
#include "types.hpp"

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

void spmv_kernel(
    hls::stream<int>& r_beg,
    hls::stream<int>& c_idx,
    hls::stream<float>& c_val,
    int m,
    float* x,
    hls::stream<float>& out
) {

#pragma HLS STREAM variable=r_beg depth=32
#pragma HLS STREAM variable=c_idx depth=32
#pragma HLS STREAM variable=c_val depth=32
#pragma HLS STREAM variable=out depth=32

#pragma HLS INLINE off

    int r_beg_i = r_beg.read();
    int r_beg_next;

    for (int i = 0; i < m; i++) {
#pragma HLS LOOP_TRIPCOUNT min=10000 max=200000 avg=100000
#pragma HLS PIPELINE II=1
        r_beg_next = r_beg.read();
        float acc = 0;
        for (int j = r_beg_i; j < r_beg_next; j++) {
#pragma HLS PIPELINE
            int col_idx = c_idx.read();
            float val = c_val.read();
            acc += val * x[col_idx];
        }
        out.write(acc);
        r_beg_i = r_beg_next;
    }
}

template <typename T>
void s2mm(hls::stream<T>& in, T * out, int size) {
#pragma HLS INTERFACE axis port=in depth=32
#pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem
// #pragma HLS INTERFACE s_axilite port=in bundle=control
#pragma HLS INTERFACE s_axilite port=out bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < size; i++) {
#pragma HLS PIPELINE
        out[i] = in.read();
    }
}

template <typename T>
void mm2s(T* in, hls::stream<T>& out, int size) {
#pragma HLS INTERFACE m_axi port=in offset=slave bundle=gmem depth=32
#pragma HLS INTERFACE axis port=out depth=32
#pragma HLS INTERFACE s_axilite port=in bundle=control
// #pragma HLS INTERFACE s_axilite port=out bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < size; i++) {
#pragma HLS PIPELINE
        out.write(in[i]);
    }
}

void spmv(
    int* r_beg, // 0
    int* c_idx, // 1
    data_t* c_val, // 2
    data_t* x, // 3
    data_t* out, // 4
    int m, // 5
    int nnz // 6
) {

#pragma HLS INTERFACE m_axi port=r_beg bundle=gmem_r    depth=128
#pragma HLS INTERFACE m_axi port=c_val bundle=gmem_cv   depth=128
#pragma HLS INTERFACE m_axi port=c_idx bundle=gmem_c    depth=128
#pragma HLS INTERFACE m_axi port=x     bundle=gmem_x    depth=128
#pragma HLS INTERFACE m_axi port=out   bundle=gmem_o    depth=128

#pragma HLS INTERFACE s_axilite port=m      bundle=control
#pragma HLS INTERFACE s_axilite port=nnz    bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    hls::stream<int> r_beg_s;
    hls::stream<int> c_idx_s;
    hls::stream<float> c_val_s;
    hls::stream<float> out_s;

#pragma HLS STREAM variable=r_beg_s depth=32
#pragma HLS STREAM variable=c_idx_s depth=32
#pragma HLS STREAM variable=c_val_s depth=32
#pragma HLS STREAM variable=out_s depth=32

#pragma HLS DATAFLOW

    mm2s(r_beg, r_beg_s, m + 1);
    mm2s(c_idx, c_idx_s, nnz);
    mm2s(c_val, c_val_s, nnz);
    spmv_kernel(r_beg_s, c_idx_s, c_val_s, m, x, out_s);
    s2mm(out_s, out, m);
}
