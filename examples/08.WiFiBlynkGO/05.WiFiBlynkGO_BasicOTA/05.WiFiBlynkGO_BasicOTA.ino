/*******************************************************
 * ตัวอย่างพื้นฐาน การ Upload โคด แบบ OTA ไร้สาย ผ่านทาง WiFi
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
 *      การ Upload จะเป็นการ Upload แบบ OTA ไร้สาย
 *      
 *      - ให้ นำ BlynkGO Board ไปเสียบไฟ จากที่อื่น
 *        หลัง BlynkGO Board ไวไฟ ได้เชื่อมต่อแล้ว 
 *        กดเข้าไปที่ GWiFiManager ที่หน้าจอ TFT
 *        เพื่อเช็คค่า IP Address
 *      
 *      - ก่อน Upload ที่ ArduinoIDE ให้เปลี่ยน Port ไปที่ Network Port 
 *        ด้วย IP ของ BlynkGO Board นั้นๆแทน
 *        ก็จะสามารถ Upload โดยไม่ต้องใช้สาย USB
 *   
 ******************************************************/

#include <BlynkGOv3.h>

GWiFiManager wifi_manager;
GLabel       label;

namespace BasicOTA { void begin(); void handle(); }

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // ออกแบบ กราฟิกตามต้องการ
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);
  label = "ทดสอบ Upload ทางสาย USB";  // รอบแรกให้ใช้บรรทัดนี้ และเลือก Port แบบ Serail ports ที่เสียบ
  //label = "ทดสอบ Upload ทาง OTA";   // รอบถัดๆไปให้สลับมาใช้บรรทัดนี้ แล้ว Upload แบบ OTA ด้วยการเลือก Port แบบ Network ports


}

void loop() {
  BlynkGO.update();
  BasicOTA::handle();
}

WIFI_CONNECTED(){
  BasicOTA::begin();
}
