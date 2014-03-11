#include <math.h>
#include <stdio.h>

#include "ctest.h"
#include "snackpack/blas1_real.h"


static unsigned int err_count = 0;


/* A general structure for testing vector operations. */
typedef struct {

    len_t         len;
    len_t         inc;
    float       *data;

} test_vector;


static const len_t NUM_TEST_VEC = 8;

static const test_vector TEST_VEC_1 = {
    .len = 1, .inc = -1, .data = (float[1]){3.14f}};
static const test_vector TEST_VEC_2 = {
    .len = 1, .inc =  1, .data = (float[1]){-4.14f}};
static const test_vector TEST_VEC_3 = {
    .len = 10, .inc = 1,
    .data = (float[10]){
         1.2f,  7.1f, -3.0f, -1.1f, -8.9f,
        -6.5f,  3.3f, -3.4f,  8.0f, -7.6f
    }};
static const test_vector TEST_VEC_4 = {
    .len = 10, .inc = -1,
    .data = (float[10]){
        -8.0f, -7.2f, -6.6f, -6.1f, -3.7f,
        -3.7f, -5.6f, -5.0f,  7.9f,  4.1f
    }};
static const test_vector TEST_VEC_5 = {
    .len = 7, .inc = 2,
    .data = (float[14]){
         1.1f, -6.3f,  0.0f, -8.5f,  8.3f,  4.1f,  1.2f,
        -3.7f, -6.7f,  2.4f,  9.8f, -6.6f, -4.8f, -2.1f
    }};
static const test_vector TEST_VEC_6 = {
    .len = 7, .inc = -2,
    .data = (float[14]){
        -8.5f,  3.7f, -2.0f,  9.7f, -2.0f,  2.4f, -6.9f,
        -2.4f, -6.8f,  5.2f,  7.4f, -3.0f,  3.7f, -4.1f
    }};
static const test_vector TEST_VEC_7 = {
    .len = 3, .inc = 3,
    .data = (float[9]){
         0.6f,  6.6f,  1.9f, -3.3f, -4.0f, -0.9f, -1.5f, -2.8f,  1.2f
    }};
static const test_vector TEST_VEC_8 = {
    .len = 3, .inc = -3,
    .data = (float[9]){
         4.9f, -1.5f, -1.4f, -7.5f, -9.5f, -4.2f, -3.6f,  3.1f,  9.1f
    }};

static const test_vector * const TEST_VECS[NUM_TEST_VEC] = {
    &TEST_VEC_1, &TEST_VEC_2, &TEST_VEC_3, &TEST_VEC_4,
    &TEST_VEC_5, &TEST_VEC_6, &TEST_VEC_7, &TEST_VEC_8};


static void
test_sasum(void)
{
    /* True results for sasum */
    const float SASUM_RESULTS[NUM_TEST_VEC] = {
        3.14f, 4.14f, 50.1f, 57.9f, 31.9f, 37.3f, 5.4f, 16.0f};
    const float SASUM_FAC = 2.5e-1;  /* 2^-2 */

    /* Run through test vectors */
    for (unsigned int i = 0; i < 8; i++) {
        float exp = SASUM_RESULTS[i];
        float act = sp_blas_sasum(
            TEST_VECS[i]->len,
            TEST_VECS[i]->data,
            TEST_VECS[i]->inc);
        ct_assert_float_eq(act, exp, SASUM_FAC, "sasum error check");
    }
}


static void
test_snrm2(void)
{
    /* True results for snrm2 */
    const float SNRM2_RESULTS[NUM_TEST_VEC] = {
        3.140000000000000e+00,
        4.140000000000000e+00,
        1.810331461362808e+01,
        1.896233108032871e+01,
        1.534633506737032e+01,
        1.557401682290089e+01,
        3.674234614174767e+00,
        9.655050491841045e+00};
    const float SNRM2_FAC = 2.5e-1;     /* 2^-1 */

    /* Run through test vectors */
    for (unsigned int i = 0; i < 8; i++) {
        float exp = SNRM2_RESULTS[i];
        float act = sp_blas_snrm2(
            TEST_VECS[i]->len,
            TEST_VECS[i]->data,
            TEST_VECS[i]->inc);
        ct_assert_float_eq(act, exp, SNRM2_FAC, "sasum error check");
    }
}


