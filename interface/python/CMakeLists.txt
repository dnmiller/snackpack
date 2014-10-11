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
