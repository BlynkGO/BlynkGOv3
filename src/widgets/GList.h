/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GList.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GLIST_H__
#define __GLIST_H__

#include "GPage.h"
#include "GButton.h"
#include <vector>  // หาก เพิ่ม icon รูปจาก SD, SPIFFS ด้วย ไฟล์ PNG, JPG, BMP

class GList : public GPage {
  public:
    GList(GWidget& parent=GScreen);
    GList(GWidget* parent);
    virtual ~GList();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createListStyle();
            void freeListStyle();

    int16_t addItem(String item_name);
    int16_t addItem(const void * item_img_src, String item_name);
    int16_t addItem(const img_t& item_img_src, String item_name);
    int16_t addItem(String filename, String item_name);
    int16_t addFileList(String path, String file_surname="*");
    void    removeItem(uint8_t item_id);
    void    clear();  // clear of items

    uint16_t item_num();
    void    item_selected(uint8_t item_id);  // set
    String  item_selected();
    int16_t item_selected_id();

    inline  int16_t add(String item_name)                               { return this->addItem(item_name); }
    inline  int16_t add(const void * item_img_src, String item_name)    { return this->addItem(item_img_src, item_name); }
    inline  int16_t add(const img_t& item_img_src, String item_name)    { return this->addItem(item_img_src, item_name); }
    inline  int16_t add(String filename, String item_name)              { return this->addItem(filename, item_name); }
    inline  void    remove_id(uint8_t item_id)                          { this->removeItem(item_id);    }
    inline  void    remove(uint8_t item_id)                             { this->removeItem(item_id);    }
    inline  uint16_t num()                                              { return this->item_num();      }
    inline  void    selected_id(uint8_t item_id)                        { this->item_selected(item_id); }
    inline  uint8_t selected_id()                                       { return item_selected_id();    }
    inline  void    selected(uint8_t item_id)                           { this->selected_id(item_id);   }
    inline  String  selected()                                          { return this->item_selected(); }

    const char*  item_name(uint8_t item_id);
    int          item_id(const char* item_name);
    int          item_id(String item_name);

    void    single_mode(bool enable);
    bool    single_mode();

    /* API กำหนด style ต่างๆ ให้ item ของ Glist  */
    void    color(color_t color);                   // สีพื้นหลังเวลา item ถูกกดเลือก
    void    color(color_t color1, color_t color2);  // สีพื้นหลังเวลา item ถูกกดเลือก แบบไล่เฉด 2 สี
    color_t color();

    void    font(font_t &font);         // font ของ item
    void    font(font_t &font, color_t color);
    const font_t* font();
    void    text_color(color_t color);  // สี ข้อความของ item
    inline void font_color(color_t color)              {  text_color(color); }
    // void    text_long_mode();
    void    image_color(color_t color);

    void    radius(uint8_t r);          // รัศมีมุมโค้ง ของ item
    uint8_t radius();
    void    corner_radius(uint8_t r)    { this->radius(r);       }
    uint8_t corner_radius()             { return this->radius(); }

    void    border_color(color_t color);
    color_t border_color();
    void    border(uint8_t width);
    uint8_t border();

    void    padding(uint8_t padding);
    void    padding_top(uint8_t padding);
    void    padding_bottom(uint8_t padding);
    void    padding_left(uint8_t padding);
    void    padding_right(uint8_t padding);
    void    padding_inner(uint8_t padding);

    inline void padding(uint16_t padding_top,uint16_t padding_bottom, uint16_t padding_left,uint16_t padding_right, uint16_t padding_inner) 
                {  
                    this->padding_top(padding_top);
                    this->padding_bottom(padding_bottom);
                    this->padding_left(padding_left);
                    this->padding_right(padding_right);
                    this->padding_inner(padding_inner);
                }

    uint8_t padding_top();
    uint8_t padding_bottom();
    uint8_t padding_left();
    uint8_t padding_right();
    uint8_t padding_inner();
                      // ขณะไม่ได้เลือก,ขณะเลือก
    GStyle  style[5]; // RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE

    void    clear_src_dsc_list();

    //-- inner variable --------
    int16_t     _item_selected_id=-1;
  private:
    std::vector<img_t> src_dsc_list; //จากการ decoding png/bmp/jpg
    void    items_reflesh();
};

#endif //__GLIST_H__

