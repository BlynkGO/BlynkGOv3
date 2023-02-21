/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GWiFiManager.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

/*
 *********************************************************************
 * GWiFiManager 
 * Version 1.0.0
 *  คุณสมบัติ
 *    1.เชื่อมต่อได้แล้วจะมีการจำว่าเชื่อม WiFi Router ตัวไหน
 *      เมื่อ reboot ใหม่ก็จะกลับมาเชื่อมต่อ WiFi Router นั้น
 *      หาก มีการโจมตีด้วย ssid ชื่อเดียวกันซ้ำๆ ก็ยังกลับไปเชื่อมต่อ WiFi Router ตัวเดิมได้
 *    2.ตั้งค่า WiFi ได้ทั้งแบบ Auto IP และ Fixed IP
 *    3.มีระบบ WiFi Scan ค้นหา ไวไฟรอบๆ โดย scan ให้ทุกๆ 10 วินาที เมื่อกดค้นหา
 *    4.กดค้าง 5วินาที ล้าง wifi/pass เสียแพงมากๆ
 *
 * Version 1.0.1
 *    1. เปลี่ยน GRect+GLabel เป็น GButton แทน
 *
 * Version 1.0.2
 *    1. เพิ่ม API reset() สำหรับล้าง ข้อมูลเก่าได้
 *
 * Version 1.0.3
 *    1. เพิ่มให้ใส่ DNS IP แบบ Fixed IP เองได้
 *
 * Version 1.0.4
 *    1. ปรับ design ให้เข้ากับจอขนาดเล็กของ T-Watch ได้
 *
 * Version 1.0.5
 *    1. เพิ่มให้สามารถตั้ง save(String ssid, String pass)
 *
 * Version 1.0.6
 *    1. เพิ่มให้สามารถตั้ง color(...) สีของ สัญลักษณ์ไวไฟ เมื่อ connected, disconnected, และสีไวไฟที่เป็นพื้นหลังได้
 *
 * Version 1.0.7 @07/12/21
 *    1. แก้ที่แบบ fixed ip ( static ip ) แล้ว connection แสดงซ้ำ 2 รอบ
 *    2. ปรับให้ ใช้ prasanmit_40 ตั้งแต่ต้น จะได้มีขนาดใหญ่พอนิ้วจิ้มง่าย
 *
 * Version 1.0.8 @09/07/22
 *    1. ปรับขนาด font สำหรับ 800x480 Screen
 *
 * Version 1.0.9 @05/11/22
 *    1. ปรับขนาด สำหรับ 320x240 Screen
 *
 * Version 1.0.10 @10/11/22
 *    1. เพิ่ม API disconnectWiFi() สั่งตัดการเชื่อมต่อได้ ผ่าน API
 *    2. เพิ่ม API connectWiFi()    สั่งเชื่อมต่อไวไฟจากที่บันทึกไว้ ผ่าน API
 *
 * Version 1.0.11 @02/01/23
 *    1. wifi_manager.reset()  ทำการ reset ค่า wifi ทิ้ง พร้อมล้าง widget ข้อความต่างๆของ wifi_manager ออก
 *
 * Version 1.0.12 @11/01/23
 *    1. กดค้างนาน 5 วินาที    จะเข้าทำการ reset WiFi/Password แล้วค่อยเข้าไป GWiFiManager ไปตั้งค่าให้ GWiFiSetting
 *    
 *********************************************************************
 */


#ifndef __GWIFIMANAGER_H__
#define __GWIFIMANAGER_H__

#include "config/blynkgo_config.h"
#if BLYNKGO_USE_WIFI

#include "GObject.h"
#include "GContainer.h"
#include "GLabel.h"
#include "GSwitch.h"
#include "GTextArea.h"
#include "GContainer.h"
#include "GPage.h"
#include "GList.h"
#include "GTask.h"

#define WIFI_MANAGER_VERSION       "1.0.12"

class GWiFiSetting;

/******************************************************
 * GWiFiManager
 ******************************************************/
typedef struct _wifimanager_config{
  char      ssid[33];
  char      password[65];
  uint8_t   bssid[6];
  bool      ssid_enable;
  bool      auto_ip_enable;
  uint32_t  fixed_ip32;
  uint32_t  gateway_ip32;
  uint32_t  subnet32;
  uint32_t  dns32;
} wifimanager_config_t;

typedef  enum {EWM_STATE_STANDBY, EWM_STATE_DISCONNECTED, EWM_STATE_TEST_CONNECTING, EWM_STATE_CONNECTED }  ewm_state_t;

