include(CodeCoverage)

set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

add_executable(
    ctest
    ctest.c
    sptest.c)

setup_target_for_coverage(ctest_coverage ctest coverage)
