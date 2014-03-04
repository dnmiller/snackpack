#include <math.h>
#include <stdio.h>

#include "ctest.h"
#include "snackpack/blas1_real.h"
#include "snackpack/logger.h"


static unsigned int err_count = 0;


/* This determines scaling used for error measurement. Larger number means
 * tighter tolerances. Ye ol' BLAS uses 2^-10, which is very loose.
 */
// const float_t SFAC = 9.765625E-4;   /* 2^-10 */
const float_t SASUM_FAC = 2.5e-1;       /* 2^-2 */
const float_t SNRM2_FAC = 0.5f;         /* 2^-1 */


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
static const float_t
    SASUM_RESULT[TEST_DATA_COLS] = { 0.0f, 0.3f, 0.7f, 1.1f, 1.0f};

static void
test_sasum(void)
{
    float_t test_vec[TEST_DATA_ROWS] = {0.0f};

    /* Test for invalid dimensions */
    sp_blas_sasum(0, NULL, 1);
    err_count++;
    ct_assert_int_eq(
        sp_last_error_logged(), SP_ERROR_INVALID_DIM,
        "Zero vector dimension");
    ct_assert_int_eq(
        sp_errors_logged(), err_count, "Zero vector error log");

    /* Test for invalid increment */
    sp_blas_sasum(1, NULL, 0);
    err_count++;
    ct_assert_int_eq(
        sp_last_error_logged(), SP_ERROR_INVALID_INC,
        "Zero vector increment");
    ct_assert_int_eq(sp_errors_logged(), err_count, "Zero inc error log");

    /* TODO: Test negative increments. */
    for (len_t inc_x = 1; inc_x < 3; inc_x++) {
        for (len_t n = 0; n < TEST_DATA_COLS; n++) {
            for (len_t k = 0; k < TEST_DATA_ROWS; k++) {
                test_vec[k] = TEST_DATA[inc_x - 1][k][n];
            }
            len_t len = 2 * (n < 1 ? 1 : n);
            float_t exp = SASUM_RESULT[n];
            float_t act = sp_blas_sasum(n, test_vec, inc_x);
            if (n == 0.0f) {
                err_count++;
            }
            ct_assert_int_eq(err_count, sp_errors_logged(), "Error count");
            ct_assert_float_eq(act, exp, SASUM_FAC, "sasum error check");
        }
    }
}


/* True results for snrm2 */
static const float_t
    SNRM2_RESULT[TEST_DATA_COLS] = { 0.0f, 0.3f, 0.5f, 0.7f, 0.6f};

static void
test_snrm2(void)
{
    float_t test_vec[TEST_DATA_ROWS] = {0.0f};

    /* Test for invalid dimensions */
    sp_blas_snrm2(0, NULL, 1);
    err_count++;
    ct_assert_int_eq(
        sp_last_error_logged(), SP_ERROR_INVALID_DIM,
        "Zero vector dimension");
    ct_assert_int_eq(
        sp_errors_logged(), err_count, "Zero vector error log");

    /* Test for invalid increment */
    sp_blas_snrm2(1, NULL, 0);
    err_count++;
    ct_assert_int_eq(
        sp_last_error_logged(), SP_ERROR_INVALID_INC,
        "Zero vector increment");
    ct_assert_int_eq(sp_errors_logged(), err_count, "Zero inc error log");

    /* TODO: Test negative increments. */
    for (len_t inc_x = 1; inc_x < 3; inc_x++) {
        for (len_t n = 0; n < TEST_DATA_COLS; n++) {
            for (len_t k = 0; k < TEST_DATA_ROWS; k++) {
                test_vec[k] = TEST_DATA[inc_x - 1][k][n];
            }
            len_t len = 2 * (n < 1 ? 1 : n);
            float_t exp = SNRM2_RESULT[n];
            float_t act = sp_blas_snrm2(n, test_vec, inc_x);
            if (n == 0.0f) {
                err_count++;
            }
            ct_assert_int_eq(err_count, sp_errors_logged(), "Error count");
            ct_assert_float_eq(act, exp, SNRM2_FAC, "snrm2 error check");
        }
    }
}


int main(void)
{
    test_sasum();
    test_snrm2();
    printf("Done\n");
    return 0;
}
