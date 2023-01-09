/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GKnob.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GKNOB_H__
#define __GKNOB_H__

#include "GObject.h"

class GKnob : public GObject{
  public:
    GKnob(GWidget& parent=GScreen);
    GKnob(GWidget* parent);
#if BLYNKGO_USE_BLYNK
    GKnob(uint8_t virtual_pin, GWidget& parent=GScreen);
#endif
    virtual ~GKnob();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createKnobStyle();
            void freeKnobStyle();

    GKnob& operator =(int num);

    void    color(uint16_t color);
    void    range(int16_t val_min, int16_t val_max);
    void    value(int16_t val);
    int16_t value();
    int16_t value_min();
    int16_t value_max();

    GStyle style[2]; // style[BG] , style[INDIC]

#if BLYNKGO_USE_BLYNK
    void    virtualPin(uint8_t vpin);
    uint8_t virtualPin();
#endif
  private:
#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif
};

#endif //__GKNOB_H__

