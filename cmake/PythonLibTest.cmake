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


function(add_interface_target TARGET_NAME SHARED_LIB TARGET_DIR)
    set(IFACE_COPY_DIR ${CMAKE_BINARY_DIR}/${TARGET_NAME}/${TARGET_DIR})

    add_custom_target(${TARGET_NAME} COMMENT "Copying ${TARGET_NAME} files")

    foreach(isource ${ARGN})
        add_custom_command(
            TARGET ${TARGET_NAME}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${IFACE_COPY_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_DIR}/${isource} 
            ${IFACE_COPY_DIR}/${isource})
    endforeach()

    set(COPY_TARGET copy_${SHARED_LIB}_${TARGET_NAME})

    add_custom_target(${COPY_TARGET} 
        COMMENT "Copying ${SHARED_LIB} shared library")

    # Add a command to copy the target library into the same folder as the
    # python files.
    add_custom_command(
        TARGET ${COPY_TARGET}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SHARED_LIB}>
        ${IFACE_COPY_DIR}/$<TARGET_FILE_NAME:${SHARED_LIB}>)

    # Make sure there's a lib to copy.
    add_dependencies(${COPY_TARGET} ${SHARED_LIB})
    add_dependencies(${TARGET_NAME} ${COPY_TARGET})
endfunction()


function(add_python_test_target TARGET_NAME TARGET_LIB IFACE_TARGET)
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
            --with-path=${CMAKE_BINARY_DIR}/${IFACE_TARGET}
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

    # Make the copy target a dependency of the testing target to ensure it
    # gets done first.
    add_dependencies(${TARGET_NAME} ${COPY_TARGET})
    add_dependencies(${TARGET_NAME} ${IFACE_TARGET})
endfunction()
