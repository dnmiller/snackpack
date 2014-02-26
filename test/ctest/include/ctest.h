#ifndef _CTEST_H_
#define _CTEST_H_


void
ctest_log_result(
    long double err,
    long double tol,
    const char *func);

int
ctest_end_test(void);


void
ct_assert_almost_eq(
    long double expected,
    long double actual,
    long double tolerance,
    const char *msg);


void
ct_assert_eq(
    long double expected,
    long double actual,
    const char *msg);


extern bool exit_on_fail;


#define ctest_log(...) printf(__VA_ARGS__)


#endif
