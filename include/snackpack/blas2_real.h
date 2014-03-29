#ifndef _SNACKPACK_BLAS2_REAL_H_
#define _SNACKPACK_BLAS2_REAL_H_

#include <math.h>
#include <stdbool.h>
#include "snackpack/snackpack.h"


void
sp_blas_sgemv(
    bool is_trans,
    len_t m,
    len_t n,
    float_t alpha,
    const float_t * const a,
    len_t lda,
    const float_t * const x,
    len_t inc_x,
    float_t beta,
    float_t * const y,
    len_t inc_y);


void
sp_blas_strmv(
    bool is_upper,
    bool is_trans,
    bool is_unit,
    len_t n,
    const float * const A,
    len_t lda,
    float * const x,
    len_t inc_x);


#endif
