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

GMultiScreen  MultiScreen;    // วิตเจ็ต GMultiScreen
GWiFiManager  WifiManager;    // วิตเจ็ต GWiFiManager
GLabel        lb_clock;       // วิตเจ็ต ข้อความ สำหรับแสดงเวลา
GTimer        timer;          // ตัวตั้งเวลาให้ทำงานเป็นช่วงๆ

GSwitch3D     sw3d;           // วิตเจ็ต สวิตซ์ 3D
GGauge        gauge_temp;     // วิตเจ็ต gauge หน้าปัด สำหรับแสดงอุณหภูมิ
GGauge        gauge_humid;    // วิตเจ็ต gauge หน้าปัด สำหรับแสดงความชื้น

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
   // กำหนดสีพื้นหลังแบบไล่เฉด 2 สี ด้วยสีแบบ hex color
  BlynkGO.fillScreen(TFT_COLOR_HEX(0x58126A), TFT_COLOR_HEX(0xF6B2E1));


  MultiScreen.addScreen(3);             // เพิ่ม 3 MultiScreen;
  MultiScreen.indic_color(TFT_WHITE, TFT_DARKGRAY); // กำหนดสีของ indic  สี indic ขณะ action และ non-action

  sw3d        .parent(MultiScreen[0]);  // ให้สวิตซ์ไปอยู่ หน้า 0
  gauge_temp  .parent(MultiScreen[1]);  // ให้ gauge_temp ไปอยู่ หน้า 1
  gauge_humid .parent(MultiScreen[2]);  // ให้ gauge_humid ไปอยู่ หน้า 2

  gauge_temp.title("อุณหภูมิ");
  gauge_temp.color(TFT_RED);
  gauge_temp.clickable(false);          // ไม่ให้รับสัญญาณการกดสัมผัส
  gauge_temp = 24;

  gauge_humid.title("ความชื้น");
  gauge_humid.color(TFT_BLUE);
  gauge_humid.clickable(false);         // ไม่ให้รับสัญญาณการกดสัมผัส
  gauge_humid = 58;

  lb_clock = "--/--/--";
  lb_clock.font(prasanmit_30,TFT_WHITE);

  WifiManager.font(prasanmit_40);
  WifiManager.align(ALIGN_TOP_RIGHT,-5, -2);    // วางตำแหน่ง วิตเจ็ตไว้ขวาบน ปรับเลื่อนไปซ้าย 5 pixel เลื่อนขึ้น 2 pixel

  lb_clock.align(WifiManager, ALIGN_LEFT, -5);  // วางตำแหน่ง ไว้ถัดจาก Wifimanager ไปทางซ้าย 5 pixel  

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
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}
