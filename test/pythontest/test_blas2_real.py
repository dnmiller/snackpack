from itertools import product

import numpy as np
from numpy.random import randn
from numpy.testing import (
    assert_equal, assert_array_equal, assert_array_almost_equal_nulp,
    assert_almost_equal, assert_allclose)

from snackpack import blas
from snackpack.util import (
    FloatArray, matrix_generator, square_matrix_generator, indexed_vector,
    assert_nonindexed_unchanged)
from snackpack.blas import SP_TRANS


vec_inc = (-3, -1, 1, 3)


def test_sgemv_no_trans():
    """Test sp_blas_sgemv with no transpose"""
    alpha = randn(3)
    alpha[0] = 0
    beta = randn(3)
    beta[0] = 0
    for a, b in product(alpha, beta):
        for lda, rows, cols, A in matrix_generator():
            x = FloatArray(randn(cols))
            y = FloatArray(randn(rows))

            A_slice = np.reshape(A, (lda, cols), 'F')[:rows, :]
            x0 = x.copy()
            y0 = y.copy()

            expected = a * A_slice.dot(x) + b * y
            blas.sgemv(False, rows, cols, a, A, lda, x, 1, b, y, 1)

            assert_array_equal(x0, x)
            assert_allclose(expected, expected, 1e-5, 5e-5)

    for lda, rows, cols, A in matrix_generator():
        for inc_x, inc_y in product(vec_inc, vec_inc):
            a = randn()
            b = randn()

            len_x = cols * abs(inc_x)
            len_y = rows * abs(inc_y)

            x = FloatArray(randn(len_x))
            y = FloatArray(randn(len_y))

            x_idx = indexed_vector(x, cols, inc_x)
            y_idx = indexed_vector(y, rows, inc_y)

            A_slice = np.reshape(A, (lda, cols), 'F')[:rows, :]
            x0 = x.copy()
            y0 = y.copy()

            expected = a * A_slice.dot(x_idx) + b * y_idx
            blas.sgemv(False, rows, cols, a, A, lda, x, inc_x, b, y, inc_y)

            assert_array_equal(x0, x)
            assert_allclose(expected, y_idx, 1e-5, 5e-5)
            assert_nonindexed_unchanged(y0, y, rows, inc_y)


def test_sgemv_with_trans():
    """Test sp_blas_sgemv with transpose"""
    alpha = randn(4)
    alpha[0] = 0
    beta = randn(4)
    beta[0] = 0
    for a, b in product(alpha, beta):
        for lda, rows, cols, A in matrix_generator():
            x = FloatArray(randn(rows))
            y = FloatArray(randn(cols))

            A_sliceT = np.reshape(A, (lda, cols), 'F')[:rows, :].T
            x0 = x.copy()
            y0 = y.copy()

            expected = a * A_sliceT.dot(x) + b * y
            blas.sgemv(True, rows, cols, a, A, lda, x, 1, b, y, 1)

            assert_array_equal(x0, x)
            assert_allclose(expected, y, 1e-5, 5e-5)

    for lda, rows, cols, A in matrix_generator():
        for inc_x, inc_y in product(vec_inc, vec_inc):
            a = randn()
            b = randn()

            x = FloatArray(randn(rows * abs(inc_x)))
            y = FloatArray(randn(cols * abs(inc_y)))

            x_idx = indexed_vector(x, rows, inc_x)
            y_idx = indexed_vector(y, cols, inc_y)

            A_slice = np.reshape(A, (lda, cols), 'F')[:rows, :].T
            x0 = x.copy()
            y0 = y.copy()

            expected = a * A_slice.dot(x_idx) + b * y_idx
            blas.sgemv(True, rows, cols, a, A, lda, x, inc_x, b, y, inc_y)

            assert_array_equal(x0, x)
            assert_allclose(expected, y_idx, 1e-5, 5e-5)
            assert_nonindexed_unchanged(y0, y, cols, inc_y)


def test_strmv_no_trans():
    """Test sp_blas_strmv with no transpose"""
    for lda, n, A0 in square_matrix_generator():
        for inc_x in vec_inc:
            A0_slice = np.reshape(A0, (lda, n), 'F')
            x = FloatArray(randn(n * abs(inc_x)))
            x0 = x.copy()
            x_idx = indexed_vector(x, n, inc_x)

            # Upper triangular, no unit diagonal
            A = np.triu(A0_slice)
            A_f = A.T.flatten()
            expected = A.dot(x_idx)[:n]
            blas.strmv(True, False, False, n, A_f, lda, x, inc_x)
            assert_allclose(expected, x_idx, 1e-5, 5e-5)

            # Upper triangular, unit diagonal
            A = np.triu(A0_slice)
            np.fill_diagonal(A, 1.0)
            A_f = A.T.flatten()
            x = x0.copy()
            x_idx = indexed_vector(x, n, inc_x)
            expected = A.dot(x_idx)[:n]
            blas.strmv(True, False, True, n, A_f, lda, x, inc_x)
            assert_allclose(expected, x_idx, 1e-5, 5e-5)
