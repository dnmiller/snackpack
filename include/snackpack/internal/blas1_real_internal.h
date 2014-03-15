#ifndef _SNACKPACK_INTERNAL_BLAS1_REAL_INTERNAL_H_
#define _SNACKPACK_INTERNAL_BLAS1_REAL_INTERNAL_H_


float
sp_blas_sasum_inc1(
    len_t n,
    const float * const x);


float
sp_blas_sasum_incx(
    len_t n,
    const float * const x,
    len_t inc_x);


float
sp_blas_snrm2_inc1(
    len_t n,
    const float * const x);


float
sp_blas_snrm2_incx(
    len_t n,
    const float * const x,
    len_t inc_x);


void
sp_blas_saxpy_inc1(
    len_t n,
    float alpha,
    const float * const x,
    float * const y);


void
sp_blas_saxpy_incxy(
    len_t n,
    float alpha,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y);


void
sp_blas_srot_inc1(
    len_t n,
    float * const x,
    float * const y,
    float c,
    float s);


void
sp_blas_srot_incxy(
    len_t n,
    float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y,
    float c,
    float s);


void
sp_blas_sswap_inc1(
    len_t n,
    float * const x,
    float * const y);


void
sp_blas_sswap_incxy(
    len_t n,
    float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y);


void
sp_blas_scopy_inc1(
    len_t n,
    const float * const x,
    float * const y);


void
sp_blas_scopy_incxy(
    len_t n,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y);


float
sp_blas_snrm2_inc1(
    len_t n,
    const float * const x);


float
sp_blas_snrm2_incx(
    len_t n,
    const float * const x,
    len_t inc_x);


len_t
sp_blas_isamax_inc1(
    len_t n,
    const float * const x);


len_t
sp_blas_isamax_incx(
    len_t n,
    const float * const x,
    len_t inc_x);


len_t
sp_blas_isamin_inc1(
    len_t n,
    const float * const x);


len_t
sp_blas_isamin_incx(
    len_t n,
    const float * const x,
    len_t inc_x);


void
sp_blas_sscal_inc1(
    len_t n,
    float alpha,
    float * const x);


void
sp_blas_sscal_incx(
    len_t n,
    float alpha,
    float * const x,
    len_t inc_x);


float
sp_blas_sdot_inc1(
    len_t n,
    const float * const x,
    const float * const y);


float
sp_blas_sdot_incxy(
    len_t n,
    const float * const x,
    len_t inc_x,
    const float * const y,
    len_t inc_y);


float
sp_blas_sdsdot_inc1(
    len_t n,
    float sb,
    const float * const x,
    const float * const y);


float
sp_blas_sdsdot_incxy(
    len_t n,
    float sb,
    const float * const x,
    len_t inc_x,
    const float * const y,
    len_t inc_y);


#endif
