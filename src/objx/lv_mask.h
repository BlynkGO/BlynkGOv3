/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_mask.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
   @file lv_mask.h

*/

#ifndef LV_M_MASK_H
#define LV_M_MASK_H

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

#define LV_USE_MASK  1
#if LV_USE_MASK != 0


/*********************
        DEFINES
 *********************/

#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

typedef uint8_t lv_datum_t;


/**********************
        TYPEDEFS
 **********************/
/*Data of mask*/
typedef struct
{
  lv_objmask_ext_t objmask;
  lv_obj_t* canvas;
  uint8_t*  cbuf;   // สำหรับ lv_mask_alloc_buffer(..) ให้อ้างอิงภายใน
  uint16_t  w;
  uint16_t  h;
  bool      auto_resize;
} lv_mask_ext_t;

/*Styles*/
enum {
  LV_MASK_STYLE_X,
  LV_MASK_STYLE_Y,
};
typedef uint8_t lv_mask_style_t;

/**********************
   GLOBAL PROTOTYPES
 **********************/

/**
   Create a mask objects
   @param par pointer to an object, it will be the parent of the new mask
   @param copy pointer to a mask object, if not NULL then the new object will be copied from it
   @return pointer to the created mask
*/
lv_obj_t * lv_mask_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
   Add/remove functions
  =====================*/

/*=====================
   Setter functions
  ====================*/
/**
   Allocate mask's buffer and set to the mask's canvas.
   @param mask pointer to mask object
   @param w width of mask
   @param h height of mask
*/
void lv_mask_alloc_buffer(lv_obj_t* mask, uint16_t w, uint16_t h);

/**
   Set mask's buffer from outside and set to the mask's canvas.
   @param mask pointer to mask object
   @param buffer is set to mask
   @param w width of buffer
   @param h height of buffer
*/
void lv_mask_set_buffer(lv_obj_t* mask, uint8_t* buffer, uint16_t w, uint16_t h);

/**
   Set a style of a mask.
   @param mask pointer to mask object
   @param type which style should be set
   @param style pointer to a style
*/
void lv_mask_set_style(lv_obj_t * mask, lv_mask_style_t type, const lv_style_t * style);

/*=====================
   Getter functions
  ====================*/

/**
   Get mask's buffer.
   @param mask pointer to mask object
   @param buffer is set to mask
   @param pointer w for getting width of the buffer 
   @param pointer h for getting height of the buffer 
   @return buffer of a mask
*/
uint8_t* lv_mask_get_buffer(lv_obj_t* mask, uint16_t *w, uint16_t *h);

/**
   Get style of a mask.
   @param mask pointer to mask object
   @param type which style should be get
   @return style pointer to the style
*/
lv_style_t * lv_mask_get_style(const lv_obj_t * mask, lv_mask_style_t type);

/*=====================
   Other functions
  ====================*/

void lv_mask_fill_bg(lv_obj_t* mask, lv_color_t color, lv_opa_t opa);

void lv_mask_draw_text(lv_obj_t * mask, lv_coord_t x, lv_coord_t y, const lv_style_t *style, 
                            const char * txt, lv_datum_t datum);

void lv_mask_draw_rect(lv_obj_t * mask, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,
                         const lv_style_t * style);

void lv_mask_draw_line(lv_obj_t * mask, const lv_point_t * points, uint32_t point_cnt, const lv_style_t * style);

void lv_mask_draw_line_aa(lv_obj_t * mask, lv_pointf_t p1, lv_pointf_t p2, const lv_style_t * style);

void lv_mask_draw_polygon(lv_obj_t * mask, const lv_point_t * points, uint32_t point_cnt, const lv_style_t * style);

void lv_mask_draw_arc(lv_obj_t * mask, lv_coord_t x, lv_coord_t y, lv_coord_t r, int32_t start_angle,
                        int32_t end_angle, const lv_style_t * style);

void lv_mask_draw_img(lv_obj_t * mask, lv_coord_t x, lv_coord_t y, const void * src, const lv_style_t * style);

/* กลับ alpha ของ mask*/
void lv_mask_invert(lv_obj_t * mask);

/**********************
        MACROS
 **********************/

#endif /*LV_USE_MASK*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_M_MASK_H*/
