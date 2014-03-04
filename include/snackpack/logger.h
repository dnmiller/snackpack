#ifndef _SNACKPACK_LOGGER_H_
#define _SNACKPACK_LOGGER_H_
#include <stdint.h>

extern const char *SP_ERROR_DESCR[];


typedef enum {

    SP_NO_ERROR = 0,
    SP_ERROR_INVALID_DIM,
    SP_ERROR_INVALID_INC,
    SP_ERROR_NO_CONVERGENCE,
    SP_ERROR_DIM_TOO_LARGE,
    NUM_SP_ERROR

} SP_ERROR;


int
sp_log_error(
    SP_ERROR code,
    int arg,
    const char *file,
    int line,
    const char *func);


SP_ERROR
sp_last_error_logged(void);


uint64_t
sp_errors_logged(void);


#endif
