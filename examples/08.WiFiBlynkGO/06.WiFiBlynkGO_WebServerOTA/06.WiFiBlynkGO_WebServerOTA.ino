/*******************************************************
 * ตัวอย่างพื้นฐาน การ Upload โคด แบบ OTA ไร้สาย ผ่านทาง WebServer
 * -----------------------------------------------------
 * ก่อนใช้งาน WiFi
 * ให้กำหนด เปิด การใช้งาน WiFi ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 * -----------------------------------------------------
 *
 * วิธีใช้
 *   1. สำหรับ Windows ให้ติดตั้ง DNS server 
 *      คือ Bonjour Server ของ Apple โหลดได้จาก 
 *      https://support.apple.com/kb/dl999?locale=en_US
 *      แล้วทำการ reboot
 *     
 *   2. เปิด ArduinoIDE เลือก บอร์ด แล้วเลือก Partition Scheme ไปที่
 *      "BlynkGO 6.25MB OTA (...)"
 *      
 *   3. ในรอบแรกของการ Upload โคดนี้  
 *      ให้เสียบสาย USB แล้วเลือก Serial Ports ที่เสียบ บอร์ด
 *      แล้ว Upload
 *      
 *   4. ในรอบถัดๆไปของการ Upload ไม่มีความจำเป็นต้องต่อสายกับ computer แล้ว 
 *      การ Upload จะเป็นการ Upload แบบ OTA ไร้สาย ผ่านทาง WebServer
 *      
 *      - ให้ นำ BlynkGO Board ไปเสียบไฟ จากที่อื่น
 *        หลัง BlynkGO Board ไวไฟ ได้เชื่อมต่อแล้ว 
 *        ที่ computer เปิด Browser เข้าไปที่
 *        http://esp32.local
 *      
 *      - ให้ login ด้วย admin , password ด้วย admin
 *      
 *      - นำไฟล์ .bin ของโปรเจคจากการ compile แล้ว 
 *        เลือกให้ upload แบบ OTA ไร้สาย
 *        ผ่านทางหน้า WebServer ได้เลย
 *   
 ******************************************************/

#include <BlynkGOv3.h>

GWiFiManager wifi_manager;
GLabel       label;

namespace WebServerOTA { void begin(); void handle(); }

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);
  label = "ทดสอบ WebServer OTA";
    
}

void loop() {
  BlynkGO.update();
  WebServerOTA::handle();
}

WIFI_CONNECTED(){
  WebServerOTA::begin();
}

