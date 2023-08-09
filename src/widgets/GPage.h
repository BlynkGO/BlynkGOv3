/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GPage.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GPAGE_H__
#define __GPAGE_H__

#include "GObject.h"
#include "GContainer.h"

class GPage : public GObject {
  public:
    GPage(GWidget& parent=GScreen);
    GPage(GWidget* parent);
    virtual ~GPage();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createPageStyle();
            void freePageStyle();
            // void del();
 
    void        clean();  // clean วิดเจ็ทต่างๆเฉพาะบนเลเยอร์ scrl
    

    /*****************************************************************/
    /* Layer Scroll API*/
    /* layout_t : LAYOUT_OFF, LAYOUT_CENTER, 
                  LAYOUT_COL_L, LAYOUT_COL_M, LAYOUT_COL_R,
                  LAYOUT_ROW_T, LAYOUT_ROW_M, LAYOUT_ROW_B,
                  LAYOUT_PRETTY, LAYOUT_GRID */
    void        layout(layout_t layout);  // layout ของ scrl part
    layout_t    layout();

    inline void layout(layout_t layout, uint16_t padding_top,uint16_t padding_bottom, uint16_t padding_left,uint16_t padding_right, uint16_t padding_inner) 
    {
      this->layout(layout);
      this->padding( padding_top, padding_bottom, padding_left, padding_right, padding_inner);
    }

    /* fit_t : FIT_NONE, FIT_TIGHT, FIT_FLOOD, FIT_FILL */  // fit ของ scrl part
    void        fit(fit_t fit); 
    void        fit2(fit_t horizon_fit, fit_t vertical_fit);
    void        fit4(fit_t left, fit_t right, fit_t top, fit_t bottom);

    inline void fit(fit_t horizon_fit, fit_t vertical_fit)            { fit2(horizon_fit, vertical_fit); } 
    inline void fit(fit_t left, fit_t right, fit_t top, fit_t bottom) { fit4(left, right, top,bottom); } 
    fit_t       fit_left();
    fit_t       fit_right();
    fit_t       fit_top();
    fit_t       fit_bottom();


    /* Layer Scroll API */
    uint16_t    fit_width();    // ขนาดกว้างภายใน container : scrl 
    uint16_t    fit_height();   // ขนาดสูงภายใน container   : scrl

    void        padding_inner(int16_t padding_inner);   // padding ของ scrl     
    void        padding_top(int16_t padding_top);       // padding ของ scrl
    void        padding_bottom(int16_t padding_bottom); // padding ของ scrl
    void        padding_left(int16_t padding_left);     // padding ของ scrl
    void        padding_right(int16_t padding_right);   // padding ของ scrl
    int16_t     padding_inner();                        // padding ของ scrl
    int16_t     padding_top();                          // padding ของ scrl
    int16_t     padding_bottom();                       // padding ของ scrl
    int16_t     padding_left();                         // padding ของ scrl
    int16_t     padding_right();                        // padding ของ scrl

    void        padding(int16_t padding);               // padding ของ scrl
    int16_t     padding();                              // คืนค่า  padding ของ scrl

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

    /*****************************************************************/
    void        drag_direction(drag_dir_t direction); // DRAG_DIR_NONE, DRAG_DIR_HOR, DRAG_DIR_VER, DRAG_DIR_BOTH, DRAG_DIR_ONE
    drag_dir_t  drag_direction();


    void        edge_flash(bool enable);
    bool        edge_flash();


    /* SCROLLBAR_OFF, SCROLLBAR_ON, SCROLLBAR_DRAG/SCROLLBAR_AUTOHIDE, 
       SCROLLBAR_AUTO, SCROLLBAR_HIDE, SCROLLBAR_UNHIDE*/
    // สำหรับ sd_mode เป็น SCROLLBAR_ON จะกำหนด hor_show, ver_show ให้แสดงหรือไม่แสดงได้
    void        scrollbar(scrollbar_t sb_mode, bool hor_show=true, bool ver_show=true); 
    scrollbar_t scrollbar();
    void        scroll_propagation(bool enable);   // ใส่เพื่อ หาก parent เป็น page ให้ส่งต่อการ drag scroll ไปยัง parent ด้วย หลังจากที่ scroll ของตัวเองแล้ว
    bool        scroll_propagation();

    void        scroll_pos(int16_t posX, int16_t posY);
    void        scroll_posX(int16_t pos);
    void        scroll_posY(int16_t pos);
    int16_t     scroll_posX();
    int16_t     scroll_posY();
    
    void        scrollTop();           // เลื่อนไปบนสุดของ GPage
    void        scrollBottom();        // เลื่อนไปล่างสุดของ GPage
    void        scrollLeft();          // เลื่อนไปซ้ายสุดของ GPage
    void        scrollRight();         // เลื่อนไปขวาสุดของ GPage


    GContainer  bg;                     /* _style --> ลิงค์ไปที่ ของ bg */
    GContainer  scrl;  

    
  private:

};

#endif // __GPAGE_H__
