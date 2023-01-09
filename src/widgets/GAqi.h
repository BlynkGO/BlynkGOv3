/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GAqi.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GAQI_H__
#define __GAQI_H__

#include "GObject.h"

enum {
  AQI_US,
  AQI_TH,
  AQI_CN,
};

class GAqi : public GObject {
  public:
    GAqi(GWidget& parent=GScreen);
    GAqi(GWidget* parent);
    virtual ~GAqi();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
     
    void        aqi_type(aqi_type_t type); 	//  AQI_US,  AQI_TH (default),  AQI_CN,
    aqi_type_t  aqi_type();

    void        warning_hidden(bool enable);
    bool        warning_hidden();

    void        pm25(float val);		// µg/m3
    float       pm25();							// µg/m3
    void        pm10(float val);
    float       pm10();
    void        pm1(float val);
    float       pm1();

    void        setAQI(float val);
    float       getAQI();
    float       getAQI(aqi_type_t type);

  private:
};

#endif //__GAQI_H__

