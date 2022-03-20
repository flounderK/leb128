#ifndef __LEB128_H__
#define __LEB128_H__
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "leb128.h"

#define MAX_MALLOC_ULEB_STREAM_LEN 1024

typedef size_t uleb128_t;
typedef ssize_t sleb128_t;

uleb128_t uleb128_decode(uint8_t * stream, uint8_t** saveptr);
size_t sleb128_encode(sleb128_t value,
                      uint8_t* buffer,
                      size_t buffer_size);
size_t uleb128_encode(uleb128_t value,
                      uint8_t* buffer,
                      size_t buffer_size);
uint8_t* uleb128_alloc_encode(uleb128_t value, size_t* count);

#endif // __LEB128_H__
