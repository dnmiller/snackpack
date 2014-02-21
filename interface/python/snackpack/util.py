import ctypes
import struct

import numpy as np

float_t = ctypes.c_float
float_t_ptr = ctypes.POINTER(float_t)
len_t = ctypes.c_uint8
inc_t = ctypes.c_int8

# TODO: .....
assert ctypes.sizeof(float_t) == 4

float_t_dtype = np.float32

FloatArray = lambda x: np.array(x, dtype=float_t_dtype)

float_t_min = np.finfo(float_t_dtype).min
float_t_max = np.finfo(float_t_dtype).max

finfo = np.finfo(float_t_dtype)


def numpy_to_ptr_with_offset(npobj, ptr_type, offset):
    shift = npobj.itemsize * offset
    addr = ctypes.addressof(npobj.ctypes.data_as(ptr_type).contents)
    return ctypes.cast(addr + shift, ptr_type)


def library_function(clib, fname, restype, argtypes, doc):
    libfunc = getattr(clib, fname)
    libfunc.restype = restype
    libfunc.argtypes = argtypes

    def func(*args):
        libargs = []
        if len(args) != len(func.argtypes):
            raise ValueError('Invalid number of arguments')

        for arg, new_type in zip(args, func.argtypes):
            if type(arg) == new_type:
                libargs.append(arg)
            elif hasattr(arg, 'ctypes'):
                libargs.append(arg.ctypes.data_as(new_type))
            else:
                # TODO: This can result in some really dumb casting stuff.
                libargs.append(new_type(arg))

        return libfunc(*libargs)

    func.__name__ = fname
    func.__doc__ = doc
    func.argtypes = argtypes
    func.restype = restype
    return func


def _valid_range(c_type):
    """Determine the valid range of a c_type. Only works for integer types.
    """
    fmt = c_type._type_
    size = struct.calcsize(fmt)
    a = struct.unpack(fmt, ("\x00" * 32)[:size])[0]
    b = struct.unpack(fmt, ("\xFF" * 32)[:size])[0]
    c = struct.unpack(fmt, ("\x7F" + "\x00" * 32)[:size])[0]
    d = struct.unpack(fmt, ("\x80" + "\xFF" * 32)[:size])[0]
    return min(a, b, c, d), max(a, b, c, d)


len_t_max = _valid_range(len_t)[1]
inc_t_max = _valid_range(inc_t)[1]
