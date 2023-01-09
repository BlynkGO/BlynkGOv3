/******************************************************
 * ค่า BlynkGO Serial Number (BlynkGO SN.) 
 * ที่ปรากฏบนหน้าจอ TFT หรือทาง serial monitor 
 * คัดลอก และ ชำระแจ้งลงทะเบียนไปยัง blynkgo@gmail.com
 * หรือทาง https://www.facebook.com/blynkgo.os.1
 * เพื่อรับค่า BLYNKGO_KEY
 *
 * สำหรับจอสัมผัสให้ upload Sample 02.TouchCalibrate.ino
 * เพื่อทำการ calibrate ระบบสัมผัสหน้าจอ
 *
 ******************************************************/

#include <BlynkGOv3.h>

GLabel label;                                 // ประกาศตัวแปรวิตเจ็ตข้อความ

void setup() {
  Serial.begin(115200); Serial.println();     // เริ่มใช้งาน Serial Monitor
  BlynkGO.begin();                            // เริ่มใช้งาน BlynkGO
  BlynkGO.fillScreen(TFT_BLACK);              // เติมสีหน้าจอเป็นสี ดำ
  BlynkGO.rotation(ROTATION_DEFAULT);         // ปรับจอให้แนวนอน  , แนวตั้ง ROTATION_VERTICAL
  
  label = "ต้อนรับสู่ BlynkGO (" + BlynkGO.version() + ")";   // กำหนดข้อความ (ใช้เหมือนตัวแปร String ได้เลย)
  //label.center(160,120);                    // กำหนดศูนย์กลางของข้อความ ที่ตำแหน่ง พิคเซล 160,120
}

void loop() {
  BlynkGO.update();                           // ให้ BlynkGO ทำการ update สถานะต่างๆ
}
