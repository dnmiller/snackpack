import ctypes
import numpy as np

float_t = ctypes.c_float
float_t_ptr = ctypes.POINTER(float_t)
len_t = ctypes.c_uint
inc_t = ctypes.c_int

FloatArray = lambda x: np.array(x, dtype=np.float32)


def library_function(clib, fname, restype, argtypes, doc):
    libfunc = getattr(clib, fname)
    libfunc.restype = restype
    libfunc.argtypes = argtypes

    def func(*args):
        libargs = []
        if len(args) != len(func.argtypes):
            raise ValueError('Invalid number of arguments')

        for arg, type in zip(args, func.argtypes):
            if hasattr(arg, 'ctypes'):
                libargs.append(arg.ctypes.data_as(type))
            else:
                # TODO: This can result in some really dumb casting stuff.
                libargs.append(type(arg))

        return libfunc(*libargs)

    func.__name__ = fname
    func.__doc__ = doc
    func.argtypes = argtypes
    func.restype = restype
    return func
