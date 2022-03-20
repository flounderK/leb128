#ifndef __LEB128_H__
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Blursed header for C function templates
 *
 */

#ifndef T
# define T uint8_t
# include "uleb128.h"

# define T uint16_t
# include "uleb128.h"

# define T uint32_t
# include "uleb128.h"

# define T uint64_t
# include "uleb128.h"

# define T size_t
# include "uleb128.h"

# undef T
#endif

#ifdef T
# ifndef POSTFIX
#  define POSTFIX T
# endif
# define CCAT2(x, y) x ## _ ## y
# define CCAT(x, y) CCAT2(x, y)
# define FN(x) CCAT(x, POSTFIX)


# define MAX_MALLOC_ULEB_STREAM_LEN 1024

typedef size_t uleb128_t;
typedef ssize_t sleb128_t;

T FN(uleb128_decode)(uint8_t * stream, uint8_t** saveptr);
#if 0
size_t sleb128_encode(sleb128_t value,
                      uint8_t* buffer,
                      size_t buffer_size);
#endif
size_t FN(uleb128_encode)(T value,
                      uint8_t* buffer,
                      size_t buffer_size);
uint8_t* FN(uleb128_alloc_encode)(T value, size_t* count);
#endif // ifdef T

#ifndef T
# define __LEB128_H__
#else
# undef T
# undef PREFIX
# undef CCAT2
# undef CCAT
# undef FN
#endif // ifndef T
#endif // __LEB128_H__

