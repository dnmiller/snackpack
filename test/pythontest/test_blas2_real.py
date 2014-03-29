from itertools import product

import numpy as np
from numpy.random import randn
from numpy.testing import (
    assert_equal, assert_array_equal, assert_array_almost_equal_nulp,
    assert_almost_equal, assert_allclose)

from snackpack import blas
from snackpack.util import (FloatArray, matrix_generator)
from snackpack.blas import SP_TRANS


def test_sgemv_no_trans():
    """Test sp_blas_sgemv with no transpose"""
    alpha = randn(8)
    alpha[0] = 0
    beta = randn(8)
    beta[0] = 0
    for a, b in product(alpha, beta):
        for rows, cols, A in matrix_generator():
            x = FloatArray(randn(cols))
            y = FloatArray(randn(rows))

            x0 = x.copy()
            y0 = y.copy()

            expected = a * np.reshape(A, (rows, cols), 'F').dot(x) + b * y
            blas.sgemv(SP_TRANS.NONE, rows, cols, a, A, 1, x, 1, b, y, 1)

            assert_array_equal(x0, x)
            assert_allclose(expected, y, 1e-5, 5e-5)


def test_sgemv_with_trans():
    """Test sp_blas_sgemv with transpose"""
    alpha = randn(8)
    alpha[0] = 0
    beta = randn(8)
    beta[0] = 0
    for a, b in product(alpha, beta):
        for rows, cols, A in matrix_generator():
            x = FloatArray(randn(rows))
            y = FloatArray(randn(cols))

            x0 = x.copy()
            y0 = y.copy()

            expected = a * np.reshape(A, (rows, cols), 'F').T.dot(x) + b * y
            blas.sgemv(SP_TRANS.TRANSPOSE, rows, cols, a, A, 1, x, 1, b, y, 1)

            assert_array_equal(x0, x)
            assert_allclose(expected, y, 1e-5, 5e-5)
