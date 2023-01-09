/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ blynkgo_lib.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#ifndef __BLYNKGO_LIB_H__
#define __BLYNKGO_LIB_H__

#include "utils/lvgl-dev-7.0/src/lv_themes/lv_theme.h"
#include "utils/lvgl-dev-7.0/src/lv_core/lv_obj.h"
#include "utils/lvgl-dev-7.0/src/lv_core/lv_indev.h"
#include "utils/lvgl-dev-7.0/src/lv_misc/lv_math.h"
#include "utils/lvgl-dev-7.0/src/lv_misc/lv_txt.h"
#include "utils/lvgl-dev-7.0/src/lv_draw/lv_draw.h"
#include "utils/lvgl-dev-7.0/src/lv_core/lv_refr.h"
#include "utils/lvgl-dev-7.0/lvgl.h"
#include "utils/esp32_utils.h"
#include "utils/BlynkGO_Type.h"

#include "objx/lv_objx_trident.h"
#include "qrcode/qrcodegen.h"


#include <pgmspace.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN PROGMEM
#endif



#endif // __BLYNKGO_LIB_H__