static void
test_saxpy(void)
{
    const float ALPHA[3] = {-1.0, 0.0, 3.2};
    const float SAXPY_RESULTS[3][10] = {
        {-9.2000e+00, -1.4300e+01, -3.6000e+00, -5.0000e+00,  5.2000e+00,
          2.8000e+00, -8.9000e+00, -1.6000e+00, -1.0000e-01,  1.1700e+01},
        {-8.0000e+00, -7.2000e+00, -6.6000e+00, -6.1000e+00, -3.7000e+00,
         -3.7000e+00, -5.6000e+00, -5.0000e+00,  7.9000e+00,  4.1000e+00},
        {-4.1600e+00,  1.5520e+01, -1.6200e+01, -9.6200e+00, -3.2180e+01,
         -2.4500e+01,  4.9600e+00, -1.5880e+01,  3.3500e+01, -2.0220e+01}};

    sp_blas_saxpy(
        TEST_VECS[2]->len,  ALPHA[0],
        TEST_VECS[2]->data, TEST_VECS[2]->inc,
        TEST_VECS[3]->data, TEST_VECS[3]->inc);
}


static void
test_scopy(void)
{
    const len_t VEC_LEN = 10;
    const float test1[VEC_LEN] = {
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    float test2[VEC_LEN] = {
        2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
    const float test_result1[VEC_LEN] = {
        1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f};

    sp_blas_scopy(5, test1, 1, test2, -2);
    ct_assert_float_vec_eq(10, test2, test_result1, 1.0f,
                           "scopy error check");
    sp_blas_scopy(10, test1, 1, test2, 1);
    ct_assert_float_vec_eq(10, test2, test1, 1.0f,
                           "scopy error check");
}


static void
test_sswap(void)
{
    const len_t VEC_LEN = 10;
    float test1[VEC_LEN] = {
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    float test2[VEC_LEN] = {
        2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
    const float test1_result[VEC_LEN] = {
        2.0f, 2.0f, 2.0f, 2.0f, 2.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    const float test2_result[VEC_LEN] = {
        1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f};

    sp_blas_sswap(5, test1, 1, test2, 2);
    ct_assert_float_vec_eq(VEC_LEN, test1, test1_result, 1.0f,
                           "sswap error check");
    ct_assert_float_vec_eq(VEC_LEN, test2, test2_result, 1.0f,
                           "sswap error check");

    const len_t VEC_LEN2 = 4;
    float testx[VEC_LEN2] = {1.0f, 2.0f, -1.0f, -2.0f};
    float testy[VEC_LEN2] = {9.0f, 8.0f, 3.0f, 7.0f};
    const float testx_result[VEC_LEN2] = {9.0f, 8.0f, 3.0f, 7.0f};
    const float testy_result[VEC_LEN2] = {1.0f, 2.0f, -1.0f, -2.0f};
    sp_blas_sswap(4, testx, 1, testy, 1);
    ct_assert_float_vec_eq(VEC_LEN2, testx, testx_result, 1.0f,
                           "sswap error check");
    ct_assert_float_vec_eq(VEC_LEN2, testy, testy_result, 1.0f,
                           "sswap error check");
}


static void
test_isamax(void)
{
    const len_t VEC_LEN = 5;
    const float test_vec[VEC_LEN] = {1.2f, 3.3f, -1.2f, -10.3f, 0.0f};

    ct_assert_int_eq(3, sp_blas_isamax(5, test_vec, 1), "isamax check");
    ct_assert_int_eq(0, sp_blas_isamax(3, test_vec, 2), "isamax check");
    ct_assert_int_eq(2, sp_blas_isamax(3, test_vec, -2), "isamax check");
}


static void
test_isamin(void)
{
    const len_t VEC_LEN = 5;
    const float test_vec[VEC_LEN] = {1.2f, 3.3f, -1.2f, -0.3f, 5.0f};

    ct_assert_int_eq(3, sp_blas_isamin(5, test_vec, 1), "isamin check");
    ct_assert_int_eq(0, sp_blas_isamin(3, test_vec, 2), "isamin check");
    ct_assert_int_eq(2, sp_blas_isamin(3, test_vec, -2), "isamin check");
}


int main(void)
{
    test_sasum();
    test_snrm2();
    test_saxpy();
    test_scopy();
    test_sswap();
    test_isamax();
    test_isamin();

    printf("Done\n");
    return 0;
}
