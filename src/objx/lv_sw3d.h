/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_sw3d.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
   @file lv_sw3d.h

*/

#ifndef LV_SW3D_H
#define LV_SW3D_H

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

#define LV_USE_SW3D 1
#if LV_USE_SW3D != 0


/*********************
        DEFINES
 *********************/

/**********************
        TYPEDEFS
 **********************/
/*Types*/
enum {
  LV_SW3D_TYPE_RECT,
  LV_SW3D_TYPE_ROUND,
};
typedef uint8_t lv_sw3d_type_t;


/*Data of sw3d*/
typedef struct
{
  bool state;
  lv_sw3d_type_t type;
} lv_sw3d_ext_t;


/**********************
   GLOBAL PROTOTYPES
 **********************/

/**
   Create a sw3d objects
   @param par pointer to an object, it will be the parent of the new sw3d
   @param copy pointer to a sw3d object, if not NULL then the new object will be copied from it
   @return pointer to the created sw3d
*/
lv_obj_t * lv_sw3d_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
   Add/remove functions
  =====================*/

/*=====================
   Setter functions
  ====================*/
/**
   Set a type of a sw3d.
   @param sw3d pointer to sw3d object
   @param type which style should be set
*/
void lv_sw3d_set_type(lv_obj_t * sw3d, lv_sw3d_type_t type);

/**
   Set  state of a sw3d.
   @param sw3d pointer to sw3d object
   @param state which state should be set : true : on , false : off
*/
void lv_sw3d_set_state(lv_obj_t * sw3d, bool state);


/*=====================
   Getter functions
  ====================*/

/**
   Get a type of a sw3d.
   @param sw3d pointer to sw3d object
   @return type of sw3d
*/
lv_sw3d_type_t lv_sw3d_get_type(lv_obj_t * sw3d);

/**
   Get  state of a sw3d.
   @param sw3d pointer to sw3d object
   @return state of sw3d
*/
bool lv_sw3d_get_state(lv_obj_t * sw3d);


/*=====================
   Other functions
  ====================*/

/**********************
        MACROS
 **********************/

#endif /*LV_USE_SW3D*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_SW3D_H*/

