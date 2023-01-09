#ifndef __ESP32_MEMORY_H___
#define __ESP32_MEMORY_H___

#include "sdkconfig.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* esp32_malloc(size_t size);
void* esp32_calloc(size_t n, size_t size);
void* esp32_realloc(void *ptr, size_t size);
void  esp32_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif //__ESP32_MEMORY_H___
