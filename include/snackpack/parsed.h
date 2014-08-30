# 1 "blas1_real.h"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 169 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "blas1_real.h" 2




# 1 "../snackpack/snackpack.h" 1












// TODO increase this after testing





typedef int32_t len_t;



# 5 "blas1_real.h" 2










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

# 115 "blas1_real.h"


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