typedef struct {
  lv_cont_ext_t *cont;
  GLabel *lb_wifi_signal[2];      // วิตเจ็ตข้อความ ใช้แสดงระดับสัญญาณ WiFi  ให้เป็น ลูกของ GWiFiManager
  GWiFiSetting *wifisetting;      // วิตเจ็ต app สำหรับทำเป็น WiFiSetting
  color_t   color[3];             //  0 : สี สัญลักษณ์ WiFi สีเทาเข้มที่เป็นพื้นหลัง,  1 : สีแดง WiFi Disconnected , 2 : สีขาว WiFi Connected
} gwifimanager_ext_t;

extern ewm_state_t ewm_state;


class GWiFiManager : public GContainer {
  public:
    GWiFiManager(GWidget& parent=GScreen);
    GWiFiManager(GWidget* parent);
    virtual ~GWiFiManager();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);


    void  font(font_t& font);    
    void  reset();                // ล้าง config ที่บันทึกอยู่เดิมออก
    
    inline String   version()           { return WIFI_MANAGER_VERSION; }

    void  save(String ssid, String pass);
    void  connectWiFi();                  // สั่งเชื่อมต่อไวไฟจากที่เซฟไว้
    void  disconnectWiFi();               // สั่งตัดการเชื่อมต่อ
    String ssid();
    String password();

    void color(color_t color_wifi_connected, color_t color_wifi_disconnected=TFT_RED, color_t color_wifi_background=TFT_DARKGRAY);

    void bg_color(color_t color);
    void bg_color(color_t main_color, color_t grad_color);
    color_t bg_color();


    // inner APIs (don't call)
    static  GTask   WiFiSignal_Task; 
    static  void    WiFiSignalUpdate_Task_cb(void* param=NULL);
    static  void    onWiFiConnected();      // เชื่อมเสร็จได้ จะมา run WiFiSignalUpdate_Task_cb() เป็นระยะให้อัตโนมัติ
    static  void    onWiFiDisconnected();   
    static  void    onClicked_cb(GWidget* widget);
    static  void    autoconnection();
};

GWiFiManager*   new_GWiFiManager(GWidget& parent=GScreen);
GWiFiManager*   new_GWiFiManager(GWidget *parent);


/******************************************************
 * GWiFiSetting
 ******************************************************/

