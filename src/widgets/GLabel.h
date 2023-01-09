/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GLabel.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GLABEL_H__
#define __GLABEL_H__

#include "GObject.h"

#define LABEL_USE_TEXTAREA         1
#if LABEL_USE_TEXTAREA
#include "GTextArea.h"
class GTextArea;
#endif

typedef struct {
  lv_label_ext_t label;
  bool      rolling;
  bool      rolling_by_long_mode;
  uint16_t  rolling_w;
  bool      editable;
} glabel_ext_t;

class GLabel : public GObject, public String {
  public:
    GLabel(GWidget& parent=GScreen);
    GLabel(GWidget* parent);
    GLabel(const char *cstr, GWidget& parent=GScreen);
    GLabel(const String &str, GWidget& parent=GScreen);
    GLabel(const char *cstr, GWidget* parent);
    GLabel(const String &str, GWidget* parent);
    virtual ~GLabel();

    virtual void create();
            void create(GObject *parent);
            void create(GObject &parent);

    void      text(String txt);         // กำหนดค่าเข้าไปภายใน label อย่างเดียว พร้อมทั้ง กำหนด String ที่เป็น parent class
    String    text();                   // ดึงค่าจากภายในออกมา ไม่ได้ดึงจาก String แม่
    void      raw_text(String txt);     // กำหนดค่าเข้าไปภายใน label อย่างเดียว
           
    
    void      font(font_t &font);
    void      font(font_t &font, color_t color);

    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }

    const font_t* font();
    void      color(color_t color);
    color_t   color();
    void      sel_color(color_t color);
    color_t   sel_color();
    void      opa(uint8_t opa);  // ค่าความโปร่งใส 0..255  ,0 ใส - 255 ทึบ
    uint8_t   opa();

    void      body_draw(bool enable);
    bool      body_draw();
    void      body_color(color_t color);
    void      body_color(color_t color, color_t grad_color);
    void      body_opa(uint8_t opa);

    void      editable(bool enable);
    bool      editable();
    void      wrapable(bool enable);  // สำหรับ parent ที่เป็น lv_page, lv_win

    // long_mode_t : LABEL_LONG_EXPAND, LABEL_LONG_BREAK, LABEL_LONG_DOT, LABEL_LONG_SROLL, LABEL_LONG_SROLL_CIRC, LABEL_LONG_CROP
    // long_mode() ใช้คู่กับ width(xx) ให้กำหนดความกว้างด้วย
    void      long_mode(long_mode_t long_mode); 
    void      long_mode(long_mode_t long_mode, uint16_t width, uint16_t speed_pps=25); // anim_speed (px/sec)
    long_mode_t long_mode(); 
    void      text_align(label_align_t align);  // label_align_t : LABEL_ALIGN_LEFT, LABEL_ALIGN_CENTER, LABEL_ALIGN_RIGHT
    label_align_t text_align();

    void      colorful(bool enable);
    bool      colorful();

    void      rolling(bool enable, uint16_t rolling_width=200, uint16_t speed_pps=25);
    bool      rolling();
    void      rolling_width(uint16_t rolling_width);
    uint16_t  rolling_width();
    void      rolling_speed(uint16_t rolling_speed); // pixel per sec
    uint16_t  rolling_speed();

    static String printf(const char* fmt, ...)  __attribute__((format(printf, 1, 2))); 
    static int split(String source, String delimiter, String** str_array);

    GLabel& operator =(const GLabel gLabel);
#if LABEL_USE_TEXTAREA
    GLabel& operator =(GTextArea& ta);
#endif
    GLabel& operator =(const String &label);
    GLabel& operator =(const char* cstr);
    GLabel& operator =(int num);
    // GLabel& operator =(float num);
    GLabel& operator +=(const String label);
    GLabel& operator +=(const char *cstr);
    GLabel& operator +=(char c);
    GLabel& operator +=(unsigned char num);
    GLabel& operator +=(int num);
    GLabel& operator +=(unsigned int num);
    GLabel& operator +=(long num);
    GLabel& operator +=(unsigned long num);
    GLabel& operator +=(float num);
    GLabel& operator +=(double num);
    GLabel& operator += (const __FlashStringHelper *str);

    using GObject::invalidate;
    using String::operator ==;
    using String::operator !=;

    point_t   letter_position(uint16_t index); // ตำแหน่งของ letter โดย index of the letter [0 ... text length] return position of the letter
    uint16_t  letter_on(point_t pos);          // ตำแหน่งแบบ rel ภายใน GLabel ว่าตรงกับ อักษร letter id เท่าไหร่
    String    text_select();
    String    text_select(uint16_t index_start, uint16_t index_end=0x7FFF);  // NO_TXT_SEL for none select
    void      text_select_start(uint16_t index);
    uint16_t  text_select_start();     // NO_TXT_SEL for none select
    void      text_select_end(uint16_t index);
    uint16_t  text_select_end();
    void      insert(uint32_t index, const char* text);
    void      cut(uint32_t index, uint32_t count);
    uint16_t  encoded_length();

    // คืนค่า -1 แสดงว่าไม่เจอ
    int       find(const String &txt); // คล้าย indexOf ที่คืนเป็น byte id ของ utf8 แต่ find จะคืนค่า char_id
    int       find(const String &txt, unsigned int fromIndex);

    void      freeBuffer(); // สำหรับเรียกใช้ภายใน ไม่เรียกใช้ปกติ
};


#endif //__GLABEL_H__
