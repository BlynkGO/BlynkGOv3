/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GMultiScreen.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GMULTI_SCREEN_H__
#define __GMULTI_SCREEN_H__

#include "GObject.h"
#include "GContainer.h"
#include "GLabel.h"

typedef struct {
  lv_page_ext_t page;
  lv_style_t style_tight;

  GObject** screen;
  uint16_t screen_cur;
  uint16_t screen_cnt;

  GContainer* cont_indic;
    GLabel **lb_indic;
    color_t indic_color_active;
    color_t indic_color_inactive;
    uint8_t indic_opa_active;
    uint8_t indic_opa_inactive;

#if LV_USE_ANIMATION
  uint16_t anim_time;
#endif
} gmultiscr_ext_t;


class GMultiScreen : public GObject {
  public:
    GMultiScreen(GWidget& parent=GScreen);
    GMultiScreen(GWidget* parent);
    virtual ~GMultiScreen();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void     addScreen(uint8_t num=1);  // num 1...7 ; max = 8;
    GObject* screen(uint8_t screen_id);
    GObject* operator [](uint8_t screen_id);  // BGUI[0], BGUI[1], ... , BGUI[n-1]  แทน screen แต่ละหน้าได้เลย
    uint8_t  num();  // จำนวน screens ทั้งหมดขณะนี้

    void     current_screen(uint16_t screen_id, bool anim=true);
    GObject* current_screen();
    uint16_t current_screen_id();

    inline   void selected_id(uint16_t screen_id, bool anim=false)    { this->current_screen( screen_id, anim); }
    inline   void selected(uint16_t screen_id, bool anim=false)       { this->current_screen( screen_id, anim); }
    inline   uint16_t selected_id()                                   { return this->current_screen_id();       }
    inline   GObject* selected()                                      { return this->current_screen();          }

    void     hidden_indic(bool enable);
    bool     hidden_indic();
    void     indic_color(color_t color_active, color_t color_inactive);
    inline void indic_color(color_t color)       { indic_color(color, color); }
    void     indic_opa(uint8_t opa_active, uint8_t opa_inactive);

    GContainer& indic();

#if LV_USE_ANIMATION
    void     anim_time(uint16_t time);
    uint16_t anim_time();
#endif // LV_USE_ANIMATION

  private:
    void gmultiscreen_realign();
    GObject* _addScreen();                     // คำสั่งสำหรับเพิ่ม screen ใหม่ให้ BGUI มากสุดได้ 8 screen
};


#endif // __GMULTI_SCREEN_H__
