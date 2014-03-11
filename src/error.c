#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "snackpack/error.h"


/* Last logged error. */
static SP_ERROR last_error = SP_NO_ERROR;


static const char * SP_ERROR_DESCR[NUM_SP_ERROR] = {
    [SP_NO_ERROR]               = "no error",
    [SP_ERROR_INVALID_DIM]      = "invalid matrix/vector dimensions",
    [SP_ERROR_INVALID_INC]      = "invalid matrix/vector increment",
    [SP_ERROR_NO_CONVERGENCE]   = "algorithm did not converge",
    [SP_ERROR_DIM_TOO_LARGE]    = "matrix/vector dimensions too large"
};


void
sp_error_log(
    SP_ERROR error,
    int arg,
    const char *file,
    int line,
    const char *func)
{
    const char *errstr = sp_error_string(error);
    if (errstr == NULL) {
        SP_LOG_FUNC("Error: Invalid error code %d\n\t:%s(%d):%s\n",
                    error, file, line, func);
    } else {
        SP_LOG_FUNC("Error: %s %d\n\t:%s(%d):%s\n",
                    errstr, arg, file, line, func);
        last_error = error;
    }
}


bool
sp_error_reported(void)
{
    return last_error != SP_NO_ERROR;
}


SP_ERROR
sp_error_last(void)
{
    return last_error;
}


void
sp_error_clear(void)
{
    last_error = SP_NO_ERROR;
}


const char *
sp_error_string(
    SP_ERROR errno)
{
    if (errno < SP_NO_ERROR || errno >= NUM_SP_ERROR) {
        return NULL;
    } else {
        return SP_ERROR_DESCR[errno];
    }
}
