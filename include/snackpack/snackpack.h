#ifndef _SNACKPACK_SNACKPACK_H_
#define _SNACKPACK_SNACKPACK_H_

/* 
 * Include a trap to prevent pycparser from scanning standard library
 * headers.
 */
#ifndef PYCPARSER_SCAN
#include <stdint.h>
#endif

// TODO increase this after testing
#ifndef SP_MAX_DIMENSION
#define SP_MAX_DIMENSION (10000)
#endif

typedef int32_t len_t;

#endif