typedef struct {
  // เลเยอร์ลูก สำหรับหน้า WiFiManager
  // ส่วน Title Bar

  GButton   *btn_back;                // วิตเจ็ตปุ่มกด สำหรับ back ไปหน้าหลัก ให้เป็น ลูกของ GWiFiSetting
  GLabel    *lb_wifisetting_title;    // วิตเจ็ตข้อความ "ตั้งค่า WiFi"  ให้เป็น ลูกของ GWiFiSetting
  GButton   *btn_wifiscan;            // วิตเจ็ดปุ่มกด สำหรับ wifi scan  ให้เป็น ลูกของ GWiFiSetting
  GButton   *btn_wifistatus;          // วิตเจ็ดปุ่มกด สำหรับ สถานะว่า wifi เชื่อมต่อสำเร็จหรือไม่ กดเข้าไปให้บอก ข้อมูลเชื่อมต่อ ให้เป็น ลูกของ GWiFiSetting

  // ส่วน Body
  GPage     *page_body;               // วิตเจ็ต Page ส่วน body สำหรับ WiFiManager  ให้เป็น ลูกของ GWiFiSetting
    GContainer  *cont_ssid;           // วิตเจ็ต Container ส่วน ssid/password สำหรับ WiFiManager ให้เป็นลูกของ page_body
      GLabel    *lb_ssid_info;        // วิตเจ็ตข้อความ "SSID :"  ให้เป็น ลูกของ page_body
      GLabel    *lb_pass_info;        // วิตเจ็ตข้อความ "Password :" ให้เป็น ลูกของ page_body
      GTextArea *ta_ssid;             // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้ เติม ssid ให้เป็น ลูกของ page_body
      GTextArea *ta_password;         // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้ เติม password ให้เป็น ลูกของ page_body
      GSwitch   *sw_ssid_enable;      // วิตเจ็ตสวิตซ์เลื่อน สำหรับ สั่งเปิด/ปิด การเชื่อมต่อ ssid ให้เป็นลูกของ cont_ssid
    GContainer  *cont_auto_ip;        // วิตเจ็ต Container ส่วน auto ip/fix ip ให้เป็นลูกของ page_body
      GLabel    *lb_auto_ip;          // วิตเจ็ตข้อความ "Auto IP "  ให้เป็นลูกของ cont_auto_ip
      GSwitch   *sw_auto_ip;          // วิตเจ็ตสวิตซ์เลื่อน ให้เป็นลูกของ cont_auto_ip
    GContainer  *cont_fix_ip;         // วิตเจ็ต Container ใช้เป็นพื้นที่ สำหรับกรอก fix ip ต่างๆ
      GLabel    *lb_fix_ip;           // วิตเจ็ตข้อความ "Fixed IP :" ให้เป็นลูกของ rect_fix_ip
      GTextArea *ta_fix_ip;           // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้แก้ fixed ip  ให้เป็นลูกของ rect_fix_ip
      GLabel    *lb_gateway_ip;       // วิตเจ็ตข้อความ "Gateway IP :" ให้เป็นลูกของ rect_fix_ip
      GTextArea *ta_gateway_ip;       // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้แก้ gateway ip  ให้เป็นลูกของ rect_fix_ip
      GLabel    *lb_subnet;           // วิตเจ็ตข้อความ "Net Mask :" ให้เป็นลูกของ rect_fix_ip
      GTextArea *ta_subnet;           // วิตเจ็ตช่องใส่ข้อความ สำหรับให้ผู้ใช้แก้ Net Mask  ให้เป็นลูกของ rect_fix_ip
      GLabel    *lb_dns_ip;
      GTextArea *ta_dns_ip;
        
  // ตัวช่วยไว้บังไม่ให้กดได้
  GRect     *rect_disable_click;      // วิตเจ็ตสี่เหลี่ยม สำหรับบังไม่ให้สามารถ click อะไรๆได้ ให้เป็น ลูกของ GWiFiSetting

  // ส่วนของ msgbox สำหรับ WiFi Scan และ WiFi Info (ใช้ร่วมกัน)
  GRect     *app_wifi_msgbox;         // วิตเจ็ตสี่เหลี่ยม สำหรับเป็นพื้นที่ จัดการ WiFi Scan ให้เป็น ลูกของ GWiFiSetting
    // ส่วน Title Bar
    GButton *btn_wifi_msgbox_back;    // วิตเจ็ตปุ่มกด สำหรับ back ไปก่อนหน้า ให้เป็น ลูกของ app_wifi_msgbox
    GLabel  *lb_wifi_msgbox_title;    // วิตเจ็ตข้อความ สำหรับเป็น title ของ app_wifi_msgbox ให้เป็น ลูกของ app_wifi_msgbox
    GLabel  *lb_wifi_msgbox_status;   // วิตเจ็ตข้อความ สำหรับเป็น status ด้านล่าง ของ app_wifi_msgbox ให้เป็น ลูกของ app_wifi_msgbox

    // ส่วน Body สำหรับ WiFi Scan
    GList   *list_wifiscan;           // วิตเจ็ต List ส่วน body สำหรับ WiFi Scan เวลา scan พบ ssid จะมาใส่ใน list นี้ ให้เป็น ลูกของ app_wifi_msgbox

    // ส่วน Body สำหรับ WiFi Info
    GPage   *page_wifiinfo;           // วิตเจ็ต Page ให้เป็นส่วน body สำหรับ WiFi Info หากเชื่อมต่อสำเร็จจะไว้บอก IP, SubNet Mask, Domain IP เป็นต้น  ให้เป็น ลูกของ app_wifi_msgbox
      GLabel*lb_wifiinfo;             // วิตเจ็ตข้อความ ไว้รวบรวมข้อมูล WiFi info เมื่อมีการเชื่อมต่อสำเร็จ ให้เป็นลูกของ page_wifiinfo

} gwifisetting_ext_t;


class GWiFiSetting : public GObject {
  public:
    GWiFiSetting(GWidget& parent=GScreen);
    GWiFiSetting(GWidget* parent);
    virtual ~GWiFiSetting();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void onBack(event_handler_t fn)             { if(!this->isCreated()) this->create();  fn_onback = fn; }



    // inner APIs (don't call)
    event_handler_t fn_onback = NULL;
    static void onWiFiConnected();
    static void onWiFiDisconnected();

    static void update_WiFiInfo();
    static void ta_onValueChanged(GWidget*w);

    static void scan_WiFiNetworks();
    static void update_NetworkList();
    static void WiFiReScan(uint32_t period=10000);
    
    // wifi_config load/save/clean/display
    static wifimanager_config_t wifi_config;
    static void autoconnection();
    static void load_WiFiConfig();                        // สำหรับโหลด WiFiConfig
    static void save_WiFiConfig();                        // สำหรับบันทึก WiFiConfig
    static void save_WiFiConfig(String ssid, String pass);     // สำหรับบันทึก WiFiConfig
    static void clean_WiFiConfig();                       // สำหรับล้าง WiFiConfig
    static void display_WiFiConfig();                     // สำหรับแสดง WiFiConfig ออก Serial Monitor
    static void connect_WiFi(bool force_reconnect=false); // สำหรับเชื่อมต่อ WiFi จาก WiFiConfig ; force_reconnect = true คือบังคับตัด wifi แล้วเชื่อมต่อใหม่

  private:

};


GWiFiSetting*   new_GWiFiSetting(GWidget& parent=GScreen);
GWiFiSetting*   new_GWiFiSetting(GWidget *parent);


#endif // BLYNKGO_USE_WIFI
#endif //__GWIFIMANAGER_H__
