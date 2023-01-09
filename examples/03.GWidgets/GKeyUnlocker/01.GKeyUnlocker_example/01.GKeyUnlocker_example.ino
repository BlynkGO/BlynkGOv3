/*********************************************************
 * 
 * วิตเจ็ต GKeyUnlocker 
 * เป็น วิตเจ็ตสำหรับ ล็อคหน้าจอ ด้วยการลากนิ้วผ่านจุด 4 จุดขึ้นไปจาก 9 จุด
 *
 *       *  *  *       1  2  3
 *       *  *  *  -->  4  5  6
 *       *  *  *       7  8  9
 *
 * ปกติให้ใช้เรียกขึ้นมาทำงานผ่านวิตเจ็ตอื่นๆ เพื่อ lock หน้าจอป้องกัน
 * ภายหลังเมื่อปลดล็อคสำเร็จจะถูกซ่อนลงไป
 * 
 *********************************************************/

#include <BlynkGOv3.h>

GKeyUnlocker keyunlocker;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // clear รหัส สู่รหัสเดิม ตั้งต้น (รูปตัว M)
  // keyunlocker.clear();

  // แสดงค่ารหัสปลดล็อคปัจจุบัน
  // Serial.println(keyunlocker.secretkey());

  // กำหนดโหมดอย่างใดอย่างหนึ่ง
  keyunlocker.mode(GKEYUNLOCKER_MODE_LOCK);       // โหมดล็อคหน้าจอ
  // keyunlocker.mode(GKEYUNLOCKER_MODE_SETTING); // โหมดตั้งค่ารหัสใหม่  รหัสเดิมเริ่มต้น ลากรูปตัว M

  // หลังจากใส่รหัสผ่านแล้วจะให้ไปทำอะไร
  keyunlocker.onApply([](GWidget* w){
    Serial.println("ใส่รหัสถูกต้อง ผ่าน!!");
  });

  // หลังจากใส่รหัสไม่ผ่านครบ 3 ครั้งแล้ว จะให้ไปทำอะไร
  keyunlocker.onCancel([](GWidget* w){
    Serial.println("ใส่รหัสไม่ถูกต้อง 3 ครั้ง");
  });
}

void loop() {
  BlynkGO.update();
}


