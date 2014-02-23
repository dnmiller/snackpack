cmake_minimum_required(VERSION 2.8)

# Compiler flags for the standard build should be placed here.
set(CMAKE_C_FLAGS "-Weverything -Wall -Wextra -O3")

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
)

# Local header files are placed in the include directory.
include_directories(include)

# The add_subdirectory commands should be last.

# Project source files should be placed in the src subdirectory.
add_subdirectory(src)

# Test source files should be placed in the test subdirectory.
add_subdirectory(test)

# Python / Matlab interfaces
add_subdirectory(interface)
