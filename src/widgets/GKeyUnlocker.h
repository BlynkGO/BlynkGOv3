/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GKeyUnlocker.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GKEYUNLOCKER_H__
#define __GKEYUNLOCKER_H__

#include "GObject.h"

enum {
  GKEYUNLOCKER_MODE_LOCK,
  GKEYUNLOCKER_MODE_SETTING,
};
typedef uint8_t GKeyUnlocker_mode_t;

class GKeyUnlocker : public GObject {
  public:
    GKeyUnlocker(GWidget& parent=GScreen);
    GKeyUnlocker(GWidget* parent);
    virtual ~GKeyUnlocker();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);


    /*****************************************************************
     * API clear() สำหรับเคลียร์ รหัส ให้กลับมา รหัสตั้งต้น (รูปตัว M)
     *****************************************************************/
    void clear();

    /*****************************************************************
     * API mode(..) สำหรับโหมดการทำงาน
     *   parameter:
     *      mode   : โหมดการทำงาน  
     *         GKEYUNLOCKER_MODE_LOCK    -> โหมดให้ lock หน้าจอ
     *         GKEYUNLOCKER_MODE_SETTING -> โหมดตั้งค่ารหัส (จะมีการยืนย้นรหัสเดิ่มก่อนเปลี่ยน)
     *****************************************************************/
    void mode(GKeyUnlocker_mode_t mode); 

    /*****************************************************************
     * API onApply(..)   กำหนด callback function สำหรับ กำหนดการทำงาน หลัง end-user มีการปลดล็อคสำเร็จ
     * API onCancel(..)  กำหนด callback function สำหรับ กำหนดการทำงาน หลัง end-user ปลดล็อคไม่สำเร็จ
     *****************************************************************/
    void onApply(event_handler_t fn)        { if(!this->isCreated()) this->create();  fn_onapply = fn; }
    void onCancel(event_handler_t fn)       { if(!this->isCreated()) this->create();  fn_oncancel = fn; }

    /*****************************************************************
     * API secretkey() แสดงค่ารหัสปัจจุบัน
     *        1  2  3
     *        4  5  6
     *        7  8  9
     *****************************************************************/
    uint32_t secretkey();

    event_handler_t  fn_onapply = NULL;
    event_handler_t  fn_oncancel = NULL;

};

#endif //__GKEYUNLOCKER_H__

