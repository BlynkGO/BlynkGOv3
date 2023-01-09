/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_fs_spiffs.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __LV_FS_SPIFFS_H__
#define __LV_FS_SPIFFS_H__

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

#define LV_USE_FS_SPIFFS  1
#if LV_USE_FS_SPIFFS != 0

void lv_fs_spiffs_if_init();

#endif //LV_USE_FS_SPIFFS

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__LV_FS_SPIFFS_H__
