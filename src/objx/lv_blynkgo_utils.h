/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_blynkgo_utils.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
 *   สร้างโดย TridentTD
 *   
 *   2.ไม่อนุญาต ให้แจกจ่าย สำเนา หรือแก้ไข โดยไม่ได้รับอนุญาต 
 *   
 *   3.สำหรับผู้ได้รับ ลิขสิทธิ์ สามารถนำไปใช้สร้าง firmware/rom 
 *   สำหรับ บอร์ด ESP32 ที่ระบุเท่านั้น เพื่อใช้ทางการค้าต่างๆได้
 *   โดยห้ามแจกจ่าย จำหน่าย ดัดแปลง แก้ไขใดๆ ในตัว source ไลบรารี่ออกไป
 *
 *   4.หากมีการนำไปใช้คอมไพล์ถือว่าได้ยอมรับ ตามเงื่อนไขลิขสิทธิ์ดังกล่าว
 *   เป็นที่เรียบร้อยแล้ว
 * 
 *********************************************************************
 */

/********************************************
 *
 * include พื้นฐานให้ใช้กับ .c และ .cpp ได้ได้วย
 *
 ********************************************/

/**
   @file lv_blynkgo_utils.h

*/

#ifndef LV_BLYNKGO_UTILS_H
#define LV_BLYNKGO_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
        INCLUDES
 *********************/
#include "../extra/GLVGL.h"
// #ifdef LV_CONF_INCLUDE_SIMPLE
// #include "lv_conf.h"
// #else
// #include "../../lv_conf.h"
// #endif

point_t  touch_pt();              /* ตำแหน่ง touch ล่าสุด absolute point*/
point_t  touch_vect();            /* vector ของ touch */
gesture_dir_t touch_gest();       /* gesture ของ touch */
void     fin_drag();              /* สั่งให้หยุด drag */

point_t  text_size(const char* text, lv_style_t *style);
bool     is_objx_type(lv_obj_t* obj, const char* type);

uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

inline uint16_t TFT_COLOR_MIX(color_t color1, color_t color2, uint8_t alpha) {
  lv_color_t c1,c2; c1.full = color1; c2.full = color2;
  return lv_color_mix(c1,c2, alpha).full;
}



inline uint16_t TFT_COLOR(uint8_t r, uint8_t g, uint8_t b) {
  return lv_color_make(r,g,b).full;
}

inline uint16_t TFT_COLOR_HEX(uint32_t c){
  return lv_color_hex(c).full;
}

inline uint8_t TFT_COLOR_BRIGHTNESS(color_t color) {
  lv_color_t c; c.full = color;
  return lv_color_brightness(c);
}

/**
 * Convert a HSV color to color_t (16bit color)
 * @param h hue [0..359]
 * @param s saturation [0..100]
 * @param v value [0..100]
 * @return the given RGB color in RGB (with LV_COLOR_DEPTH depth)
 */
inline uint16_t TFT_COLOR_HSV(uint16_t h, uint8_t s, uint8_t v) {
  return lv_color_hsv_to_rgb(h,s,v).full;
}

inline color_hsv_t TFT_COLOR_TO_HSV(color_t color) {
  lv_color_t c; c.full= color;
  return lv_color_to_hsv(c);
}

inline uint16_t TFT_COLOR_HSV_MIX( color_t color1, color_t color2, uint8_t id255 )  {  // id255 : 0..255
  color_hsv_t hsv[2] = {  TFT_COLOR_TO_HSV( color1 ),  TFT_COLOR_TO_HSV( color2 ) };
  int _id255 = id255;
  id255 = constrain(_id255, 0, 255);
  return TFT_COLOR_HSV( map( id255, 0, 255, hsv[0].h, hsv[1].h), 
                        map( id255, 0, 255, hsv[0].s, hsv[1].s),
                        map( id255, 0, 255, hsv[0].v, hsv[1].v) );
}

extern style_t style_sb;
void  style_sb_init();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LV_BLYNKGO_UTILS_H