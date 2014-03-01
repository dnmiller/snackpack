#include <math.h>
#include <stdio.h>

#include "ctest.h"
#include "snackpack/blas1_real.h"
#include "snackpack/logger.h"


static unsigned int err_count = 0;


const len_t len_TRUE3 = 5;
const float_t TRUE3[len_TRUE3] = {0.0E0, 0.3E0, 0.7E0, 1.1E0, 1.0E0};
const float_t SFAC = 9.765625E-4;       /* 2^-10 */


static bool
stest(
    len_t n,
    float_t *actual,
    float_t *expected,
    float_t fac)
{
    float_t diff = 0.0f;

    for (len_t i = 0; i < n; i++) {
        float_t diff = actual[i] - expected[i];
        float_t abs_e = fabsf(expected[i]);

        if (abs_e + fabsf(diff * fac) - abs_e == 0.0f) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    sp_blas_sasum(0, NULL, 1);
    ct_assert(++err_count == sp_errors_logged(), "Zero dim error");
    sp_blas_sasum(1, NULL, 0);
    ct_assert(++err_count == sp_errors_logged(), "Zero inc error");
    sp_blas_sasum(len_TRUE3, TRUE3, 1);
    return 0;
}
