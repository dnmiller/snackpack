import ctypes
import os


from util import float_t, float_t_ptr, len_t, int_t, library_function


# TODO: portable filename
#_libsp_dir = os.path.dirname(
    #os.path.abspath(
        #inspect.getfile(
            #inspect.currentframe())))
_libsp_dir = '../src'
_libsp_filename = os.path.join(_libsp_dir, 'libsnackpack.dylib')

libsp = ctypes.cdll.LoadLibrary(_libsp_filename)


class SP_TRANS(object):
    NONE = 0
    TRANSPOSE = 1
    CONJUGATE = 2


sasum = library_function(
    libsp, 'sp_blas_sasum', float_t,
    [len_t, float_t_ptr, len_t],
    """Add the absolute value of the elements of a vector.
    """)

saxpy = library_function(
    libsp, 'sp_blas_saxpy', None,
    [len_t, float_t, float_t_ptr, len_t, float_t_ptr, len_t],
    """Add two vectors, scaling the first (single-precision).
    """)

srotg = library_function(
    libsp, 'sp_blas_srotg', None,
    [float_t_ptr, float_t_ptr, float_t_ptr, float_t_ptr],
    """Construct a real Givens plane rotation (single-precision).
    """)

srot = library_function(
    libsp, 'sp_blas_srot', None,
    [len_t, float_t_ptr, len_t, float_t_ptr, len_t, float_t, float_t],
    """Applies a plane rotation (single-precision).
    """)

sswap = library_function(
    libsp, 'sp_blas_sswap', None,
    [len_t, float_t_ptr, len_t, float_t_ptr, len_t],
    """Swap two real vectors (single-precision).
    """)

scopy = library_function(
    libsp, 'sp_blas_scopy', None,
    [len_t, float_t_ptr, len_t, float_t_ptr, len_t],
    """Copy a real vector into another vector (single-precision).
    """)

sdot = library_function(
    libsp, 'sp_blas_sdot', float_t,
    [len_t, float_t_ptr, len_t, float_t_ptr, len_t],
    """Calculate the dot-product of two real vectors (single-precision).
    """)

sdsdot = library_function(
    libsp, 'sp_blas_sdsdot', float_t,
    [len_t, float_t, float_t_ptr, len_t, float_t_ptr, len_t],
    """Calculate the dot-product of two real vectors in double precision and
    add a scalar term (single-precision).
    """)

snrm2 = library_function(
    libsp, 'sp_blas_snrm2', float_t,
    [len_t, float_t_ptr, len_t],
    """Calculate the 2-norm of a vector (single-precision).
    """)

# commented out until cleaned up.
#srotm = library_function(
    #libsp, 'sp_blas_srotm', None,
    #[len_t, float_t_ptr, len_t, float_t_ptr, len_t, float_t_ptr],
    #"""Apply a Givens rotation to a 2xN matrix (single-precision).
    #""")

#srotmg = library_function(
    #libsp, 'sp_blas_srotmg', None,
    #[float_t_ptr, float_t_ptr, float_t_ptr, float_t, float_t_ptr],
    #"""Apply a modified Givens rotation to a 2xN matrix (single-precision).
    #""")

sscal = library_function(
    libsp, 'sp_blas_sscal', None,
    [len_t, float_t, float_t_ptr, len_t],
    """Scale a vector by scalar (single-precision).
    """)

isamax = library_function(
    libsp, 'sp_blas_isamax', len_t,
    [len_t, float_t_ptr, len_t],
    """Return the index of the largest-magnitude element of a vector
    (single-precision).
    """)

isamin = library_function(
    libsp, 'sp_blas_isamin', len_t,
    [len_t, float_t_ptr, len_t],
    """Return the index of the smallest-magnitude element of a vector
    (single-precision).
    """)

sgemv = library_function(
    libsp, 'sp_blas_sgemv', None,
    [int_t, len_t, len_t, float_t, float_t_ptr, len_t, float_t_ptr, len_t,
     float_t, float_t_ptr, len_t],
    """Compute a general matrix-vector product (single-precision).
    """)


strmv = library_function(
    libsp, 'sp_blas_strmv', None,
    [int_t, int_t, int_t, len_t, float_t_ptr, len_t, float_t_ptr, len_t],
    """Compute the product of a triangular matrix and a vector
    (single-precision).
    """)
