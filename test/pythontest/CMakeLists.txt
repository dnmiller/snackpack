# Include custom Python test CMake functions.
include(PythonLibTest)

# All sources to be included in the unit test suite.
set(PYTHON_TEST_SOURCES
    test_blas1_real.py
)

add_python_test_target(mathtest snackpack ${PYTHON_TEST_SOURCES})
