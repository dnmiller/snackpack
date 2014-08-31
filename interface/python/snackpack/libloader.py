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

        # Load the DLL via CFFI
        ffi = FFI()
        ffi.cdef(header)
        self._dll = ffi.dlopen(libname)

        # Scan for a list of functions that begin with fun_prefix
        if func_prefix is None:
            func_prefix = ''

        # This is a hack into the CFFI implementation and may break in future
        # CFFI versions.
        funcs = [f[len('function '):]
                 for f in ffi._parser._declarations.keys()
                 if f.startswith('function ' + func_prefix)]

        for func in funcs:
            attr_name = func
            if strip_prefix:
                attr_name = attr_name[len(func_prefix):]
                print(attr_name)
            setattr(self, attr_name, getattr(self._dll, func))
