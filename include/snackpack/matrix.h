#ifndef _SNACKPACK_MATRIX_H_
#define _SNACKPACK_MATRIX_H_

/* Needed for float_t typedef. */
#include <math.h>

#include "snackpack/snackpack.h"
#include "snackpack/sp_rep_macros.h"


typedef struct {

    len_t           rows;
    len_t           cols;
    float_t * const data;

} sp_matrix;


#define SP_MATRIX_NEW(_rows, _cols) \
    { .rows = (_rows), .cols = (_cols), \
      .data = (float_t[]){ SP_REP##_cols##X( _rows, 0.0f) } };


#define SP_MATRIX_NEW_INIT(_rows, _cols, _val) \
    { .rows = (_rows), .cols = (_cols), \
      .data = (float_t[]){ SP_REP##_cols##X( _rows, (_val) ) } };


#endif
