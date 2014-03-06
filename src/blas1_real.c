#include <float.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "snackpack/blas1_real.h"
#include "snackpack/error.h"
#include "snackpack/internal/blas1_real_internal.h"


/* sasum for vectors with inc_x = 1 */
float
sp_blas_sasum_inc1(
    len_t n,
    const float * const x)
{
    float tmp = 0.0f;
    for (len_t i = 0; i < n; i++) {
        tmp += fabsf(x[i]);
    }
    return tmp;
}


/* sasum for vectors with inc_x != x */
float
sp_blas_sasum_incx(
    len_t n,
    const float * const x,
    len_t inc_x)
{
    float tmp = 0.0f;
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    for (len_t i = 0, ix = 0; i < n; i++) {
        tmp += fabsf(x[ix]);
        ix += inc_x;
    }
    return tmp;
}


/**
 * Return the sum of the absolute values of a vector (1-norm).
 *
 * \param[in] n         Number of elements add
 * \param[in] x         Pointer to the first element of the vector
 * \param[in] inc_x     Increment (stride) to sum over
 * \returns             Sum of absolute values of the vector elements
 */
float
sp_blas_sasum(
    len_t n,
    const float * const x,
    len_t inc_x)
{
    float result = 0.0f;

    SP_ASSERT_VALID_DIM(n);
    SP_ASSERT_VALID_INC(inc_x);

    if (inc_x == 1) {
        result = sp_blas_sasum_inc1(n, x);
    } else {
        result = sp_blas_sasum_incx(n, x, inc_x);
    }

fail:
    return result;
}


void
sp_blas_saxpy_inc1(
    len_t n,
    float_t alpha,
    const float_t * const x,
    float_t * const y)
{
    for (len_t i = 0; i < n; i++) {
        y[i] += alpha * x[i];
    }
}


void
sp_blas_saxpy_incxy(
    len_t n,
    float alpha,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

    for (len_t i = 0; i < n; i++) {
        y[iy] += alpha * x[ix];
        ix += inc_x;
        iy += inc_y;
    }
}


/**
 * Compute a*x + y where a is a scalar and x and y are vectors, and store
 * the result in y.
 *
 * \param[in] n             Number of elements in x and y
 * \param[in] alpha         Scaler to multiply x by
 * \param[in] x             x vector
 * \param[in] inc_x         Increment (stride) to iterate over x
 * \param[in,out] y         y vector, stored result
 * \param[in] inc_y         Increment (stride) to iterate over y
 *
 * If inc_x or inc_y is negative, then iteration is backwards starting with
 * element (1 - n) * inc. For example, n = 5 and inc_x = -2 would iterate
 * over x[8], x[6], x[4], x[2], x[0].
 */
void
sp_blas_saxpy(
    len_t n,
    float alpha,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y)
{
    SP_ASSERT_VALID_DIM(n);
    SP_ASSERT_VALID_INC(inc_x);
    SP_ASSERT_VALID_INC(inc_y);

    if (alpha == 0.0f) {
        /* Nothing to do */
        return;
    }
    else if (inc_x == 1 && inc_y == 1) {
        sp_blas_saxpy_inc1(n, alpha, x, y);
    } else {
        sp_blas_saxpy_incxy(n, alpha, x, inc_x, y, inc_y);
    }

fail:
    return;
}


/**
 * Compute a Givens plane rotation.
 *
 * The z parameter is something of a mystery. None of the LAPACK functions
 * call this function since the modified Givens rotation (srotmg) is more
 * efficient.
 *
 * \param[in,out] a     On entry, the first element of vector to be rotated.
 *                      On exit, equal to the first (non-zero) element of
 *                      the rotated vector.
 * \param[in,out] b     On entry, the second element of vector to be
 *                      rotated. On exit, equal to the z parameter of the
 *                      Givens rotation (see notes).
 * \param[out] c        Cosine of rotation angle
 * \param[out] s        Sine of rotation angle
 */
