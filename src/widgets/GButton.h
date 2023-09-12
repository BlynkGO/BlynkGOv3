/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GButton.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GBUTTON_H__
#define __GBUTTON_H__

#include "GContainer.h"
#include "GLabel.h"

typedef struct {
  lv_btn_ext_t btn;
  GLabel *label;
} gbutton_ext_t;

class GButton : public GContainer {
  public:
    GButton(GWidget& parent=GScreen);
    GButton(GWidget* parent);

    GButton(String label, GWidget& parent=GScreen);
    GButton(const char* label, GWidget& parent=GScreen);
    GButton(const char* label, GWidget* parent);
    GButton(button_type_t type, const char* label, GWidget& parent=GScreen);
    GButton(button_type_t type, const char* label, GWidget* parent);
#if BLYNKGO_USE_BLYNK
    GButton(button_type_t type, String label, uint8_t virtual_pin, GWidget& parent=GScreen);
    GButton(button_type_t type, String label, uint8_t virtual_pin, GWidget* parent);
    GButton(button_type_t type, const char* label, uint8_t virtual_pin, GWidget& parent=GScreen);
    GButton(button_type_t type, const char* label, uint8_t virtual_pin, GWidget* parent);
#endif

    virtual ~GButton();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createButtonStyle();
            void freeButtonStyle();
            
    /* button's mode : BUTTON_MODE_NORMAL or BUTTON_MODE_TOGGLE or BUTTON_MODE_INACTIVE */
    void          mode(button_mode_t mode);  
    button_mode_t mode();

                      //สวิทซ์ ปล่อย  กด       สวิทซ์ปล่อย  ในโหมด Toggle
    /* button's state : RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE, */
    void     state(button_state_t button_state); 
    button_state_t state();

    /* API สำหรับ BUTTON_SWITCH */
           void toggle();                   // สำหรับ BUTTON_SWITCH ให้สลับ toggle เปิด->ปิด, หรือ ปิด->เปิด
           void ON();                       // สำหรับ BUTTON_SWITCH ให้สั่ง turn on
           void OFF();                      // สำหรับ BUTTON_SWITCH ให้สั่ง turn off
    inline bool isON()                      { return this->state() == TOGGLE_RELEASED;}   // เฃ็คว่าปุ่มกำลัง turn on อยู่ไหม?
    inline bool isOFF()                     { return this->state() != TOGGLE_RELEASED;}   // เฃ็คว่าปุ่มกำลัง turn off อยู่ไหม?

    /* API ใช้ได้ทั้ง BUTTON_SWITCH และ BUTTON_PUSH */
           void inactive(bool enable);                                                         // กำหนดให้ปุ่ม inactive หรือไม่
    inline bool inactive()                  { return (this->mode() == BUTTON_MODE_INACTIVE); } // เช็คว่าปุ่ม inactive หรือไม่

    GButton& operator =(String label);
    GButton& operator =(const char* cstr);
    GButton& operator +=(const String label);
    GButton& operator +=(const char *cstr);
    GButton& operator +=(char c);
    GButton& operator +=(unsigned char num);
    GButton& operator +=(int num);
    GButton& operator +=(unsigned int num);
    GButton& operator +=(long num);
    GButton& operator +=(unsigned long num);
    GButton& operator +=(float num);
    GButton& operator +=(double num);
    GButton& operator += (const __FlashStringHelper *str);
    

    void     text(String text);
    String   text();
    void     text_align(label_align_t text_align);
    void     font(font_t &font);
    void     font(font_t &font, color_t color);

    inline void font(font_t *font)                      { this->font(*font);        }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }

    const font_t* font();
    void     text_color(color_t color);
    color_t  text_color();
    
    inline void     font_color(color_t color)           { this->text_color(color);   }
    inline color_t  font_color()                        { return this->text_color(); }
    
    GStyle   style[5]; // RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE,

    void     rounded(bool enable);
    bool     rounded();

    void     radius(uint8_t r);
    uint8_t  radius();
    void     corner_radius(uint8_t r)       { this->radius(r); }

    void     color(color_t color);
    color_t  color();

    // API ปรับทั้ง 5 styles
    void     border(uint8_t width);
    void     border(uint8_t width, color_t color);
    uint8_t  border();
    void     border_color(color_t color);
    color_t  border_color();

    void     shadow(uint16_t width);
    void     shadow(uint16_t width, color_t color);
    uint16_t shadow();
    void     shadow_color(color_t color);
    color_t  shadow_color();
    void     shadow_opa(uint8_t opa);
    uint8_t  shadow_opa();
    void     shadow_offset(point_t point);
    void     shadow_offset(int16_t offset_x, int16_t offset_y);
    point_t  shadow_offset();
    void     shadow_spread(uint16_t spread);
    uint16_t shadow_spread();
    
    void     padding(uint8_t padding);
    void     padding_top(uint8_t padding);
    void     padding_bottom(uint8_t padding);
    void     padding_left(uint8_t padding);
    void     padding_right(uint8_t padding);
    void     padding_inner(uint8_t padding);

    inline void padding(uint16_t padding_top,uint16_t padding_bottom, uint16_t padding_left,uint16_t padding_right, uint16_t padding_inner) 
                {  
                    this->padding_top(padding_top);
                    this->padding_bottom(padding_bottom);
                    this->padding_left(padding_left);
                    this->padding_right(padding_right);
                    this->padding_inner(padding_inner);
                }
    inline void padding_top_bottom(uint16_t padding)
                {
                    this->padding_top(padding);
                    this->padding_bottom(padding);
                }

    inline void padding_top_bottom(uint16_t padding_top, uint16_t padding_bottom)
                {
                    this->padding_top(padding_top);
                    this->padding_bottom(padding_bottom);
                }

    inline void padding_left_right(uint16_t padding)
                {
                    this->padding_left(padding);
                    this->padding_right(padding);
                }

    inline void padding_left_right(uint16_t padding_left, uint16_t padding_right)
                {
                    this->padding_left(padding_left);
                    this->padding_right(padding_right);
                }

    void     round_design();        // ปรับให้เป็นปุ่มวงกลมสำหรับ SYMBOL_XXXX ที่จะใช้เป็นปุ่มกด
    
    
    void     gpio(uint8_t pin, bool active_high = HIGH);
    uint8_t  gpio();
    void     active_high(bool active_high);
    bool     active_high();

    GLabel&  label();

#if BLYNKGO_USE_BLYNK
    void     virtualPin(uint8_t vpin);
    uint8_t  virtualPin();
#endif
  
  private:
    button_type_t _type = BUTTON_PUSH;
    bool   _round=false;
    String _btn_text = "Button";
    void   btn_reflesh();

    uint8_t gpin;
    uint8_t gpin_active=HIGH;
#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif
};



#endif //__GBUTTON_H__
