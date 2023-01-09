/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ lv_fs_esp32.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __LV_FS_ESP32_H__
#define __LV_FS_ESP32_H__

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

#include "lv_fs_sd.h"
#include "lv_fs_spiffs.h"

#define LV_USE_FS_ESP32  1
#if LV_USE_FS_ESP32 != 0

void lv_fs_esp32_if_init();

// ESP32's FileSystem API
// - SPIFFS Drive F:
// - SD Drive S:

int  listDir(const char* directory); /* list files/directories in the directory*/
bool typeFile(const char* path);     /* show data in the file */
bool renameFile(const char* path_old, const char* path_new); /* rename file */
bool copyFile(const char* path_source, const char* path_target);  /* copy file */
bool moveFile(const char* path_source, const char* path_target);  /* move file */
bool deleteFile(const char* path);  /* delete file */
void changeDir(const char* path);   /* change current directory */
void changeDrive(const char* drive);  /* charge current drive */
const char* currentDir();	        /* current directory*/
uint8_t existPath(const char* path);  /* file/directory exists?    0 : not exist; 1 : file exist ; 2 : directory exist */


#endif //LV_USE_FS_ESP32

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__LV_FS_ESP32_H__
