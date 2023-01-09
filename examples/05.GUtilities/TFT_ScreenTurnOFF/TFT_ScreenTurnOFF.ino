/*****************************************************
 * ตัวอย่างนี้ ใช้สำหรับ BlynkGO Emma-II
 *        และ BlynkGO Enterprise เท่านั้น
 * -------------------------------------------------
 * ตัวอย่าง สำหรับตั้งเวลา เพื่อดับไฟหน้าจอ Screen 
 *  - หาก ไม่มีการสัมผัสที่หน้าจอ เกิน 30 วินาที หน้าจอจะดับลงไป 
 *  - และเมื่อมีการสัมผัสหน้าจอ หน้าจอจะติดกลับมา
 *
 ****************************************************/

#include <BlynkGOv3.h>

namespace ScreenTurnOFF { void set(uint32_t timeout_sec); }

GButton btn;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  ScreenTurnOFF::set(30);       // กำหนดเวลา 30 วินาที ที่จอจะดับ หากไม่มีการสัมผัส

  // ออกแบบกราฟิก ตามต้องการ
  btn.onClicked([](GWidget*widget){
    Serial.println("Clicked");  // เมื่อหน้าจอดับ ปุ่มนี้จะกดไม่ติดไปโดยอัตโนมัติ
  });
}

void loop() {
  BlynkGO.update();
}


