#include "spmv.hpp"
#include "types.hpp"

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

void load_x(data_t* x, data_t * x_local) {
    for (int i = 0; i < MAX_VECTOR; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=MIN_ROWS max=MAX_ROWS
        // #pragma HLS PIPELINE II=1
        // #pragma HLS PIPELINE II=2
        // #pragma HLS UNROLL factor=LD_X_FACTOR
        x_local[i] = x[i];
    }
}

void spmv_kernel(
    int* r_beg,
    hls::stream<int>& c_idx,
    hls::stream<float>& c_val,
    int m,
    float* x,
    hls::stream<float>& out
) {

    // #pragma HLS STREAM variable=r_beg depth=1024
    // #pragma HLS STREAM variable=c_idx depth=1024
    // #pragma HLS STREAM variable=c_val depth=1024
    // #pragma HLS STREAM variable=out depth=1024

    for (int i = 0; i < m; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=100 max=200000 avg=100000
        #pragma HLS PIPELINE II=1

        float acc = 0;

        for (int j = r_beg[i]; j < r_beg[i+1]; j++) {
            #pragma HLS PIPELINE
            int col_idx = c_idx.read();
            float val = c_val.read();
            acc += val * x[col_idx];
        }

        out.write(acc);

    }
}

template <typename T>
void s2mm(hls::stream<T>& in, T * out, int size) {
    #pragma HLS INTERFACE axis port=in depth=1024
    #pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem
    // #pragma HLS INTERFACE s_axilite port=in bundle=control
    #pragma HLS INTERFACE s_axilite port=out bundle=control
    #pragma HLS INTERFACE s_axilite port=size bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < size; i++) {
        #pragma HLS PIPELINE
        #pragma HLS LOOP_TRIPCOUNT min=0 max=MAX_VECTOR
        out[i] = in.read();
    }
}

template <typename T>
void mm2s(T* in, hls::stream<T>& out, int size) {
    #pragma HLS INTERFACE m_axi port=in offset=slave bundle=gmem depth=1024
    #pragma HLS INTERFACE axis port=out depth=1024
    #pragma HLS INTERFACE s_axilite port=in bundle=control
    // #pragma HLS INTERFACE s_axilite port=out bundle=control
    #pragma HLS INTERFACE s_axilite port=size bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < size; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=0 max=MAX_VECTOR
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

    #pragma HLS INTERFACE m_axi port=r_beg bundle=gmem_r    depth=1024
    #pragma HLS INTERFACE m_axi port=c_val bundle=gmem_cv   depth=1024
    #pragma HLS INTERFACE m_axi port=c_idx bundle=gmem_c    depth=1024
    #pragma HLS INTERFACE m_axi port=x     bundle=gmem_x    depth=1024
    #pragma HLS INTERFACE m_axi port=out   bundle=gmem_o    depth=1024

    #pragma HLS INTERFACE s_axilite port=m      bundle=control
    #pragma HLS INTERFACE s_axilite port=nnz    bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // hls::stream<int> r_beg_s;
    hls::stream<int> c_idx_s;
    hls::stream<float> c_val_s;
    hls::stream<float> out_s;

    // #pragma HLS STREAM variable=r_beg_s depth=1024
    #pragma HLS STREAM variable=c_idx_s depth=1024
    #pragma HLS STREAM variable=c_val_s depth=1024
    #pragma HLS STREAM variable=out_s depth=1024

    #pragma HLS DATAFLOW

    // mm2s(r_beg, r_beg_s, m + 1);
    mm2s(c_idx, c_idx_s, nnz);
    mm2s(c_val, c_val_s, nnz);
    spmv_kernel(r_beg, c_idx_s, c_val_s, m, x, out_s);
    s2mm(out_s, out, m);
}


