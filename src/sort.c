#include <stdlib.h>
#include "snackpack/sort.h"


/* These are used to implement the LAPACK slasrt sorting function. No custom
 * types here because this is a standard-library prototype.
 */
static int sort_increase(const void * const p1, const void * const p2);
static int sort_decrease(const void * const p1, const void * const p2);


/**
 * Sort an array in increasing or decreasing order.
 *
 * Note that the documentation for this in the reference LAPACK says
 * something about returning the index of an "illegal value" on error, but
 * this doesn't appear to be true. The values of the return SP_STATUS enum
 * correspond to what the LAPACK function actually returns.
 */
SP_STATUS
sp_slasrt(
    char id,
    len_t n,
    float_t * const d)
{
    if (n == 0) {
        return SP_STATUS_INVALID_DIM;
    } else if (n == 1) {
        /* Nothing to do */
        return SP_STATUS_OK;
    }

    switch (id) {
        case 'I':
            qsort(d, n, sizeof(float_t), sort_increase);
            return  SP_STATUS_OK;
            break;
        case 'D':
            qsort(d, n, sizeof(float_t), sort_decrease);
            return  SP_STATUS_OK;
            break;
        default:
            return SP_STATUS_ERROR;
    }
}


static int
sort_increase(
    const void * const p1,
    const void * const p2)
{
    if (*(float_t *)p1 < *(float_t *)p2) {
        return -1;
    } else if (*(float_t *)p1 > *(float_t *)p2) {
        return 1;
    } else {
        return 0;
    }
}


static int
sort_decrease(
    const void * const p1,
    const void * const p2)
{
    if (*(float_t *)p1 < *(float_t *)p2) {
        return 1;
    } else if (*(float_t *)p1 > *(float_t *)p2) {
        return -1;
    } else {
        return 0;
    }
}
