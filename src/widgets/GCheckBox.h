/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GCheckBox.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GCHECKBOX_H__
#define __GCHECKBOX_H__

#include "GObject.h"

class GCheckBox : public GObject {
  public:
    GCheckBox(GWidget& parent=GScreen);
    GCheckBox(GWidget* parent);
    virtual ~GCheckBox();

    virtual void create(); 
            void create(GWidget& parent);
            void create(GWidget* parent);

    GCheckBox& operator =(String checkbox_text);

    void    text(String checkbox_text);
    String  text();
    void    checked(bool enable);
    bool    checked();

    void    color(uint16_t color);
    uint16_t color();
    void    font(font_t &font);
    void    font(font_t &font, color_t color);
    
    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }

    const font_t* font();
};

#endif //__GCHECKBOX_H__
