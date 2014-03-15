from itertools import izip

from numpy.testing import (
    assert_equal, assert_array_equal, assert_array_almost_equal_nulp,
    assert_almost_equal)

import numpy as np
from numpy.random import randn

from snackpack import blas
from snackpack.util import (
    FloatArray, float_t, len_t, indexed_vector)


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
                yield [n, x, inc, idx]
            size += min([10 ** (size / 10), 1000])


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


def test_sasum():
    """Test sp_blas_sasum"""
    for n, x, inc, idx in vector_generator():
        x0 = x.copy()
        expected = sum(map(abs, idx))
        result = blas.sasum(n, x, inc)
        assert_almost_equal(expected / result, 1.0, decimal=5)
        assert_array_equal(x0, x)


def test_saxpy():
    """Test sp_blas_saxpy"""
    alpha = randn(50)
    for a in alpha:
        for n, x, inc_x, x_idx, y, inc_y, y_idx in double_vector_generator():
            y0 = y.copy()
            # Compute the expected from the indexed versions
            expected = a * x_idx + y_idx
            blas.saxpy(n, a, x, inc_x, y, inc_y)
            assert_array_almost_equal_nulp(expected, y_idx)
            assert_nonindexed_unchanged(y0, y, n, inc_y)


def test_sswap():
    """Test sp_blas_sswap"""
    for n, x, inc_x, x_idx, y, inc_y, y_idx in double_vector_generator():
        x0 = x.copy()
        y0 = y.copy()

        # First make fresh array copies and find the result.
        x_res = x.copy()
        y_res = y.copy()
        blas.sswap(n, x_res, inc_x, y_res, inc_y)
        x_res_idx = indexed_vector(x_res, n, inc_x)
        y_res_idx = indexed_vector(y_res, n, inc_y)

        # Find the expected. Remember that _idx vectors are slices here.
        tmp = x_idx.copy()
        x_idx[:] = y_idx[:]
        y_idx[:] = tmp

        assert_array_equal(x_idx, x_res_idx)
        assert_array_equal(y_idx, y_res_idx)
        assert_nonindexed_unchanged(x0, x, n, inc_x)
        assert_nonindexed_unchanged(y0, y, n, inc_y)


def test_scopy():
    """Test sp_blas_scopy"""
    for n, x, inc_x, x_idx, y, inc_y, y_idx in double_vector_generator():
        x0 = x.copy()
        y0 = y.copy()

        # First make fresh array copies and find the result.
        x_res = x.copy()
        y_res = y.copy()
        blas.scopy(n, x_res, inc_x, y_res, inc_y)
        x_res_idx = indexed_vector(x_res, n, inc_x)
        y_res_idx = indexed_vector(y_res, n, inc_y)

        # Find the expected.
        y_idx[:] = x_idx[:]

        assert_array_equal(x_idx, x_res_idx)
        assert_array_equal(y_idx, y_res_idx)
        assert_nonindexed_unchanged(x0, x, n, inc_x)
        assert_nonindexed_unchanged(y0, y, n, inc_y)


def test_sdot():
    """Test sp_blas_sdot"""
    for n, x, inc_x, x_idx, y, inc_y, y_idx in double_vector_generator():
        x0 = x.copy()
        y0 = y.copy()
        expected = np.dot(x_idx, y_idx)
        result = blas.sdot(n, x, inc_x, y, inc_y)
        assert_almost_equal(expected / result, 1.0, decimal=4)
        assert_array_equal(x, x0)
        assert_array_equal(y, y0)


def test_sdsdot():
    """Test sp_blas_sdsdot"""
    for n, x, inc_x, x_idx, y, inc_y, y_idx in double_vector_generator():
        base = randn()
        x0 = x.copy()
        y0 = y.copy()
        expected = np.dot(x_idx, y_idx) + base
        result = blas.sdsdot(n, base, x, inc_x, y, inc_y)
        # I think we're actually testing the NumPy precision more than SP
        assert_almost_equal(expected / result, 1.0, decimal=4)
        assert_array_equal(x, x0)
        assert_array_equal(y, y0)


def test_snrm2():
    """Test sp_blas_snrm2"""
    for n, x, inc_x, x_idx in vector_generator():
        x0 = x.copy()
        expected = np.linalg.norm(x_idx)
        result = blas.snrm2(n, x, inc_x)
        assert_almost_equal(expected / result, 1.0, decimal=6)
        assert_array_equal(x, x0)


def test_sscal():
    """Test sp_blas_sscal"""
    alpha = randn(50)
    for a in alpha:
        for n, x, inc_x, x_idx in vector_generator():
            x0 = x.copy()
            x_res = x.copy()
            blas.sscal(n, a, x_res, inc_x)
            x_idx[:] = a * x_idx[:]
            assert_array_almost_equal_nulp(x_res, x)
            assert_nonindexed_unchanged(x0, x_res, n, inc_x)


def test_isamax():
    """Test sp_blas_isamax"""
    for n, x, inc_x, x_idx in vector_generator():
        x0 = x.copy()
        expected = np.where(np.amax(np.abs(x_idx)) == np.abs(x))[0][0]
        result = blas.isamax(n, x, inc_x)
        assert_equal(expected, result)
        assert_array_equal(x, x0)


def test_isamin():
    """Test sp_blas_isamin"""
    for n, x, inc_x, x_idx in vector_generator():
        x0 = x.copy()
        expected = np.where(np.amin(np.abs(x_idx)) == np.abs(x))[0][0]
        result = blas.isamin(n, x, inc_x)
        assert_equal(expected, result)
        assert_array_equal(x, x0)


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
