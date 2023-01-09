/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GCircle.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GCIRCLE_H__
#define __GCIRCLE_H__

#include "GObject.h"

class GCircle : public GObject {
  public:
    GCircle(GWidget& parent=GScreen);
    GCircle(GWidget* parent);
    GCircle(uint16_t radius, uint16_t color , GWidget& parent=GScreen);
    GCircle(uint16_t radius, uint16_t color , GWidget* parent);
    virtual ~GCircle();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    /*
     * สำหรับ กำหนดรัศมีของวงกลม
     * r : ตัวแปรกำหนดรัศมีวงกลม
     */
    void radius(uint16_t r);

    /*
     * สำหรับ รับค่ารัศมีของวงกลม
     * คืนค่า รัศมีวงกลม
     */
    uint16_t radius();

  private:
    uint16_t _radius =30;
    uint16_t _color = TFT_GREEN;
    // static void cir_event_cb(obj_t *obj, event_t event);
};

#endif //_GCIRCLE_H__
