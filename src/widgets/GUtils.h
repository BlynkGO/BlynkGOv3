#ifndef __GUTILS_H___
#define __GUTILS_H___

#include <Arduino.h>
#include <memory>

#include "../config/blynkgo_config.h"
// #include "utils/lv_arduino.h"
#include "../objx/lv_blynkgo_utils.h"
#include "../extra/GLVGL.h"


extern "C" {
#if BLYNKGO_USE_KBD
#include "../kbd/lv_keyboard.h"
#endif //BLYNKGO_USE_KBD

#include "../img_decoder/img_decoder.h"
}

#ifndef MEM_CHECK
#define MEM_CHECK  Serial.printf("[%s][%d] heap %d  psram %d\n", __func__,  __LINE__, ESP.getFreeHeap(), ESP.getFreePsram())
#define STR_MEM_CHECK   StringX::printf("[%s][%d] heap %d  psram %d\n", __func__,  __LINE__, ESP.getFreeHeap(), ESP.getFreePsram())
#endif

#define GWIDGET       GWidget*widget
#define GWIDGET_CB    [](GWidget*widget)
#define GANIMATION    GAnimation*a
#define GANIMATION_CB [](GAnimation*a)
#define GANIM_CB      [](GAnimation*a)


#if BLYNKGO_USE_KBD
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
  return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}
#endif

extern uint32_t screen_timer;
void  screen_timer_update();


#endif //__GUTILS_H___
