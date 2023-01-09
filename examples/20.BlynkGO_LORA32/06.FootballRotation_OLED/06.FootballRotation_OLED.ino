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

IMG_DECLARE(img_football);

GImage football(img_football);
GAnimation anim_football[2];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  football.create();

  // แอนิเมชั่น สำหรับหมุน รอบ 360องศาใน 3.5 วินาที
  anim_football[0].init( football, ANIM_ANGLE, 0, 360, 3500);
  anim_football[0].repeat(true);
  anim_football[0].create();

  // แอนิเมชั่น สำหรับ ย้ายตำแหน่งแกน x ใน 10 วินาที
  anim_football[1].init( football, ANIM_X, -football.width(), GScreen.width(), 10000);
  anim_football[1].repeat(true);
  anim_football[1].create();
}

void loop() {
  BlynkGO.update();
}
