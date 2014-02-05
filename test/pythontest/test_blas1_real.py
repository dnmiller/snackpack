import nose.tools as nt

from snackpack import blas
from snackpack.util import FloatArray


def test_sasum():
    """Test sp_blas_sasum"""
    x = FloatArray([1, 2, 3])
    y = blas.sasum(3, x, 1)
    nt.assert_equal(y, 6)
