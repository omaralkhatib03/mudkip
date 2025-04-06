#pragma once

#include <cstdint>
#include "svdpi.h"

extern "C" uint8_t dpi_fmul(int exp_prec, int mant_prec,
            const long a,
            const long b,
            svBitVecVal * result);
