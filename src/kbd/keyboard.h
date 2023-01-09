/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ keyboard.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __keyboard_h___
#define __keyboard_h___

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/lvgl-dev-7.0/src/lv_core/lv_obj.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_btnm.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_kb.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_ta.h"
#include "../utils/BlynkGO_Type.h"

#define KB_CTRL_BTN_FLAGS (LV_BTNM_CTRL_NO_REPEAT | LV_BTNM_CTRL_CLICK_TRIG)

const lv_btnm_ctrl_t kbd_ctrl_map[] = {
  KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 5,
  KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 5,
  KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 5,
  5, 3, 6, 3, 5,
};

const lv_btnm_ctrl_t kbd_num_ctrl_map[] = {
  KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 5,
  KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 5,
  KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 5,
  5, 4, 4, 4, 5,
};


#include "en_kbd.h"
#include "th_kbd.h"
#include "la_kbd.h"
#include "vn_kbd.h"

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__keyboard_h___
