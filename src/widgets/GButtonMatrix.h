/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GButtonMatrix.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 *  [History]
 *    [Version 3.0]   @13/08/22
 *       - เพิ่ม API GButtonMatrix(const char* buttons_map[], GWidget& parent=GScreen);
 *       - เพิ่ม API GButtonMatrix(const char* buttons_map[], GWidget* parent);
 * 
 *********************************************************************
 */

#ifndef __GBUTTONMATRIX_H__
#define __GBUTTONMATRIX_H__

#include "GObject.h"

class GButtonMatrix : public GObject {
  public:
    GButtonMatrix(GWidget& parent=GScreen);
    GButtonMatrix(GWidget* parent);
    GButtonMatrix(const char* buttons_map[], GWidget& parent=GScreen);
    GButtonMatrix(const char* buttons_map[], GWidget* parent);
    virtual ~GButtonMatrix();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createBTNMStyle();
            void freeBTNMStyle();

    void    map(const char * buttons_map[]);
    const char ** map();
    void    control_map(const button_ctrl_t ctrl_map[]);
    void    control_all(const button_ctrl_t control);

    void    button_width(uint16_t button_id, uint8_t button_w);
    void    button_control(uint16_t button_id, button_ctrl_t control);      //CTRL_HIDDEN, CTRL_NO_REPEAT, CTRL_INACTIVE, CTRL_TOGGLE
    String  active_button();
    uint16_t pressed_button_id();

    void    bg_color(uint16_t bg_color);   // สีพื้นหลัง
    void    bg_radius(uint8_t r);          // รัศมีมุม พื้นหลัง
    void    bg_opa(uint8_t opa);           // ความโปร่งใสพื้นหลัง
    void    bg_border(uint8_t width);      // ความหน้าขอบพื้นหลัง
    void    bg_border(uint8_t width, color_t color);      // ความหน้าขอบพื้นหลัง
    void    bg_border_opa(uint8_t opa);      // ความหน้าขอบพื้นหลัง
    void    bg_border_color(color_t color);      // ความหน้าขอบพื้นหลัง

    // คำสั่งปรับแต่งปุ่มแบบรวมๆทุก style
    void    color(uint16_t color);         // สีของปุ่ม
    void    radius(uint8_t r);
    uint8_t radius();
    void    border(uint8_t width);
    uint8_t border();
    void    border_color(color_t color);
    color_t border_color();
    void    font(font_t &font);
    void    font(font_t &font, color_t color);

    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }

    const font_t* font();
    void    text_color(color_t color);
    color_t text_color();

    GStyle   style[6]; // RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE,  BTNM_BG
  protected:
    const char** _map=NULL;
};

#endif //__GBUTTONMATRIX_H__

