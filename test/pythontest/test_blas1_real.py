import nose.tools as nt
import numpy.testing as nptest
import numpy as np

from snackpack import blas
from snackpack.util import (FloatArray, finfo, float_t_dtype, len_t_max,
                            float_t)


# TODO: determine clean way of checking dimensions

def test_sasum():
    """Test sp_blas_sasum"""
    def assert_same_result(vec, numel, inc):
        arg = FloatArray(vec)
        if inc == 0:
            expected = 0
        else:
            expected = sum(map(abs, vec[::inc][:numel]))

        result = blas.sasum(numel, arg, inc)
        nt.assert_equal(expected, result)

    vec = list(range(0, 5) + range(6, -10, -2))
    inc = range(0, 2, 1)
    for i in inc:
        n = len(vec) / abs(i) if i > 0 else 1
        assert_same_result(vec, n, i)

    # Test the 1-norm of 1 and eps
    for eps in (finfo.eps, finfo.epsneg):
        arg = FloatArray([1, eps])
        res = blas.sasum(2, arg, 1)
        exp = float_t_dtype(1 + eps if eps > 0 else 1 - eps)
        nt.assert_equal(res, exp)

    # Test two tiny floats
    arg = FloatArray([finfo.tiny] * 2)
    res = blas.sasum(2, arg, 1)
    exp = finfo.tiny * 2
    nt.assert_equal(res, exp)

    # Min has sign correctly switched.
    arg = FloatArray([finfo.min])
    res = blas.sasum(1, arg, 1)
    exp = -finfo.min
    nt.assert_equal(res, exp)

    # Overflow handled correctly (?)
    arg = FloatArray([finfo.max] * 2)
    res = blas.sasum(2, arg, 1)
    exp = float_t_dtype(finfo.max * 2)
    nt.assert_equal(res, exp)

    # Can handle largest size
    vec = range(0, len_t_max)
    arg = FloatArray(vec)
    res = blas.sasum(len_t_max, arg, 1)
    exp = sum(vec)
    nt.assert_equal(res, exp)


def test_saxpy():
    """Test sp_blas_saxpy"""
    alpha = 0.2
    x = FloatArray([1, 2, 3])
    inc_x, inc_y = 1, 1
    y = FloatArray([2, 3, 4])
    z = alpha * x + y
    blas.saxpy(x.shape[0], alpha, x, inc_x, y, inc_y)
    nptest.assert_array_equal(z, y)


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
        # more accurate.
        nptest.assert_almost_equal(
            a * c_f.value + b * s_f.value, a_f.value, decimal=5)
        nptest.assert_almost_equal(
            -a * s_f.value + b * c_f.value, 0, decimal=6)
