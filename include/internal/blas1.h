#ifndef _SNACKPACK_BLAS1_H_
#define _SNACKPACK_BLAS1_H_

#include "snackpack/snackpack.h"
#include <math.h>


#if FLT_EVAL_METHOD == 0
    #define SP_ABS(x)           fabsf(x)
    #define SP_SQRT(x)          sqrtf(x)
    #define SP_COPYSIGN(x, y)   copysignf((x), (y))
    #define FMIN                (FLT_MIN)
#elif FLT_EVAL_METHOD == 2
    #define SP_ABS(x)           fabsl(x)
    #define SP_SQRT(x)          sqrtl(x)
    #define SP_COPYSIGN(x, y)   copysignl((x), (y))
    #define FMIN                (LDBL_MIN)
#else
    /* Use double for undefined and implementation-defined case. */
    #define SP_ABS(x)           fabs(x)
    #define SP_SQRT(x)          sqrt(x)
    #define SP_COPYSIGN(x, y)   copysign((x), (y))
    #define FMIN                (DBL_MIN)
#endif


void
sp_blas_rotg(
    float_t *c,
    float_t *s,
    float_t *sa,
    float_t *sb);


void
sp_blas_rot(
    const len_t len,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y,
    const float_t c,
    const float_t s);


void
sp_blas_swap(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y);

#endif
