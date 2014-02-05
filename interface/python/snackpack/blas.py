import ctypes
import numpy as np


from util import float_t, float_t_ptr, len_t, inc_t, library_function


# TODO: Clean this up...
_libsp = ctypes.cdll.LoadLibrary('../../bin/libsnackpack.dylib')


sasum = library_function(
    'sasum', float_t, [len_t, float_t_ptr, inc_t],
    """Sum an array of numbers (single-precision).
    """)
