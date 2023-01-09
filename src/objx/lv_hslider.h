/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_hslider.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file lv_hslider.h
 *
 */

#ifndef LV_HSLIDER_H
#define LV_HSLIDER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../extra/GLVGL.h"
// #ifdef LV_CONF_INCLUDE_SIMPLE
// #include "lv_conf.h"
// #else
// #include "../../lv_conf.h"
// #endif

#define LV_USE_HSLIDER 1
#if LV_USE_HSLIDER != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of hslider*/
typedef struct {
  lv_obj_t* bar_l;
  lv_obj_t* bar_r;
  lv_obj_t* knob;
  lv_obj_t* lb_name;
  lv_obj_t* lb_val;
  lv_point_t points[3];
  const char* unit;
  int16_t   px_min;
  int16_t   px_max;
  int16_t   val_min;
  int16_t   val_max;
  int16_t   val;
} hslider_ext_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
 
/**
 * Create a hslider object
 * @param par pointer to an object, it will be the parent of the new canvas
 * @param copy pointer to a canvas object, if not NULL then the new object will be copied from it
 * @return pointer to the created canvas
 */
lv_obj_t * lv_hslider_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the text of a hslider
 * @param hslider pointer to hslider object
 * @param text     : the text for the hslider
 */
void lv_hslider_set_text(lv_obj_t* hslider, const char* text);

/**
 * Set the unit of a hslider
 * @param hslider pointer to hslider object
 * @param unit     : the text for the hslider
 */
void lv_hslider_set_unit(lv_obj_t* hslider, const char* unit);

/**
 * Set the range of value for a hslider
 * @param hslider pointer to hslider object
 * @param val_min : the minimum value for the hslider
 * @param val_max : the maximum value for the hslider
 */
void lv_hslider_set_range(lv_obj_t* hslider, int16_t val_min, int16_t val_max);


/**
 * Set the value of a hslider
 * @param hslider pointer to hslider object
 * @param val     : the value for the hslider
 */
void lv_hslider_set_value(lv_obj_t* hslider, int16_t val);


/**
 * Set a style of a hslider.
 * @param hslider pointer to hslider object
 * @param type which style should be set
 * @param style pointer to a style
 *  */
//void lv_hslider_set_style(lv_obj_t* hslider, const lv_style_t * style);

/*=====================
 * Getter functions
 *====================*/
/**
 * Get the text of a hslider
 * @param hslider pointer to hslider object
 * @return the text of the hslider
 */
const char* lv_hslider_get_text(lv_obj_t* hslider);

/**
 * Get the unit of a hslider
 * @param hslider pointer to hslider object
 * @return the uint of the hslider
 */
const char* lv_hslider_get_unit(lv_obj_t* hslider);

/**
 * Get the value of a hslider
 * @param hslider pointer to hslider object
 * @return the value of the hslider
 */
 
int16_t lv_hslider_get_value(lv_obj_t* hslider);

/**
 * Get the minimum value of a hslider
 * @param hslider pointer to hslider object
 * @return the value of the hslider
 */
 
int16_t lv_hslider_get_value_min(lv_obj_t* hslider);

/**
 * Get the maximum value of a hslider
 * @param hslider pointer to hslider object
 * @return the value of the hslider
 */
 
int16_t lv_hslider_get_value_max(lv_obj_t* hslider);


/**
 * Get a style of a hslider.
 * @param hslider pointer to hslider object
 * @param type which style should be set
 *  */
//const lv_style_t* lv_hslider_get_style(lv_obj_t* hslider);

/**********************
 *      MACROS
 **********************/
 
#endif /*LV_USE_HSLIDER*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif //LV_HSLIDER_H