void spmv_man_unroll(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
) {
    #pragma HLS INTERFACE m_axi port=r_beg  offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=c_idx  offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=c_val  offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=x      offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=out    offset=slave depth=1024

    // data_t x_local[MAX_VECTOR];
    // #pragma HLS BIND_STORAGE variable=x_local type=RAM_1WNR impl=URAM
    // #pragma HLS ARRAY_PARTITION variable=x_local cyclic factor=BIND_FACTOR dim=1
    // #pragma HLS ARRAY_PARTITION variable=x_local type=complete factor=BIND_FACTOR

    // load_x(x, x_local, MAX_VECTOR);

rows: for (int i = 0; i < m; i++)
    {
        #pragma HLS loop_tripcount min=MIN_ROWS max=MAX_ROWS
        data_t y = 0.0;

    nnz: for (int j = r_beg[i]; j < r_beg[i + 1]; j+=S)
        {

            #pragma HLS loop_tripcount min=MIN_NNZ max=MAX_NNZ
            #pragma HLS pipeline II=S

            data_t yt = c_val[j] * x[c_idx[j]];

        man_u: for (int k = 1; k < S; k++)
            {
                if (k + j < r_beg[i + 1])
                {
                    yt += c_val[k + j] * x[c_idx[k+j]];
                }
            }

            y += yt;

        }
        out[i] = y;

    }
}

void spmv_man_unroll_kernel(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x0,     // 3
    data_t* x1,     // 3
    data_t* x2,     // 3
    data_t* x3,     // 3
    data_t* x4,     // 3
    data_t* x5,     // 3
    data_t* x6,     // 3
    data_t* x7,     // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
) {

rows: for (int i = 0; i < m; i++) {
        #pragma HLS loop_tripcount min=MIN_ROWS max=MAX_ROWS
        data_t y = 0.0;

    nnz: for (int j = r_beg[i]; j < r_beg[i + 1]; j += S) {
            #pragma HLS loop_tripcount min=MIN_NNZ max=MAX_NNZ
            #pragma HLS pipeline II=S

            data_t yt = 0.0;

        man_u: for (int k = 0; k < S; k++) {
                #pragma HLS unroll
                int idx = j + k;
                if (idx < r_beg[i + 1]) {
                    int col = c_idx[idx];
                    data_t x_val;

                    // Select memory port based on column index modulo 8
                    switch (col % 8) {
                        case 0: x_val = x0[col]; break;
                        case 1: x_val = x1[col]; break;
                        case 2: x_val = x2[col]; break;
                        case 3: x_val = x3[col]; break;
                        case 4: x_val = x4[col]; break;
                        case 5: x_val = x5[col]; break;
                        case 6: x_val = x6[col]; break;
                        case 7: x_val = x7[col]; break;
                    }

                    yt += c_val[idx] * x_val;
                }
            }

            y += yt;
        }

        out[i] = y;
    }

}

void spmv_naive(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
) {
    #pragma HLS INTERFACE m_axi port=r_beg bundle=gmem_r    depth=1024
    #pragma HLS INTERFACE m_axi port=c_val bundle=gmem_cv   depth=1024
    #pragma HLS INTERFACE m_axi port=c_idx bundle=gmem_c    depth=1024
    #pragma HLS INTERFACE m_axi port=x     bundle=gmem_x    depth=1024
    #pragma HLS INTERFACE m_axi port=out   bundle=gmem_o    depth=1024

    #pragma HLS INTERFACE s_axilite port=m bundle=control
    #pragma HLS INTERFACE s_axilite port=nnz bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control


    // data_t x_local[MAX_VECTOR];
    // #pragma HLS BIND_STORAGE variable=x_local type=RAM_1WNR impl=URAM
    // #pragma HLS ARRAY_PARTITION variable=x_local type=complete factor=BIND_FACTOR
    //
    // load_x(x, x_local, MAX_VECTOR);

rows: for (int i = 0; i < m; i++)
    {
        #pragma HLS loop_tripcount min=MIN_ROWS max=MAX_ROWS
        data_t y = 0.0;
    nnz: for (int j = r_beg[i]; j < r_beg[i + 1]; j++)
        {
            #pragma HLS loop_tripcount min=MIN_NNZ max=MAX_NNZ
            #pragma HLS PIPELINE off
            y += c_val[j] * x[c_idx[j]];
        }
        out[i] = y;
    }
}


