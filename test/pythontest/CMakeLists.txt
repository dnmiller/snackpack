# Include custom Python test CMake functions.
include(PythonLibTest)

# All sources to be included in the unit test suite. These are copied into a
# local directory in the build tree along side a binary.
set(PYTHON_TEST_SOURCES
    test_blas1_real.py
)

add_python_test_target(
    mathtest snackpack python_interface ${PYTHON_TEST_SOURCES})
