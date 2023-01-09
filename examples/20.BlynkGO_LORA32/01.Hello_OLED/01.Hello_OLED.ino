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

GLabel label = "สวัสดีครับ";

void setup(){
  BlynkGO.begin();
  label.font(prasanmit_25);
}

void loop(){
  BlynkGO.update();
}


