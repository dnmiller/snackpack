#ifndef _SNACKPACK_SNACKPACK_H_
#define _SNACKPACK_SNACKPACK_H_
#include <stdint.h>


// TODO increase this after testing
#ifndef SP_MAX_DIMENSION
#define SP_MAX_DIMENSION (1000)
#endif


typedef int_fast16_t    len_t;
typedef int_fast16_t    inc_t;
typedef uint_fast8_t    order_t;


typedef enum {

    SP_STATUS_OK = 0,
    SP_STATUS_ERROR = -1,
    SP_STATUS_INVALID_DIM = -2

} SP_STATUS;


typedef enum {

    SP_TRI_UPLO_UPPER,
    SP_TRI_UPLO_LOWER

} SP_UPLO_TRI;


typedef enum {

    SP_TRANS_NONE,
    SP_TRANS_TRANSPOSE,
    SP_TRANS_CONJUGATE

} SP_TRANS;


#ifndef log_error
#define log_error(code) \
    sp_log_error(code, __FILE__, __LINE__, __func__)
#endif


#endif
