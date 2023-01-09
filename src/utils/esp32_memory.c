#include "esp32_memory.h"
#include "esp_attr.h"

void* IRAM_ATTR esp32_malloc(size_t size) { 
  void* buf;
#if defined (ESP32) && defined (CONFIG_SPIRAM_SUPPORT)
  if ( psramFound() ) 
     buf = ps_malloc(size);
  else
#endif
  buf = malloc(size);

  if(buf !=NULL) memset(buf,0, size);
  return buf;
}

void* IRAM_ATTR esp32_calloc(size_t n, size_t size){
  void* buf;
#if defined (ESP32) && defined (CONFIG_SPIRAM_SUPPORT)
  if ( psramFound() ) 
     buf = ps_calloc(n, size);
  else
#endif
  buf = calloc(n, size);

  if(buf !=NULL) memset(buf,0, n*size);
  return buf;
}

void* IRAM_ATTR esp32_realloc(void *ptr, size_t size){
  if(ptr == NULL) return esp32_malloc(size);
  
#if defined (ESP32) && defined (CONFIG_SPIRAM_SUPPORT)
  if ( psramFound() ) 
     return ps_realloc(ptr, size);
  else
#endif
  return realloc(ptr, size);
}

void IRAM_ATTR esp32_free(void *ptr){
  if(ptr != NULL) {
    free(ptr);
    ptr = NULL;
  }
}

