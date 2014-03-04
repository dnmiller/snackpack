#ifndef _CTEST_H_
#define _CTEST_H_

#include <stdbool.h>
#include <stdio.h>


#ifndef ct_log
/** Function used for all logging output. */
#define ct_log(...) printf(__VA_ARGS__)
#endif


#ifndef CT_MAX_ERROR_RECORDS
/** Maximum number of errors allowed to be recorded. */
#define CT_MAX_ERROR_RECORDS (10000)
#endif


void
ct_assert(
    bool cond,
    const char *test_name);


void
ct_assert_almost_eq(
    long double expected,
    long double actual,
    long double tolerance,
    const char *test_name);


void
ct_assert_eq(
    long double expected,
    long double actual,
    const char *test_name);


void
ct_assert_float_eq(
    float_t act,
    float_t exp,
    float_t fac,
    const char *test_name);


void
ct_assert_int_eq(
    unsigned int exp,
    unsigned int act,
    const char *test_name);


void
ct_log_result(
    long double err,
    long double tol,
    const char *func);


void
ct_print_errors(void);


void
ct_record_error(
    long double error,
    const char *test_name);


extern bool ct_exit_on_fail;
extern bool ct_log_verbose;
extern bool ct_log_numeric_error;


#endif
