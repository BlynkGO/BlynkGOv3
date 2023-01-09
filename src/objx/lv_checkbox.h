/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_checkbox.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
   @file lv_checkbox.h

*/

#ifndef LV_CHECKBOX_H
#define LV_CHECKBOX_H

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

#define LV_USE_CHECKBOX 1
#if LV_USE_CHECKBOX != 0


/*********************
        DEFINES
 *********************/

/**********************
        TYPEDEFS
 **********************/
/*Data of checkbox*/
typedef struct
{
  lv_cont_ext_t cont; /*Ext. of ancestor*/
  lv_obj_t* bullet;     // bullet
  lv_obj_t* label;      // label
  bool checked;
  bool inactive;
} lv_checkbox_ext_t;

/*Styles*/
enum {
  LV_CHECKBOX_STYLE_MAIN,
};
typedef uint8_t lv_checkbox_style_t;

/**********************
   GLOBAL PROTOTYPES
 **********************/

/**
   Create a checkbox objects
   @param par pointer to an object, it will be the parent of the new checkbox
   @param copy pointer to a checkbox object, if not NULL then the new object will be copied from it
   @return pointer to the created checkbox
*/
lv_obj_t * lv_checkbox_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
   Add/remove functions
  =====================*/

/*=====================
   Setter functions
  ====================*/

/**
   Set a text of a checkbox.
   @param checkbox pointer to checkbox object
   @param text which a text should be set
*/
void lv_checkbox_set_text(lv_obj_t * checkbox, const char* text);

/**
   Set a checked of a checkbox.
   @param checkbox pointer to checkbox object
   @param checked which a checked should be set
*/
void lv_checkbox_set_checked(lv_obj_t * checkbox, bool checked);

/**
   Set a style of a checkbox.
   @param checkbox pointer to checkbox object
   @param type which style should be set
   @param style pointer to a style
*/
void lv_checkbox_set_style(lv_obj_t * checkbox, lv_checkbox_style_t type, const lv_style_t * style);

/*=====================
   Getter functions
  ====================*/
/**
   Get text of a checkbox.
   @param checkbox pointer to checkbox object
   @return text of a checkbox
*/
const char* lv_checkbox_get_text(lv_obj_t * checkbox);

/**
   Get checked of a checkbox.
   @param checkbox pointer to checkbox object
   @return checked of a checkbox
*/
bool lv_checkbox_is_checked(lv_obj_t * checkbox);


/**
   Get style of a checkbox.
   @param checkbox pointer to checkbox object
   @param type which style should be get
   @return style pointer to the style
*/
lv_style_t * lv_checkbox_get_style(const lv_obj_t * checkbox, lv_checkbox_style_t type);

/*=====================
   Other functions
  ====================*/

/**********************
        MACROS
 **********************/

#endif /*LV_USE_CHECKBOX*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_CHECKBOX_H*/
