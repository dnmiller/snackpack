#include <float.h>
#include <stdint.h>

#include "internal/blas1.h"



void
sp_blas_rotg(
    float_t * c,
    float_t * s,
    float_t * sa,
    float_t * sb)
{
    float_t scale = SP_ABS(*sa) + SP_ABS(*sb);

    if (SP_ABS(scale) <= FMIN) {
        *c = 1.0f;
        *s = 0.0f;
        *sa = 0.0f;
        *sb = 0.0f;
    } else {
        float_t roe = SP_ABS(*sa) > SP_ABS(*sb) ? *sa : *sb;
        float_t r = scale * SP_SQRT(
            (*sa/scale)*(*sa/scale) + (*sb/scale)*(*sb/scale));
        r = copysign(r, roe);

        *c = *sa/r;
        *s = *sb/r;
        *sb = SP_ABS(*sa) > SP_ABS(*sb) ? *s : 1.0f / *c;
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
sp_blas_rot(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y,
    const float_t c,
    const float_t s)
{
    float_t tmp;

    if (n == 0)
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
sp_blas_swap(
    const len_t n,
    float_t *x,
    const inc_t inc_x,
    float_t *y,
    const inc_t inc_y)
{
    float_t tmp;

    if (n == 0)
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

