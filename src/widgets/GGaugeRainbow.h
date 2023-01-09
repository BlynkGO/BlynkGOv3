#ifndef __GGAUGE_RAINBOW_H__
#define __GGAUGE_RAINBOW_H__

#include "GObject.h"
#include "GLabel.h"
#include "GArc.h"
#include "GGauge.h"

typedef struct {
  GGauge_type_t gauge_type;
  GLabel  *lb_val;
  GArc   **arc;
  uint8_t arc_max_num;
  float  val_min;
  float  val_max;
  float  val;
  uint8_t decimal;
  color_t color[3];  // grad1_color, grad2_color, bg_color  
  float angle_offset;
  float angle_total;
  float angle_space;
  float angle_unit;
  uint8_t arc_id;
} ggaugerainbow_ext_t;

class GGaugeRainbow : public GObject {
  public:
    GGaugeRainbow(GWidget& parent=GScreen);
    GGaugeRainbow(GWidget* parent);
    virtual ~GGaugeRainbow();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void      gauge_type(GGauge_type_t type);  // type : GAUGE_TYPE_ARC (defaut) , GAUGE_TYPE_FULL
    GGauge_type_t gauge_type();

    void      range(float val_min, float val_max);
    void      decimal(uint8_t decimal_num);  // แสดงทศนิยม 0 หรือ 1 หรือ 2 หรือ 3 ตำแหน่ง
    uint8_t   decimal();
    void      value(float val , int8_t decimal_num=-1);  // ถ้า decimal_num = -1 จะใช้ค่าที่ระบบกำหนดจาก decimal(..)
    float     value();
    float     value_max();
    float     value_min();
            
    void      color(color_t color_begin, color_t color_end);
    color_t   color_begin();
    color_t   color_end();
    color_t   color_bg();
    void      color_bg(color_t color);
    void      thickness(uint8_t thick);
    uint8_t   thickness();
    
    void      font(font_t &font);        // font สำหรับแสดง value ตรงกลาง
    const font_t* font();
    void      font_color(color_t color);  
    color_t   font_color();
    inline void  font(font_t &font, color_t color)      { this->font(font); this->font_color(color); }
    inline void  text_color(color_t color)              { this->font_color(color); }

    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }


    GLabel&   label();    // ข้อความตรงกลาง    
    GGaugeRainbow&   operator =(float num);

    // inner API
    void      refresh_size();
  private:
    void      _set_arc(uint8_t id, bool active);
};

#endif //__GGAUGE_RAINBOW_H__
