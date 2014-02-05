import ctypes
import numpy as np
import os
import inspect


from util import float_t, float_t_ptr, len_t, inc_t, library_function


_libsp_dir = os.path.dirname(
        os.path.abspath(
            inspect.getfile(
                inspect.currentframe())))
# TODO: portable filename
_libsp_filename = os.path.join(_libsp_dir, 'libsnackpack.dylib')

libsp = ctypes.cdll.LoadLibrary(_libsp_filename)


sasum = library_function(
    libsp, 'sasum', float_t, [len_t, float_t_ptr, inc_t],
    """Sum an array of numbers (single-precision).
    """)
