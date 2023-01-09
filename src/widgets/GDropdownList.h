/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GDropdownList.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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



#ifndef __GDROPDOWNMENU_H__
#define __GDROPDOWNMENU_H__

#include "GObject.h"

class GDropdownList : public GObject {
  public:
    GDropdownList(GWidget& parent=GScreen);
    GDropdownList(GWidget* parent);
    virtual ~GDropdownList();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createDDListStyle();
            void freeDDListStyle();

    void        options(String* option, uint16_t option_num);  // ใส่ด้วย String Array
    void        options(int from, int to, bool zero_front=false);
    void        options(const char * options_list);  // ตัวอย่าง "aaa\nbbb\nccc\nddd"
    const char* options();
    uint16_t    options_count();
    void        option_align(label_align_t align);
    label_align_t option_align();

    void        selected(uint16_t option_id);
    String      selected();
    uint16_t    selected_id();

    inline String toString()    { return selected();                    }
    inline int    toInt()       { return this->toString().toInt();      }
    inline float  toFloat()     { return this->toString().toFloat();    }
    inline double toDouble()    { return this->toString().toDouble();   }
    inline void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0){
      this->toString().toCharArray(buf, bufsize, index);
    }
    
    void        size(uint16_t w, uint16_t h, uint16_t h_open=0);
    void        width(uint16_t w);
    uint16_t    width();
    void        height(uint16_t h);            // height : close
    uint16_t    height();
    void        height_open(uint16_t h);       // height : open
    uint16_t    height_open();

    void        stay_open(bool enable);
    bool        stay_open();
    void        open(lv_anim_enable_t anim = true);
    void        close(lv_anim_enable_t anim = true);

    void        arrow(bool enable);
    void        arrow_symbol(const char * symbol);
    const char* arrow_symbol();

    // ให้ กำหนดก่อน options(..)
    void        font(font_t &font);
    void        font(font_t &font, color_t color);
    
    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }

    const font_t* font();
    void        color(uint16_t selected_color);
    color_t     color();

    /* SCROLLBAR_OFF, SCROLLBAR_ON, SCROLLBAR_DRAG/SCROLLBAR_AUTOHIDE, 
       SCROLLBAR_AUTO, SCROLLBAR_HIDE, SCROLLBAR_UNHIDE*/
    void        scrollbar(scrollbar_t sb_mode);
    scrollbar_t scrollbar();

    GStyle style[2]; // style[BG] (_style), style[SELECT]
  private:
    uint16_t h_close;
};


#endif //__GDROPDOWNMENU_H__
