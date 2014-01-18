import ctypes
from ctypes import c_float, c_int
import nose.tools as nt
import numpy as np

libsp = ctypes.cdll.LoadLibrary('libsnackpack.dylib')

float_t     = ctypes.c_float
float_t_ptr = ctypes.POINTER(float_t)
len_t       = ctypes.c_uint
inc_t       = ctypes.c_int

FloatArray = lambda x: np.array(x, dtype=np.float32)


func_signatures = {
    'sp_blas_sasum': {
        'restype': float_t,
        'argtypes': (
            len_t, float_t_ptr, inc_t)},
    'sp_blas_saxpy': {
        'restype': None,
        'argtypes': (
            len_t, float_t, float_t_ptr, inc_t, float_t_ptr, inc_t)}
    }


for func_name, sig in func_signatures.iteritems():
    func = getattr(libsp, func_name)
    func.restype = sig['restype']
    func.argtypes = sig['argtypes']


def test_sp_blas_sasum():
    """Test sp_blas_sasum"""
    x = FloatArray([1, 2, 3])
    y = libsp.sp_blas_sasum(x.shape[0], x.ctypes.data_as(float_t_ptr), 1)
    nt.assert_equal(y, 6)
