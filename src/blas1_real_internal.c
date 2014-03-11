#include <math.h>

#include "snackpack/snackpack.h"
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
    for (len_t i = 0; i < n; i++) {
        tmp += fabsf(x[ix]);
        ix += inc_x;
    }
    return tmp;
}


/* saxpy for inc_x = inc_y = 1 */
void
sp_blas_saxpy_inc1(
    len_t n,
    float alpha,
    const float * const x,
    float * const y)
{
    if (alpha != 0.0f) {
        for (len_t i = 0; i < n; i++) {
            y[i] += alpha * x[i];
        }
    }
    /* If alpha == 0, nothing to do */
}


/* saxpy for inc_x != 1 or inc_y != 1 */
void
sp_blas_saxpy_incxy(
    len_t n,
    float alpha,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y)
{
    if (alpha != 0.0f) {
        len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
        len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

        for (len_t i = 0; i < n; i++) {
            y[iy] += alpha * x[ix];
            ix += inc_x;
            iy += inc_y;
        }
    }
    /* If alpha == 0, nothing to do */
}


/* srot for inc_x = inc_y = 1 */
void
sp_blas_srot_inc1(
    len_t n,
    float * const x,
    float * const y,
    float c,
    float s)
{
    for (len_t i = 0; i < n; i++) {
        float tmp = c * x[i] + s * y[i];
        y[i] = c * y[i] - s * x[i];
        x[i] = tmp;
    }
}


/* srot for inc_x or inc_y != 1 */
void
sp_blas_srot_incxy(
    len_t n,
    float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y,
    float c,
    float s)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

    for (len_t i = 0; i < n; i++) {
        float tmp = c * x[ix] + s * y[iy];
        y[iy] = c * y[iy] - s * x[ix];
        x[ix] = tmp;
        ix += inc_x;
        iy += inc_y;
    }
}


/* sswap for inc_x = inc_y = 1 */
void
sp_blas_sswap_inc1(
    len_t n,
    float * const x,
    float * const y)
{
    for (len_t i = 0; i < n; i++) {
        float tmp = x[i];
        x[i] = y[i];
        y[i] = tmp;
    }
}


/* sswap for inc_x, inc_y != 1 */
void
sp_blas_sswap_incxy(
    len_t n,
    float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

    for (len_t i = 0; i < n; i++) {
        float tmp = x[ix];
        x[ix] = y[iy];
        y[iy] = tmp;
        ix += inc_x;
        iy += inc_y;
    }
}


/* scopy for inc_x, inc_y = 1 */
void
sp_blas_scopy_inc1(
    len_t n,
    const float * const x,
    float * const y)
{
    for (len_t i = 0; i < n; i++) {
        y[i] = x[i];
    }
}


/* scopy for inc_x, inc_y != 1 */
void
sp_blas_scopy_incxy(
    len_t n,
    const float * const x,
    len_t inc_x,
    float * const y,
    len_t inc_y)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

    for (len_t i = 0; i < n; i++) {
        y[iy] = x[ix];
        ix += inc_x;
        iy += inc_y;
    }
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


/* isamax for inc = 1 */
len_t
sp_blas_isamax_inc1(
    len_t n,
    const float * const x)
{
    len_t imax = 0;
    float max = fabsf(x[imax]);
    for (len_t i = 1; i < n; i++) {
        float max_xi = fabsf(x[i]);
        if (max_xi > max) {
            max = max_xi;
            imax = i;
        }
    }
    return imax;
}


/* isamax for incx != 1 */
len_t
sp_blas_isamax_incx(
    len_t n,
    const float * const x,
    len_t inc_x)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    float max = fabsf(x[ix]);
    len_t imax = ix;
    ix += inc_x;

    for (len_t i = 1; i < n; i++) {
        float max_xi = fabsf(x[ix]);
        if (max_xi > max) {
            max = max_xi;
            imax = ix;
        }
        ix += inc_x;
    }
    return imax;
}


/* isamin for inc = 1 */
len_t
sp_blas_isamin_inc1(
    len_t n,
    const float * const x)
{
    len_t imin = 0;
    float min = fabsf(x[imin]);
    for (len_t i = 1; i < n; i++) {
        float min_xi = fabsf(x[i]);
        if (min_xi < min) {
            min = min_xi;
            imin = i;
        }
    }
    return imin;
}


/* isamin for inc != 1 */
len_t
sp_blas_isamin_incx(
    len_t n,
    const float * const x,
    len_t inc_x)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    float min = fabsf(x[ix]);
    len_t imin = ix;
    ix += inc_x;

    for (len_t i = 1; i < n; i++) {
        float min_xi = fabsf(x[ix]);
        if (min_xi < min) {
            min = min_xi;
            imin = ix;
        }
        ix += inc_x;
    }
    return imin;
}


/* sscal for inc = 1 */
void
sp_blas_sscal_inc1(
    len_t n,
    float alpha,
    float * const x)
{
    for (len_t i = 0; i < n; i++) {
        x[i] *= alpha;
    }
}


/* sccal for inc != 1 */
void
sp_blas_sscal_incx(
    len_t n,
    float alpha,
    float * const x,
    len_t inc_x)
{
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    for (len_t i = 0; i < n; i++) {
        x[ix] *= alpha;
        ix += inc_x;
    }
}


/* sdot for inc_x = inc_y = 1 */
float
sp_blas_sdot_inc1(
    len_t n,
    const float * const x,
    const float * const y)
{
    float tmp = 0.0f;
    for (len_t i = 0; i < n; i++) {
        tmp += x[i] * y[i];
    }
    return tmp;
}


/* sdot for inc_x or inc_y != 1 */
float
sp_blas_sdot_incxy(
    len_t n,
    const float * const x,
    len_t inc_x,
    const float * const y,
    len_t inc_y)
{
    float tmp = 0.0f;
    len_t ix = inc_x < 0 ? (len_t)((1 - n) * inc_x) : 0;
    len_t iy = inc_y < 0 ? (len_t)((1 - n) * inc_y) : 0;

    for (len_t i = 0; i < n; i++) {
        tmp += x[ix] * y[ix];
        ix += inc_x;
        iy += inc_y;
    }
    return tmp;
}
