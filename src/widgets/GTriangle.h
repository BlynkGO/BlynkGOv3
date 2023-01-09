/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GTriangle.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GTRIANGLE_H__
#define __GTRIANGLE_H__

#include "GObject.h"

class GTriangle : public GObject {
  public:
    GTriangle(GWidget& parent=GScreen);
    GTriangle(GWidget* parent);
    virtual ~GTriangle();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    /* Point p0,p1,p2 หรือ points[3] หรือ pivot  
       เป็น จุดที่เทียบภายใน parent ของ triangle นี้ */
    void setPoints(Point p0, Point p1, Point p2);
    void setPoints(Point points[3]);
    void setPoints(point_t points[3]);

    void angle(float angle, Point pivot); // มุมวัดจาก มุม angle_offset
    void angle_offset(float offset);      // มุมวัดจากแกน x+ ไปตามทิศที่กำหนดใน direction(..)
    void direction(rotate_direction_t type); // ทิศของมุม ทวนเข็มนาฬิกา ANTICLOCKWISE หรือ ตามเข็มนาฬิกา CLOCKWISE

  private:
    Point _points[3];
    rotate_direction_t _direction = CLOCKWISE;
    float  _angle_offset = 0;
};

#endif //__GTRIANGLE_H__

