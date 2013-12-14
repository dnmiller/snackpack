#include <float.h>
#include <stdint.h>

#include "internal/blas1.h"


float_t
sp_blas_sasum(
    const len_t n,
    const float_t *x,
    const inc_t inc_x)
{
    if (n <= 0)
        return 0.0f;

    float_t tmp = 0.0f;
    if (inc_x == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp += fabsf(x[i]);
        }
    } else {
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;

        for (len_t i = 0; i < n; i++) {
            tmp += fabsf(x[i]);
            ix += inc_x;
        }
    }
    return tmp;
}


void
sp_blas_saxpy(
    const len_t n,
    const float_t alpha,
    const float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y)
{
    if (n <= 0 || alpha == 0.0f)
        return;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            y[i] += alpha * x[i];
        }
    } else {
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        len_t iy = inc_y < 0 ? (1 - n) * inc_y : 0;

        for (len_t i = 0; i < n; i++) {
            y[iy] += alpha * x[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }
}


void
sp_blas_srotg(
    float_t *c,
    float_t *s,
    float_t *sa,
    float_t *sb)
{
    float_t scale = fabsf(*sa) + fabsf(*sb);

    /* We do an actual comparison to zero here because the scaling should
     * account for the very small case.
     */
    if (fabsf(scale) == 0.0f) {
        *c = 1.0f;
        *s = 0.0f;
        *sa = 0.0f;
        *sb = 0.0f;
    } else {
        float_t roe = fabsf(*sa) > fabsf(*sb) ? *sa : *sb;
        float_t r = scale * sqrtf(
            (*sa/scale)*(*sa/scale) + (*sb/scale)*(*sb/scale));
        r = copysignf(r, roe);

        *c = *sa/r;
        *s = *sb/r;
        *sb = fabsf(*sa) > fabsf(*sb) ? *s : 1.0f / *c;
        *sa = r;
    }
}


/**
 *
 * @param n         Number of elements in x and y
 * @param x         Array of dimension at least (1 + (n-1)*abs(inc_x))
 * @param inc_x     Increment for the elements of x
 * @param y         Array of dimension at least (1 + (n-2)*abs(inc_y))
 * @param inc_y     Increment for the elements of y
 * @param c         Cosine part of the Givens rotation
 * @param s         Sine part of the Givens rotation
 */
void
sp_blas_srot(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y,
    const float_t c,
    const float_t s)
{
    float_t tmp;

    if (n <= 0)
        return;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp = c * x[i] + s * y[i];
            y[i] = c * y[i] - s * x[i];
            x[i] = tmp;
        }
    } else {
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        len_t iy = inc_y < 0 ? (1 - n) * inc_y : 0;

        for (len_t i = 0; i < n; i++) {
            tmp = c * x[ix] + s * y[iy];
            y[iy] = c * y[iy] - s * x[ix];
            x[ix] = tmp;
            ix += inc_x;
            iy += inc_y;
        }
    }
}


void
sp_blas_sswap(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y)
{
    float_t tmp;

    if (n <= 0)
        return;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp = x[i];
            x[i] = y[i];
            y[i] = tmp;
        }
    } else {
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        len_t iy = inc_y < 0 ? (1 - n) * inc_y : 0;

        for (len_t i = 0; i < n; i++) {
            tmp = x[ix];
            x[ix] = y[ix];
            y[ix] = tmp;
            ix += inc_x;
            iy += inc_y;
        }
    }
}


