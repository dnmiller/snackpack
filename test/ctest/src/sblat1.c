#include <math.h>
#include <stdio.h>

#include "ctest.h"
#include "snackpack/blas1_real.h"
#include "snackpack/error.h"


static unsigned int err_count = 0;


/* This determines scaling used for error measurement. Larger number means
 * tighter tolerances. Ye ol' BLAS uses 2^-10, which is very loose.
 */
// const float SFAC = 9.765625E-4;   /* 2^-10 */
const float SASUM_FAC = 2.5e-1;       /* 2^-2 */
const float SNRM2_FAC = 0.5f;         /* 2^-1 */


const len_t TEST_DATA_ROWS = 8;
const len_t TEST_DATA_COLS = 5;
const float TEST_DATA[2][TEST_DATA_ROWS][TEST_DATA_COLS] = {
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
static const float
    SASUM_RESULT[TEST_DATA_COLS] = { 0.0f, 0.3f, 0.7f, 1.1f, 1.0f};

static void
test_sasum(void)
{
    float test_vec[TEST_DATA_ROWS] = {0.0f};

    /* Test for invalid dimensions */
    sp_blas_sasum(0, NULL, 1);
    ct_assert_last_error(SP_ERROR_INVALID_DIM, "sasum: zero dim");

    /* Test for invalid increment */
    sp_blas_sasum(1, NULL, 0);
    ct_assert_last_error(SP_ERROR_INVALID_INC, "sasum: zero inc");


    /* TODO: Test negative increments. */
    for (len_t inc_x = 1; inc_x < 3; inc_x++) {
        for (len_t n = 0; n < TEST_DATA_COLS; n++) {
            for (len_t k = 0; k < TEST_DATA_ROWS; k++) {
                test_vec[k] = TEST_DATA[inc_x - 1][k][n];
            }
            len_t len = 2 * (n < 1 ? 1 : n);
            float exp = SASUM_RESULT[n];
            float act = sp_blas_sasum(n, test_vec, inc_x);
            ct_assert_float_eq(act, exp, SASUM_FAC, "sasum error check");
        }
    }
}


/* True results for snrm2 */
static const float
    SNRM2_RESULT[TEST_DATA_COLS] = { 0.0f, 0.3f, 0.5f, 0.7f, 0.6f};

static void
test_snrm2(void)
{
    float test_vec[TEST_DATA_ROWS] = {0.0f};

    /* Test for invalid dimensions */
    sp_blas_snrm2(0, NULL, 1);
    ct_assert_last_error(SP_ERROR_INVALID_DIM, "snrm2: zero dim");

    /* Test for invalid increment */
    sp_blas_snrm2(1, NULL, 0);
    ct_assert_last_error(SP_ERROR_INVALID_INC, "snrm2: zero inc");

    /* TODO: Test negative increments. */
    for (len_t inc_x = 1; inc_x < 3; inc_x++) {
        for (len_t n = 0; n < TEST_DATA_COLS; n++) {
            for (len_t k = 0; k < TEST_DATA_ROWS; k++) {
                test_vec[k] = TEST_DATA[inc_x - 1][k][n];
            }
            len_t len = 2 * (n < 1 ? 1 : n);
            float exp = SNRM2_RESULT[n];
            float act = sp_blas_snrm2(n, test_vec, inc_x);
            ct_assert_float_eq(act, exp, SNRM2_FAC, "snrm2 error check");
        }
    }
}


const len_t LEN_SAXPY_DATA = 7;
const float SAXPY_X_DATA[LEN_SAXPY_DATA] = {
    0.6f, 0.1f, -0.5f, 0.8f, 0.9f, -0.3f, -0.4f};
const float SAXPY_Y_DATA[LEN_SAXPY_DATA] = {
    0.5f, -0.9f, 0.3f, 0.7f, -0.6f, 0.2f, 0.8f};

const float SAXPY_RESULT[] = {
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.68f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.68f, -0.87f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.68f, -0.87f,
    0.15f, 0.94f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.68f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.35f, -0.9f, 0.48f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.38f, -0.9f, 0.57f, 0.7f, -0.75f, 0.2f, 0.98f,
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.68f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.35f, -0.72f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.38f, -0.63f,
    0.15f, 0.88f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.68f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.68f, -0.9f, 0.33f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.68f, -0.9f, 0.33f, 0.7f, -0.75f, 0.2f, 1.04f};

const len_t TEST_SAXPY_NUM_INC = 4;
const len_t TEST_SAXPY_INC_X[TEST_SAXPY_NUM_INC] = {1, 2, -2, -1};
const len_t TEST_SAXPY_INC_Y[TEST_SAXPY_NUM_INC] = {1, -2, 1, -2};

const len_t TEST_SAXPY_NUM_LEN = 4;
const len_t TEST_SAXPY_LEN[TEST_SAXPY_NUM_LEN] = {0, 1, 2, 4};

static void
test_saxpy(void)
{
    float alpha = 0.3f;
    float x_tmp[LEN_SAXPY_DATA] = {0.0f};
    float y_tmp[LEN_SAXPY_DATA] = {0.0f};

    for (len_t i_inc = 0; i_inc < TEST_SAXPY_NUM_INC; i_inc++) {
        len_t inc_x = TEST_SAXPY_INC_X[i_inc];
        len_t inc_y = TEST_SAXPY_INC_Y[i_inc];

        for (len_t i_len = 0; i_len < TEST_SAXPY_NUM_LEN; i_len++) {
            len_t n = TEST_SAXPY_LEN[i_len];

            for (len_t i = 0; i < LEN_SAXPY_DATA; i++) {
                x_tmp[i] = SAXPY_X_DATA[i];
                y_tmp[i] = SAXPY_Y_DATA[i];
                sp_blas_saxpy(n, alpha, x_tmp, inc_x, y_tmp, inc_y);
            }
        }
    }
}


int main(void)
{
    test_sasum();
    test_snrm2();
    test_saxpy();
    printf("Done\n");
    return 0;
}
