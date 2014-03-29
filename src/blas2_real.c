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
 * \param[in] is_trans  True to take the transpose of A
 * \param[in] rows      Number of rows in A
 * \param[in] cols      Number of columns in A
 * \param[in] alpha     Scalar alpha
 * \param[in] A         Matrix A
 * \param[in] lda       Leading dimension of A - must be at least 
 *                      max(1, rows)
 * \param[in] x         Vector x
 * \param[in] inc_x     Increment (stride) for x
 * \param[in] beta      Scalar beta
 * \param[in,out] y     Vector y, stores result
 * \param[in] inc_y     Increment (stride) for y
 */
void
sp_blas_sgemv(
    bool is_trans,
    len_t rows,
    len_t cols,
    float alpha,
    const float * const A,
    len_t lda,
    const float * const x,
    len_t inc_x,
    float beta,
    float * const y,
    len_t inc_y)
{
    SP_ASSERT_VALID_DIM(rows);
    SP_ASSERT_VALID_DIM(cols);
    SP_ASSERT_VALID_LDA(lda, rows);
    SP_ASSERT_VALID_INC(inc_x);
    SP_ASSERT_VALID_INC(inc_y);

    /* Determine the lengths of the x and y vectors.*/
    len_t len_x, len_y;
    if (is_trans) {
        len_x = rows;
        len_y = cols;
    } else {
        len_x = cols;
        len_y = rows;
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

    if (!is_trans) {
        if (inc_x == 1 && inc_y == 1) {
            for (len_t i = 0; i < len_x; i++) {
                float tmp = x[i] * alpha;
                len_t a_offset = i * lda;
                for (len_t j = 0; j < len_y; j++) {
                    y[j] += A[j + a_offset] * tmp;
                }
            }
        } else {
            len_t ix = inc_x < 0 ? (len_t)((1 - len_x) * inc_x) : 0;
            for (len_t i = 0; i < len_x; i++) {
                float tmp = x[ix] * alpha;
                len_t a_offset = i * lda;
                len_t iy = inc_y < 0 ? (len_t)((1 - len_y) * inc_y) : 0;
                for (len_t j = 0; j < len_y; j++) {
                    y[iy] += A[j + a_offset] * tmp;
                    iy += inc_y;
                }
                ix += inc_x;
            }
        }
    } else {
        if (inc_x == 1 && inc_y == 1) {
            for (len_t i = 0; i < len_y; i++) {
                float tmp = 0.0f;
                len_t a_offset = i * lda;
                for (len_t j = 0; j < len_x; j++) {
                    tmp += A[j + a_offset] * x[j];
                }
                y[i] += alpha * tmp;
            }
        } else {
            len_t iy = inc_y < 0 ? (len_t)((1 - len_y) * inc_y) : 0;
            for (len_t i = 0; i < len_y; i++) {
                float tmp = 0.0f;
                len_t a_offset = i * lda;
                len_t ix = inc_x < 0 ? (len_t)((1 - len_x) * inc_x) : 0;
                for (len_t j = 0; j < len_x; j++) {
                    tmp += A[j + a_offset] * x[ix];
                    ix += inc_x;
                }
                y[iy] += alpha * tmp;
                iy += inc_y;
            }
        }
    }

fail:
    return;
}


/**
 * Compute the product of a triangular matrix and a vector.
 *
 * Performs one of the operations
 *  
 *      x = A*x
 * or
 *      x = A^T*x
 *
 * \param[in] is_upper  True if A is upper triangular, false otherwise
 * \param[in] is_trans  True to use the transpose of A
 * \param[in] is_unit   True if A is unit-triangular
 * \param[in] n         Number of rows/columns in A
 * \param[in] lda       Leading dimension of A
 * \param[in,out] x     On enter, vector x to multiply. On exit, result.
 *                      Must have dimension at least 1 + (n - 1) * |inc_x|.
 * \param[in] inc_x     Increment of x
 */
void
sp_blas_strmv(
    bool is_upper,
    bool is_trans,
    bool is_unit,
    len_t n,
    const float * const A,
    len_t lda,
    float * const x,
    len_t inc_x)
{
    SP_ASSERT_VALID_DIM(n);
    SP_ASSERT_VALID_LDA(lda, n);
    SP_ASSERT_VALID_INC(inc_x);

    if (is_trans) {
    } else {
        if (is_upper) {
            if (inc_x == 1) {
                for (len_t i = 0; i < n; i++) {
                    float tmp = x[i];
                    len_t a_offset = i * lda;
                    for (len_t j = 0; j < i; j++) {
                        x[j] += A[j + a_offset] * tmp;
                    }
                    if (!is_unit) {
                        x[i] *= A[i + a_offset];
                    }
                }
            } else {
                len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
                for (len_t i = 0; i < n; i++) {
                    float tmp = x[ix];
                    len_t a_offset = i * lda;
                    len_t jx = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
                    for (len_t j = 0; j < i; j++) {
                        x[jx] += A[j + a_offset] * tmp;
                        jx += inc_x;
                    }
                    if (!is_unit) {
                        x[ix] *= A[i + a_offset];
                    }
                    ix += inc_x;
                }
            }
        } else {
        }
    }

fail:
    return;
}


#if 0

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
