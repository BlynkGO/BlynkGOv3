/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GTextArea.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GTEXTAREA_H__
#define __GTEXTAREA_H__

#include "GObject.h"

#define TEXTAREA_USE_LABEL  1
#if TEXTAREA_USE_LABEL
#include "GLabel.h"
class GLabel;
#endif


typedef struct {
  lv_ta_ext_t ta;
  bool  default_kbd;
} GTextArea_ext_t;

class GTextArea : public GObject {
  public:
    GTextArea(GWidget& parent=GScreen);
    GTextArea(GWidget* parent);
    // GTextArea(const char *cstr, GWidget& parent=GScreen);
    // GTextArea(const String &str, GWidget& parent=GScreen);
    // GTextArea(const char *cstr, GWidget* parent);
    // GTextArea(const String &str, GWidget* parent);
    GTextArea(const String str);
    GTextArea(const String str, GWidget& parent);
    GTextArea(const String str, GWidget* parent);
    virtual ~GTextArea();
    
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            
    void     default_keyboard(bool enable);  // กำหนดว่าใช้ default keyboard หรือไม่ ; ค่าเริ่มต้นจะ enable true

    void     text(String txt);
    String   text();
    const char* c_str();
    
    inline String toString()           { return text();                     }
    inline long   toInt(void)          { return this->toString().toInt();   }
    inline float  toFloat(void)        { return this->toString().toFloat(); }
    inline double toDouble(void)       { return this->toString().toDouble();}
    inline void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0){
      this->toString().toCharArray(buf, bufsize, index);
    }
    inline size_t length(void)         { return text().length();            }

    void     add(String txt);
    void     add(uint32_t c);
    void     backspace();       // ลบตัวด้านซ้าย
    void     del_forward();     // ลบตัวด้านขวา
    void     text_align(label_align_t align);  //LABEL_ALIGN_RIGHT, LABEL_ALIGN_CENTER, LABEL_ALIGN_LEFT
    void     accepted_chars(String accepted_chars_list);
    String   accepted_chars();
    void     insert_replace(String text);

    void     cursor_pos(int16_t pos);  // 0 .. CURSOR_LAST
    int16_t  cursor_pos();
    void     cursor_type(cursor_type_t cur_type); // CURSOR_NONE, CURSOR_LINE, CURSOR_BLOCK, CURSOR_OUTLINE,CURSOR_UNDERLINE, CURSOR_HIDDEN
    cursor_type_t cursor_type();
    void     cursor_click_pos(bool enable);
    bool     cursor_click_pos();
    void     cursor_right();
    void     cursor_left();
    void     cursor_up();
    void     cursor_down();

    void     placeholder(String txt);            // ข้อความ placeholder เพื่อบอกว่า ช่องใส่ข้อความนี้ สำหรับอะไร
    String   placeholder();                      // คืนค่า ข้อความ placeholder

    /* placeholder_font(..) และ placeholder_color(..) จะเปลี่ยน ฟอนต์/สีของ placeholder สำหรับ ta ทั้งหมดทุก ta */
    void     placeholder_font(font_t& font);     // กำหนดฟอนต์ให้ placeholder
    void     placeholder_font(font_t& font, color_t color);  // กำหนดฟอนต์ และ สี ให้ placeholder
    void     placeholder_color(color_t color, uint8_t opa=128);   // กำหนดสีให้ placeholder
    color_t  placeholder_color();                // คืนค่า สี ของ placeholder

    inline void placeholder_font(font_t *font)                      { this->placeholder_font(*font); }
    inline void placeholder_font(font_t *font, color_t color)       { this->placeholder_font(*font, color); }

    void     oneline(bool enable);
    bool     oneline();

    void     password_mode(bool enable);
    bool     password_mode();
    void     password_show_time(uint16_t time);   // เวลาในหน่วย milliseconds (ms) ที่จะให้ password แสดงก่อนจะกลายเป็น ***
    uint16_t password_show_time();

    void     max_length(uint16_t len);
    uint16_t max_length();


    void     text_select(bool enable);
    bool     text_select();
    bool     text_isSelected();

    /* SCROLLBAR_OFF, SCROLLBAR_ON, SCROLLBAR_DRAG/SCROLLBAR_AUTOHIDE, 
       SCROLLBAR_AUTO, SCROLLBAR_HIDE, SCROLLBAR_UNHIDE*/
    void        scrollbar(scrollbar_t sb_mode);
    scrollbar_t scrollbar();
    void        scroll_propagation(bool enable);   // ใส่เพื่อ หาก parent เป็น page ให้ส่งต่อการ drag scroll ไปยัง parent ด้วย หลังจากที่ scroll ของตัวเองแล้ว
    bool        scroll_propagation();


    GTextArea& operator =(String label);
    GTextArea& operator =(const char* cstr);
    GTextArea& operator =(int num);

#if TEXTAREA_USE_LABEL
    GTextArea& operator =(GLabel& label);
#endif

  private:
    String _text="";

};

#endif // __GTEXTAREA_H__
