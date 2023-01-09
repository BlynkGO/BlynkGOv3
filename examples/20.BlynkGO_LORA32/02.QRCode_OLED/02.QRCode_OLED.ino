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


#include <BlynkGOv3.h>

GQRCode qrcode;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  qrcode.size(64,64);
  qrcode.version(6);
  qrcode.encode("สวัสดี BlynkGO");

}

void loop() {
  BlynkGO.update();
}



