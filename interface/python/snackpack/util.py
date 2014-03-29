import ctypes
import struct
from itertools import izip

import numpy as np
from numpy.random import randn
from numpy.testing import assert_array_equal

float_t = ctypes.c_float
float_t_ptr = ctypes.POINTER(float_t)
len_t = ctypes.c_int32
int_t = ctypes.c_int

# TODO: .....
assert ctypes.sizeof(float_t) == 4

float_t_dtype = np.float32

FloatArray = lambda x: np.array(x, dtype=float_t_dtype)

float_t_min = np.finfo(float_t_dtype).min
float_t_max = np.finfo(float_t_dtype).max

finfo = np.finfo(float_t_dtype)


def indexed_vector(vec, n, inc):
    """Create a list representation of a vector indexed over a given
    increment.

    Given a list, a number of elements `n` and an increment `inc`, this
    returns a new list containing elements from the original list in an
    order that is equivalent to the indexing from the BLAS reference
    implementation.

    For example, if the BLAS function sasum is given the arguments

        x = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], n = 5, inc = -2

    then it will access the vector `x` as if it were the vector [8, 6, 4, 2,
    0]. This function will return a list equivalent to this vector.
    """
    if inc > 0:
        return vec[:n * inc:inc]
    elif inc < 0:
        return vec[(1 - n) * inc::inc]
    else:
        raise ValueError('Invalid increment')


def indexed_matrix(mat, n, m, inc):
    """Create a list of lists representing a matrix indexed over a given
    leading dimension.
    """


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
            elif hasattr(arg, 'value'):
                try:
                    libargs.append(new_type(arg.value))
                except TypeError:
                    # TODO: This can result in some really dumb casting stuff.
                    libargs.append(new_type(arg))
            else:
                libargs.append(new_type(arg))

        return libfunc(*libargs)

    func.__name__ = fname
    func.__doc__ = doc
    func.argtypes = argtypes
    func.restype = restype
    return func


single_increments = (-3, -2, -1, 1, 2, 3)
double_increments = [(-3, -2, -1, 1, 2, 3), (3, -2, 1, -1, 2, -3)]


def vector_generator(max_size=1e3, increments=single_increments):
    for inc in increments:
        size = 1
        while size <= max_size:
            x = FloatArray(randn(size))
            n = len(x) / abs(inc)
            if n != 0:
                idx = indexed_vector(x, n, inc)
                yield n, x, inc, idx
            size += min([10 ** (size / 10), 1000])


def matrix_generator(max_size=1e2):
    """Return a generator for random matrices"""
    lda = 1
    while lda <= max_size:
        cols = 1
        while cols <= max_size:
            A = FloatArray(randn(lda * cols))
            for rows in range(1, lda + 1, 2):
                yield lda, rows, cols, A
            cols += min([10 ** (cols / 10), 1000])
        lda += min([10 ** (lda / 10), 1000])


def square_matrix_generator(max_size=1e3):
    """Return a generator for random square matrices"""
    lda = 1
    while lda <= max_size:
        for order in range(1, lda + 1):
            A = FloatArray(randn(lda * order))
            yield lda, order, A
        lda += min([10 ** (lda / 10), 1000])


def double_vector_generator(max_size=1e3, increments=double_increments):
    x_gen = vector_generator(increments=double_increments[0])
    y_gen = vector_generator(increments=double_increments[1])
    for x_par, y_par in izip(x_gen, y_gen):
        n, x, inc_x, x_idx = x_par
        y, inc_y, y_idx = y_par[1:]
        yield n, x, inc_x, x_idx, y, inc_y, y_idx


def assert_nonindexed_unchanged(original, new, n, inc):
    """Assert that elements that should not be touched given the `n` and
    `inc` arguments remain the same as the original.
    """
    x0 = original.copy()
    x1 = new.copy()
    # Set the values that ought to have changed to 0
    if inc > 0:
        x0[:n * inc:inc] = 0.0
        x1[:n * inc:inc] = 0.0
    elif inc < 0:
        x0[(1 - n) * inc::inc] = 0.0
        x1[(1 - n) * inc::inc] = 0.0
    else:
        raise ValueError('Invalid increment')
    # Assert that the remaining elements are equal
    assert_array_equal(x0, x1)


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
