/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GTabview.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GTABVIEW_H__
#define __GTABVIEW_H__

#include "GPage.h"

typedef GPage GTab;

class GTabview : public GObject {
  public:
    GTabview(GWidget& parent=GScreen);
    GTabview(GWidget* parent);
    virtual ~GTabview();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createTabviewStyle();
            void freeTabviewStyle();
    
    void      tabbar_color(uint16_t color, uint16_t text_color=TFT_NAVY, uint16_t indic_color=TFT_NAVY);
    void      tabbar_opa(uint8_t opa);
    void      tabbar_hidden(bool enable);
    bool      tabbar_hidden();
    // void  tab_height(uint8_t height); // dev-7.0 จะปิดไปก่อน

    void      addTab(GTab& tab, String tab_name="");
    GTab*     getTab(uint16_t tab_id);            // คืนค่า tab จาก tab_id
    void      clean();                            // clean all tabs

    uint16_t  tab_count();

    uint16_t  current_tab_id();
    void      current_tab_id(uint16_t tab_id, bool anim_show=true);    // กำหนด tab ปัจจุบัน
    const char* current_tab_name();               // คืนค่า ชื่อ tab_name ของ tab_id ปัจจุบัน
    const char* tab_name(uint16_t tab_id);        // คืนค่า ชื่อ tab_name จาก tab_id

    void      anim_time(uint16_t time);
    uint16_t  anim_time();

    /* style[BG], style[INDIC], style[TAB_BG], 
       style[TAB_RELEASED], style[TAB_PRESSED], 
       style[TAB_TOGGLE_RELEASED], style[TAB_TOGGLE_PRESSED],*/
    GStyle    style[7];
};

#endif //__GTABVIEW_H__

