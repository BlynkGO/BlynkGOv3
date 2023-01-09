/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_joystick.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file lv_joystick.h
 *
 */

#ifndef LV_JOYSTICK_H
#define LV_JOYSTICK_H

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

#define LV_USE_JOYSTICK 1
#if LV_USE_JOYSTICK != 0


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of joystick*/
typedef struct {
  lv_obj_t* joy;
  float bg_r;
  float joy_r;
  float joy_arm;
  float joy_angle;
  uint16_t val_x;  // 0 ... 1024
  uint16_t val_y;  // 0 ... 1024
} joystick_ext_t;

enum {
  LV_JOYSTICK_STYLE_BG,
  LV_JOYSTICK_STYLE_JOY,
};
typedef uint8_t lv_joystick_style_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
 
/**
 * Create a joystick object
 * @param par pointer to an object, it will be the parent of the new canvas
 * @param copy pointer to a canvas object, if not NULL then the new object will be copied from it
 * @return pointer to the created canvas
 */
lv_obj_t * lv_joystick_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set a style of a joystick.
 * @param joystick pointer to joystick object
 * @param type which style should be set
 * @param style pointer to a style
 *  */
void lv_joystick_set_style(lv_obj_t* joystick, lv_joystick_style_t type, const lv_style_t * style);

void lv_joystick_set_xy(lv_obj_t* joystick, int16_t x, int16_t y);

/*=====================
 * Getter functions
 *====================*/
/**
 * Get the value of a joystick
 * @param joystick pointer to joystick object
 * @return the value of the joystick
 */
 
uint16_t lv_joystick_get_value_x(lv_obj_t* joystick);

/**
 * Get the value of a joystick
 * @param joystick pointer to joystick object
 * @return the value of the joystick
 */
 
uint16_t lv_joystick_get_value_y(lv_obj_t* joystick);


/**
 * Get a style of a joystick.
 * @param joystick pointer to joystick object
 * @param type which style should be set
 *  */
const lv_style_t* lv_joystick_get_style(lv_obj_t* joystick, lv_joystick_style_t type);

/**********************
 *      MACROS
 **********************/
 
#endif /*LV_USE_JOYSTICK*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif //LV_JOYSTICK_H
