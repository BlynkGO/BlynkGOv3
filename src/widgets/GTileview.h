/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GTileview.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GTILEVIEW_H__
#define __GTILEVIEW_H__

#include "GPage.h"

class GTileview : public GPage {
  public:
    GTileview(GWidget& parent=GScreen);
    GTileview(GWidget* parent);
    virtual ~GTileview();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void valid_positions(point_t* valid_pos, uint16_t valid_pos_count);
    void edge_flash(bool enable);

    bool addElement(GWidget &element, point_t valid_pos); // เพิ่ม element ที่ tileview ที่ พิกักด valid_pos โดย valid_pos ต้องเป็นตำแหน่งที่ได้กำหนดใน valid_positions(..) ก่อนหน้า
    bool addElement(GWidget *element, point_t valid_pos);
    
    /* (x,y) is valid_point in valid_points list */
    void current_tile(uint16_t x, uint16_t y, bool enable_anim=true);
    point_t current_tile();  // point_t  is valid point in valid_points list

};

#endif //__GTILEVIEW_H__