void
sp_blas_srotg(
    float_t * const a,
    float_t * const b,
    float_t * const c,
    float_t * const s)
{
    /* We do an actual comparison to zero here because the scaling should
     * prevent underflows.
     */
    float_t scale = fabsf(*a) + fabsf(*b);
    if (scale == 0.0f) {
        *c = 1.0f;
        *s = 0.0f;
        *a = 0.0f;
        *b = 0.0f;
    } else {
        float_t abs_a = fabsf(*a);
        float_t abs_b = fabsf(*b);
        float_t a_scale = *a/scale;
        float_t b_scale = *b/scale;

        float_t r = scale * sqrtf(a_scale*a_scale + b_scale*b_scale);
        r = copysignf(r, abs_a > abs_b ? *a : *b);

        *c = *a/r;
        *s = *b/r;

        /* Determine the z parameter. */
        if (abs_a > abs_b) {
            *b = *s;
        } else if (abs_a >= abs_b && *c != 0.0f) {
            *b = 1.0f / *c;
        } else {
            *b = 1.0f;
        }
        *a = r;
    }
}


/**
 * Apply a plane rotation.
 *
 * \param[in] n         Number of elements in x and y
 * \param[in,out] x     Array of dimension at least (1 + (n-1)*abs(inc_x))
 * \param[in] inc_x     Increment (stride) for the elements of x
 * \param[in,out] y     Array of dimension at least (1 + (n-2)*abs(inc_y))
 * \param[in] inc_y     Increment (stride) for the elements of y
 * \param[in] c         Cosine part of the Givens rotation
 * \param[in] s         Sine part of the Givens rotation
 */
void
sp_blas_srot(
    len_t n,
    float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y,
    float_t c,
    float_t s)
{
    float_t tmp;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp = c * x[i] + s * y[i];
            y[i] = c * y[i] - s * x[i];
            x[i] = tmp;
        }
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

        for (len_t i = 0; i < n; i++) {
            tmp = c * x[ix] + s * y[iy];
            y[iy] = c * y[iy] - s * x[ix];
            x[ix] = tmp;
            ix += inc_x;
            iy += inc_y;
        }
    }
}


/**
 * Swap the contents of two vectors.
 */
void
sp_blas_sswap(
    len_t n,
    float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y)
{
    float_t tmp;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp = x[i];
            x[i] = y[i];
            y[i] = tmp;
        }
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

        for (len_t i = 0; i < n; i++) {
            tmp = x[ix];
            x[ix] = y[ix];
            y[ix] = tmp;
            ix += inc_x;
            iy += inc_y;
        }
    }
}


/**
 * Copy the contents of one vector to another.
 */
void
sp_blas_scopy(
    len_t n,
    const float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y)
{
    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            y[i] = x[i];
        }
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

        for (len_t i = 0; i < n; i++) {
            y[iy] = x[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }
}


/**
 * Take the dot product of two vectors.
 */
float_t
sp_blas_sdot(
    len_t n,
    const float_t * const x,
    len_t inc_x,
    const float_t * const y,
    len_t inc_y)
{
    float_t tmp = 0.0f;
    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp += x[i] * y[i];
        }
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

        for (len_t i = 0; i < n; i++) {
            tmp += x[ix] * y[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }

    return tmp;
}


/**
 * Take the dot product of two single-precision vectors, doing the
 * accumulation in double precision.
 */
float_t
sp_blas_sdsdot(
    len_t n,
    float_t sb,
    const float_t * const x,
    len_t inc_x,
    const float_t * const y,
    len_t inc_y)
{
    double_t tmp = (double_t)sb;

    if (n <= 0)
        return (float_t)tmp;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp += (double_t)x[i] * (double_t)y[i];
        }
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

        for (len_t i = 0; i < n; i++) {
            tmp += (double_t)x[ix] * (double_t)y[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }

    return (float_t)tmp;
}


/* snrm2 for inc_x = 1 */
float
sp_blas_snrm2_inc1(
    len_t n,
    const float * const x)
{
    float scale = 0.0f;
    float sq = 1.0f;

    for (len_t i = 0; i < n; i++) {
        if (x[i] != 0.0f) {
            float absx = fabsf(x[i]);
            if (scale < absx) {
                float tmp = scale / absx;
                sq = 1.0f + sq * tmp * tmp;
                scale = absx;
            } else {
                float tmp = absx / scale;
                sq += tmp * tmp;
            }
        }
    }
    return scale * sqrtf(sq);
}


/* snrm2 for inc_x != 1 */
float
sp_blas_snrm2_incx(
    len_t n,
    const float * const x,
    len_t inc_x)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    float scale = 0.0f;
    float sq = 1.0f;

    for (len_t i = 0; i < n; i++) {
        if (x[ix] != 0.0f) {
            float absx = fabsf(x[ix]);
            if (scale < absx) {
                float tmp = scale / absx;
                sq = 1.0f + sq * tmp * tmp;
                scale = absx;
            } else {
                float tmp = absx / scale;
                sq += tmp * tmp;
            }
        }
        ix += inc_x;
    }
    return scale * sqrtf(sq);
}


