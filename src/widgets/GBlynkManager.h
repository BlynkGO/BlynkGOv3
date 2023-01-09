/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GBlynkManager.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

/*********************************************************************
 *
 * Version 1.0.0
 *    - Blynk Connection with green/red indicator for connection
 *
 * Version 1.0.1
 *    - add GSwitch for blynk auth enable
 *    - ta_blynk_auth set to password_mode
 *
 * Version 1.0.2
 *    - design for TTGO TWatch จอเล็ก
 *
 * Version 1.0.3
 *    - เพิ่มคำสั่ง save(..)
 *
 *********************************************************************/

#ifndef __GBLYNKMANAGER_H__
#define __GBLYNKMANAGER_H__

#include "config/blynkgo_config.h"
#if BLYNKGO_USE_WIFI 
#if BLYNKGO_USE_BLYNK

#include <BlynkBlynkGO.h>
#include "GObject.h"
#include "GContainer.h"
#include "GLabel.h"
#include "GSwitch.h"
#include "GTextArea.h"
#include "GContainer.h"
#include "GPage.h"
#include "GButton.h"

#define BLYNK_MANAGER_VERSION       "1.0.3"

class GBlynkSetting;

typedef struct _blynkmanager_config{   
  bool      std_host_enable;  // switch state of std <--> manual blynk host/port
  char      auth[40];         // blynk auth
  char      host[256];        // manual blynk host
  uint16_t  port;             // manual blynk port
  bool      blynk_auth_enable;// switch state of blynk auth anable
} blynkmanager_config_t;      // สำหรับจัดเก็บ ข้อมูล blynk ในการเชื่อมต่อของ GBlynkManager


typedef struct {
  lv_cont_ext_t *cont;
  GLabel *lb_blynk_status;      // วิตเจ็ตข้อความ ใช้แสดงระดับสัญญาณ WiFi  ให้เป็น ลูกของ GWiFiManager
  GBlynkSetting *blynk_setting;      // วิตเจ็ต app สำหรับทำเป็น WiFiSetting
} gblynkmanager_ext_t;


class GBlynkManager : public GContainer {
  public:
    GBlynkManager(GWidget& parent=GScreen);
    GBlynkManager(GWidget* parent);
    virtual ~GBlynkManager();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void   reset();
    inline String version()               { return BLYNK_MANAGER_VERSION; }
    void   save(String blynk_auth, String domain = BLYNK_DEFAULT_DOMAIN, uint16_t port = BLYNK_DEFAULT_PORT);

    String    auth();
    String    host();
    uint16_t  port();
    inline  String port_str()             { return String(port()); }

    // inner APIs (don't call)    
    static void onClicked_cb(GWidget*w);

};

GBlynkManager*   new_GBlynkManager(GWidget& parent=GScreen);
GBlynkManager*   new_GBlynkManager(GWidget *parent);

/******************************************************
 * GBlynkSetting
 ******************************************************/
typedef struct {
  // เลเยอร์ลูก สำหรับหน้า BlynkManager
  // ส่วน Title Bar
  GButton   *btn_back;                // วิตเจ็ตปุ่มกด สำหรับ back ไปหน้าหลัก ให้เป็น ลูกของ GBlynkSetting
  GLabel    *lb_blynksetting_title;   // วิตเจ็ตข้อความ "ตั้งค่า Blynk"  ให้เป็น ลูกของ GBlynkSetting
//  GButton   *btn_blynk_status;        // วิตเจ็ดปุ่มกด สำหรับ สถานะว่า blynk เชื่อมต่อสำเร็จหรือไม่ กดเข้าไปให้บอก ข้อมูลเชื่อมต่อ ให้เป็น ลูกของ GBlynkSetting
  GLabel    *lb_wait_a_minute;
  GRect     *btn_blynk_status;
    GLabel    *lb_blynk_status;
    
  // ส่วน Body
  GPage     *page_body;               // วิตเจ็ต Page ส่วน body สำหรับ GBlynkSetting  ให้เป็น ลูกของ GBlynkSetting
    GContainer  *cont_blynk_auth;     // วิตเจ็ต Container ส่วน blynk auth ให้เป็นลูกของ page_body
      GLabel    *lb_blynk_auth;       // วิตเจ็ตข้อความ "Blynk Auth :"  ให้เป็น ลูกของ page_body
      GTextArea *ta_blynk_auth;       // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้ เติม Blynk Auth ให้เป็น ลูกของ page_body
      GSwitch   *sw_blynk_auth_enable;// วิตเจ็ตสวิตซ์เลื่อน ให้ Blynk ทำงานไหม
    GContainer  *cont_std_host;       // วิตเจ็ต Container ส่วน standard host หรือ manual host ให้เป็นลูกของ page_body
      GLabel    *lb_std_host;         // วิตเจ็ตข้อความ "Standar Blynk Host "  ให้เป็นลูกของ cont_std_host
      GSwitch   *sw_std_host;         // วิตเจ็ตสวิตซ์เลื่อน ให้เป็นลูกของ cont_std_host
    GContainer  *cont_man_host;       // วิตเจ็ต Container ใช้เป็นพื้นที่ สำหรับกรอก Manual Blynk Host
      GLabel    *lb_blynk_host;       // วิตเจ็ตข้อความ "Blynk Host :" ให้เป็นลูกของ cont_man_host
      GTextArea *ta_blynk_host;       // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้แก้ Blynk Host  ให้เป็นลูกของ cont_man_host
      GLabel    *lb_blynk_port;       // วิตเจ็ตข้อความ "Blynk Port :" ให้เป็นลูกของ cont_man_host
      GTextArea *ta_blynk_port;       // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้แก้ Blynk Port  ให้เป็นลูกของ cont_man_host
} gblynksetting_ext_t;

class GBlynkSetting : public GObject {
  public:
    GBlynkSetting(GWidget& parent=GScreen);
    GBlynkSetting(GWidget* parent);
    virtual ~GBlynkSetting();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void hidden(bool enable);
    bool hidden();
    static void ta_onValueChanged(GWidget*w);

    void onBack(event_handler_t fn)             { if(!this->isCreated()) this->create();  fn_onback = fn; }


    event_handler_t fn_onback = NULL;
    // blynk_config load/save/clean/display
    static blynkmanager_config_t blynk_config;    
    static void load_BlynkConfig();                         // สำหรับโหลด BlynkConfig
    static void save_BlynkConfig();                         // สำหรับบันทึก BlynkConfig
    static void save_BlynkConfig(String blynk_auth, String domain = BLYNK_DEFAULT_DOMAIN, uint16_t port = BLYNK_DEFAULT_PORT);
    static void clean_BlynkConfig();                        // สำหรับล้าง BlynkConfig
    static void display_BlynkConfig();                      // สำหรับแสดง BlynkConfig ออก Serial Monitor
    static void BlynkConnection();
};

GBlynkSetting*   new_GBlynkSetting(GWidget& parent=GScreen);
GBlynkSetting*   new_GBlynkSetting(GWidget *parent);

#endif
#endif

#endif // __GBLYNKMANAGER_H__
