# Include custom Python test CMake functions.
include(PythonLibTest)

set(PYTHON_INTERFACE_DIRECTORY snackpack)

#add_interface_target(python_interface ${PYTHON_INTERFACE_DIRECTORY})

set(PYTHON_INTERFACE_SOURCES
    __init__.py
    blas.py
    util.py
)

add_interface_target(
    python_interface 
    snackpack
    ${PYTHON_INTERFACE_DIRECTORY}
    ${PYTHON_INTERFACE_SOURCES})
