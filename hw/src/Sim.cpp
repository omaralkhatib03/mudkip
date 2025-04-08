
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <regex.h>
#include "FloatOps.h"
#include "floating_point_v7_1_bitacc_cmodel.h"
#include "mpfr.h"
#include "svdpi.h"

unsigned long xfpo_to_unsigned_long(xip_fpo_t value)
{
    mpfr_t x;
    mpfr_init(x);
    mpfr_set_prec(x, xip_fpo_get_prec_mant(value));
    xip_fpo_get_fr(x, value);
    unsigned long m_prec        = mpfr_get_prec(x);
    unsigned long e_prec        = xip_fpo_get_prec_exp(value);
    auto exp                    = value[0]._xip_fpo_exp - 1;
    unsigned sign               = mpfr_get_flt(x, MPFR_RNDD) < 0;
    unsigned long mant          =  *value[0]._xip_fpo_d;
    unsigned long shifted_man   = (mant >> (64 - m_prec));
    unsigned long non_imp_man   = shifted_man & ((1UL << (m_prec - 1)) - 1);
    signed bias                 = (1UL << (e_prec - 1)) - 1;
    unsigned long ret           = (exp + bias) << (m_prec - 1) | non_imp_man;
    ret                         &= ~(1UL << (m_prec - 1 + e_prec));
    ret                         |= ((0x1 & sign) << (m_prec - 1 + e_prec));

    if (xip_fpo_get_flt(value) == 0)
    {
        // printf("----------------------------- xfpo_to_unsigned_long --------------------------\n");
        // printf("Biased Exp: %lx Mant Prec: %ld\n", exp + bias, xip_fpo_get_prec_mant(value));
        // printf("MPFR Exp: %lx Mant Prec: %ld, Bias: %d\n", mpfr_get_exp(x) - 1, mpfr_get_prec(x), bias);
        // printf("Exp: %lx, Mant: %lx, Val: %lx, sign: %x\n", exp, mant, ret, sign);
        ret = 0x0;
    }
    else if (std::abs(xip_fpo_get_flt(value)) == INFINITY)
    {
        unsigned long inf_exp = (1UL << e_prec) - 1;
        ret = (inf_exp << (m_prec - 1));
        ret |= ((0x1 & sign) << (m_prec - 1 + e_prec));
    }
    else if (mpfr_nan_p(x))
    {
        unsigned long nan_exp = (1UL << e_prec) - 1;
        unsigned long nan_mant = 1UL << ((m_prec - 1) - 1);
        ret = (nan_exp << (m_prec - 1)) | nan_mant;
        ret |= ((0x1 & sign) << (m_prec - 1 + e_prec));
    }

    // printf("----------------------------- xfpo_to_unsigned_long --------------------------\n");
    // printf("Biased Exp: %lx Mant Prec: %ld\n", exp + bias, xip_fpo_get_prec_mant(value));
    // printf("Exp Prec: %lx Mant Prec: %ld, Bias: %d\n", xip_fpo_get_prec_exp(value), mpfr_get_prec(x), bias);
    // printf("Exp: %lx, Mant: %lx, Val: %lx, sign: %x\n", exp, mant, ret, sign);

    if (xip_fpo_get_prec_exp(value) == 8 && xip_fpo_get_prec_mant(value) == 24)
    {
        auto status = xip_fpo_get_flt(value);

        // if (status != 0)
        // {
            // printf("Status: %f\n", status);
        // }

        Fint fx;
        fx.i = ret;
        Fint fxtheirs;
        fxtheirs.f = xip_fpo_get_flt(value);
        // printf("Ours: %x, Theirs: %x\n", fx.i, fxtheirs.i);
        // printf("Ours: %f, Theirs: %f\n", fx.f, fxtheirs.f);
        assert(fx.f == xip_fpo_get_flt(value));
    }
    else if (xip_fpo_get_prec_exp(value) == 11 && xip_fpo_get_prec_mant(value) == 53)
    {
        Dlong dx;
        dx.l = ret;
        assert(dx.d == xip_fpo_get_d(value));
    }

    mpfr_clear(x);

    return ret;
}

