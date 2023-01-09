
/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GLevel.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GLEVEL_H__
#define __GLEVEL_H__

#include "GObject.h"
#include "GButton.h"

typedef struct _glevel_ext_t {
  GObject *bar_bg;      /* bar background */
  GObject *bar_indic;   /* bar indic */
  GButton *btn_up;      /* button level up */
  GButton *btn_down;    /* button level down */
  int16_t val_min;
  int16_t val_max;
  int16_t val;
} glevel_ext_t;

class GLevel :public GObject {
  public:
    GLevel(GWidget& parent=GScreen);
    GLevel(GWidget* parent);
    virtual ~GLevel();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    GLevel& operator =(int16_t val);

    void    range(int16_t val_min, int16_t val_max);
    void    value(int16_t val);
    int16_t value();
    int16_t value_min();
    int16_t value_max();

    void    color(color_t color);
    void    color(color_t color, color_t gradian_color);
    color_t color();

    void    bg_color(color_t color);
    color_t bg_color();

    // inner api ; does't call this APIs
    void    _vertical_design();
    void    _horizontal_design();

};


#endif //__GLEVEL_H__
