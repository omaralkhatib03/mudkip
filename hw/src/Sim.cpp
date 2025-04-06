
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include "FloatOps.h"
#include "floating_point_v7_1_bitacc_cmodel.h"

extern "C" uint8_t dpi_fmul(int exp_prec, int mant_prec,
            const long a,
            const long b,
            svBitVecVal * result)
{
    return 0;
}

