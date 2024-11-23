/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSlider.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GSLIDER_H__
#define __GSLIDER_H__

#include "GObject.h"

class GSlider : public GObject {
  public:
    GSlider(GWidget& parent=GScreen);
    GSlider(GWidget* parent);
    GSlider(int16_t val_min, int16_t val_max, GWidget& parent=GScreen);
    GSlider(int16_t val_min, int16_t val_max, GWidget* parent);

#if BLYNKGO_USE_BLYNK==0
    GSlider(int16_t val_min, int16_t val_max, slider_type_t type, GWidget& parent=GScreen);
    GSlider(int16_t val_min, int16_t val_max, slider_type_t type, GWidget* parent);
#else
    GSlider(uint8_t virtual_pin, GWidget& parent=GScreen);
    GSlider(uint8_t virtual_pin, GWidget* parent);
    GSlider(int16_t val_min, int16_t val_max, uint8_t virtual_pin, GWidget& parent=GScreen);
    GSlider(int16_t val_min, int16_t val_max, uint8_t virtual_pin, GWidget* parent);
    GSlider(int16_t val_min, int16_t val_max, uint8_t virtual_pin, slider_type_t type, GWidget& parent=GScreen);
    GSlider(int16_t val_min, int16_t val_max, uint8_t virtual_pin, slider_type_t type, GWidget* parent);
#endif

    virtual ~GSlider();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createSliderStyle();
            void freeSliderStyle();

    GSlider& operator =(int num);

    void    slider_type(slider_type_t type);        // SLIDER_TYPE_NORMAL, SLIDER_TYPE_SYM, SLIDER_TYPE_RANGE
    slider_type_t slider_type();

    void    range(int16_t val_min, int16_t val_max);    // กำหนดค่าน้อยสุด ค่ามากสุดของ GSlider ที่จะเลือกได้
    int16_t max_value();                                // คืนค่า ค่ามากสุด
    int16_t min_value();                                // คืนค่า ค่าน้อยสุด
    void    value(int16_t val, bool anim = true);       // กำหนดค่า ให้ GSlider หรือจะใช้ operator = ในการกำหนดก็ได้
    int16_t value();                                    // คืนค่า ปัจจุบันของ GSlider 
    inline int16_t toInt()                                                  { return this->value();   }

    // สำหรับ slider_type : SLIDER_TYPE_RANGE
    inline void    right_value(int16_t val, bool anim = true)               { value(val, anim); }
    inline int16_t right_value()                                            { return value(); }
           void    left_value(int16_t val , bool anim = true);
           int16_t left_value();

    inline void    upper_value(int16_t val, bool anim = true)               { right_value(val, anim); }
    inline int16_t upper_value()                                            { return right_value();   }
    inline void    lower_value(int16_t val , bool anim = true)              { left_value(val, anim);  }
    inline int16_t lower_value()                                            { return left_value();    }

    void knob_image(const img_t & src_img);             // กำหนดรูปให้ ปุ่มเลื่อน
    void knob_image(const img_t * src_img);
    
#if LV_USE_ANIMATION
    void     anim_time(uint16_t time);
    uint16_t anim_time();
#endif // LV_USE_ANIMATION

#if BLYNKGO_USE_BLYNK
    void    virtualPin(uint8_t vpin);
    uint8_t virtualPin();
#endif

    // style[BG], style[INDIC], style[KNOB] ;  default --> style[INDIC]
    GStyle style[3]; 
  private:
    int16_t val;
    int16_t val_min;
    int16_t val_max;
    slider_type_t _slider_type;

#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif
};

#endif //__GSLIDER_H__
