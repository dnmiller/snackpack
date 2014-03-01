#include <stdio.h>
#include "snackpack/logger.h"


const char * SP_ERROR_DESCR[NUM_SP_ERROR] = {
    [SP_ERROR_INVALID_DIM]      = "invalid dimensions",
    [SP_ERROR_NO_CONVERGENCE]   = "algorithm did not converge",
    [SP_ERROR_DIM_TOO_LARGE]    = "matrix/vector dimensions too large"};


static uint64_t errors_logged = 0;


int
sp_log_error(
    SP_ERROR code,
    const char *file,
    int line,
    const char *func)
{
    if (code >= NUM_SP_ERROR) {
        printf("Error: Invalid error code! - %s:%d:%s\n", file, line, func);
    } else {
        printf("Error: %s - %s:%d:%s\n",
            SP_ERROR_DESCR[code], file, line, func);
    }

    errors_logged++;
    return 0;
}


/**
 * Return the total number of errors that have been logged since start of
 * program.
 */
uint64_t
sp_errors_logged(void)
{
    return errors_logged;
}
