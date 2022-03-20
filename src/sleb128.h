#ifndef __SLEB128_H__
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Blursed header for C function templates
 *
 */

#ifndef T
# define T int8_t
# include "sleb128.h"

# define T int16_t
# include "sleb128.h"

# define T int32_t
# include "sleb128.h"

# define T int64_t
# include "sleb128.h"

# define T ssize_t
# include "sleb128.h"

# undef T
#endif

#ifdef T
# ifndef POSTFIX
#  define POSTFIX T
# endif
# define CCAT2(x, y) x ## _ ## y
# define CCAT(x, y) CCAT2(x, y)
# define FN(x) CCAT(x, POSTFIX)


# define MAX_MALLOC_SLEB_STREAM_LEN 1024

T FN(sleb128_decode)(uint8_t * stream, uint8_t** saveptr);
size_t FN(sleb128_encode)(T value,
                          uint8_t* buffer,
                          size_t buffer_size);
uint8_t* FN(sleb128_alloc_encode)(T value, size_t* count);
//uint8_t* FN(sleb128_alloc_encode)(T value, size_t* count);
#endif // ifdef T

#ifndef T
# define __SLEB128_H__
#else
# undef T
# undef PREFIX
# undef CCAT2
# undef CCAT
# undef FN
#endif // ifndef T
#endif // __SLEB128_H__

