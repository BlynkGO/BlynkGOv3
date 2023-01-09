/*******************************************************
 * ตัวอย่างพื้นฐาน GWiFiManager 
 * เชื่อมต่อ WiFi แบบอัตโนมัติ
 * จากค่าที่ User ได้กำหนด SSID, PASSWORD ( Fixed IP, Gateway, Subnet )
 * ผ่านทาง Interface สัมผัสทางหน้าจอ
 * ---------------------------------------
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 ******************************************************/

#include <BlynkGOv3.h>

GWiFiManager  wifi_manager;    // วิตเจ็ต GWiFiManager
GLabel        labelClock;     // วิตเจ็ตข้อความ สำหรับทำนาฬิกาแสดงเวลา
GTimer        timer;          // timer ตั้งเวลาทำงาน

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  labelClock = "";
  labelClock.font(prasanmit_40,TFT_GREEN);
  
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);    // วางตำแหน่ง วิตเจ็ตไว้ขวาบน ปรับเลื่อนไปซ้าย 5 pixel ลงมา 5 pixel
//  wifi_manager.font(prasanmit_40);           // กำหนดเปลียน font สำหรัย สัญญาณความแรง wifi ของวิตเจ็ต

  // customize สีสรร ของ WiFiManager เองได้
  // ให้เปิด comment ส่วนข้างล่างนี้ แล้วปรับสีตามต้องการ
//  wifi_manager.onClicked([](GWidget*w){
//    GWiFiManager::onClicked_cb(w);    
//    gwifimanager_ext_t *ext_wifimanager = (gwifimanager_ext_t*) wifi_manager.ext_attr();
//    GWiFiSetting *wifisetting = ext_wifimanager->wifisetting;
//    gwifisetting_ext_t *ext_wifisetting = (gwifisetting_ext_t *) wifisetting->ext_attr();
//    
//    wifisetting->color(TFT_COLOR_HEX(0x001446));                        // สีพื้นหลัง title bar
//    ext_wifisetting->page_body->color(TFT_COLOR_HEX(0x275C7A));         // สีพื้นหลัง body
//    ext_wifisetting->cont_ssid->color(TFT_COLOR_HEX(0xF7A41A));         // สีพื้นหลัง container ที่ใส่ค่า ssid/password
//    ext_wifisetting->cont_auto_ip->color(TFT_COLOR_HEX(0xF7A41A));      // สีพื้นหลัง container ที่เลือก โหมด auto/manual ip
//    ext_wifisetting->sw_ssid_enable->color(TFT_COLOR_HEX(0xFFEF38));    // สีของ switch เลือก ssid_enable
//    ext_wifisetting->sw_auto_ip->color(TFT_COLOR_HEX(0xFFEF38));        // สีของ switch เลือก auto/manual ip
//  });


}

void loop() {
  BlynkGO.update();
}

// เมื่อ WiFi มีการเชื่อมต่อสำเร็จ (มีหรือไม่ก็ได้)
WIFI_CONNECTED(){
  // สั่งให้ ไปเชื่อมต่อ Server ไหน ให้วางภายในนี้
  
}

// เมื่อ WiFi หลุดการเชื่อมต่อ (มีหรือไม่ก็ได้)
WIFI_DISCONNECTED(){
  
}

// เมื่อ NTP ซิงค์เวลา internet สำเร็จ (มีหรือไม่ก็ได้)
NTP_SYNCED(){
  timer.setInterval(1000L, [](){    // ตั้งเวลาทำงานทุกๆ 1000ms (1วินาที)  
    labelClock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