void
sp_blas_scopy(
    const len_t n,
    const float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y)
{
    if (n <= 0)
        return;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            y[i] = x[i];
        }
    } else {
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        len_t iy = inc_y < 0 ? (1 - n) * inc_y : 0;

        for (len_t i = 0; i < n; i++) {
            y[iy] = x[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }
}


float_t
sp_blas_sdot(
    const len_t n,
    const float_t *x,
    const inc_t inc_x,
    const float_t *y,
    const inc_t inc_y)
{
    if (n <= 0)
        return 0.0f;

    float_t tmp = 0.0f;
    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp += x[i] * y[i];
        }
    } else {
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        len_t iy = inc_y < 0 ? (1 - n) * inc_y : 0;

        for (len_t i = 0; i < n; i++) {
            tmp += x[ix] * y[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }

    return tmp;
}


float_t
sp_blas_sdsdot(
    const len_t n,
    const float_t sb,
    const float_t *x,
    const inc_t inc_x,
    const float_t *y,
    const inc_t inc_y)
{
    double_t tmp = (double_t)sb;

    if (n <= 0)
        return (float_t)tmp;

    if (inc_x == 1 && inc_y == 1) {
        for (len_t i = 0; i < n; i++) {
            tmp += (double_t)x[i] * (double_t)y[i];
        }
    } else {
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        len_t iy = inc_y < 0 ? (1 - n) * inc_y : 0;

        for (len_t i = 0; i < n; i++) {
            tmp += (double_t)x[ix] * (double_t)y[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }

    return (float_t)tmp;
}


float_t
sp_blas_snrm2(
    const len_t n,
    const float_t *x,
    const inc_t inc_x)
{
    if (n < 1 || inc_x < 1) {
        return 0.0f;
    } else if (n == 1)
        return fabsf(*x);

    len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
    float_t scale = 0.0f;
    float_t sq = 1.0f;

    for (len_t i = 0; i < n; i++) {
        if (x[ix] != 0.0f) {
            float_t absx = fabsf(x[ix]);
            if (scale < absx) {
                float_t tmp = scale / absx;
                sq = 1.0f + sq * tmp * tmp;
                scale = absx;
            } else {
                float_t tmp = absx / scale;
                sq += tmp * tmp;
            }
        }
        ix += inc_x;
    }
    return scale * sqrtf(sq);
}


void
sp_blas_srotm(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y,
    const float_t *p)
{
    int_fast8_t flag = (int_fast8_t)p[0];

    if (n <= 0)
        return;

    float_t h11, h12, h21, h22;
    float_t w, z;

    if (inc_x == inc_y && inc_x > 0) {
        len_t n_steps = n * inc_x;
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
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        len_t iy = inc_y < 0 ? (1 - n) * inc_y : 0;
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


void
sp_blas_srotmg(
    float_t *d1,
    float_t *d2,
    float_t *x,
    const float_t y,
    float_t *p)
{
    const float_t GAMMA = 4096.0f;
    const float_t R_GAMMA_SQ = 5.96046e-8f;
    const float_t GAMMA_SQ = 1.67772e7f;
    int_fast8_t flag;

    float_t h11, h12, h21, h22;
    float_t p1, p2, q1, q2, u;

    /* Line 120 */
    if (*d1 < 0.0f) {
        flag = -1;
        h11 = h12 = h21 = h22 = 0.0f;
        *x = *d1 = *d2 = 0.0f;
    } else {
        /* Line 132 */
        p2 = *d2 * y;
        if (p2 == 0.0f) {
            flag = -2;
            p[0] = -2.0f;
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


void
sp_blas_sscal(
    const len_t n,
    const float_t alpha,
    float_t *x,
    const inc_t inc_x)
{
    if (n <= 0 || inc_x <= 0)
        return;

    if (inc_x == 1) {
        for (len_t i = 0; i < n; i++) {
            x[i] *= alpha;
        }
    } else {
        for (len_t i = 0; i < n * inc_x; i += inc_x) {
            x[i] *= alpha;
        }
    }
}


inc_t
sp_blas_isamax(
    const len_t n,
    const float_t *x,
    const inc_t inc_x)
{
    if (n < 1 || inc_x <= 0)
        return -1;
    if (n == 1)
        return 0;

    if (inc_x == 1) {
        inc_t imax = 0;
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
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        float_t max = fabsf(x[ix]);
        inc_t imax = ix;
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


inc_t
sp_blas_isamin(
    const len_t n,
    const float_t *x,
    const inc_t inc_x)
{
    if (n < 1 || inc_x <= 0)
        return -1;
    if (n == 1)
        return 0;

    if (inc_x == 1) {
        inc_t imin = 0;
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
        len_t ix = inc_x < 0 ? (1 - n) * inc_x : 0;
        float_t min = fabsf(x[ix]);
        inc_t imin = ix;
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

