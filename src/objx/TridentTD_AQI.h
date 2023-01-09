/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ TridentTD_AQI.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __TRIDENTTD_AQI_H__
#define __TRIDENTTD_AQI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct _aqi_level{
  uint16_t max_aqi;
  uint16_t font_color;
  uint16_t fg_color;
  uint16_t bg_color;
  const char* warning_abv;
  const char* warning_msg;
} aqi_level_t;

extern aqi_level_t US_AQI[7];
extern aqi_level_t TH_AQI[5];
extern aqi_level_t CN_AQI[6];

/*=====================
   AQI functions
  ====================*/

#define US_AQI_MAX_LEVEL   7
#define TH_AQI_MAX_LEVEL   5
#define CN_AQI_MAX_LEVEL   6

int     PM25_to_usAQI(float pm25);
int     PM25_to_thAQI(float pm25);
int     PM25_to_cnAQI(float pm25);

int     PM10_to_usAQI(float pm10);
int     CO_to_usAQI(float co);// ppm
int     O3_8hr_to_usAQI(float o3);//ppb
int     NO2_to_usAQI(float no2);//ppb
int     SO2_1hr_to_usAQI(float so2); //ppb


uint8_t usAQI_level(int usAQI);  // us aqi -> level 0..6
uint8_t thAQI_level(int thAQI);  // th aqi -> level 0..4
uint8_t cnAQI_level(int cnAQI);  // cn aqi -> level 0..5


uint16_t usAQI_to_color(float aqi);
uint16_t thAQI_to_color(float aqi);
uint16_t cnAQI_to_color(float aqi);

float   usAQI_to_PM25(float aqi);
float   thAQI_to_PM25(float aqi);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //__TRIDENTTD_AQI_H__
