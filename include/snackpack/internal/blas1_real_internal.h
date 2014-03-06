#ifndef _SNACKPACK_INTERNAL_BLAS1_REAL_INTERNAL_H_
#define _SNACKPACK_INTERNAL_BLAS1_REAL_INTERNAL_H_


/* sasum */
float
sp_blas_sasum_inc1(
    len_t n,
    const float * const x);


float
sp_blas_sasum_incx(
    len_t n,
    const float * const x,
    len_t inc_x);


/* snrm2 */
float
sp_blas_snrm2_inc1(
    len_t n,
    const float * const x);


float
sp_blas_snrm2_incx(
    len_t n,
    const float * const x,
    len_t inc_x);


/* saxpy */
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


#endif
