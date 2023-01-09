/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GLine.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GLINE_H__
#define __GLINE_H__

#include "GObject.h"
#include "vector"

typedef std::vector<Point> Points;

class GLine : public GObject {
  public:
    GLine(GWidget& parent=GScreen);
    GLine(GWidget* parent);
    GLine(uint16_t color, GWidget& parent);
    GLine(uint16_t color, GWidget* parent);
    virtual ~GLine();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void setPoints(Point p0, Point p1);
    void setPoints(uint16_t arm_len, float angle_deg, float angle_deg_offset, Point c, rotate_direction_t rotate= CLOCKWISE);
    void setPoints(Point* points, uint16_t point_num);
    void setPoints(Points points );
    void setPoints(point_t* points, uint16_t point_num);

    void color(color_t color);
    void thickness(uint8_t t);
    inline void thickness(uint8_t thick, color_t color)      { this->thickness(thick); this->color(color); }
    void rounded(bool enable);
    void opa(uint8_t opa);

    // dont' call this api , this is inner function
    void freeAllocPoints();
  private:
    point_t* _points=NULL;
    uint16_t _point_num;
    uint16_t _color = TFT_WHITE;
};

#endif //__GLINE_H__
