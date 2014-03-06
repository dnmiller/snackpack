#ifndef _SNACKPACK_ERROR_H_
#define _SNACKPACK_ERROR_H_
#include "snackpack/snackpack.h"


#ifdef SP_NO_PRINT
#define SP_LOG_FUNC(...)
#else
#define SP_LOG_FUNC(...) printf(__VA_ARGS__)
#endif


typedef enum {

    SP_NO_ERROR = 0,
    SP_ERROR_INVALID_DIM,
    SP_ERROR_INVALID_INC,
    SP_ERROR_NO_CONVERGENCE,
    SP_ERROR_DIM_TOO_LARGE,
    NUM_SP_ERROR

} SP_ERROR;


bool
sp_error_reported(void);


SP_ERROR
sp_error_last(void);


const char *
sp_error_string(
    SP_ERROR errno);


void
sp_error_log(
    SP_ERROR error,
    int arg,
    const char *file,
    int line,
    const char *func);


#define SP_ASSERT_CONDITION(cond, errno) \
    do { \
    if (!(cond)) { \
        sp_error_log((errno), n, __FILE__, __LINE__, __func__); \
        goto fail; \
    }} while (0)


#define SP_ASSERT_VALID_DIM(n) \
    do { \
    SP_ASSERT_CONDITION((n) > 0, SP_ERROR_INVALID_DIM); \
    SP_ASSERT_CONDITION((n) <= SP_MAX_DIMENSION, SP_ERROR_DIM_TOO_LARGE); \
    } while (0)


#define SP_ASSERT_VALID_INC(n) \
    do { \
    SP_ASSERT_CONDITION((n) != 0, SP_ERROR_INVALID_INC); \
    SP_ASSERT_CONDITION((n) <= SP_MAX_DIMENSION, SP_ERROR_DIM_TOO_LARGE); \
    } while (0)


#endif
