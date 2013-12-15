# Build a library to use for unit testing.
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

# Compiler flags for the standard build should be placed here.
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

# Set the directory for "make install" to place the binary file.
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
