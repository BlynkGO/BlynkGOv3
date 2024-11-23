/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GThermo.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GTHERMO_H__
#define __GTHERMO_H__

#include "GObject.h"

class GThermo : public GObject {
  public:
    GThermo(GWidget& parent=GScreen);
    GThermo(GWidget* parent);
    GThermo(float value_min, float value_max, GWidget& parent=GScreen);
    virtual ~GThermo();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createThermoStyle();
            void freeThermoStyle();

    GThermo& operator =(float value);

    void    range(float val_min, float val_max);
    void    value(float val);
    int16_t value();
    inline int16_t toInt()         { return this->value();   }
    
    int16_t value_min();
    int16_t value_max();
    void    decimal(uint8_t float_decimal);  // 0, 1, 2 only
    uint8_t decimal();
    void    level_draw(bool enable);
    bool    level_draw();

    /* Customize Style[2] :  style[BG] , style[INDIC] */
    GStyle  style[2];
  private:
    float   val     =  50;
    float   val_min =   0;
    float   val_max = 100;
};

#endif //__GTHERMO_H__
