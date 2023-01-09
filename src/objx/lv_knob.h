/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_knob.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file lv_knob.h
 *
 */

#ifndef LV_KNOB_H
#define LV_KNOB_H

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

#define LV_USE_KNOB 1
#if LV_USE_KNOB != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of knob*/
typedef struct {
  lv_arc_ext_t arc;
  lv_obj_t* cir_big;
  lv_obj_t* indic;

  const lv_style_t *styles[2];
  float     indic_angle;
  uint16_t  indic_arm_length;
  int16_t   val;
  int16_t   val_min;
  int16_t   val_max;
} lv_knob_ext_t;

enum {
  // LV_KNOB_STYLE_KNOB,
  // LV_KNOB_STYLE_INDIC,
  // LV_KNOB_STYLE_ARC,
  LV_KNOB_STYLE_BG,
  LV_KNOB_STYLE_INDIC,
};
typedef uint8_t lv_knob_style_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
 
/**
 * Create a knob object
 * @param par pointer to an object, it will be the parent of the new canvas
 * @param copy pointer to a canvas object, if not NULL then the new object will be copied from it
 * @return pointer to the created canvas
 */
lv_obj_t * lv_knob_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/
/**
 * Set the range of value for a knob
 * @param knob pointer to knob object
 * @param val_min : the minimum value for the knob
 * @param val_max : the maximum value for the knob
 */
void lv_knob_set_range(lv_obj_t* knob, int16_t val_min, int16_t val_max);


/**
 * Set the value of a knob
 * @param knob pointer to knob object
 * @param val     : the value for the knob
 */
void lv_knob_set_value(lv_obj_t* knob, int16_t val);

/**
 * Set the width of a knob
 * @param knob pointer to knob object
 * @param width     : the width for the knob
 */
//void lv_knob_set_width(lv_obj_t* knob, uint16_t width);

/**
 * Set the height of a knob
 * @param knob pointer to knob object
 * @param height     : the height for the knob
 */
//void lv_knob_set_height(lv_obj_t* knob, uint16_t height);

/**
 * Set a style of a knob.
 * @param knob pointer to knob object
 * @param type which style should be set
 * @param style pointer to a style
 *  */
void lv_knob_set_style(lv_obj_t* knob, lv_knob_style_t type, const lv_style_t * style);

/*=====================
 * Getter functions
 *====================*/
/**
 * Get the value of a knob
 * @param knob pointer to knob object
 * @return the value of the knob
 */
 
int16_t lv_knob_get_value(lv_obj_t* knob);

/**
 * Get the minimum value of a knob
 * @param knob pointer to knob object
 * @return the value of the knob
 */
 
int16_t lv_knob_get_value_min(lv_obj_t* knob);

/**
 * Get the maximum value of a knob
 * @param knob pointer to knob object
 * @return the value of the knob
 */
 
int16_t lv_knob_get_value_max(lv_obj_t* knob);

/**
 * Get a style of a knob.
 * @param knob pointer to knob object
 * @param type which style should be set
 *  */
const lv_style_t* lv_knob_get_style(lv_obj_t* knob, lv_knob_style_t type);

/**********************
 *      MACROS
 **********************/
 
#endif /*LV_USE_KNOB*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif //LV_KNOB_H


/*
 * style[BG].body.padding.inner = ระยะระหว่าง arc กับ cir_big
 * style[BG].body.main_color & grad_color = สีของ cir_big
 * 
 * style[INDIC].line.color & width = ใช้กับ arc
 */
