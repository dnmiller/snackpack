from cffi import FFI


class LibInterface(object):
    """
    """
    def __init__(self, libname, headername, func_prefix=None,
                 strip_prefix=False):
        # Some preprocessor lines in headers confuse CFFI (or more accurately
        # pycparser).
        with open(headername, 'r') as header_file:
            header = ''.join(l for l in header_file if not l.startswith('#'))

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
        return self._func.__repr__()
