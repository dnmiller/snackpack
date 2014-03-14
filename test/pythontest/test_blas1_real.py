from itertools import product, izip

import nose.tools as nt
from numpy.testing import (
        assert_array_equal, assert_array_almost_equal_nulp,
        assert_almost_equal)

import numpy as np
from numpy.random import randint, randn

from snackpack import blas
from snackpack.util import (
    FloatArray, finfo, float_t_dtype, len_t_max, float_t, len_t,
    indexed_vector)


def randint_vector_generator(max_int=int(1e3), max_size=1e4):
    def generator():
        size = 1
        while size <= max_size:
            yield FloatArray(randint(-max_int, max_int, size=size))
            size += min([10**(size/10), 1000])
    return generator


def randn_vector_generator(max_size=1e4):
    def generator():
        size = 1
        while size <= max_size:
            yield FloatArray(randn(size))
            size += min([10**(size/10), 1000])
    return generator



increments = (-3, -2, -1, 1, 2, 3)


def vector_set(max_size=1e3, increments=increments):
    for inc in increments:
        size = 1
        while size <= max_size:
            x = FloatArray(randn(size))
            n = len(x) / abs(inc)
            if n != 0:
                idx = indexed_vector(x, n, inc)
                yield [n, x, inc, idx]
            size += min([10**(size/10), 1000])


def assert_nonindexed_unchanged(original, new, n, inc):
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


def test_sasum():
    """Test sp_blas_sasum"""
    for n, x, inc, idx in vector_set():
        x0 = x.copy()
        expected = sum(map(abs, idx))
        result = blas.sasum(n, x, inc)
        assert_almost_equal(expected/result, 1.0, decimal=5)
        assert_array_equal(x0, x)


def test_saxpy():
    """Test sp_blas_saxpy"""
    alpha = randn(50)
    for a in alpha:
        for x_param, y_param in izip(vector_set(), vector_set()):
            assert x_param[0] == y_param[0]
            n, x, inc_x, x_idx = x_param
            y, inc_y, y_idx = y_param[1:]
            y0 = y.copy()
            # Compute the expected from the indexed versions
            expected = a * x_idx + y_idx
            blas.saxpy(n, a, x, inc_x, y, inc_y)
            # Re-index y to find the result
            result = indexed_vector(y, n, inc_y)
            assert_array_almost_equal_nulp(expected, result)
            assert_nonindexed_unchanged(y0, y, n, inc_y)


def test_srotg():
    """Test sp_blas_srotg"""
    # TODO: Find some pathological cases
    for a, b in np.random.randn(1000, 2):
        a_f = float_t(a)
        b_f = float_t(b)
        c_f = float_t()
        s_f = float_t()
        blas.srotg(a_f, b_f, c_f, s_f)
        # Kind of week precision here - LAPACK reference implementation is
        # more accurate than BLAS implementation.
        assert_almost_equal(
            a * c_f.value + b * s_f.value, a_f.value, decimal=5)
        assert_almost_equal(
            -a * s_f.value + b * c_f.value, 0, decimal=6)


def test_srot():
    """Test sp_blas_srot"""
    inc_x = len_t(1)
    inc_y = len_t(1)
    n = len_t(100)

    # TODO: find some pathological cases
    # TODO: clean this up
    for c, s in np.random.randn(10, 2):
        c_f = float_t(c)
        s_f = float_t(s)
        x = FloatArray(np.random.randn(n.value, 1))
        y = FloatArray(np.random.randn(n.value, 1))
        x_res, y_res = [], []
        for i in range(n.value):
            x_res.append(c_f * x[i] + s_f * y[i])
            y_res.append(c_f * y[i] - s_f * x[i])
        x_res = FloatArray(x_res)
        y_res = FloatArray(y_res)
        blas.srot(n, x, inc_x, y, inc_y, c_f, s_f)
