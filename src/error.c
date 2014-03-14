#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "snackpack/error.h"


const char * SP_ERROR_DESCR[NUM_SP_ERROR] = {
    [SP_NO_ERROR]               = "no error",
    [SP_ERROR_INVALID_DIM]      = "invalid matrix/vector dimensions",
    [SP_ERROR_INVALID_INC]      = "invalid matrix/vector increment",
    [SP_ERROR_INVALID_TRANS]    = "invalid value for matrix transpose flag",
    [SP_ERROR_INVALID_TRI]      = "invalid value for matrix triangular flag",
    [SP_ERROR_NO_CONVERGENCE]   = "algorithm did not converge",
    [SP_ERROR_DIM_TOO_LARGE]    = "matrix/vector dimensions too large"
};

