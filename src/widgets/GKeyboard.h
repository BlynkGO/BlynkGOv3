/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GKeyboard.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GKEYBOARD_H__
#define __GKEYBOARD_H__

#include "GButtonMatrix.h"
#include "GTextArea.h"

class GKeyboard : public GButtonMatrix {
  public:
    GKeyboard(GWidget& parent=GScreen);
    GKeyboard(GWidget* parent);
    virtual ~GKeyboard();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void      setTextArea(GTextArea& textarea);
    GTextArea* getTextArea();

    void      cursor_manage(bool enable);
    bool      cursor_manage();

    // kb_mode_t : KB_MODE_TEXT_LOWER, KB_MODE_TEXT_UPPER, KB_MODE_SPECIAL,KB_MODE_NUM
    void      mode(kb_mode_t mode); 
    kb_mode_t mode();
    void      map(kb_mode_t mode, const char * map[]); 
    void      control_map(kb_mode_t mode, const button_ctrl_t ctrl_map[]);

};


#endif //__GKEYBOARD_H__