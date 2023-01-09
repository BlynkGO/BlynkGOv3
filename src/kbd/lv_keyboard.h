/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ lv_keyboard.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef LV_KEYBOARD_H
#define LV_KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#else
#include "../utils/lv_conf.h"
#endif

#include <stdio.h>

#include "../utils/lvgl-dev-7.0/src/lv_core/lv_obj.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_cont.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_ta.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_label.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_kb.h"


/**********************
 *      TYPEDEFS
 **********************/
typedef enum {KBD_ENG, KBD_THAI, KBD_LAOS, KBD_VIETNAM, KBD_MAX } kbd_lang_t;
typedef void(*lv_apply_cb_t)(void);

typedef struct {
  lv_obj_t *kb;                 /* kb object */
  lv_obj_t *kb_bg;              /* backgroud รองพื้น สำหรับ kb */
  lv_obj_t *kb_ok_bg;           /* background ด้านปุ่มกด ok */
  lv_obj_t *kb_ctrl1_bg;        /* background ด้านข้างซ้าย */
  lv_obj_t *kb_ctrl2_bg;        /* background ด้านข้างขวา */
  lv_obj_t *ta_cont;            /* cont สำหรับรวมรวม textarea + eye */
  lv_obj_t *ta_inner;           /* textarea object ภายในที่จะเชื่อมกับ kb ตลอด */
  lv_obj_t *kbd_eye;            /* eye object สำหรับกด */
  lv_obj_t *kbd_eye_lb;         /* symbol ของดวงตา : SYMBOL_EYE_OPEN หรือ SYMBOL_EYE_CLOSE*/
  bool      kbd_eye_state;      /* สถานะดวงตา ขณะอยู๋ password mode */
  lv_obj_t *kbd_obj_outside;    /* obj ภายนอก เป็นได้ 2 ชนิด คือ lv_label หรือ lv_ta */
  kbd_lang_t language;          /* สถานะแป้นภาษาปัจจุบันของ kb */
  kbd_lang_t lang2nd;           /* แป้นพิมพ์ที่จะไว้สลับกับ แป้นอังกฤษ เพิ่มมาใหม่ */ 
  lv_event_cb_t kb_event_cb;    /* หากจะกำหนด kb_event_cb เอง ไม่ใช้ของ default */
  lv_apply_cb_t kb_apply_cb;    /* หากให้มีการส่งคืนเมื่อ apply */
  void *user_data;
} keyboard_ext_t;


/**********************
 *   GLOBAL VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a TridentTD's keyboard objects
 * @param par pointer to an object, it will be the parent of the new keyboard
 * @param copy pointer to a keyboard object, always NULL
 * @return pointer to the created keyboard
 */
lv_obj_t * lv_keyboard_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the source for editing by the keyboard 
 * @param keyboard pointer to the keyboard
 * @param source pointer to the source ( only lv_label or lv_ta )
 */
void lv_keyboard_set_src(lv_obj_t *keyboard, lv_obj_t * source, const char* manual_placeholder) ;

/**
 * Set the language for the keyboard 
 * @param keyboard pointer to the keyboard
 * @param lang to the language of keyboard
 */
void lv_keyboard_set_language(lv_obj_t *keyboard, kbd_lang_t lang);

void lv_keyboard_set_language2nd(lv_obj_t *keyboard, kbd_lang_t lang2nd);  //แป้นหลักจะเป็นอังกฤษ แป้นรองจะให้กำหนดที่นี่

void lv_keyboard_set_event_cb(lv_obj_t *keyboard, lv_event_cb_t event_cb);

void lv_keyboard_set_apply_cb(lv_obj_t *keyboard, lv_apply_cb_t apply_cb);

void lv_keyboard_set_user_data(lv_obj_t *keyboard, void* user_data);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the language for the keyboard 
 * @param keyboard pointer to the keyboard
 * @return language of 'kbd_lang_t'
 */
kbd_lang_t lv_keyboard_get_language(lv_obj_t *keyboard);

/**
 * Get the src of the keyboard  ( lv_obj_t*  lv_label or lv_ta only)
 * @param keyboard pointer to the keyboard
 * @return language of 'lv_obj_t*'
 */

lv_obj_t* lv_keyboard_get_src(lv_obj_t *keyboard);


void * lv_keyboard_get_user_data(lv_obj_t *keyboard);

/*=====================
 * Other functions
 *====================*/

/**
 * Default keyboard event after kb is applied and send edited text to the outside obj.
 * @param kb pointer to a  keyboard_ext's kb obj
 * @param event the triggering event  only LV_EVENT_APPLY and LV_EVENT_CANCEL
 */
void keyboard_def_event_cb(lv_obj_t* kb, lv_event_t event);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LV_KEYBOARD_H
