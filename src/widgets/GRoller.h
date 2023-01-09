/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GRoller.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GROLLER_H__
#define __GROLLER_H__

#include "GObject.h"

class GRoller : public GObject {
  public:
    GRoller(GWidget& parent=GScreen);
    GRoller(GWidget* parent);
    virtual ~GRoller();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createRollerStyle();
            void freeRollerStyle();

    void     options(String* option, uint16_t option_num);
    void     options(int from, int to, bool zero_front=false);
    uint16_t options_count();

    void     option_align(label_align_t align);  // align : LABEL_ALIGN_LEFT, LABEL_ALIGN_RIGHT, LABEL_ALIGN_CENTER
    void     visible_rows(uint16_t count);

    void     selected(uint16_t option_id, lv_anim_enable_t anim = false);  // (0 ... number of option - 1)
    String   selected();
    uint16_t selected_id();

    inline String toString()    { return selected();                    }
    inline int    toInt()       { return this->toString().toInt();      }
    inline float  toFloat()     { return this->toString().toFloat();    }
    inline double toDouble()    { return this->toString().toDouble();   }
    inline void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0){
      this->toString().toCharArray(buf, bufsize, index);
    }

    void     width(uint16_t w);
    uint16_t width();

    // ให้ กำหนดก่อน options(..)
    void      font(font_t &font);
    void      font(font_t &font, color_t color);
    const font_t* font();
    void    font_color(color_t color);  // สีฟอนต์ตอนตรงกลาง   (ตรงอื่นสีดำ)
    color_t font_color();
    void    color(color_t color);       // สีตอนเลือกตรงกลาง    (ตรงอื่นพื้นขาว)
    color_t color();
    void    opa(uint8_t opa);
    uint8_t opa();

    GStyle style[2]; // style[BG] (_style), style[SELECT] 
};

#endif //__GROLLER_H__
