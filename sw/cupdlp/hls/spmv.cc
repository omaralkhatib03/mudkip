#include "spmv.hpp"

void spmv(int *r_beg, int *c_idx, data_t *c_val, int m, data_t * x, data_t * out)
{
#pragma HLS INLINE off

    for (int i = 0; i < m; i++)
    {
#pragma HLS UNROLL factor=8
        float y0 = 0;

        for (int j = r_beg[i]; j < r_beg[i + 1]; j++)
        {
#pragma HLS PIPELINE 
            y0 += c_val[j] * x[c_idx[j]];
        }

        out[i] = y0;
    }
}
