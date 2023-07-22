/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GTimer.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GTIMER_H__
#define __GTIMER_H__

#include <Arduino.h>
#include <Ticker.h>

using namespace std;

class GTimer : public Ticker {
  public:
    GTimer() : Ticker() {}
    inline void setInterval(uint32_t milliseconds, callback_t callback, bool first_run=false) {
      if(first_run) { callback(); }
      attach_ms(milliseconds, callback);
    }

    inline void setInterval(uint32_t milliseconds, void (*callback)(void* arg), void* arg, bool first_run=false) {
      if(first_run) { callback(arg); }
      attach_ms(milliseconds, callback, arg);
    }

    inline void setOnce(uint32_t milliseconds, callback_t callback) { once_ms(milliseconds, callback); }
    inline void setOnce(uint32_t milliseconds, void (*callback)(void* arg), void* arg)  { 
      once_ms(milliseconds, callback, arg);
    }

    inline void delay(uint32_t milliseconds, callback_t callback)   { setOnce( milliseconds, callback); }
    inline void delay(uint32_t milliseconds, void (*callback)(void* arg), void* arg)  {  setOnce(milliseconds, callback, arg );  }

    inline void del()  { detach(); }
};

#endif // __GTIMER_H__
