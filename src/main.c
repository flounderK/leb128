#include "uleb128.h"
#include "sleb128.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    int res = -1;

    uint8_t* buffer = (uint8_t*)malloc(MAX_MALLOC_ULEB_STREAM_LEN);
    if (buffer == NULL){
        perror("main malloc failed");
        goto exit;
    }
    memset(buffer, 0, MAX_MALLOC_ULEB_STREAM_LEN);
    size_t value = 624485;

    printf("value = %lu %p\n", value, (void*)value);
    size_t size = uleb128_encode_size_t(value, buffer,
                      MAX_MALLOC_ULEB_STREAM_LEN);
    for (int i = 0; i < sizeof(uleb128_t); i++){
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    size_t decoded_value = 0;
    uint8_t * saveptr = NULL;
    decoded_value = uleb128_decode_size_t(buffer, &saveptr);
    printf("decoded %lu %p\n", decoded_value, (void*)decoded_value);
    printf("saveptr %p\n", saveptr);


    memset(buffer, 0, MAX_MALLOC_ULEB_STREAM_LEN);

    ssize_t s_value = -123456;
    printf("value = %ld %p\n", s_value, (void*)s_value);
    size = sleb128_encode_ssize_t(s_value, buffer,
                      MAX_MALLOC_SLEB_STREAM_LEN);
    for (int i = 0; i < size; i++){
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    ssize_t s_decoded_value = 0;

    s_decoded_value = sleb128_decode_ssize_t(buffer, &saveptr);
    printf("decoded %ld %p\n", s_decoded_value, (void*)s_decoded_value);
    printf("saveptr %p\n", saveptr);



    res = 0;
exit:
    return res;
}
