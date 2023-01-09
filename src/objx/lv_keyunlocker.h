/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_keyunlocker.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file lv_keyunlocker.h
 *
 */

#ifndef LV_KEYUNLOCKER_H
#define LV_KEYUNLOCKER_H

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

#define LV_USE_KEYUNLOCKER 1
#if LV_USE_KEYUNLOCKER != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum { 
  LV_KEYUNLOCKER_STATE_LOCK, 
  LV_KEYUNLOCKER_STATE_SETTING,

  // สำหรับใช้ภายใน
  LV_KEYUNLOCKER_STATE_SETTING_FIRST,
  LV_KEYUNLOCKER_STATE_SETTING_SECOND,
  LV_KEYUNLOCKER_STATE_CHECK_PREVKEY,
  LV_KEYUNLOCKER_STATE_PRE_LOCK,
  LV_KEYUNLOCKER_STATE_PRE_SETTING,
  LV_KEYUNLOCKER_STATE_PRE_CHECK_PREVKEY,
  LV_KEYUNLOCKER_STATE_PRE_DEL, 
}  keyunlocker_state_t;

/*Data of keyunlocker*/
typedef struct {
  lv_obj_t*   btnm;
  lv_obj_t*   label;
  lv_obj_t*   line;
  point_t *   line_points;
  uint8_t *   key_inputs;
  uint8_t     key_count;
  uint32_t    released_timer;
  bool        released_checker;
  lv_style_t  styles[3];
  keyunlocker_state_t state;
  uint32_t     secretkey;
  lv_task_t   *task;
} lv_keyunlocker_ext_t;

enum {
  // LV_KEYUNLOCKER_STYLE_MAIN,
  LV_KEYUNLOCKER_STYLE_BG,
  LV_KEYUNLOCKER_STYLE_RELEASED,
  LV_KEYUNLOCKER_STYLE_PRESSED,
};
typedef uint8_t lv_keyunlocker_style_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
 
/**
 * Create a keyunlocker object
 * @param par pointer to an object, it will be the parent of the new canvas
 * @param copy pointer to a canvas object, if not NULL then the new object will be copied from it
 * @return pointer to the created canvas
 */
lv_obj_t * lv_keyunlocker_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the value of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @param secretkey     : the secretkey for the keyunlocker
 */
void lv_keyunlocker_set_secretkey(lv_obj_t* keyunlocker, uint32_t secretkey);


/**
 * Set the value of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @param state     : the state for the keyunlocker
 */
void lv_keyunlocker_set_state(lv_obj_t* keyunlocker, keyunlocker_state_t state );




/**
 * Set the range of value for a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @param val_min : the minimum value for the keyunlocker
 * @param val_max : the maximum value for the keyunlocker
 */
// void lv_keyunlocker_set_range(lv_obj_t* keyunlocker, int16_t val_min, int16_t val_max);


/**
 * Set the value of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @param val     : the value for the keyunlocker
 */
// void lv_keyunlocker_set_value(lv_obj_t* keyunlocker, int16_t val);

/**
 * Set the width of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @param width     : the width for the keyunlocker
 */
//void lv_keyunlocker_set_width(lv_obj_t* keyunlocker, uint16_t width);

/**
 * Set the height of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @param height     : the height for the keyunlocker
 */
//void lv_keyunlocker_set_height(lv_obj_t* keyunlocker, uint16_t height);

/**
 * Set a style of a keyunlocker.
 * @param keyunlocker pointer to keyunlocker object
 * @param type which style should be set
 * @param style pointer to a style
 *  */
// void lv_keyunlocker_set_style(lv_obj_t* keyunlocker, lv_keyunlocker_style_t type, const lv_style_t * style);

/*=====================
 * Getter functions
 *====================*/
/**
 * Get the value of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @return the value of the keyunlocker's secretkey
 */
 
uint32_t lv_keyunlocker_get_secretkey(lv_obj_t* keyunlocker);

/**
 * Get the minimum value of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @return the value of the keyunlocker
 */
 
// int16_t lv_keyunlocker_get_value_min(lv_obj_t* keyunlocker);

/**
 * Get the maximum value of a keyunlocker
 * @param keyunlocker pointer to keyunlocker object
 * @return the value of the keyunlocker
 */
 
// int16_t lv_keyunlocker_get_value_max(lv_obj_t* keyunlocker);

/**
 * Get a style of a keyunlocker.
 * @param keyunlocker pointer to keyunlocker object
 * @param type which style should be set
 *  */
// const lv_style_t* lv_keyunlocker_get_style(lv_obj_t* keyunlocker, lv_keyunlocker_style_t type);

/**********************
 *      MACROS
 **********************/
 
#endif /*LV_USE_KEYUNLOCKER*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif //LV_KEYUNLOCKER_H

