/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GSwitch3D.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GSWITCH3D_H__
#define __GSWITCH3D_H__

#include "GObject.h"

class GSwitch3D : public GObject {
  public:
    GSwitch3D(GWidget &parent = GScreen);
    GSwitch3D(GWidget *parent);
    GSwitch3D(uint16_t color , sw3d_type_t type=SW3D_TYPE_RECT, GWidget &parent = GScreen);
#if BLYNKGO_USE_BLYNK
    GSwitch3D(uint16_t color , sw3d_type_t type, uint8_t virtual_pin, GWidget &parent = GScreen);
#endif
    virtual ~GSwitch3D();
    
    virtual void create();
            void create(GWidget *parent);
            void create(GWidget &parent);

    void    sw3d_type(sw3d_type_t type); //  SW3D_TYPE_RECT,  SW3D_TYPE_ROUND,
    sw3d_type_t sw3d_type();

    void    ON();
    void    OFF();
    bool    isON();
    bool    isOFF();
    void    toggle();
    bool    state();
    void    state(bool sw_state);


    void    gpio(uint8_t pin, bool active_high = HIGH);
    uint8_t gpio();
    void    active_high(bool active_high);
    bool    active_high();

#if BLYNKGO_USE_BLYNK
    void    virtualPin(uint8_t vpin);
    uint8_t virtualPin();
#endif

  private:
    uint16_t _color = TFT_RED;
    sw3d_type_t _sw3d_type = SW3D_TYPE_RECT;

    uint8_t gpin;
    uint8_t gpin_active=HIGH;
#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif

};

#endif //__GSWITCH3D_H__
