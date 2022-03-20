
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef T
#include "uleb128.h"
#define T uint8_t
#include "uleb128.c"

#define T uint16_t
#include "uleb128.c"

#define T uint32_t
#include "uleb128.c"

#define T uint64_t
#include "uleb128.c"

#define T size_t
#endif

#ifndef POSTFIX
# define POSTFIX T
#endif
#define CCAT2(x, y) x ## _ ## y
#define CCAT(x, y) CCAT2(x, y)
#define FN(x) CCAT(x, POSTFIX)

T FN(uleb128_decode)(uint8_t * stream, uint8_t** saveptr){
    T res = 0;
    size_t shift = 0;
    if (stream == NULL){
        perror("uleb128_decode NULL argument");
        goto exit;
    }
    if (saveptr == NULL){
        perror("uleb128_decode saveptr is NULL");
        goto exit;
    }

    size_t ind = 0;
    uint8_t byte = 0;
    while (1){
        if (ind > sizeof(T)){
            perror("uleb128_decode value too large for this type");
            goto exit;
        }
        byte = stream[ind];
        res |= (byte & 0x7f) << shift;
        ind += 1;
        if ((byte & 0x80) == 0){
            break;
        }
        shift += 7;
    }

    *saveptr = &stream[ind];

exit:
    return res;
}
# if 0
size_t sleb128_encode(sleb128_t value,
                      uint8_t* buffer,
                      size_t buffer_size){
    int more = 1;
    int negative = (value < 0);
    size_t size = sizeof(value);
    uint8_t byte = 0;
    size_t ind = 0;
    while (more > 0){
        byte = value & 0x7f;
        value >>= 7;
        if (negative != 0){
            value |= (~0 << (size - 7)); // sign extend
        }
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
# endif

size_t FN(uleb128_encode)(T value,
                      uint8_t* buffer,
                      size_t buffer_size){
    uint8_t byte = 0;
    size_t ind = 0;

    if (buffer == NULL){
        perror("uleb128_encode buffer is NULL");
        goto exit;
    }
    do {
        byte = value & 0x7f;
        value >>= 7;
        if (value != 0){
            byte |= 0x80;
        }
        buffer[ind] = byte;
        ind += 1;
    } while (value != 0);

exit:
    return ind;
}


uint8_t* FN(uleb128_alloc_encode)(T value, size_t* count){
    uint8_t* result_buf = NULL;
    if (count == NULL){
        perror("uleb128_encode count is NULL");
        goto exit;
    }
    result_buf = (uint8_t*)malloc(MAX_MALLOC_ULEB_STREAM_LEN);
    if (result_buf == NULL){
        perror("uleb128_encode malloc");
        goto exit;
    }

    *count = FN(uleb128_encode)(value, result_buf, MAX_MALLOC_ULEB_STREAM_LEN);

exit:
    return result_buf;
}

#undef T
#undef PREFIX
#undef CCAT2
#undef CCAT
#undef FN

