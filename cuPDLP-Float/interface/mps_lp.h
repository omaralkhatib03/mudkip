#ifndef mps_lp_cuda_h
#define mps_lp_cuda_h

#include "../cupdlp/cupdlp.h"
#ifdef __cplusplus
extern "C" {
#endif

cupdlp_retcode data_alloc(CUPDLPdata *data, cupdlp_int nRows, cupdlp_int nCols,
                          void *matrix, CUPDLP_MATRIX_FORMAT src_matrix_format,
                          CUPDLP_MATRIX_FORMAT dst_matrix_format);

cupdlp_retcode problem_alloc(
    CUPDLPproblem *prob, cupdlp_int nRows, cupdlp_int nCols, cupdlp_int nEqs,
    cupdlp_float *cost, cupdlp_float offset, cupdlp_float sense_origin,
    void *matrix, CUPDLP_MATRIX_FORMAT src_matrix_format,
    CUPDLP_MATRIX_FORMAT dst_matrix_format, cupdlp_float *rhs,
    cupdlp_float *lower, cupdlp_float *upper, cupdlp_float *alloc_matrix_time,
    cupdlp_float *copy_vec_time);
// problem and data part
cupdlp_retcode problem_create(CUPDLPproblem **prob);
void freealldata(int *Aeqp, int *Aeqi, cupdlp_float *Aeqx, int *Aineqp, int *Aineqi,
                 cupdlp_float *Aineqx, int *colUbIdx, cupdlp_float *colUbElem, cupdlp_float *rhs,
                 cupdlp_float *cost, cupdlp_float *x, cupdlp_float *s, cupdlp_float *t, cupdlp_float *sx,
                 cupdlp_float *ss, cupdlp_float *st, cupdlp_float *y, cupdlp_float *lower,
                 cupdlp_float *upper);

void print_script_usage();
#ifdef __cplusplus
}
#endif
#endif
