/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GLed.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GLED_H__
#define __GLED_H__

#include "GObject.h"

class GLed : public GObject {
  public:
    GLed(GWidget& parent=GScreen);
    GLed(GWidget* parent);
    GLed(uint16_t color, GWidget& parent=GScreen);
#if BLYNKGO_USE_BLYNK
    GLed(uint16_t color, uint8_t virtual_pin, GWidget &parent = GScreen);
#endif
    virtual ~GLed();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void    color(uint16_t color);
    void    color(uint16_t color, uint16_t gradian_color);
    void    OFF();
    void    ON();
    bool    isON();
    bool    isOFF();
    bool    toggle();   // invert state
    void    brightness(uint8_t brightness);
    uint8_t brightness();  // get brightness

    bool    state();
    void    state(bool led_state);


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
    bool    _state = false;

    uint8_t gpin;
    uint8_t gpin_active=HIGH;
#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif

};


#endif // __GLED_H__
