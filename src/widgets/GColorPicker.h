/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GColorPicker.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef _GCOLORPICKER_H__
#define _GCOLORPICKER_H__

#include "GObject.h"

#define TYPE_RECT   0
#define TYPE_DISK   1

class GColorPicker : public GObject {
  public:
    GColorPicker(GWidget& parent=GScreen);
    GColorPicker(GWidget* parent);
    virtual ~GColorPicker();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void    type(uint8_t type); // TYPE_RECT or TYPE_DISC
    void    color(color_t color);
    color_t color();

};


#endif //_GCOLORPICKER_H__