/**
 * Take the two-norm of a vector.
 *
 * \param[in] n         Number of elements add
 * \param[in] x         Pointer to the first element of the vector
 * \param[in] inc_x     Increment (stride) to sum over
 * \returns             Two-norm of the vector elements
 */
float
sp_blas_snrm2(
    len_t n,
    const float * const x,
    len_t inc_x)
{
    float result = 0.0f;

    SP_ASSERT_VALID_DIM(n);
    SP_ASSERT_VALID_INC(inc_x);

    if (n == 1) { return fabsf(*x); }

    if (inc_x == 0) {
        result = sp_blas_snrm2_inc1(n, x);
    } else {
        result = sp_blas_snrm2_incx(n, x, inc_x);
    }

fail:
    return result;
}


/**
 * Apply a modified Givens rotation to two vectors.
 */
void
sp_blas_srotm(
    len_t n,
    float_t * const x,
    len_t inc_x,
    float_t * const y,
    len_t inc_y,
    const float_t * const p)
{
    int_fast8_t flag = (int_fast8_t)p[0];

    float_t h11, h12, h21, h22;
    float_t w, z;

    if (inc_x == inc_y && inc_x > 0) {
        len_t n_steps = n * (len_t)(inc_x);
        switch (flag) {
            case -1:
                h11 = p[1];
                h12 = p[3];
                h21 = p[2];
                h22 = p[4];
                for (len_t i = 0; i < n_steps; i += inc_x) {
                    w = x[i];
                    z = y[i];
                    x[i] = w * h11 + z * h12;
                    y[i] = w * h21 + z * h22;
                }
                break;
            case 0:
                h12 = p[3];
                h21 = p[2];
                for (len_t i = 0; i < n_steps; i += inc_x) {
                    w = x[i];
                    z = y[i];
                    x[i] = w + z * h12;
                    y[i] = w * h21 + z;
                }
                break;
            case 1:
                h11 = p[1];
                h22 = p[4];
                for (len_t i = 0; i < n_steps; i += inc_x) {
                    w = x[i];
                    z = y[i];
                    x[i] = w * h11 + z;
                    y[i] = -w + z * h22;
                }
                break;
            default:
                return;
        }
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;
        switch (flag) {
            case -1:
                h11 = p[1];
                h12 = p[3];
                h21 = p[2];
                h22 = p[4];
                for (len_t i = 0; i < n; i++) {
                    w = x[ix];
                    z = y[iy];
                    x[ix] = w * h11 + z * h12;
                    y[iy] = w * h21 + z * h22;
                    ix += inc_x;
                    iy += inc_y;
                }
                break;
            case 0:
                h12 = p[3];
                h21 = p[2];
                for (len_t i = 0; i < n; i++) {
                    w = x[ix];
                    z = y[iy];
                    x[ix] = w + z * h12;
                    y[iy] = w * h21 + z;
                    ix += inc_x;
                    iy += inc_y;
                }
                break;
            case 1:
                h11 = p[1];
                h22 = p[4];
                for (len_t i = 0; i < n; i++) {
                    w = x[ix];
                    z = y[iy];
                    x[ix] = w * h11 + z;
                    y[iy] = -w + z * h22;
                    ix += inc_x;
                    iy += inc_y;
                }
                break;
            default:
                return;
        }
    }
}