void unsigned_long_to_xfpo(xip_fpo_t result, unsigned long bits)
{
    unsigned long e_prec = xip_fpo_get_prec_exp(result);
    unsigned long m_prec = xip_fpo_get_prec_mant(result);

    unsigned long exp_mask  = (1UL << e_prec) - 1;
    unsigned long exp_bits  = (bits >> (m_prec - 1)) & exp_mask;
    unsigned long mant_mask = (1UL << (m_prec - 1)) - 1;
    unsigned long mant_bits = bits & mant_mask;
    unsigned sign           = (bits >> (e_prec + m_prec - 1)) & 0x1;

    unsigned long bias = (1UL << (e_prec - 1)) - 1;

    // Normal case
    unsigned long m_imp    = bits & ((1UL << (m_prec - 1)) - 1);
    unsigned long m_no_imp = (1UL << (m_prec - 1)) | m_imp;
    double shifted_up      = static_cast<double>(m_no_imp) * (1ULL << (64 - m_prec));
    unsigned long biased   = bits >> (m_prec - 1);
    unsigned long exp      = biased - bias + 1;

    *result[0]._xip_fpo_d  = shifted_up;
    result[0]._xip_fpo_exp = exp;
    result[0]._xip_fpo_sign = sign;

    if (bits == 0)
    {
        *result[0]._xip_fpo_d  = 0x0;
        result[0]._xip_fpo_exp = 0x8000000000000001;
        result[0]._xip_fpo_sign = 0;
        return;
    }
    else if (exp_bits == exp_mask && mant_bits == 0) // Inf
    {
        *result[0]._xip_fpo_d  = 0xCAFECAFE;
        result[0]._xip_fpo_exp = 0x8000000000000003;
        result[0]._xip_fpo_sign = sign;
        return;
    }
    else if (exp_bits == exp_mask && mant_bits != 0) // NaN
    {
        *result[0]._xip_fpo_d  = 0xDEADBEAF;
        result[0]._xip_fpo_exp = 0x8000000000000002;
        result[0]._xip_fpo_sign = sign;
        return;
    }

}

// void unsigned_long_to_xfpo(xip_fpo_t result, unsigned long bits)
// {
//     //TODO: Add expections here as well
//     // if (bits == 0)
//     // {
//     //     *result[0]._xip_fpo_d               = 0x8;
//     //     result[0]._xip_fpo_exp              = ;
//     //     result[0]._xip_fpo_sign             = ;
//         // return;
//     // }
//
//     unsigned long e_prec                = xip_fpo_get_prec_exp(result);
//     unsigned long m_prec                = xip_fpo_get_prec_mant(result);
//     auto m_imp                          = bits & ((1 << (m_prec - 1)) - 1);
//     auto m_no_imp                       = (1 << (m_prec - 1)) | m_imp;
//     double shifted_up                   =  m_no_imp << (64 - m_prec);
//     auto bias                           = (1 << (e_prec - 1)) - 1;
//     auto biased                         = bits >> (m_prec - 1);
//     unsigned long exp                   = biased - bias + 1;
//     *result[0]._xip_fpo_d               = shifted_up;
//     result[0]._xip_fpo_exp              = exp;
//     result[0]._xip_fpo_sign             = (bits >> (e_prec + m_prec - 1));
//
//     // printf("------------------------------ unsigned_long_to_xfpo --------------------------\n");
//     // printf("Exp Prec: %ld Mant Prec: %ld, Bias: %d\n", xip_fpo_get_prec_exp(result), xip_fpo_get_prec_mant(result), bias);
//     // printf("Biased Exp: %lx, Exp: %lx, Mant: %lx, Bits: %lx \n", biased, exp, (unsigned long) shifted_up, bits);
//     // printf("Sign: %x \n", result[0]._xip_fpo_sign);
// }

void getStringRepr(const unsigned long a, char * a_str)
{
    sprintf(a_str, "%lx", a);
}

extern "C" uint8_t dpi_fmul(int exp_prec, int mant_prec,
            const unsigned long a,
            const unsigned long b,
            svBitVecVal * result)
{
    int status = 0;

    xip_fpo_prec_t e_width, m_width;
    xip_fpo_t ax, bx, c;

    e_width = exp_prec;
    m_width = mant_prec;

    xip_fpo_inits2(e_width, m_width, ax, bx, c, NULL);

    unsigned_long_to_xfpo(ax, a);
    unsigned_long_to_xfpo(bx, b);

    status |= xip_fpo_mul(c, ax, bx);

    unsigned long res = xfpo_to_unsigned_long(c);

    result[0] = static_cast<svBitVecVal>(res & 0xFFFFFFFF);         // lower 32 bits
    result[1] = static_cast<svBitVecVal>((res >> 32) & 0xFFFFFFFF); // upper 32 bits

    // printf("A: %lx, B: %lx Res: %lx\n", a, b, res);
    // printf("A: %f, B: %f, C: %lf\n", xip_fpo_get_flt(ax), xip_fpo_get_flt(bx), xip_fpo_get_flt(c));

    xip_fpo_clears(ax, bx, c, NULL);

    return status;
}

