/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSliderH.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GSliderH_H__
#define __GSliderH_H__

#include "GObject.h"

class GSliderH : public GObject {
  public:
    GSliderH(GWidget& parent=GScreen);
    GSliderH(GWidget* parent);
    GSliderH(int16_t val_min, int16_t val_max, GWidget& parent=GScreen);
    GSliderH(int16_t val_min, int16_t val_max, GWidget* parent);
#if BLYNKGO_USE_BLYNK
    GSliderH(uint8_t virtual_pin, GWidget& parent=GScreen);
    GSliderH(int16_t val_min, int16_t val_max, uint8_t virtual_pin, GWidget& parent=GScreen);
#endif
    virtual ~GSliderH();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    GSliderH& operator =(int num);

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

#endif //__GSliderH_H__
