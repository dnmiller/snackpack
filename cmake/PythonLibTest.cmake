# Custom cmake target for running nosetests on compiled libraries.
#
# This creates a custom target for running nosetests with the intent of
# verifying a compiled library using interpreted Python unit tests. It is
# meant to be used in addition to, not in lieu of, compiled unit test code.
# This can be particularly useful for math-heavy code where a compiled unit
# test could be quite cumbersome. The Python test code is responsible for
# importing the libraries (probably using ctypes).
#
# Usage: 
# 
#   add_python_test_target(TARGET_NAME LIBRARY_DEPENDENCY SOURCE_FILES)
#
# Released into the public domain. No warranty implied.

find_program(NOSETESTS_PATH nosetests)
if(NOT NOSETESTS_PATH)
    message(WARNING 
        "nosetests not found! Python library tests will not be available.")
endif()


function(add_copy_target TARGET_NAME TARGET_SOURCES)
endfunction()


function(add_python_test_target TARGET_NAME TARGET_LIB)
    # Try again to find nosetests here. We may have switched virtualenvs or
    # something since first running cmake.
    find_program(NOSETESTS_PATH nosetests)
    if(NOT NOSETESTS_PATH)
        message(FATAL_ERROR "nosetests not found! Aborting...")
    endif()

    # This is where we copy the files. Make up a directory.
    set(COPY_DIR ${CMAKE_BINARY_DIR}/${TARGET_NAME}_files)

    add_custom_target(${TARGET_NAME}
        COMMAND ${NOSETESTS_PATH}
        WORKING_DIRECTORY ${COPY_DIR}
        DEPENDS ${TARGET_LIB} ${SOURCE_FILES}
        COMMENT "Running ${TARGET_NAME}")

    set(COPY_TARGET copy_${TARGET_NAME}_files)

    add_custom_target(${COPY_TARGET} COMMENT "Copying ${TARGET_NAME} files")

    foreach(pysource ${ARGN})
        add_custom_command(
            TARGET ${COPY_TARGET}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${COPY_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy 
            ${CMAKE_CURRENT_SOURCE_DIR}/${pysource} ${COPY_DIR})
    endforeach()

    get_target_property(TARGET_LIB_NAME ${TARGET_LIB} LOCATION)

    # Add a command to copy the target library into the same folder as the
    # python files.
    add_custom_command(
        TARGET ${COPY_TARGET}
        COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_LIB_NAME} ${COPY_DIR})

    # Make the copy target a dependency of the testing target to ensure it
    # gets done first.
    add_dependencies(${TARGET_NAME} ${TARGET_LIB})
    add_dependencies(${TARGET_NAME} ${COPY_TARGET})
endfunction()
