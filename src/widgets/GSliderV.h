/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSliderV.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GSliderV_H__
#define __GSliderV_H__

#include "GObject.h"

class GSliderV : public GObject {
  public:
    GSliderV(GWidget& parent=GScreen);
    GSliderV(GWidget* parent);
    GSliderV(int16_t val_min, int16_t val_max, GWidget& parent=GScreen);
    GSliderV(int16_t val_min, int16_t val_max, GWidget* parent);
    virtual ~GSliderV();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            // void createSliderVStyle();

    GSliderV& operator =(int num);

    void    range(int16_t val_min, int16_t val_max);
    void    value(int16_t val);
    int16_t value();
    inline int16_t toInt()         { return this->value();   }
    
    void    title(const char* title);
    const char*  title();
    void    unit(const char* unit);
    const char*  unit();

#if BLYNKGO_USE_BLYNK
    void    virtualPin(uint8_t vpin);
    uint8_t virtualPin();
#endif

  private:
    int16_t val;
    int16_t val_min;
    int16_t val_max;

#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif
};

#endif //__GSliderV_H__
