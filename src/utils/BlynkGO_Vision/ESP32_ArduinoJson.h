#ifndef __ESP32_ARDUINOJSON_H__
#define __ESP32_ARDUINOJSON_H__

#include <ArduinoJson.h>      // Version 6.x
#include "../esp32_memory.h"

struct ESP32_Allocator {
  inline void* allocate(size_t size)                  { return esp32_malloc(size); }
  inline void* reallocate(void* ptr, size_t new_size) { return esp32_realloc(ptr, new_size); }
  inline void  deallocate(void* pointer)              { esp32_free(pointer); }
};

using ESP32_JsonDocument = BasicJsonDocument<ESP32_Allocator>;

#endif
