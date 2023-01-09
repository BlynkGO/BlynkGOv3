/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GContainer.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GCONTAINER_H__
#define __GCONTAINER_H__

#include "GObject.h"

class GContainer : public GObject {
  public:
    GContainer(GWidget& parent=GScreen);
    GContainer(GWidget* parent);
    virtual ~GContainer();

    virtual void create();
            void create(GObject *parent);
            void create(GObject &parent);

    /* layout_t : LAYOUT_OFF,    LAYOUT_CENTER, 
                  LAYOUT_COL_L,  LAYOUT_COL_M,  LAYOUT_COL_R,
                  LAYOUT_ROW_T,  LAYOUT_ROW_M,  LAYOUT_ROW_B,
                  LAYOUT_PRETTY, LAYOUT_GRID */
    void        layout(layout_t layout);
    layout_t    layout();

    /* ใช้สำหรับปรับขนาดให้พอดี แบบต่างๆ ใช้แทน size(w,h) */
    /* fit_t : FIT_NONE, FIT_TIGHT, FIT_FLOOD, FIT_FILL */
    void        fit(fit_t fit_type); 
    inline void fit(fit_t fit_horizon, fit_t fit_vertical)              { fit2(fit_horizon, fit_vertical); }
    inline void fit(fit_t left, fit_t right, fit_t top, fit_t bottom)   { fit4(left, right, top, bottom);  }

    void        fit2(fit_t fit_horizon, fit_t fit_vertical);
    void        fit4(fit_t left, fit_t right, fit_t top, fit_t bottom);

    fit_t       fit_left();
    fit_t       fit_right();
    fit_t       fit_top();
    fit_t       fit_bottom();

    /* การใช้ size(w,h), width(w), heighth) ปรับขนาด
       หากเคยกำหนด fit(...) เดิมจะถูกยกเลิก */
    void        size(uint16_t w, uint16_t h);
    void        size(GWidget& widget);
    void        size(GWidget* widget);
    void        width(uint16_t w);
    void        width(GWidget& widget);
    void        width(GWidget* widget);
    void        height(uint16_t h);
    uint16_t    width();
    void        height(GWidget& widget);
    void        height(GWidget* widget);
    uint16_t    height();

    uint16_t    fit_width();    // ขนาดกว้างภายใน container ที่หัก padding_left/right ออกแล้ว
    uint16_t    fit_height();   // ขนาดสูงภายใน container   ที่หัก padding_top/bottom ออกแล้ว

};

#endif //__GCONTAINER_H__

