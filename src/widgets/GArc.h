/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GArc.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef _GARC_H__
#define _GARC_H__

#include "GObject.h"

typedef enum{
  ARC_CLOCKWISE,
  ARC_ANTICLOCKWISE,
} arc_direction_t;

class GArc : public GObject {
  public:
    GArc(GWidget& parent=GScreen);
    GArc(GWidget* parent);
    GArc(uint16_t radius, uint8_t thick, uint16_t color , GWidget& parent=GScreen);
    GArc(uint16_t radius, uint8_t thick, uint16_t color , GWidget* parent);
    virtual ~GArc();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void      direction(arc_direction_t type);
    void      angle_offset(float offset);
    inline float angle_offset()                 {return _angle_offset; }
    void      angle(int16_t start, int16_t end);
    uint16_t  angle_start();
    uint16_t  angle_end();

    void      color(color_t color);
    color_t   color();
    void      opa(uint8_t opa);
    uint8_t   opa();
    void      thickness(uint8_t t);
    uint8_t   thickness();
    void      radius(uint16_t r);
    uint16_t  radius();
    void      rounded(bool enable);
    bool      rounded();

  private:
    uint16_t _radius =30;
    uint16_t _color = TFT_GREEN;
    uint8_t  _thick = 10;
    arc_direction_t _direction = ARC_CLOCKWISE;
    float    _angle_offset = 0;
};


#endif //_GARC_H__