void spmv_stream_manu_loc(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
) {
    #pragma HLS INTERFACE m_axi port=r_beg  offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=c_idx  offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=c_val  offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=x      offset=slave depth=1024
    #pragma HLS INTERFACE m_axi port=out    offset=slave depth=1024

    data_t x_local[MAX_VECTOR];
    #pragma HLS BIND_STORAGE variable=x_local type=RAM_1WNR impl=URAM
    #pragma HLS ARRAY_RESHAPE variable=x_local cyclic factor=S

    load_x(x, x_local);

rows: for (int i = 0; i < m; i++)
    {
        #pragma HLS loop_tripcount min=MIN_ROWS max=MAX_ROWS
        data_t y = 0.0;

    nnz: for (int j = r_beg[i]; j < r_beg[i + 1]; j+=S)
        {

            #pragma HLS loop_tripcount min=MIN_NNZ max=MAX_NNZ
            #pragma HLS pipeline II=S

            data_t yt = c_val[j] * x_local[c_idx[j]];

        man_u: for (int k = 1; k < S; k++)
            {
                if (k + j < r_beg[i + 1])
                {
                    yt += c_val[k + j] * x_local[c_idx[k+j]];
                }
            }

            y += yt;

        }
        out[i] = y;

    }
}

void spmv_kernel_unrolled(
    hls::stream<int>& r_beg,
    hls::stream<int>& c_idx,
    hls::stream<float>& c_val,
    int m,
    float* x,
    hls::stream<float>& out
) {

    #pragma HLS STREAM variable=r_beg depth=1024
    #pragma HLS STREAM variable=c_idx depth=1024
    #pragma HLS STREAM variable=c_val depth=1024
    #pragma HLS STREAM variable=out depth=1024

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
            #pragma HLS UNROLL factor=8
            int col_idx = c_idx.read();
            float val = c_val.read();
            acc += val * x[col_idx];
        }
        out.write(acc);
        r_beg_i = r_beg_next;
    }
}

void spmv_steam_local_unrolled(
    int* r_beg, // 0
    int* c_idx, // 1
    data_t* c_val, // 2
    data_t* x, // 3
    data_t* out, // 4
    int m, // 5
    int nnz // 6
) {

    #pragma HLS INTERFACE m_axi port=r_beg bundle=gmem_r    depth=1024
    #pragma HLS INTERFACE m_axi port=c_val bundle=gmem_cv   depth=1024
    #pragma HLS INTERFACE m_axi port=c_idx bundle=gmem_c    depth=1024
    #pragma HLS INTERFACE m_axi port=x     bundle=gmem_x    depth=1024
    #pragma HLS INTERFACE m_axi port=out   bundle=gmem_o    depth=1024

    #pragma HLS INTERFACE s_axilite port=m      bundle=control
    #pragma HLS INTERFACE s_axilite port=nnz    bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    data_t x_local[MAX_VECTOR];
    #pragma HLS BIND_STORAGE variable=x_local type=RAM_1WNR impl=URAM
    #pragma HLS ARRAY_RESHAPE variable=x_local type=cyclic factor=16

    load_x(x, x_local);

    hls::stream<int> r_beg_s;
    hls::stream<int> c_idx_s;
    hls::stream<float> c_val_s;
    hls::stream<float> out_s;

    #pragma HLS STREAM variable=r_beg_s depth=1024
    #pragma HLS STREAM variable=c_idx_s depth=1024
    #pragma HLS STREAM variable=c_val_s depth=1024
    #pragma HLS STREAM variable=out_s depth=1024

    #pragma HLS DATAFLOW

    mm2s(r_beg, r_beg_s, m + 1);
    mm2s(c_idx, c_idx_s, nnz);
    mm2s(c_val, c_val_s, nnz);
    spmv_kernel_unrolled(r_beg_s, c_idx_s, c_val_s, m, x_local, out_s);
    s2mm(out_s, out, m);
}


