
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef T
#include "sleb128.h"
#define T int8_t
#include "sleb128.c"

#define T int16_t
#include "sleb128.c"

#define T int32_t
#include "sleb128.c"

#define T int64_t
#include "sleb128.c"

#define T ssize_t
#endif

#ifndef POSTFIX
# define POSTFIX T
#endif
#define CCAT2(x, y) x ## _ ## y
#define CCAT(x, y) CCAT2(x, y)
#define FN(x) CCAT(x, POSTFIX)

size_t FN(sleb128_encode)(T value,
                      uint8_t* buffer,
                      size_t buffer_size){
    int more = 1;
    int negative = (value < 0);
    size_t size = sizeof(value)*8;
    uint8_t byte = 0;
    size_t ind = 0;
    while (more > 0){
        byte = value & 0x7f;
        value >>= 7;
#if 1
        if (negative != 0){
            value |= (~0 << (size - 7)); // sign extend
        }
#endif
        if ((value == 0 && ((byte & 0x40) == 0)) ||
            (value == -1 && ((byte & 0x40) != 0))){
            more = 0;
        } else {
            byte |= 0x80;
        }
        buffer[ind] = byte;
        ind += 1;
    }

    return ind;
}


uint8_t* FN(sleb128_alloc_encode)(T value, size_t* count){
    uint8_t* result_buf = NULL;
    if (count == NULL){
        perror("sleb128_encode count is NULL");
        goto exit;
    }
    result_buf = (uint8_t*)malloc(MAX_MALLOC_SLEB_STREAM_LEN);
    if (result_buf == NULL){
        perror("sleb128_encode malloc");
        goto exit;
    }
    memset(result_buf, 0, MAX_MALLOC_SLEB_STREAM_LEN);

    *count = FN(sleb128_encode)(value, result_buf, MAX_MALLOC_SLEB_STREAM_LEN);

exit:
    return result_buf;
}

T FN(sleb128_decode)(uint8_t * stream, uint8_t** saveptr){
    T res = 0;
    size_t shift = 0;
    if (stream == NULL){
        perror("sleb128_decode NULL argument");
        goto exit;
    }
    if (saveptr == NULL){
        perror("sleb128_decode saveptr is NULL");
        goto exit;
    }

    size_t size = sizeof(T)*8;
    size_t ind = 0;
    uint8_t byte = 0;
    do {
        if (ind > sizeof(T)){
            perror("sleb128_decode value too large for this type");
            goto exit;
        }
        byte = stream[ind];
        res |= (byte & 0x7f) << shift;
        ind += 1;
        shift += 7;
    } while ((byte & 0x80) != 0);

    if ((shift < size) && ((byte & 0x40) != 0)){
        res |= (~0 << shift);
    }

    *saveptr = &stream[ind];

exit:
    return res;
}

#undef T
#undef PREFIX
#undef CCAT2
#undef CCAT
#undef FN

