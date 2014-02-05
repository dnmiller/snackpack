import ctypes
from ctypes import c_float, c_int

float_t     = ctypes.c_float
float_t_ptr = ctypes.POINTER(float_t)
len_t       = ctypes.c_uint
inc_t       = ctypes.c_int

FloatArray = lambda x: np.array(x, dtype=np.float32)


def library_function(fname, restype, argtypes, doc):
    libfunc = getattr(_libsp, 'sp_blas_' + fname)
    libfunc.restype = restype
    libfunc.argtypes = argtypes

    def func(*args):
        libargs = []
        for arg, type in zip(args, func._argtypes):
            if hasattr(arg, 'ctypes'):
                libargs.append(arg.ctypes.data_as(type))
            else:
                libargs.append(type(arg))
        return func._libfunc(*libargs)

    func.__name__ = fname
    func.__doc__ = doc
    func._libfunc = libfunc
    func._argtypes = argtypes
    return func