// TODO: Move this gibberish to a new file
/**
 * Generate a modified Givens rotation.
 *
 * The modified Givens rotation zeros elements of a vector or matrix without
 * the square root needed for the traditional Givens rotation (see srotg).
 *
 * The Givens rotation applied to two vectors \f$x\f$ and \f$y\f$ nullifies
 * the first element of \f$y\f$, such that
 *
 * \f[
 *      \begin{bmatrix}
 *      c & s \\ -s & c
 *      \end{bmatrix}
 *      \begin{bmatrix}
 *      x_1 & x_2 & \cdots & x_n \\ y_1 & y_2 & \cdots & y_n
 *      \end{bmatrix}
 *      =
 *      \begin{bmatrix}
 *      \bar x_1 & \bar x'_2 & \cdots & \bar x'_n \\
 *      0 & \bar y'_2 & \cdots & \bar y'_n
 *      \end{bmatrix}
 *      = G X
 * \f]
 *
 * where \f$ c = x_1/r \f$, \f$ s = y_1/r \f$, and
 * \f$ r = \pm \sqrt{x_1^2 + y_1^2} \f$. The sign of \f$ r \f$ is equal to
 * the sign of \f$ a \f$ if \f$ |a| \geq |b| \f$ or \f$ b \f$ otherwise.
 *
 * The modified Givens rotation reduces the number of floating-point
 * operations and removes the square-root necessary to produce the plane
 * rotation by traditional means.
 *
 * Suppose that \f$ X \f$ is available as a factored matrix
 *
 * \f[
 *      X = D^{1/2} \bar X =
 *      \begin{bmatrix}
 *      d_1^{1/2} & 0 \\ 0 & d_2^{1/2}
 *      \end{bmatrix}
 *      \begin{bmatrix}
 *      \bar x_1 & \bar x_2 & \cdots & \bar x_n \\
 *      \bar y_1 & \bar y_2 & \cdots & \bar y_n
 *      \end{bmatrix}
 * \f]
 *
 * where \f$ d_1 \f$ and \f$ d_2 \f$ are yet to be determined. Substituting
 * for \f$ X \f$ in \f$ G X \f$ and refactoring \f$ G D^{1/2} \f$ as
 * \f$ \bar D^{1/2} H \f$ results in
 *
 * \f[
 *      GX = G D^{1/2} \bar X = \bar D^{1/2} H \bar X =
 *      \begin{bmatrix}
 *      \bar d_1^{1/2} & 0 \\ 0 & \bar d_2^{1/2}
 *      \end{bmatrix}
 *      \begin{bmatrix}
 *      h_{11} & h_{12} \\ h_{21} & h_{22}
 *      \end{bmatrix}
 *      \bar X
 * \f]
 *
 * The trick in the modified Givens approach is to choose \f$ d_1 \f$ and
 * \f$ d_2 \f$ such that two elements of \f$ H \f$ are exactly 1,
 * eliminating two multiplications per column in \f$ \bar X \f$ when the
 * rotation is applied.
 *
 * We ignore the two cases where s or c is 0, since the rotation is trivial
 * in these cases. The non-zero case is split into two separate cases for
 * numerical precision issues. First, suppose \f$ |s| < |c| \f$. We can
 * factor \f$ GD^{1/2} \f$ as
 *
 * \f{eqnarray*}
 *      GD^{1/2} =
 *      \begin{bmatrix}
 *      d_1^{1/2} c & d_2^{1/2} s \\ -d_1^{1/2} s & d_2^{1/2} c
 *      \end{bmatrix} &=&
 *      \begin{bmatrix}
 *      d_1^{1/2} c & 0 \\ 0 & d_2^{1/2} c
 *      \end{bmatrix}
 *      \begin{bmatrix}
 *      1 & (s/c)(d_2/d_1)^{1/2} \\ -(s/c)(d_1/d_2)^{1/2} & 1
 *      \end{bmatrix} \\
 *      &=&
 *      \bar D^{1/2} H
 * \f}
 *
 * Because \f$ s/c = y_1 / x_1 \f$, we have
 * \f$ x_1 = d_1^{1/2} \bar x_1 \f$ and \f$ y_1 = d_2^{1/2} \bar y_1 \f$,
 * and so
 *
 * \f[
 *      \bar D^{1/2} H =
 *      \begin{bmatrix}
 *      \bar d_1^{1/2} & 0 \\ 0 & \bar d_2^{1/2}
 *      \end{bmatrix}
 *      \begin{bmatrix}
 *      1 & d_2 \bar y_1 / d_1 \bar x_1 \\ -\bar y_1 / \bar x_1 & 1
 *      \end{bmatrix}
 * \f]
 *
 * where \f$ \bar d_1^{1/2} = d_1^{1/2} c \f$ and
 * \f$ \bar d_2^{1/2} = d_2^{1/2} c \f$.
 * Notice that multiplication on the right by the first column of
 * \f$ \bar X \f$ results in
 *
 * \f[
 *      \bar D^{1/2} H
 *      \begin{bmatrix}
 *      \bar x_1 \\ \bar y_1
 *      \end{bmatrix} =
 *      \begin{bmatrix}
 *      \bar x_1 + d_2 {\bar y_1}^2 / d_1 \bar x_1 \\ 0
 *      \end{bmatrix}
 * \f]
 *
 * indeed creating the expected zero in \f$ \bar X \f$. Thus for some chosen
 * \f$ d_1 \f$ and \f$ d_2 \f$, we can create a plane rotation to rotate the
 * first column of \f$ \bar X \f$, and since \f$ \bar X \f$ is just
 * \f$ X \f$ scaled, we have also created a zero in \f$ X \f$.
 *
 * Now suppose \f$ |s| \geq |c| \f$. We can similarly factor \f$ GD^{1/2} \f$
 * as
 *
 * \f{eqnarray*}
 *      GD^{1/2} =
 *      \begin{bmatrix}
 *      d_1^{1/2} c & d_2^{1/2} s \\ -d_1^{1/2} s & d_2^{1/2} c
 *      \end{bmatrix} &=&
 *      \begin{bmatrix}
 *      d_2^{1/2} s & 0 \\ 0 & d_1^{1/2} s
 *      \end{bmatrix}
 *      \begin{bmatrix}
 *      (c / s) (d_1^{1/2} / d_2^{1/2}) & 1 \\
 *      -1 & (c / s) (d_2^{1/2} / d_1^{1/2} )
 *      \end{bmatrix} \\
 *      &=&
 *      \begin{bmatrix}
 *      \bar d_1^{1/2} s & 0 \\ 0 & \bar d_2^{1/2} s
 *      \end{bmatrix}
 *      \begin{bmatrix}
 *      d_1 \bar x_1 / d_2 \bar y_1 & 1 \\
 *      -1 & \bar x_1 / \bar y_1
 *      \end{bmatrix} \\
 *      &=&
 *      \bar D^{1/2} H
 * \f}
 *
 * Finally, we must calculate \f$ \bar d_1 \f$ and \f$ \bar d_2 \f$ without
 * explicitly computing \f$ |s| \f$ or \f$ |c| \f$.
 *
 * This function is a huge mess and doesn't appear to be used by any
 * higher-level BLAS/LAPACK functions, so this may never be finished...
 */
