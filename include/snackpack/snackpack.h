#ifndef _SNACKPACK_SNACKPACK_H_
#define _SNACKPACK_SNACKPACK_H_
#include <stdint.h>


// TODO increase this after testing
#ifndef SP_MAX_DIMENSION
#define SP_MAX_DIMENSION (10000)
#endif


typedef int32_t len_t;


typedef enum {

    SP_TRI_UPPER,
    SP_TRI_LOWER,
    NUM_SP_TRI

} SP_TRI_TYPE;


typedef enum {

    SP_TRANS_NONE,
    SP_TRANS_TRANSPOSE,
    SP_TRANS_CONJUGATE,
    NUM_SP_TRANS

} SP_TRANS;


typedef enum {

    SP_UNIT_YES,
    SP_UNIT_NO

} SP_UNIT;


#endif
