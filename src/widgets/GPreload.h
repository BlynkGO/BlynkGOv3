/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GPreload.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GPRELOAD_H__
#define __GPRELOAD_H__

#include "GObject.h"

enum {
  PRELOAD_TYPE_SPINNING_ARC,
  PRELOAD_TYPE_FILLSPIN_ARC,
  PRELOAD_TYPE_CONSTANT_ARC,
};
typedef uint8_t preload_type_t;

enum {
  PRELOAD_DIR_FORWARD=0,
  PRELOAD_DIR_BACKWARD=1,
  PRELOAD_CLOCKWISE=0,
  PRELOAD_ANTICLOCKWISE=1,
};
typedef uint8_t preload_dir_t;

class GPreload : public GObject {
  public:
    GPreload(GWidget& parent=GScreen);
    GPreload(GWidget* parent);
    virtual ~GPreload();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void      arc_length(uint16_t degree);
    uint16_t  arc_length();
    void      spin_time(uint16_t time);
    uint16_t  spin_time();

    // PRELOAD_TYPE_SPINNING_ARC, PRELOAD_TYPE_FILLSPIN_ARC, PRELOAD_TYPE_CONSTANT_ARC 
    void      preload_type(preload_type_t type);
    preload_type_t preload_type();

    void      direction(preload_dir_t dir);
    preload_dir_t direction();

    void      thickness(uint16_t thick);
    uint16_t  thickness();

    void      color(color_t color);
    color_t   color();

    void      opa(uint8_t opa);
    uint8_t   opa();
  private:

};

#endif //__GPRELOAD_H__
