/**********************************************************************
 *
 *  ก่อนใช้งาน
 *  เข้าไปที่ BlynkGOv3/src/config/blynkgo_config.h
 *    เปลี่ยนบอร์ดเป็น
 *       #define  BLYNKO_LORA32
 *
 *    และที่ ArduinoIDE เลือกบอร์ดเป็น BlynkGO Lora32
 *
 **********************************************************************/


/**********************************************************************
 *  ตัวอย่างนี้ เป็นการเล่น GIF file ที่วางบน SPIFFS
 *  ให้ติดตั้ง "ESP32 Sketch Data Upload" ซึ่งเป็น plugin ให้ ArduinoIDE ได้จาก
 *
 *     https://github.com/me-no-dev/arduino-esp32fs-plugin
 *     (อ่านวิธีติดตั้งได้จากในหน้านั้น)
 *
 *  หลังติดตั้ง ปิดแล้วเปิด ArduinoIDE จะพบว่ามีเมนู "ESP32 Sketch Data Upload" 
 *  เกิดขึืนมาให้ ที่เมนู Tools > ESP32 Sketch Data Upload
 *
 *  จากนั้น ให้ upload ไฟล์ GIF ซึ่งอยู่ในโฟลเดอร์ data ของโปรเจค (กด Ctrl+K จะเห็นโฟลเดอร์ data )
 *  เพื่อนำไฟล์ GIF ขึืนสู่พื้นที่ SPIFFS ของ ESP32 ผ่านทางเมนู Tools > ESP32 Sketch Data Upload
 *
 **********************************************************************/

#include <BlynkGOv3.h>

// BlynkGO Board v1.2
//GImage stickman("SD://stickman.gif");       // โหลด GIF บน SD

// BlynkGO Mini (TTGO T-Display 1.14")
// BlynkGO T-Watch2020
GImage stickman("SPIFFS://stickman.gif");     // โหลด GIF บน SPIFFS

void setup() {
  BlynkGO.begin();
  stickman.create();
}

void loop() {
  BlynkGO.update();
}
