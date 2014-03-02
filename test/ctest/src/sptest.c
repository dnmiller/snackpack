#include <math.h>
#include <stdio.h>

#include "ctest.h"
#include "snackpack/blas1_real.h"
#include "snackpack/logger.h"


static unsigned int err_count = 0;


/* This determines scaling used for error measurement. Larger number means
 * tighter tolerances. Ye ol' BLAS uses 2^-10, which is very loose.
 */
/*const float_t SFAC = 9.765625E-4; */  /* 2^-10 */
const float_t SFAC = 2.5e-1;        /* Using 2^-2 here */

const len_t TEST_DATA_ROWS = 8;
const len_t TEST_DATA_COLS = 5;

const float_t TEST_DATA[2][TEST_DATA_ROWS][TEST_DATA_COLS] = {
    {
        {0.1f,  0.3f,  0.3f,  0.2f,  0.1f},
        {2.0f,  3.0f, -0.4f, -0.6f, -0.3f},
        {2.0f,  3.0f,  4.0f,  0.3f,  0.5f},
        {2.0f,  3.0f,  4.0f,  5.0f, -0.1f},
        {2.0f,  3.0f,  4.0f,  5.0f,  6.0f},
        {2.0f,  3.0f,  4.0f,  5.0f,  6.0f},
        {2.0f,  3.0f,  4.0f,  5.0f,  6.0f},
        {2.0f,  3.0f,  4.0f,  5.0f,  6.0f}
    }, {
        {0.1f,  0.3f,  0.3f,  0.2f,  0.1f},
        {8.0f,  9.0f,  2.0f,  3.0f,  4.0f},
        {8.0f,  9.0f, -0.4f, -0.6f, -0.3f},
        {8.0f,  9.0f,  2.0f,  5.0f,  6.0f},
        {8.0f,  9.0f,  2.0f,  0.3f, -0.5f},
        {8.0f,  9.0f,  2.0f,  2.0f,  7.0f},
        {8.0f,  9.0f,  2.0f,  2.0f, -0.1f},
        {8.0f,  9.0f,  2.0f,  2.0f,  3.0f}
    }};

/* True results for sasum */
const float_t SASUM_RESULT[TEST_DATA_COLS] = {0.0f, 0.3f, 0.7f, 1.1f, 1.0f};


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

    float_t test_vec[TEST_DATA_ROWS] = {0.0f};

    for (inc_t inc_x = 1; inc_x < 3; inc_x++) {
        for (len_t n = 0; n < TEST_DATA_COLS; n++) {
            for (len_t k = 0; k < TEST_DATA_ROWS; k++) {
                test_vec[k] = TEST_DATA[inc_x - 1][k][n];
            }

            len_t len = 2 * (n < 1 ? 1 : n);

            float_t exp = SASUM_RESULT[n];
            float_t act = sp_blas_sasum(n, test_vec, inc_x);
            float_t diff = act - exp;
            float_t abs_exp = fabsf(exp);

            ct_assert_eq((abs_exp + fabsf(diff * SFAC)) - abs_exp, 0.0f,
                    "wut");

        }
    }

    return 0;
}
