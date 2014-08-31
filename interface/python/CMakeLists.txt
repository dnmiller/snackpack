# Include custom Python test CMake functions.
include(PythonLibTest)

set(PYTHON_INTERFACE_DIRECTORY snackpack)

set(PYINTERFACE_INSTALL_DIR
    ${CMAKE_SOURCE_DIR}/bin/interface/python/snackpack)

#add_interface_target(python_interface ${PYTHON_INTERFACE_DIRECTORY})

set(PYTHON_INTERFACE_SOURCES
    snackpack/__init__.py
    snackpack/util.py
    snackpack/libloader.py
)

install(
    FILES ${PYTHON_INTERFACE_SOURCES}
    DESTINATION ${PYINTERFACE_INSTALL_DIR})


add_custom_command(
    OUTPUT __blas1_real_preparsed.h
    COMMAND "${CMAKE_C_COMPILER}" 
        -E ${CMAKE_SOURCE_DIR}/include/snackpack/blas1_real.h 
        -I ${CMAKE_SOURCE_DIR}/include
        -o ${PYINTERFACE_INSTALL_DIR}/headers/__blas1_real_preparsed.h
        -D_PYCPARSER_SCAN_
    )

add_custom_target(
    process_headers ALL
    DEPENDS __blas1_real_preparsed.h
    COMMENT "Doin it")

