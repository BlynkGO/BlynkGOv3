/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ img_decoder.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * @file img_decoder.h
 *
 */

#ifndef IMG_DECODER_H
#define IMG_DECODER_H

#ifdef __cplusplus
extern "C" {
#endif


#include "../extra/GLVGL.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "lodepng/lodepng.h"
#include "libbmp/libbmp.h"
#include "gifdec/gifdec.h"
#include "esp32/rom/tjpgd.h"
#include "esp_heap_caps.h"


lv_res_t png_to_img_dsc(const char* filename, lv_img_dsc_t* img_dsc);
lv_res_t jpg_to_img_dsc(const char* filename, lv_img_dsc_t* img_dsc);
lv_res_t bmp_to_img_dsc(const char* filename, lv_img_dsc_t* img_dsc);
void     free_img_dsc(lv_img_dsc_t* img_dsc);

lv_res_t jpg_data_to_img_dsc(uint8_t* input_data, size_t input_size, lv_img_dsc_t* img_dsc);


bool     isPngFile(const char* filename);
bool     isJpgFile(const char* filename);
bool     isBmpFile(const char* filename);
bool     isGifFile(const char* filename);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*IMG_DECODER_H*/
