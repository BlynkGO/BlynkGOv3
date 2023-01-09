/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GGauge.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

/*
 *********************************************************************
 * GGauge 
 * Version 1.0
 *  คุณสมบัติ
 *    1. support 3 sizes  :  size(120,120)   size(240,240)  size(320,320)
 *    2. support 2 types  :  GAUGE_TYPE_ARC, GAUGE_TYPE_FULL
 *    3. support 2 endings:  rounded or not rounded ending
 *
 * Version 1.1
 *    1. add size : size(600,600) for BlynkGO Enterprise (TFT 800x480)
 *    2. add API thickness(..)
 *
 *********************************************************************
 */

#ifndef __GGAUGE_H__
#define __GGAUGE_H__

#include "GObject.h"
#include "GArc.h"
#include "GLabel.h"

enum {
  GAUGE_TYPE_ARC,
  GAUGE_TYPE_FULL,
};
typedef uint8_t GGauge_type_t;

typedef struct {
  GArc *arc_bg;
  GArc *arc_fg;
  GLabel *title;
  GLabel *lb_val;
  GLabel *lb_val_min;
  GLabel *lb_val_max;
  GGauge_type_t gauge_type;
} ggauge_ext_t;

class GGauge : public GObject {
  public:
    GGauge(GWidget& parent=GScreen);
    GGauge(GWidget* parent);
    GGauge(uint16_t color, GWidget& parent=GScreen);
    GGauge(uint16_t color, GWidget* parent);
    GGauge(String title, uint16_t color, GWidget& parent=GScreen);
    GGauge(String title, uint16_t color, GWidget* parent);
    virtual ~GGauge();
    
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void      title(String title);
    String    title();
    void      title_color(color_t color);
    color_t   title_color();
    void      title_font(font_t &font);
    void      title_font(font_t &font, color_t color);

    inline void title_font(font_t *font)                      { this->title_font(*font); }
    inline void title_font(font_t *font, color_t color)       { this->title_font(*font, color); }

    void      range(float val_min, float val_max);
    void      range_font(font_t &font);
    void      range_font(font_t &font, color_t color);
    void      range_color(color_t color);
    void      range_hidden(bool hidden);
    bool      range_hidden();

    void      decimal(uint8_t decimal_num);  // แสดงทศนิยม 0 หรือ 1 หรือ 2 หรือ 3 ตำแหน่ง
    void      value(float val , int8_t decimal_num=-1);  // ถ้า decimal_num = -1 จะใช้ค่าที่ระบบกำหนดจาก decimal(..)
    float     value();
    float     value_max();
    float     value_min();

    void      color(color_t color);
    color_t   color();

    void      color(color_t arc_fg_color, color_t arc_bg_color);
    void      arc_bg_color(color_t color);
    color_t   arc_bg_color();

    void      font(font_t &font);				// font สำหรับแสดง value ตรงกลาง
    void      font(font_t &font, color_t color);

    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }
    
    void      text_color(color_t color);
    inline void font_color(color_t color)     { this->text_color(color); }

    void      gauge_type(GGauge_type_t type);  // type : GAUGE_TYPE_ARC (defaut) , GAUGE_TYPE_FULL
    GGauge_type_t gauge_type();

    void      rounded(bool enable);
    bool      rounded();

    void      thickness(uint8_t thick);
    uint8_t   thickness();

    GGauge&   operator =(float num);

  private:
    uint16_t _color = TFT_BLUE;
    float    _val_min = 0;
    float    _val_max = 100;
    float    _val     = 0;
    uint8_t  _decimal_point = 0;
    String   _title = "";
};


#endif //__GGAUGE_H__

