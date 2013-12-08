#ifndef _SNACKPACK_VECTOR_H_
#define _SNACKPACK_VECTOR_H_

/* Needed for float_t typedef. */
#include <math.h>

#include "snackpack/snackpack.h"
#include "snackpack/sp_rep_macros.h"


typedef struct {

    len_t           len;
    float_t * const data;

} sp_vector;


#define SP_VECTOR_NEW(_len) \
    { .len = (_len), .data = (float_t[]){SP_REP##_len(0.0f)}}

#define SP_VECTOR_NEW_INIT(_len, _val) \
    { .len = (_len), .data = (float_t[]){SP_REP##_len(_val)}}


#endif
