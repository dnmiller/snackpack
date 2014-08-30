import inspect, os
from cffi import FFI

# TODO: Make this platform independent
_libpath = '../../lib/libsnackpack.dylib'

# Find the header directory.
this_file = inspect.getfile(inspect.currentframe())
this_dir = os.path.dirname(os.path.abspath(this_file))
header_dir = os.path.join(this_dir, 'headers')

blas_header_list = [
    'blas1_real_pyparsed.h']


ffi = FFI()
clean_header = ''
with open(os.path.join(header_dir, header_list[0])) as header:
    clean_header = ''.join(
        line for line in header if not line.startswith('#'))


ffi.cdef(clean_header)
lib = ffi.dlopen(_libpath)


def get_funcs_from_ffi(ffi):
    # This is a bit of a hack and may not work in future CFFI updates.
    return [f.lstrip('function ') for f in ffi._parser._declarations.keys()
                                  if f.startswith('function')]


class _BLAS(object):
    def __init__(self, ffi):
        for f in get_funcs_from_ffi(ffi):
            setattr(self, f.lstrip('sp_blas_'), getattr(lib, f))


blas = _BLAS(ffi)
