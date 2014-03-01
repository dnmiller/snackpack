#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "ctest.h"

/**
 * If true, then the test will exit immediately on the first failure.
 * (default true)
 */
bool ct_exit_on_fail = true;

/**
 * If true, then messages are printed to ct_log while tests are running.
 * (default true)
 */
bool ct_log_verbose = true;

/**
 * If true, then all differences between expected and actual results are
 * logged. These can be printed with ct_print_errors. (default true)
 */
bool ct_log_numeric_error = true;

/* Exit status */
static int result = 0;

/* Number of calls to assert_* that have failed. */
static unsigned int fail_count = 0;

/* Fed to qsort. */
static int
compare_errors(
    const void *r1,
    const void *r2);


void
ct_log_result(
    long double err,
    long double tol,
    const char *func)
{
    if (err > tol) {
        ct_log("%-20s | %Le | %Le | !!!FAIL!!!\n", func, err, tol);
        result = -1;
    } else {
        ct_log("%-20s | %Le | %Le | pass\n", func, err, tol);
    }
}


void
ct_assert(
    bool cond,
    const char *test_name)
{
    if (!cond) {
        ct_log("Test failed: %s\n", test_name);
        result = -1;
    }
}


void
ct_assert_eq(
    long double expected,
    long double actual,
    const char *test_name)
{
    if (expected != actual) {
        fail_count++;
        ct_log("\n"
            "Test failed: %s\n"
            "-----------\n"
            "    Expected: %Le\n"
            "    Actual:   %Le\n", test_name, expected, actual);
        long double err = expected > actual ?
                          expected - actual : actual - expected;
        if (ct_log_numeric_error) {
            ct_record_error(err, test_name);
        }
        if (ct_log_verbose) {
            ct_log_result(err, 0.0, test_name);
        }
        if (ct_exit_on_fail) {
            exit(-1);
        }
    } else {
        ct_log(".");
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
        ct_log("\n"
            "Test failed: %s\n"
            "-----------\n"
            "    Expected: %Le\n"
            "    Actual:   %Le\n"
            "    Error:    %Le\n", test_name, expected, actual, err);
        if (ct_log_numeric_error) {
            ct_record_error(err, test_name);
        }
        if (ct_log_verbose) {
            ct_log_result(err, 0.0, test_name);
        }
        if (ct_exit_on_fail) {
            exit(-1);
        }
    } else {
        ct_log(".");
    }
}


/* Record for numerical error.
 *
 * This is a trivial little system for logging test results. All results are
 * stored in a big statically allocated array.
 */
typedef struct {

    const char *test_name;
    long double error;

} ct_error_record;

static unsigned long num_records = 0;
static ct_error_record records[CT_MAX_ERROR_RECORDS];

static int
compare_errors(
    const void *r1,
    const void *r2);


/**
 * Record a test result.
 *
 * \param error     Numerical error in result.
 * \param test_name Name of test.
 *
 * The accumulated errors are printed in a table by the function
 * ct_print_errors.
 */
void
ct_record_error(
    long double error,
    const char *test_name)
{
    if (num_records == CT_MAX_ERROR_RECORDS) {
        ct_log("Cannot record test error for %s, max records reached.\n",
                  test_name);
        return;
    }
    records[num_records].error = error;
    records[num_records].test_name = test_name;
    num_records++;
}


/* Comparison function passed to qsort. */
static int
compare_errors(
    const void *r1,
    const void *r2)
{
    ct_error_record *e1 = (ct_error_record *)r1;
    ct_error_record *e2 = (ct_error_record *)r2;
    if (e1->error < e2->error) {
        return 1;
    } else if (e1->error > e2->error) {
        return -1;
    } else {
        return 0;
    }
}


/**
 * Print a list of all ctest results, sorted by largest to smallest error.
 */
void
ct_print_errors(void)
{
    if (num_records == 0) {
        ct_log("No tests results were logged.\n");
    }

    /* First sort the errors from greatest to least. */
    qsort(records, num_records, sizeof(ct_error_record), compare_errors);

    for (unsigned long i = 0; i < num_records; i++) {
        ct_log("%-40s: %Le\n", records[i].test_name, records[i].error);
    }
    ct_log(
"----------------------------------------------------------------------------\n");
}