void spmv_jin_mori(
    int* r_beg, // 0
    int* c_idx, // 1
    data_t* c_val, // 2
    data_t* x, // 3
    data_t* out, // 4
    int m, // 5
    int nnz // 6
) {

    #pragma HLS INTERFACE m_axi port=r_beg bundle=gmem_r    depth=1024
    #pragma HLS INTERFACE m_axi port=c_val bundle=gmem_cv   depth=1024
    #pragma HLS INTERFACE m_axi port=c_idx bundle=gmem_c    depth=1024
    #pragma HLS INTERFACE m_axi port=x     bundle=gmem_x    depth=1024
    #pragma HLS INTERFACE m_axi port=out   bundle=gmem_o    depth=1024

    #pragma HLS INTERFACE s_axilite port=m      bundle=control
    #pragma HLS INTERFACE s_axilite port=nnz    bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    data_t x_local[MAX_VECTOR];
    #pragma HLS BIND_STORAGE variable=x_local type=RAM_1WNR impl=URAM
    #pragma HLS ARRAY_RESHAPE variable=x_local type=cyclic factor=16

    load_x(x, x_local);

    // hls::stream<int> r_beg_s;
    hls::stream<int> c_idx_s;
    hls::stream<float> c_val_s;
    hls::stream<float> out_s;

    // #pragma HLS STREAM variable=r_beg_s depth=1024
    #pragma HLS STREAM variable=c_idx_s depth=1024
    #pragma HLS STREAM variable=c_val_s depth=1024
    #pragma HLS STREAM variable=out_s depth=1024

    #pragma HLS DATAFLOW

    // mm2s(r_beg, r_beg_s, m + 1);
    mm2s(c_idx, c_idx_s, nnz);
    mm2s(c_val, c_val_s, nnz);
    spmv_kernel(r_beg, c_idx_s, c_val_s, m, x_local, out_s);
    s2mm(out_s, out, m);
}


void spmv_naive_pipe(
    int* r_beg,     // 0
    int* c_idx,     // 1
    data_t* c_val,  // 2
    data_t* x,      // 3
    data_t* out,    // 4
    int m,          // 5
    int nnz         // 6
) {
    #pragma HLS INTERFACE m_axi port=r_beg bundle=gmem_r    depth=1024
    #pragma HLS INTERFACE m_axi port=c_val bundle=gmem_cv   depth=1024
    #pragma HLS INTERFACE m_axi port=c_idx bundle=gmem_c    depth=1024
    #pragma HLS INTERFACE m_axi port=x     bundle=gmem_x    depth=1024
    #pragma HLS INTERFACE m_axi port=out   bundle=gmem_o    depth=1024

    #pragma HLS INTERFACE s_axilite port=m bundle=control
    #pragma HLS INTERFACE s_axilite port=nnz bundle=control
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    // data_t x_local[MAX_VECTOR];
    // #pragma HLS BIND_STORAGE variable=x_local type=RAM_1WNR impl=URAM
    // #pragma HLS ARRAY_PARTITION variable=x_local type=complete factor=BIND_FACTOR
    //
    // load_x(x, x_local, MAX_VECTOR);

rows: for (int i = 0; i < m; i++)
    {
        #pragma HLS loop_tripcount min=MIN_ROWS max=MAX_ROWS
        data_t y = 0.0;
    nnz: for (int j = r_beg[i]; j < r_beg[i + 1]; j++)
        {
            #pragma HLS loop_tripcount min=MIN_NNZ max=MAX_NNZ
            #pragma HLS PIPELINE II=3
            y += c_val[j] * x[c_idx[j]];
        }
        out[i] = y;
    }
}
