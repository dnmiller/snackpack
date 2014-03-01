# Compiler flags for the standard build
set(CMAKE_C_FLAGS "-Weverything -Wall -Wextra -O3")

# Build a library to use for unit testing
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

# Set the directory for "make install" to place the binary file
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
