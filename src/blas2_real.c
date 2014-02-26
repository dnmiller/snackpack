
#include "snackpack/blas2_real.h"
#include "snackpack/blas1_real.h"


/**
 * Compute a general matrix-vector product.
 *
 * \param[in] trans     Whether or not to take the transpose of A
 * \param[in] m         Number of rows in A
 * \param[in] n         Number of columns in A
 * \param[in] alpha     Scalar alpha
 * \param[in] a         Matrix A
 * \param[in] lda       Leading dimension of A - must be at least max(1, m)
 * \param[in] x         Vector x
 * \param[in] inc_x     Increment for x
 * \param[in] beta      Scalar beta
 * \param[in,out] y     Vector y, stores result
 * \param[in] inc_y     Increment for y
 */
void
sp_blas_sgemv(
    SP_TRANS trans,
    len_t m,
    len_t n,
    float_t alpha,
    const float_t * const a,
    len_t lda,
    const float_t * const x,
    inc_t inc_x,
    float_t beta,
    float_t * const y,
    inc_t inc_y)
{
    if (alpha == 0.0f && beta == 0.0f) {
        return;
    }

    len_t len_x, len_y;
    switch (trans) {
        case SP_TRANS_NONE:
            len_x = n;
            len_y = m;
            break;
        case SP_TRANS_TRANSPOSE:
            len_x = m;
            len_y = n;
            break;
        default:
            /* TODO: Raise an error */
            return;
            break;
    }

    /* First, find beta * y */
    sp_blas_sscal(len_y, beta, y, inc_y);

    if (alpha == 0.0f) {
        /* If alpha is 0, we're done. */
        return;
    } else {
        len_t iy = inc_y < 0 ? (len_t)((1 - len_y) * inc_y) : 0;

        if (trans == SP_TRANS_NONE) {
            /* For no transpose, iterate over the rows of A. */
            for (len_t i = 0; i < len_y; i++) {
                y[iy] += alpha * sp_blas_sdot(len_x, a, lda, x, inc_x);
                iy += inc_y;
            }
        }
    }
}
