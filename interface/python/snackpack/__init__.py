import inspect
import os

from libloader import LibInterface


# TODO: Make this platform independent
_libpath = '../../lib/libsnackpack.dylib'

# Find the header directory.
this_file = inspect.getfile(inspect.currentframe())
this_dir = os.path.dirname(os.path.abspath(this_file))
header_dir = os.path.join(this_dir, 'headers')

header_list = [
    '__blas1_real_preparsed.h'
]

header_file = os.path.join(header_dir, header_list[0])

blas = LibInterface(_libpath, header_file, 'sp_blas_', strip_prefix=True)
