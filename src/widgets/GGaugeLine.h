/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GGaugeLine.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GGAUGELINE_H__
#define __GGAUGELINE_H__

#include "GObject.h"

class GGaugeLine : public GObject {
  public:
    GGaugeLine(GWidget& parent=GScreen);
    GGaugeLine(GWidget* parent);
    virtual ~GGaugeLine();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void      range(int16_t value_min, int16_t value_max);
    int16_t   value_min();
    int16_t   value_max();
    void      value(uint8_t needle_id, int16_t value);
    // void      value(int16_t value);  // ใส่ให้ค่า needle_id = 0
    int16_t   value(uint8_t needle_id=0);

    void      critical_value(int16_t value);
    int16_t   critical_value();
    void      critical_color(color_t color);
    color_t   critical_color();

    void      scale(uint16_t scale_angle, uint8_t line_count, uint8_t label_count);
    uint16_t  scale_angle();
    uint8_t   line_count();
    uint8_t   label_count();

    inline void scale_color(color_t color_start, color_t color_end)    { this->color(color_start,color_end);}
    inline void scale_color(color_t color)                             { this->color(color);}
    void      scale_length(uint8_t len);
    uint8_t   scale_length();
    void      scale_padding(uint8_t label_padding);
    uint8_t   scale_padding();
    void      scale_width(uint8_t width);
    uint8_t   scale_width();
    void      circle_middle_color(color_t color);
    color_t   circle_middle_color();
    void      circle_middle_radius(uint16_t radius);
    uint16_t  circle_middle_radius();

    void      needle_color(lv_color_t color[], uint8_t num);
    void      needle_color(color_t color[], uint8_t num);
    void      needle_color(color_t color);
    uint8_t   needle_count();

    void      operator=(int16_t value);
    void      size(uint16_t w, uint16_t h);

    class needle_class{
    	public:
    		needle_class() {}
    		void init(GGaugeLine* gauge) { this->_gauge = gauge; }
    		int16_t&  operator [](uint8_t needle_id) {
          lv_gauge_ext_t * ext = (lv_gauge_ext_t*) _gauge->ext_attr();
          return ext->values[needle_id];
    		}
    		GGaugeLine* _gauge;
    };

    needle_class needle;

    // inner use (don't call)
    lv_color_t *_color =NULL;
  private:
    uint16_t _w=240;
    uint16_t _h=240;

};

#endif //__GGAUGELINE_H__