void
sp_blas_srotmg(
    float_t * const d1,
    float_t * const d2,
    float_t * const x,
    float_t y,
    float_t * const p)
{
    /* These magic numbers appear to date from the 70s (well before
     * IEEE-754) and GAMMA can possibly be made much larger on modern
     * machines. 2^12 suggest half-precision, but I could be wrong...
     */
    const float_t GAMMA = 4096.0f;  /* 2^12 */
    const float_t R_GAMMA_SQ = 5.96046e-8f;
    const float_t GAMMA_SQ = 1.67772e7f;
    int_fast8_t flag;

    float_t h11, h12, h21, h22;
    float_t p1, q1, q2, u;

    if (*d1 < 0.0f) {
        /* This condition (eventually) prevents divide by zero when dividing
         * by u.
         */
        p[0] = -1.0f;
        h11 = h12 = h21 = h22 = *d1 = *d2 = *x = 0.0f;
    } else {
        /* Line 132 */
        float_t p2 = *d2 * y;
        if (p2 == 0.0f) {
            /* In this case, we're already parallel to the x-axis in the
             * plane and there's nothing to rotate. (what about d2 = 0?)
             */
            p[0] = -2.0f;
            /* Stuff identity in H. (Not done in reference implementation).
             */
            p[1] = p[3] = 1.0f;
            p[2] = p[4] = 0.0f;
            return;
        }
        /* Line 140 */
        p1 = *d1 * *x;
        q2 = p2 * y;
        q1 = p1 * *x;

        /* Line 144 */
        if (fabsf(q1) > fabsf(q2)) {
            h21 = -y / *x;
            h12 = p2 / p1;
            u = 1.0f - h12 * h21;

            /* Line 150 */
            if (u > 0) {
                flag = 0;
                *d1 /= u;
                *d2 /= u;
                *x *= u;
            }
            // else need flag assignment here.
        }
        else {
            /* Line 158 */
            if (q2 < 0.0f) {
                flag = -1;
                h11 = h12 = h21 = h22 = *d1 = *d2 = *x = 0.0f;
            } else {
                /* Line 169 */
                float_t tmp;
                flag = 1;
                h22 = p1 / p2;
                h22 = *x / y;
                u = 1.0f + h11 * h22;
                tmp = *d2 / u;
                *d2 = *d1 / u;
                *d1 = tmp;
                *x = y * u;
            }
        }
        /* Line 182 */
        if (*d1 != 0.0f) {
            while (*d1 <= R_GAMMA_SQ || *d1 >= GAMMA_SQ) {
                /* Line 184 */
                if (flag == 0) {
                    h11 = h22 = 1.0f;
                    flag = -1;
                } else {
                    h21 = -1.0f;
                    h12 = 1.0f;
                    flag = -1;
                }
                /* Line 193 */
                if (*d1 < R_GAMMA_SQ) {
                    *d1 *= GAMMA_SQ;
                    *x /= GAMMA;
                    h11 /= GAMMA;
                    h12 /= GAMMA;
                } else {
                    *d1 /= GAMMA_SQ;
                    *x *= GAMMA;
                    h11 *= GAMMA;
                    h12 *= GAMMA;
                }
            }
        }

        /* Line 207 */
        if (*d2 != 0.0f)  {
            while (fabsf(*d2) <= R_GAMMA_SQ || fabsf(*d2) >= GAMMA_SQ) {
                if (flag == 0) {
                    h11 = h22 = 1.0f;
                    flag = -1;
                } else {
                    h21 = -1.0f;
                    h12 = 1.0f;
                    flag = -1;
                }
                /* Line 218 */
                if (fabs(*d2) <= R_GAMMA_SQ) {
                    *d2 *= GAMMA_SQ;
                    h21 /= GAMMA;
                    h22 /= GAMMA;
                } else {
                    *d2 /= GAMMA_SQ;
                    h21 *= GAMMA;
                    h22 *= GAMMA;
                }
            }
        }
    }

    /* Line 232 */
    if (flag < 0) {
        p[1] = h11;
        p[2] = h21;
        p[3] = h12;
        p[4] = h22;
    } else if (flag == 0) {
        p[2] = h21;
        p[3] = h12;
    } else {
        p[1] = h11;
        p[4] = h22;
    }

    p[0] = (float_t)(flag);
}


