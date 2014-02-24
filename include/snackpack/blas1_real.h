#ifndef _SNACKPACK_BLAS1_H_
#define _SNACKPACK_BLAS1_H_

#include "snackpack/snackpack.h"
#include <math.h>


float_t
sp_blas_sasum(
    const len_t n,
    const float_t * const x,
    const inc_t inc_x);


void
sp_blas_saxpy(
    const len_t n,
    const float_t alpha,
    const float_t * const x,
    const inc_t inc_x,
    float_t * const y,
    const inc_t inc_y);


void
sp_blas_srotg(
    float_t * const sa,
    float_t * const sb,
    float_t * const c,
    float_t * const s);


void
sp_blas_srot(
    const len_t n,
    float_t * const x,
    const inc_t inc_x,
    float_t * const y,
    const inc_t inc_y,
    const float_t c,
    const float_t s);


void
sp_blas_sswap(
    const len_t n,
    float_t * const x,
    const inc_t inc_x,
    float_t * const y,
    const inc_t inc_y);


void
sp_blas_scopy(
    const len_t n,
    const float_t * const x,
    const inc_t inc_x,
    float_t * const y,
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
    const float_t * const x,
    const inc_t inc_x,
    const float_t * const y,
    const inc_t inc_y);


float_t
sp_blas_snrm2(
    const len_t n,
    const float_t * const x,
    const inc_t inc_x);


void
sp_blas_srotm(
    const len_t n,
    float_t * const x,
    const inc_t inc_x,
    float_t * const y,
    const inc_t inc_y,
    const float_t * const p);


void
sp_blas_srotmg(
    float_t * const d1,
    float_t * const d2,
    float_t * const x,
    const float_t y,
    float_t * const p);


void
sp_blas_sscal(
    const len_t n,
    const float_t alpha,
    float_t * const x,
    const inc_t inc_x);


len_t
sp_blas_isamax(
    const len_t n,
    const float_t * const x,
    const inc_t inc_x);


len_t
sp_blas_isamin(
    const len_t n,
    const float_t * const x,
    const inc_t inc_x);


#endif
