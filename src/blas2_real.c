#include "snackpack/blas2_real.h"
#include "snackpack/blas1_real.h"
#include "snackpack/internal/blas1_real_internal.h"
#include "snackpack/error.h"


/**
 * Compute a general matrix-vector product.
 *
 * Performs one of the operations
 *
 *      y = alpha*A*x + beta*y
 * or
 *      y = alpha*A^T*x + beta*y
 *
 * \param[in] trans     Whether or not to take the transpose of A
 * \param[in] m         Number of rows in A
 * \param[in] n         Number of columns in A
 * \param[in] alpha     Scalar alpha
 * \param[in] A         Matrix A
 * \param[in] lda       Leading dimension of A - must be at least max(1, m)
 * \param[in] x         Vector x
 * \param[in] inc_x     Increment (stride) for x
 * \param[in] beta      Scalar beta
 * \param[in,out] y     Vector y, stores result
 * \param[in] inc_y     Increment (stride) for y
 */
void
sp_blas_sgemv(
    SP_TRANS trans,
    len_t m,
    len_t n,
    float alpha,
    const float * const A,
    len_t lda,
    const float * const x,
    len_t inc_x,
    float beta,
    float * const y,
    len_t inc_y)
{
    SP_ASSERT_VALID_TRANS(trans);
    SP_ASSERT_VALID_DIM(m);
    SP_ASSERT_VALID_DIM(n);
    SP_ASSERT_VALID_INC(lda);
    SP_ASSERT_VALID_INC(inc_x);
    SP_ASSERT_VALID_INC(inc_y);

    /* Determine the lengths of the x and y vectors.*/
    len_t len_x, len_y;
    switch (trans) {
        case SP_TRANS_NONE:
            len_x = n;
            len_y = m;
            break;
        case SP_TRANS_TRANSPOSE:
        case SP_TRANS_CONJUGATE:
            len_x = m;
            len_y = n;
            break;
        /* Unreachable if asserts are enabled. */
        case NUM_SP_TRANS:
        default:
            goto fail;
    }

    /* Save some flops if we're all 0. */
    if (alpha == 0.0f && beta == 0.0f) {
        if (inc_y == 1) {
            for (len_t i = 0; i < len_y; i++) {
                y[i] = 0.0f;
            }
            return;
        } else {
            len_t iy = inc_y < 0 ? (len_t)((1 - len_y) * inc_y) : 0;
            for (len_t i = 0; i < len_y; i++) {
                y[iy] = 0.0f;
                iy += inc_y;
            }
            return;
        }
    }

    /* First, find beta * y */
    if (inc_y == 1) {
        sp_blas_sscal_inc1(len_y, beta, y);
    } else {
        sp_blas_sscal_incx(len_y, beta, y, inc_y);
    }

    /* If alpha is 0, we're done. */
    if (alpha == 0.0f) {
        return;
    }

    if (trans == SP_TRANS_NONE) {
        if (inc_x == 1 && inc_y == 1) {
            for (len_t i = 0; i < len_x; i++) {
                float tmp = x[i] * alpha;
                len_t ix_offset = i * len_y;
                for (len_t j = 0; j < len_y; j++) {
                    y[j] += A[j + ix_offset] * tmp;
                }
            }
        } 
    } 
    else {
        if (inc_x == 1 && inc_y == 1) {
            for (len_t i = 0; i < len_y; i++) {
                float tmp = 0.0f;
                len_t ix_offset = i * len_x;
                for (len_t j = 0; j < len_x; j++) {
                    tmp += A[j + ix_offset] * x[j];
                }
                y[i] += alpha * tmp;
            }
        }
    }

fail:
    return;
}


#if 0
/* Branch for strmv when no transpose is taken and the argument is
 * upper-triangular.
 */
static void
strmv_no_trans_upper(
    SP_UNIT unit,
    len_t n,
    const float * const A,
    len_t lda,
    len_t inc_x)
{
    if (inc_x == 1) {
        for (len_t i = 0; i < n; i++) {
        }
    } else {
        len_t kx = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    }
}


static void
strmv_trans_upper(
    SP_UNIT unit,
    len_t n,
    const float * const A,
    len_t lda,
    len_t inc_x)
{
}


void
sp_blas_strmv(
    SP_TRI_TYPE up_or_lo,
    SP_TRANS trans,
    SP_UNIT unit,
    len_t n,
    const float * const A,
    len_t lda,
    len_t inc_x)
{
    if (n <= 0) {
        // TODO: error
        return;
    }
    if (up_or_lo == SP_TRI_UPPER) {

    } else if (up_or_lo == SP_TRI_LOWER) {
        // TODO: implement
        return;
    } else {
        // TODO: error
        return;
    }
}


sp_blas_sgbmv

sp_blas_sger

sp_blas_ssbmv

sp_blas_sspmv

sp_blas_sspr

sp_blas_sspr2

sp_blas_ssymv

sp_blas_ssyr

sp_blas_ssyr2

sp_blas_stbmv

sp_blas_stbsv

sp_blas_stpmv

sp_blas_stbsv

sp_blas_stpmv

sp_blas_stpsv

sp_blas_strmv

sp_blas_strsv


Level 3:


Used in svd:

LAPACK:
sbdsdc
sisnan
slaisnan
slacpy

#endif
