#ifndef _SNACKPACK_SNACKPACK_H_
#define _SNACKPACK_SNACKPACK_H_

#include <stdint.h>


typedef uint_fast8_t    len_t;
typedef int_fast8_t     inc_t;


typedef enum {

    SP_STATUS_OK,
    SP_STATUS_ERROR,
    SP_STATUS_INVALID_DIM

} SP_STATUS;


#endif
