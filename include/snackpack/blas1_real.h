#ifndef _SNACKPACK_BLAS1_H_
#define _SNACKPACK_BLAS1_H_

#include "snackpack/snackpack.h"

/* 
 * Include a trap to prevent pycparser/CFFI from scanning standard library
 * headers.
 */
#ifndef PYCPARSER_SCAN
#include <math.h>
#endif


float
sp_blas_sasum(
    len_t n,
    const float * const x,
    len_t inc_x);


void
sp_blas_saxpy(
    len_t n,
    float alpha,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y);


void
sp_blas_srotg(
    float * const sa,
    float * const sb,
    float * const c,
    float * const s);


void
sp_blas_srot(
    len_t n,
    float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y,
    float c,
    float s);


void
sp_blas_sswap(
    len_t n,
    float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y);


void
sp_blas_scopy(
    len_t n,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y);


float
sp_blas_sdot(
    len_t n,
    const float * const x,
    len_t inc_x,
    const float * const y,
    len_t inc_y);


float
sp_blas_sdsdot(
    len_t n,
    float sb,
    const float * const x,
    len_t inc_x,
    const float * const y,
    len_t inc_y);


float
sp_blas_snrm2(
    len_t n,
    const float * const x,
    len_t inc_x);


// Modified Givens is not finished yet.
#if 0
void
sp_blas_srotm(
    len_t n,
    float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y,
    const float * const p);


void
sp_blas_srotmg(
    float * const d1,
    float * const d2,
    float * const x,
    float y,
    float * const p);
#endif


void
sp_blas_sscal(
    len_t n,
    float alpha,
    float * const x,
    len_t inc_x);


len_t
sp_blas_isamax(
    len_t n,
    const float * const x,
    len_t inc_x);


len_t
sp_blas_isamin(
    len_t n,
    const float * const x,
    len_t inc_x);


#endif
