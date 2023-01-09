/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_triangle.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file lv_triangle.h
 *
 */


#ifndef LV_TRIANGLE_H
#define LV_TRIANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../extra/GLVGL.h"

#define LV_USE_TRIANGLE       1
#if LV_USE_TRIANGLE != 0



/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of triangle*/
typedef struct
{
  lv_point_t points_relative[3];
} lv_triangle_ext_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a triangle objects
 * @param par pointer to an object, it will be the parent of the new triangle
 * @param copy pointer to a triangle object, if not NULL then the new object will be copied from it
 * @return pointer to the created triangle
 */
lv_obj_t * lv_triangle_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
 * Add/remove functions
 *=====================*/

/*=====================
 * Setter functions
 *====================*/

void lv_triangle_set_points(lv_obj_t* triangle, lv_point_t *points );

/*=====================
 * Getter functions
 *====================*/

lv_point_t * lv_triangle_get_points(lv_obj_t* triangle, lv_point_t *points );

/*=====================
 * Other functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_TRIANGLE*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_TRIANGLE_H*/
