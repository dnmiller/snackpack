#ifndef _SNACKPACK_ERROR_H_
#define _SNACKPACK_ERROR_H_
#include "snackpack/snackpack.h"


#ifdef SP_NO_PRINT
#define SP_PRINTF(...)
#else
#define SP_PRINTF(...) printf(__VA_ARGS__)
#endif


typedef enum {

    SP_NO_ERROR = 0,
    SP_ERROR_INVALID_DIM,
    SP_ERROR_INVALID_INC,
    SP_ERROR_NO_CONVERGENCE,
    SP_ERROR_DIM_TOO_LARGE,
    NUM_SP_ERROR

} SP_ERROR;


extern const char * SP_ERROR_DESCR[NUM_SP_ERROR];


#define SP_PRINT_ERROR(errno) \
{ \
    if ((errno) < NUM_SP_ERROR) { \
        SP_PRINTF("Error \"%s\" in %s(%d):%s\n", \
            SP_ERROR_DESCR[(errno)], __FILE__, __LINE__, __func__); \
    } else { \
        SP_PRINTF("Invalid error code %d!\n", (errno)); \
    } \
}



#define SP_ASSERT_CONDITION(cond, errno) \
{ \
    if (!(cond)) { \
        SP_PRINT_ERROR((errno)); \
        goto fail; \
    } \
}


#define SP_ASSERT_VALID_DIM(n) \
    { \
    SP_ASSERT_CONDITION((n) > 0, SP_ERROR_INVALID_DIM); \
    SP_ASSERT_CONDITION((n) <= SP_MAX_DIMENSION, SP_ERROR_DIM_TOO_LARGE); \
    }


#define SP_ASSERT_VALID_INC(n) \
    do { \
    SP_ASSERT_CONDITION((n) != 0, SP_ERROR_INVALID_INC); \
    SP_ASSERT_CONDITION((n) <= SP_MAX_DIMENSION, SP_ERROR_DIM_TOO_LARGE); \
    } while (0)


#endif
