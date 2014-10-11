from subprocess import Popen, PIPE
from cffi import FFI


def pycparsify_headers(header_list, include_paths):
    """Given some headers, try to clean them up for pycparser.
    """
    includes = ['-I' + i for i in include_paths]
    pyc_header = ''
    for header in header_list:
        # This works for clang on OS X
        args = ['cpp'] + includes + ['-DPYCPARSER_SCAN', header]
        cpp = Popen(args, stdout=PIPE)
        # pycparser doesn't like some preprocessor output, so we scrub any
        # remaining lines
        pyc_header += ''.join(
            l for l in cpp.stdout.readlines() if not l.startswith('#'))
    return pyc_header


class LibInterface(object):
    """Library interface to C ABI

    Given a shared library file and a header file, this returns an object
    with CFFI-wrapped functions from the header as attributes. It's intended to
    act like a Python module with the exported C functions as module-level
    functions.

    It can optionally restrict loaded functions to those with a given prefix and
    remove the prefix if desired, allowing for more concise names than typical
    c_style_naming_conventions.
    """
    def __init__(self, libname, header, func_prefix=None,
                 strip_prefix=False):
        # Load the DLL via CFFI.
        ffi = FFI()
        ffi.cdef(header)
        dll = ffi.dlopen(libname)

        # Scan for a list of functions that begin with func_prefix.
        if func_prefix is None:
            func_prefix = ''

        # This is a hack into the CFFI implementation and may break in future
        # CFFI versions.
        funcs = [
            f[len('function '):] for f in ffi._parser._declarations.keys()
            if f.startswith('function ' + func_prefix)]

        # Bind the functions from dll to self.
        for func in funcs:
            attr_name = func
            if strip_prefix:
                attr_name = attr_name[len(func_prefix):]
            setattr(self, attr_name, WrappedCFunction(ffi, dll, func))


class WrappedCFunction(object):
    """Wrap a CFFI-imported function so that it automatically casts numpy
    arrays into the correct pointer type.

    The wrapped function is still agnostic of the precision of the contents of
    the array. I.e., functions that take 32-bit floats will not complain if
    provided numpy arrays with dtype=float64, but the answer will be wrong.
    """
    def __init__(self, ffi, dll, function_name):
        """Initialize a wrapped CFFI-imported function.
        """
        self._ffi = ffi
        dll_full_name = 'function ' + function_name
        if dll_full_name not in ffi._parser._declarations:
            raise RuntimeError('Function name has not been parsed.')

        signature = ffi._parser._declarations[dll_full_name]
        self._arg_types = map(lambda x: x.get_c_name(), signature.args)
        self._func = getattr(dll, function_name)

    def __call__(self, *args):
        casted = []
        for arg, arg_type in zip(args, self._arg_types):
            try:
                casted.append(self._ffi.cast(arg_type, arg.ctypes.data))
            except (TypeError, AttributeError):
                casted.append(self._ffi.cast(arg_type, arg))
        return self._func(*casted)

    def __repr__(self):
        """Return a string representation of the function. This calls the
        wrapped CFFI function's __repr__ method.
        """
        return self._func.__repr__()
