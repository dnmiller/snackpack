include(CodeCoverage)

set(CMAKE_C_FLAGS "-g -O0 -DSP_NO_PRINT -fprofile-arcs -ftest-coverage")

add_executable(
    ctest
    ctest.c
    sblat1.c
    ../../../src/blas1_real.c
    ../../../src/error.c
    ../../../src/blas1_real_internal.c
)

setup_target_for_coverage(ctest_coverage ctest coverage)
