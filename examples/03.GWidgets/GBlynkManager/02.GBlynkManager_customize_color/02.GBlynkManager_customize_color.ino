/********************************************************************************
 * ตัวอย่าง การเชื่อมต่อ WiFi ด้วย วิตเจ็ต GWiFiManager
 *  และ เชื่อมต่อ Blynk ด้วย วิตเจ็ต GBlynkManager
 *  
 * ------------------------------------------------------------------
 *  วิธีการติดตั้ง
 *     1. ให้ copy ไฟล์ ในโฟลเดอร์ BlynkGOv3/blynk/src  นี้ไปวางไว้ที่ 
 *        โฟลเดอร์ไลบรารี่ Blynk หากมีไฟล์ซ้ำให้ copy ทับ
 *     2. เปิด BlynkGOv3/src/config/blynkgo_config.h
 *        ตั้ง ให้ 
 *          #define BLYNKGO_USE_WIFI      1
 *          #define BLYNKGO_USE_NTP       1
 *          #define BLYNKGO_USE_BLYNK     1
 * 
 ********************************************************************************/

#include <BlynkGOv3.h>

GWiFiManager  wifi_manager;
GBlynkManager blynk_manager;
GLabel        lb_clock;

void wifi_manager_customize();
void blynk_manager_customize();

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5); 
  wifi_manager.font(prasanmit_40);
  blynk_manager.align(wifi_manager, ALIGN_LEFT,-5);
  lb_clock.font(prasanmit_40);
  lb_clock.color(TFT_GREEN);
  lb_clock.align(ALIGN_CENTER);
    
  wifi_manager_customize();      // customize สีสรร ของ GWiFiManager เองได้
  blynk_manager_customize();     // customize สีสรร ของ GBlynkManager เองได้

}

void loop() {
  BlynkGO.update();
}

// มีหรือไม่ก็ได้
WIFI_CONNECTED(){
  Serial.println("[WiFi] Connected");
  Serial.println(WiFi.localIP());
}

// มีหรือไม่ก็ได้
NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

// มีหรือไม่ก็ได้
BLYNK_CONNECTED(){
  Serial.println("[Blynk] Connected");
}

// มีหรือไม่ก็ได้
BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] Disconnected");
}


void wifi_manager_customize(){
  wifi_manager.onClicked([](GWidget*w){
    GWiFiManager::onClicked_cb(w);
    gwifimanager_ext_t *ext_wifimanager  = (gwifimanager_ext_t*) wifi_manager.ext_attr();
    GWiFiSetting *wifisetting            = ext_wifimanager->wifisetting;
    gwifisetting_ext_t *ext_wifisetting  = (gwifisetting_ext_t *) wifisetting->ext_attr();
    
    wifisetting->color(TFT_COLOR_HEX(0x514945));                        // สีพื้นหลัง title bar
    ext_wifisetting->page_body->color(TFT_COLOR_HEX(0x894242));         // สีพื้นหลัง body
    ext_wifisetting->cont_ssid->color(TFT_COLOR_HEX(0xEBD4CC));         // สีพื้นหลัง container ที่ใส่ค่า ssid/password
    ext_wifisetting->cont_auto_ip->color(TFT_COLOR_HEX(0xEBD4CC));      // สีพื้นหลัง container ที่เลือก โหมด auto/manual ip
    ext_wifisetting->sw_ssid_enable->color(TFT_COLOR_HEX(0xD55C5A));    // สีของ switch เลือก ssid_enable
    ext_wifisetting->sw_auto_ip->color(TFT_COLOR_HEX(0xD55C5A));        // สีของ switch เลือก auto/manual ip

    ext_wifisetting->cont_ssid->font_color(TFT_BLACK);                  // สีฟอนต์ ของวิตเจ็ตลูกๆ บน container นี้
    ext_wifisetting->cont_auto_ip->font_color(TFT_BLACK);               // สีฟอนต์ ของวิตเจ็ตลูกๆ บน container นี้
    
    ext_wifisetting->ta_ssid->font_color(TFT_BLACK);                    // สีฟอนต์ ของวิตเจ็ต ta_ssid
    ext_wifisetting->ta_password->font_color(TFT_BLACK);
    ext_wifisetting->ta_fix_ip->font_color(TFT_BLACK);
    ext_wifisetting->ta_gateway_ip->font_color(TFT_BLACK);
    ext_wifisetting->ta_subnet->font_color(TFT_BLACK);

    ext_wifisetting->ta_ssid->placeholder_color(TFT_DARKGRAY);          // สีฟอนต์ของ placeholder ของ ta_ssid
    ext_wifisetting->ta_password->placeholder_color(TFT_DARKGRAY);
    ext_wifisetting->ta_fix_ip->placeholder_color(TFT_DARKGRAY);
    ext_wifisetting->ta_gateway_ip->placeholder_color(TFT_DARKGRAY);
    ext_wifisetting->ta_subnet->placeholder_color(TFT_DARKGRAY);
  });
}

void blynk_manager_customize(){
  blynk_manager.onClicked([](GWidget*w){
    GBlynkManager::onClicked_cb(w);    
    gblynkmanager_ext_t *ext_blynkmanager = (gblynkmanager_ext_t*) blynk_manager.ext_attr();
    GBlynkSetting *blynksetting = ext_blynkmanager->blynk_setting;
    gblynksetting_ext_t *ext_blynksetting = (gblynksetting_ext_t *) blynksetting->ext_attr();
    
    blynksetting->color(TFT_COLOR_HEX(0x514945));                        // สีพื้นหลัง title bar
    ext_blynksetting->page_body->color(TFT_COLOR_HEX(0x894242));         // สีพื้นหลัง body
    ext_blynksetting->cont_blynk_auth->color(TFT_COLOR_HEX(0xEBD4CC));   // สีพื้นหลัง container ที่ใส่ค่า blynk auth
    ext_blynksetting->cont_std_host->color(TFT_COLOR_HEX(0xEBD4CC));     // สีพื้นหลัง container ที่เลือก โหมด standard/manual blynk host
    ext_blynksetting->sw_std_host->color(TFT_COLOR_HEX(0xD55C5A));       // สีของ switch เลือก standard/manual blynk host

    ext_blynksetting->cont_blynk_auth->font_color(TFT_BLACK);
    ext_blynksetting->cont_std_host->font_color(TFT_BLACK);
    
    ext_blynksetting->ta_blynk_auth->font_color(TFT_BLACK);
    ext_blynksetting->ta_blynk_host->font_color(TFT_BLACK);
    ext_blynksetting->ta_blynk_port->font_color(TFT_BLACK);
    
    ext_blynksetting->ta_blynk_auth->placeholder_color(TFT_DARKGRAY);
    ext_blynksetting->ta_blynk_host->placeholder_color(TFT_DARKGRAY);
    ext_blynksetting->ta_blynk_port->placeholder_color(TFT_DARKGRAY);
  });
}


