cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME snackpack)
project(${PROJECT_NAME})

# Tell cmake where to find the files that tell it where to find files. The
# cmake subdirectory should contain .cmake files that define custom
# functions and Find*.cmake files that tell cmake where to look for
# libraries. This must be done before the find_package commands.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


# Source files for the standard build. These are defined here so that the
# test suite knows which source files to compile against. Compiler options
# for the standard build are in the CMakeLists.txt file in the src
# subdirectory.
set(PROJECT_SOURCES
    blas1_real.c
    blas1_real_internal.c
    error.c
)

# Local header files
include_directories(include)

# Project source files
add_subdirectory(src)

# Python / Matlab interfaces
add_subdirectory(interface)

# Test source files
add_subdirectory(test)
