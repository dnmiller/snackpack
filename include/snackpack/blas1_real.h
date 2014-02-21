#ifndef _SNACKPACK_BLAS1_H_
#define _SNACKPACK_BLAS1_H_

#include "snackpack/snackpack.h"
#include <math.h>


float_t
sp_blas_sasum(
    const len_t n,
    const float_t *x,
    const inc_t inc_x);


void
sp_blas_saxpy(
    const len_t n,
    const float_t alpha,
    const float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y);


void
sp_blas_srotg(
    float_t *sa,
    float_t *sb,
    float_t *c,
    float_t *s);


void
sp_blas_srot(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y,
    const float_t c,
    const float_t s);


void
sp_blas_sswap(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y);


void
sp_blas_scopy(
    const len_t n,
    const float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y);


float_t
sp_blas_sdot(
    const len_t n,
    const float_t *x,
    const inc_t inc_x,
    const float_t *y,
    const inc_t inc_y);


float_t
sp_blas_sdsdot(
    const len_t n,
    const float_t sb,
    const float_t *x,
    const inc_t inc_x,
    const float_t *y,
    const inc_t inc_y);


float_t
sp_blas_snrm2(
    const len_t n,
    const float_t *x,
    const inc_t inc_x);


void
sp_blas_srotm(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y,
    const float_t *p);


void
sp_blas_srotmg(
    float_t *d1,
    float_t *d2,
    float_t *x,
    const float_t y,
    float_t *p);


void
sp_blas_sscal(
    const len_t n,
    const float_t alpha,
    float_t *x,
    const inc_t inc_x);


inc_t
sp_blas_isamax(
    const len_t n,
    const float_t *x,
    const inc_t inc_x);


inc_t
sp_blas_isamin(
    const len_t n,
    const float_t *x,
    const inc_t inc_x);


#endif
