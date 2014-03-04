#ifndef _SNACKPACK_BLAS1_H_
#define _SNACKPACK_BLAS1_H_

#include "snackpack/snackpack.h"
#include <math.h>


float_t
sp_blas_sasum(
    len_t n,
    const float_t * const x,
    len_t inc_x);


void
sp_blas_saxpy(
    len_t n,
    float_t alpha,
    const float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y);


void
sp_blas_srotg(
    float_t * const sa,
    float_t * const sb,
    float_t * const c,
    float_t * const s);


void
sp_blas_srot(
    len_t n,
    float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y,
    float_t c,
    float_t s);


void
sp_blas_sswap(
    len_t n,
    float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y);


void
sp_blas_scopy(
    len_t n,
    const float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y);


float_t
sp_blas_sdot(
    len_t n,
    const float_t * const x,
    len_t inc_x,
    const float_t * const y,
    len_t inc_y);


float_t
sp_blas_sdsdot(
    len_t n,
    float_t sb,
    const float_t * const x,
    len_t inc_x,
    const float_t * const y,
    len_t inc_y);


float_t
sp_blas_snrm2(
    len_t n,
    const float_t * const x,
    len_t inc_x);


void
sp_blas_srotm(
    len_t n,
    float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y,
    const float_t * const p);


void
sp_blas_srotmg(
    float_t * const d1,
    float_t * const d2,
    float_t * const x,
    float_t y,
    float_t * const p);


void
sp_blas_sscal(
    len_t n,
    float_t alpha,
    float_t * const x,
    len_t inc_x);


len_t
sp_blas_isamax(
    len_t n,
    const float_t * const x,
    len_t inc_x);


len_t
sp_blas_isamin(
    len_t n,
    const float_t * const x,
    len_t inc_x);


#endif
