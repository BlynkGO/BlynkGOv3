/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSwitch.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GSWITCH_H__
#define __GSWITCH_H__

#include "GObject.h"

class GSwitch : public GObject {
  public:
    GSwitch(GWidget& parent=GScreen);
    GSwitch(GWidget* parent);
#if BLYNKGO_USE_BLYNK
    GSwitch(uint8_t virtual_pin, GWidget &parent = GScreen);
#endif
    virtual ~GSwitch();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createSwitchStyle();
            void freeSwitchStyle();

    void    ON(bool anim_enable=false);
    void    OFF(bool anim_enable=false);
    bool    isON();
    bool    isOFF();
    void    toggle();   // invert state
    bool    state();
    void    state(bool switch_on);

    void    knob_color(color_t color);
    void    knob_color(color_t color, color_t grad_color);
    color_t knob_color();
    color_t knob_grad_color();


    void    gpio(uint8_t pin, bool active_high = HIGH);
    uint8_t gpio();
    void    active_high(bool active_high);
    bool    active_high();

#if BLYNKGO_USE_BLYNK
    void    virtualPin(uint8_t vpin);
    uint8_t virtualPin();
#endif

    // style[BG], style[INDIC], style[KNOB_OFF], style[KNOB_ON]
    GStyle style[4];
    // GSwitch's style -> style[INDIC]
  private:
    uint8_t gpin;
    uint8_t gpin_active=HIGH;

#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif
};

#endif //__GSWITCH_H__
