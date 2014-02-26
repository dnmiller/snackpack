#ifndef _SNACKPACK_BLAS2_REAL_H_
#define _SNACKPACK_BLAS2_REAL_H_

#include <math.h>
#include "snackpack/snackpack.h"


void
sp_blas_strmv(
    SP_TRI_UPLO uplo,
    SP_TRANS trans,
    SP_DIAG diag,
    order_t order,
    const float_t * const A,
    order_t lda,
    float_t * const x,
    inc_t x);


void
sp_blas_sgemv(
    SP_TRANS trans,
    order_t m,
    order_t n,
    float_t alpha,
    const float_t * const a,
    len_t lda,
    const float_t * const x,
    inc_t inc_x,
    float_t beta,
    float_t * const y,
    inc_t inc_y);

#endif
