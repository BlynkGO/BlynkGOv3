/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GJoystick.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GJOYSTICK_H__
#define __GJOYSTICK_H__

#include "GObject.h"

class GJoystick : public GObject {
  public:
    GJoystick(GWidget& parent=GScreen);
    GJoystick(GWidget* parent);
#if BLYNKGO_USE_BLYNK
    GJoystick(uint8_t virtual_pin, GWidget& parent=GScreen);
#endif
    virtual ~GJoystick();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createJoystickStyle();
            void freeJoystickSytle();

    void    color(color_t color);
    color_t color();
    void    border(uint8_t w);
    uint8_t border();

    void     setXY(uint16_t x, uint16_t y);
    uint16_t x();
    uint16_t y();

    GStyle style[2];   // style[BG], style[JOY]

#if BLYNKGO_USE_BLYNK
    void    virtualPin(uint8_t vpin);
    uint8_t virtualPin();
  private:
    uint8_t vpin;
#endif

};

#endif //__GJOYSTICK_H__
