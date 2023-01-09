/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GTimeSelector.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GTIMESELECTOR_H__
#define __GTIMESELECTOR_H__

#include "GObject.h"
#include "GLabel.h"
#include "GButton.h"
#include "GCircle.h"
#include "GLine.h"

FONT_DECLARE(BebasNeueBold_num_45);
#define AM        1
#define PM        0

typedef struct _timeselector_ext {
  uint8_t   selector_mode;        // 0 : hour selector; 1 minute selector
  int8_t    h;                    // hour value   (0..11)
  int8_t    m;                    // minute value (0..59)
  time_t    *timestamp;
  GLabel    *lb_outside;          // pointer วิตเจ็ตข้อความที่ชี้ไปยัง GLabel ภายนอกที่จะเปลี่ยนแปลงค่า time จาก GTimeSelector นี้
  GLabel    *lb_info;             // pointer วิตเจ็ตข้อความ  "ON" หรือ "OFF"  ("เปิด" หรือ )
  GLabel    *lb_time_h;           // pointer วิตเจ็ตข้อความ  แสดงค่า hour ด้านบน
  GLabel    *lb_collon;           // pointer วิตเจ็ตข้อความ  แสดงเครื่องหมาย : ด้านบน
  GLabel    *lb_time_m;           // pointer วิตเจ็ตข้อความ  แสดงค่า minute ด้านบน
  GButton   *btn_am;              // pointer วิตเจ็ตปุ่ม AP ด้านขวาบน
  GButton   *btn_pm;              // pointer วิตเจ็ตปุ่ม PM ด้านขวาบน
  GCircle   *cir_frame;           // pointer วิตเจ็ตวงกลม พื้นที่หน้าปัด
  GCircle   *cir_selector;        // pointer วิตเจ็ตวงกลม สำหรับใช้สัมผัสหมุนปรับ hour/minute
  GLine     *line_selector;       // pointer วิตเจ็ตเส้นตรง เป็นเส้นเชื่อมจุดศก. กับ cir_selector
  GLabel    *lb_frame_tick[12];   // pointer วิตเจ็ตข้อความ ตัวเลขนาฬิกาบนหน้าปัด
  GButton   *btn_ok;              // pointer วิตเจ็ตปุ่ม OK / ตกลง
  GButton   *btn_cancel;          // pointer วิตเจ็ตปุ่ม Cancel / ยกเลิก

} gtimeselector_ext_t;

class GTimeSelector : public GObject {
  public:
    GTimeSelector(GWidget& parent=GScreen);
    GTimeSelector(GWidget* parent);
    virtual ~GTimeSelector();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            
    // User APIs
    void    init(uint8_t h, uint8_t m, bool am , String info = "");  // 00:00-12:00  AM/PM  ex.  TimeSelector.init( 3, 45, PM);
    void    init(uint8_t h, uint8_t m, String info = "");            // 00:00-23:59         ex.  TimeSelector.init( 15, 45 );
    void    init(time_t &timestamp, String info = "");
    void    init(GLabel &label, time_t &timestamp, String info = "");

    uint8_t hour(bool format24=false);     // คืนค่า hour ที่ได้มีการเลือกผ่าน GTimeSelector
    uint8_t minute();   // คืนค่า minute ที่ได้มีการเลือกผ่าน GTimeSelector
    bool    isAM();     // คืนค่า ว่าเป็น AM หรือไม่ ที่ได้มีการเลือกผ่าน GTimeSelector
    time_t  timestamp();// คืนค่า เป็น timestamp ภายใน 24 ชม. (86400วินาที)
    inline  void timestamp(time_t &timestamp, String info = "")    { init(timestamp, info); }

    void    onApply(event_handler_t fn)            { if(!this->isCreated()) this->create();  fn_onapply = fn; }
    void    onCancel(event_handler_t fn)           { if(!this->isCreated()) this->create();  fn_oncancel = fn; }

    // System APIs  ( don't call )
    event_handler_t  fn_onapply   = NULL;
    event_handler_t  fn_oncancel  = NULL;
    void    refresh();


  private:

};

// //--------------------------------------------------------------
// // PSRAM / HEAP MEM Widget Allocation
// GTimeSelector*  new_GTimeSelector(GWidget& parent=GScreen);
// GTimeSelector*  new_GTimeSelector(GWidget *parent);

#endif //__GTIMESELECTOR_H__
