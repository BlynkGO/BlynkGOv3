/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GBar.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GBAR_H__
#define __GBAR_H__

#include "GObject.h"

class GBar : public GObject {
  public:
    GBar(GWidget& parent=GScreen);
    GBar(GWidget* parent);
    GBar(int16_t val_min, int16_t val_max, GWidget& parent=GScreen);
    virtual ~GBar();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void createBarStyle();
            void freeBarStyle();

    GBar& operator =(int num);

    void    range(int16_t val_min, int16_t val_max);
    void    value(int16_t val, bool anim = false);
    void    value(int16_t val_start, int16_t val_end, uint16_t anim_time);
    int16_t value();
    int16_t value_min();
    int16_t value_max();

    void    value_start(int16_t val_start, bool anim = false);
    int16_t value_start();
    inline  void start_value(int16_t val_start, bool anim = false)       { value_start(val_start, anim); }
    inline  int16_t start_value()                                       { return value_start();         }

    void    symmetric(bool sym);
    bool    symmetric();

    void    anim_time(uint16_t anim_time);
    uint16_t anim_time();

    /* Customize Style[2] :  style[BG] , style[INDIC]  ;  default -> INDIC */
    GStyle style[2]; 
  private:
    int16_t val     =  50;
    int16_t val_min =   0;
    int16_t val_max = 100;
};

#endif //__GBAR_H__
