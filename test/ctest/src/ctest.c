#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "ctest.h"


static long double max_error = 0.0;
static const char *max_error_func = NULL;
bool exit_on_fail = true;

static int result = 0;

static unsigned int fail_count = 0;

void
ctest_log_result(
    long double err,
    long double tol,
    const char *func)
{
    if (err > max_error) {
        max_error = err;
    }

    if (err > tol) {
        ctest_log("%-20s | %Le | %Le | !!!FAIL!!!\n", func, err, tol);
        result = -1;
    } else {
        ctest_log("%-20s | %Le | %Le | pass\n", func, err, tol);
    }
}


int
ctest_end_test(void)
{
    ctest_log(
" ------------------------------------------------------------------------------\n"
"|                                 TEST COMPLETE                                |\n"
" ------------------------------------------------------------------------------\n"
"\n"
"    Maximum error: %Le\n"
"    Result: %s\n"
"\n", max_error, result == 0 ? "Pass" : "FAIL!");

    return result;
}


void
ct_assert_eq(
    long double expected,
    long double actual,
    const char *test_name)
{
    if (expected != actual) {
        fail_count++;
        ctest_log("\n"
            "Test failed: %s\n"
            "-----------\n"
            "    Expected: %Le\n"
            "    Actual:   %Le\n", test_name, expected, actual);
        if (exit_on_fail) {
            exit(-1);
        }
    } else {
        ctest_log(".");
    }
}


void
ct_assert_almost_eq(
    long double expected,
    long double actual,
    long double tolerance,
    const char *test_name)
{
    long double err = expected > actual ?
                      expected - actual : actual - expected;
    if (err > tolerance) {
        fail_count++;
        ctest_log("\n"
            "Test failed: %s\n"
            "-----------\n"
            "    Expected: %Le\n"
            "    Actual:   %Le\n"
            "    Error:    %Le\n", test_name, expected, actual, err);
        if (exit_on_fail) {
            exit(-1);
        }
    } else {
        ctest_log(".");
    }
}


/* Stuff for recording errors. */
#define MAX_ERROR_RECORDS (10000)

typedef struct {

    const char *test_name;
    long double error;

} ct_error_record;


static struct {

    ct_error_record     records[MAX_ERROR_RECORDS];
    unsigned long       num_records;

} ct_records;


void
ct_record_error(
    long double error,
    const char *test_name)
{
    if (ct_records.num_records == MAX_ERROR_RECORDS) {
        ctest_log("Cannot record test error for %s, max records reached.\n",
                  test_name);
        return;
    }
    ct_records.records[ct_records.num_records].error = error;
    ct_records.records[ct_records.num_records].test_name = test_name;
    ct_records.num_records++;
}

