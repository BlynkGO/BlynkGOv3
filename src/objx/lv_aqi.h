/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ lv_aqi.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file lv_aqi.h
 *
 */

#ifndef LV_AQI_H
#define LV_AQI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
//#include "../lv_conf_internal.h"
#define LV_USE_AQI  1
#if LV_USE_AQI != 0

#include "../extra/GLVGL.h"

//#include "../lv_core/lv_obj.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
enum {
  AQI_TYPE_US,
  AQI_TYPE_TH,
  AQI_TYPE_CN,
  AQI_TYPE_MAX,
};
typedef uint8_t aqi_type_t;

enum {
  DISPLAY_AQI,
  DISPLAY_PM25,
  DISPLAY_PM10,
  DISPLAY_PM1,
  DISPLAY_MAX,
};
typedef uint8_t display_type_t;
/*Data of aqi*/
typedef struct
{
  display_type_t display_type;
  aqi_type_t aqi_type;
  lv_obj_t* tri_indic;
  lv_obj_t* val_lb;
  lv_obj_t* warning_lb;

  float aqi_val;
  float pm25_val;
  float pm10_val;
  float pm1_val;
  
  lv_point_t pivot;

  lv_style_t *style[3];
} lv_aqi_ext_t;

/*Styles*/
enum {
    LV_AQI_STYLE_MAIN,
    LV_AQI_STYLE_WARNING,
    LV_AQI_STYLE_X,
    LV_AQI_STYLE_Y,
};
typedef uint8_t lv_aqi_style_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a aqi objects
 * @param par pointer to an object, it will be the parent of the new aqi
 * @param copy pointer to a aqi object, if not NULL then the new object will be copied from it
 * @return pointer to the created aqi
 */
lv_obj_t * lv_aqi_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
 * Add/remove functions
 *=====================*/

/*=====================
 * Setter functions
 *====================*/

void lv_aqi_set_aqi(lv_obj_t* aqi, float aqi_val);

void lv_aqi_set_pm25(lv_obj_t* aqi, float pm25_val);

void lv_aqi_set_pm10(lv_obj_t* aqi, float pm10_val);  /* ยัง impliment ไม่เสร็จ */

void lv_aqi_set_pm1(lv_obj_t* aqi, float pm1_val);  /* ยัง impliment ไม่เสร็จ */

void lv_aqi_set_type(lv_obj_t* aqi, aqi_type_t type); 

void lv_aqi_set_warning_hidden(lv_obj_t* aqi, bool hidden);

/**
 * Set a style of a aqi.
 * @param aqi pointer to aqi object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_aqi_set_style(lv_obj_t * aqi, lv_aqi_style_t type, const lv_style_t * style);

/*=====================
 * Getter functions
 *====================*/

float lv_aqi_get_pm25(lv_obj_t* aqi);

float lv_aqi_get_pm10(lv_obj_t* aqi);

float lv_aqi_get_pm1(lv_obj_t* aqi);

float lv_aqi_get_aqi(lv_obj_t* aqi, aqi_type_t type);

aqi_type_t lv_aqi_get_type(lv_obj_t* aqi);

bool lv_aqi_get_warning_hidden(lv_obj_t* aqi);

/**
 * Get style of a aqi.
 * @param aqi pointer to aqi object
 * @param type which style should be get
 * @return style pointer to the style
 */
lv_style_t * lv_aqi_get_style(const lv_obj_t * aqi, lv_aqi_style_t type);

/*=====================
 * Other functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_AQI*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_AQI_H*/
