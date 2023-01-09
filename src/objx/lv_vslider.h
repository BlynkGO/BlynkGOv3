/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_vslider.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file lv_vslider.h
 *
 */

#ifndef LV_VSLIDER_H
#define LV_VSLIDER_H

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

#define LV_USE_VSLIDER 1
#if LV_USE_VSLIDER != 0


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of vslider*/
typedef struct {
  lv_obj_t* bar_l;
  lv_obj_t* bar_r;
  lv_obj_t* knob;
  lv_obj_t* lb_name;
  lv_obj_t* lb_val;
  lv_point_t points[3];
  const char* unit;
  int16_t   py_min;
  int16_t   py_max;
  int16_t   val_min;
  int16_t   val_max;
  int16_t   val;
} vslider_ext_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
 
/**
 * Create a vslider object
 * @param par pointer to an object, it will be the parent of the new canvas
 * @param copy pointer to a canvas object, if not NULL then the new object will be copied from it
 * @return pointer to the created canvas
 */
lv_obj_t * lv_vslider_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the text of a vslider
 * @param vslider pointer to vslider object
 * @param text     : the text for the vslider
 */
void lv_vslider_set_text(lv_obj_t* vslider, const char* text);

/**
 * Set the unit of a vslider
 * @param vslider pointer to vslider object
 * @param unit     : the text for the vslider
 */
void lv_vslider_set_unit(lv_obj_t* vslider, const char* unit);

/**
 * Set the range of value for a vslider
 * @param vslider pointer to vslider object
 * @param val_min : the minimum value for the vslider
 * @param val_max : the maximum value for the vslider
 */
void lv_vslider_set_range(lv_obj_t* vslider, int16_t val_min, int16_t val_max);


/**
 * Set the value of a vslider
 * @param vslider pointer to vslider object
 * @param val     : the value for the vslider
 */
void lv_vslider_set_value(lv_obj_t* vslider, int16_t val);


/**
 * Set a style of a vslider.
 * @param vslider pointer to vslider object
 * @param type which style should be set
 * @param style pointer to a style
 *  */
//void lv_vslider_set_style(lv_obj_t* vslider, const lv_style_t * style);

/*=====================
 * Getter functions
 *====================*/
/**
 * Get the text of a vslider
 * @param vslider pointer to vslider object
 * @return the text of the vslider
 */
const char* lv_vslider_get_text(lv_obj_t* vslider);

/**
 * Get the unit of a vslider
 * @param vslider pointer to vslider object
 * @return the uint of the vslider
 */
const char* lv_vslider_get_unit(lv_obj_t* vslider);

/**
 * Get the value of a vslider
 * @param vslider pointer to vslider object
 * @return the value of the vslider
 */
 
int16_t lv_vslider_get_value(lv_obj_t* vslider);

/**
 * Get the minimum value of a vslider
 * @param vslider pointer to vslider object
 * @return the value of the vslider
 */
 
int16_t lv_vslider_get_value_min(lv_obj_t* vslider);

/**
 * Get the maximum value of a vslider
 * @param vslider pointer to vslider object
 * @return the value of the vslider
 */
 
int16_t lv_vslider_get_value_max(lv_obj_t* vslider);


/**
 * Get a style of a vslider.
 * @param vslider pointer to vslider object
 * @param type which style should be set
 *  */
//const lv_style_t* lv_vslider_get_style(lv_obj_t* vslider);

/**********************
 *      MACROS
 **********************/
 
#endif /*LV_USE_VSLIDER*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif //LV_VSLIDER_H