/**
 * Scale the contents of a vector.
 *
 * For a vector \f$x\f$ and a scalar \f$ \alpha \f$, this computes the
 * scalar-vector product \f$\alpha x\f$ and stores the result in \f$x\f$.
 *
 * \param[in] n         Length of the vector
 * \param[in] alpha     Scalar to apply
 * \param[in,out] x     Vector to scale
 * \param[in] inc_x     Increment (stride) of x vector.
 *
 * If inc_x is negative, then iteration is backwards starting with element
 * (1 - n) * inc_x. For example, n = 5 and inc_x = -2 would iterate over
 * x[8], x[6], x[4], x[2], x[0].
 *
 * Note: The reference implementation does not support negative increments.
 * It is added here because the operation is used higher level BLAS
 * functions. (Netlib just re-implements it.)
 */
void
sp_blas_sscal(
    len_t n,
    float_t alpha,
    float_t * const x,
    len_t inc_x)
{
    if (alpha == 0.0f) {
        memset(x, 0, sizeof(float_t) * n);
    } else if (inc_x == 1) {
        for (len_t i = 0; i < n; i++) {
            x[i] *= alpha;
        }
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        for (len_t i = 0; i < n; i++) {
            x[ix] *= alpha;
            ix += inc_x;
        }
    }
}


len_t
sp_blas_isamax(
    len_t n,
    const float_t * const x,
    len_t inc_x)
{
    if (inc_x == 1) {
        len_t imax = 0;
        float_t max = fabsf(x[imax]);
        for (len_t i = 1; i < n; i++) {
            float_t max_xi = fabsf(x[i]);
            if (max_xi > max) {
                max = max_xi;
                imax = i;
            }
        }
        return imax;
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        float_t max = fabsf(x[ix]);
        len_t imax = ix;
        ix += inc_x;

        for (len_t i = 1; i < n; i++) {
            float_t max_xi = fabsf(x[ix]);
            if (max_xi > max) {
                max = max_xi;
                imax = ix;
            }
            ix += inc_x;
        }
        return imax;
    }
}


len_t
sp_blas_isamin(
    len_t n,
    const float_t * const x,
    len_t inc_x)
{
    if (inc_x == 1) {
        len_t imin = 0;
        float_t min = fabsf(x[imin]);
        for (len_t i = 1; i < n; i++) {
            float_t min_xi = fabsf(x[i]);
            if (min_xi < min) {
                min = min_xi;
                imin = i;
            }
        }
        return imin;
    } else {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        float_t min = fabsf(x[ix]);
        len_t imin = ix;
        ix += inc_x;

        for (len_t i = 1; i < n; i++) {
            float_t min_xi = fabsf(x[ix]);
            if (min_xi < min) {
                min = min_xi;
                imin = ix;
            }
            ix += inc_x;
        }
        return imin;
    }
}

