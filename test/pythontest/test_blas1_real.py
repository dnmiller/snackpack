import nose.tools as nt
import numpy as np

from snackpack import blas, util

FloatArray = lambda x: np.array(x, dtype=np.float32)
from snackpack.util import float_t, float_t_ptr, len_t, inc_t, library_function

def test_sasum():
    """Test sp_blas_sasum"""
    x = FloatArray([1, 2, 3])
    y = blas.sasum(x.shape[0], x.ctypes.data_as(float_t_ptr), 1)
    nt.assert_equal(y, 6)
