/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_thermo.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

 /**
   @file lv_thermo.h

*/


#ifndef LV_THERMO_H
#define LV_THERMO_H

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

#define LV_USE_THERMO  1
#if LV_USE_THERMO != 0


/*********************
        DEFINES
 *********************/

/**********************
        TYPEDEFS
 **********************/
/*Data of thermo*/
typedef struct
{
  float val;
  float val_min;
  float val_max;
  uint8_t decimal;
  bool   level_draw;
  lv_style_t *style_indic;
} lv_thermo_ext_t;

/*Styles*/
enum {
  LV_THERMO_STYLE_BG, /** Bar background style. */
  LV_THERMO_STYLE_INDIC, /** Bar fill area style. */
};
typedef uint8_t lv_thermo_style_t;

/**********************
   GLOBAL PROTOTYPES
 **********************/

/**
   Create a thermo objects
   @param par pointer to an object, it will be the parent of the new thermo
   @param copy pointer to a thermo object, if not NULL then the new object will be copied from it
   @return pointer to the created thermo
*/
lv_obj_t * lv_thermo_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
   Add/remove functions
  =====================*/

/*=====================
   Setter functions
  ====================*/

/**
   Set a style of a thermo.
   @param thermo pointer to thermo object
   @param type which style should be set
   @param style pointer to a style
*/
void lv_thermo_set_style(lv_obj_t * thermo, lv_thermo_style_t type, const lv_style_t * style);

void lv_thermo_set_range(lv_obj_t* thermo, float value_min, float value_max);
void lv_thermo_set_value(lv_obj_t* thermo, float value);
void lv_thermo_set_decimal(lv_obj_t* thermo, uint8_t decimal);
void lv_thermo_set_level_draw(lv_obj_t* thermo, bool enable);
/*=====================
   Getter functions
  ====================*/

/**
   Get style of a thermo.
   @param thermo pointer to thermo object
   @param type which style should be get
   @return style pointer to the style
*/
lv_style_t * lv_thermo_get_style(const lv_obj_t * thermo, lv_thermo_style_t type);

float lv_thermo_get_value(lv_obj_t* thermo);
float lv_thermo_get_value_min(lv_obj_t* thermo);
float lv_thermo_get_value_max(lv_obj_t* thermo);
uint8_t lv_thermo_get_decimal(lv_obj_t* thermo); 
bool lv_thermo_get_level_draw(lv_obj_t* thermo);

/*=====================
   Other functions
  ====================*/

/**********************
        MACROS
 **********************/

#endif /*LV_USE_THERMO*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_THERMO_H*/
