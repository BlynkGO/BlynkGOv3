/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GStyle.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef _GSTYLE_H__
#define _GSTYLE_H__

#include "GUtils.h"

class GStyle {
  public:
    GStyle();												  // ไม่มีการ alloc หรือ copy ต้องเรียกเองอีกที
    GStyle(style_t *style_copy);			// จะ alloc + copy ในตัว
    GStyle(GStyle &GStyle_copy);			// จะ alloc + copy ในตัว
    ~GStyle();

    void      init();
    void      alloc(style_t *style_copy = NULL);
    void      alloc(style_t &style_copy);
    void      release();
    void      del()     { this->release(); }

    void      copy(GStyle &style);
    void      copy(style_t *style);
    void      copy(style_t &style);
    void      link(style_t *style);
    void      link(style_t &style);
    void      link(GStyle &style);
    void      link(GStyle *style);
    GStyle&   operator =(GStyle &style);

    void      color(color_t color, color_t gradian_color, uint8_t opa=255);
    void      color(color_t color);
    color_t   color();
    void      grad_color(uint16_t gradian_color);
    uint16_t  grad_color();
    void      grad_dir(grad_dir_t type);
    grad_dir_t grad_dir();
    void      opa(uint8_t opa);
    uint8_t   opa();
    void      radius(uint16_t r);
    uint16_t  radius();
    void      corner_mask(bool enable);
    bool      corner_mask();
    inline void      corner_radius(uint16_t r)         { this->radius(r); }
    inline uint16_t  corner_radius()                   { return this->radius(); }

    void      border(uint16_t width);
    void      border(uint16_t width, color_t color);
    uint16_t  border();
    void      border_color(color_t color);
    color_t   border_color();
    void      border_opa(uint8_t opa);
    uint8_t   border_opa();
    void      border_part(border_part_t part_type); /* border_poart_t : BORDER_NONE , BORDER_BOTTOM, BORDER_TOP, BORDER_LEFT, BORDER_RIGHT, BORDER_FULL, BORDER_INTERNAL */
    border_part_t border_part();

    void      shadow(uint16_t width);
    void      shadow(uint16_t width, color_t color);
    uint16_t  shadow();
    void      shadow_color(color_t color);
    color_t   shadow_color();
    void      shadow_opa(uint8_t opa);
    uint8_t   shadow_opa();
    void      shadow_offset(point_t point);
    void      shadow_offset(int16_t offset_x, int16_t offset_y);
    point_t   shadow_offset();
    void      shadow_spread(uint16_t spread);
    uint16_t  shadow_spread();

    void      padding(int16_t padding);
    void      padding_inner(int16_t padding_inner);
    void      padding_top(int16_t padding_top);
    void      padding_bottom(int16_t padding_bottom);
    void      padding_left(int16_t padding_left);
    void      padding_right(int16_t padding_right);
    inline void padding(uint16_t padding_top,uint16_t padding_bottom, uint16_t padding_left,uint16_t padding_right, uint16_t padding_inner) 
                {  
                    this->padding_top(padding_top);
                    this->padding_bottom(padding_bottom);
                    this->padding_left(padding_left);
                    this->padding_right(padding_right);
                    this->padding_inner(padding_inner);
                }
    inline void padding_top_bottom(uint16_t padding)
                {
                    this->padding_top(padding);
                    this->padding_bottom(padding);
                }

    inline void padding_top_bottom(uint16_t padding_top, uint16_t padding_bottom)
                {
                    this->padding_top(padding_top);
                    this->padding_bottom(padding_bottom);
                }

    inline void padding_left_right(uint16_t padding)
                {
                    this->padding_left(padding);
                    this->padding_right(padding);
                }

    inline void padding_left_right(uint16_t padding_left, uint16_t padding_right)
                {
                    this->padding_left(padding_left);
                    this->padding_right(padding_right);
                }

    int16_t   padding();
    int16_t   padding_inner();
    int16_t   padding_top();
    int16_t   padding_bottom();
    int16_t   padding_left();
    int16_t   padding_right();

    void      font(font_t &font);
    void      font(font_t &font, color_t color);

    inline void font(font_t *font)                      { this->font(*font); }
    inline void font(font_t *font, color_t color)       { this->font(*font, color); }
    
    const font_t* font();
    int16_t   font_height();
    void      text_color(color_t color);
    color_t   text_color();
    void      text_opa(uint8_t opa);
    uint8_t   text_opa();
    void      text_sel_color(color_t color);
    color_t   text_sel_color();
    void      text_line_space(int16_t line_space);
    int16_t   text_line_space();
    void      text_letter_space(int16_t letter_space);
    int16_t   text_letter_space();
    void      text_blend_mode(uint8_t blend_mode);
    uint8_t   text_blend_mode();
    void      underline(bool enable);
    bool      underline();
    void      strikethrough(bool enable);
    bool      strikethrough();

    inline void      font_color(color_t color)         { this->text_color(color); }
    inline color_t   font_color()                      { return this->text_color(); }
    inline void      line_space(uint16_t line_space)   { this->text_line_space(line_space); }
    inline uint16_t  line_space()                      { return this->text_letter_space(); }
    inline void      letter_space(uint16_t letter_space)   { this->text_letter_space(letter_space); }
    inline uint16_t  letter_space()                    { return this->text_letter_space(); }

    void      line_width(uint16_t width);
    void      line_width(uint16_t width, color_t color);
    uint16_t  line_width();
    void      line_color(color_t color);
    color_t   line_color();
    void      line_opa(uint8_t opa);
    uint8_t   line_opa();
    void      line_rounded(bool enable);
    bool      line_rounded();

    void      image_color(color_t color);
    color_t   image_color();
    void      image_intense(uint8_t opa);
    uint8_t   image_intense();
    void      image_opa(uint8_t opa);
    uint8_t   image_opa();
    inline void image_filter(color_t color, uint8_t intense)    { this->image_color(color); this->image_intense(intense); }
    inline void image_reset()                                   { this->image_intense(0); }

    style_t*  raw();
  private:
    style_t  *style = NULL;
    style_t  *copy_style = NULL;
    GStyle   *copy_gstyle = NULL;
    bool     _link_style = false;
};


extern GStyle GStyle_transp;
extern GStyle GStyle_transp_fit;
extern GStyle GStyle_transp_tight;
extern GStyle GStyle_plain;
extern GStyle GStyle_plain_color;
extern GStyle GStyle_pretty;
extern GStyle GStyle_pretty_color;
extern GStyle GStyle_btn_rel;
extern GStyle GStyle_btn_pr;
extern GStyle GStyle_btn_tgl_rel;
extern GStyle GStyle_btn_tgl_pr;
extern GStyle GStyle_btn_ina;

void GStyle_init();

#endif //_GSTYLE_H__
