#pragma once

#include <cstdint>
#include "floating_point_v7_1_bitacc_cmodel.h"
#include "svdpi.h"
#include "mpfr.h"

union Fint
{
    float f;
    unsigned int i;
};

union Dlong
{
    double d;
    unsigned long l;
};

unsigned long xfpo_to_unsigned_long(xip_fpo_t value);
void unsigned_long_to_xfpo(xip_fpo_t result, unsigned long bits);
void getStringRepr(const unsigned long a, char * a_str);

extern "C" uint8_t dpi_fmul(int exp_prec, int mant_prec,
            const unsigned long a,
            const unsigned long b,
            svBitVecVal * result);


