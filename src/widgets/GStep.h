/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GStep.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GSTEP_H__
#define __GSTEP_H__

#include "GObject.h"
#include "GLabel.h"
#include "GButton.h"

typedef struct {
  GButton *btnPlus;
  GButton *btnMinus;
  GLabel  *lbStep;
  float   step_internal;
  float   step;
  uint8_t decimal;
} gstep_ext_t;

class GStep : public GObject {
  public:
    GStep(GWidget& parent=GScreen);
    GStep(GWidget* parent);
    GStep(int16_t val_min, int16_t val_max, GWidget& parent=GScreen);
    virtual ~GStep();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void    range(float val_min, float val_max);
    void    value(float val);
    float   value();
    float   value_min();
    float   value_max();
    void    step(float step);
    float   step();
    void    decimal(uint8_t decimal);
    uint8_t decimal();

    void    plus(float step=1);
    void    minus(float step=1);

    void    font(font_t &font);
    void    font(font_t &font, color_t color);
    
    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }

    const font_t* font();

    void    text_color(color_t color);
    void    font_color(color_t color)    { this->text_color(color); }
    void    color(color_t color);
    color_t color();

    GStep&  operator =(float num);

  private:
    float   _val=0, _val_min=0, _val_max=100;

    void    refresh_size();
};

#endif //__GSTEP_H__
