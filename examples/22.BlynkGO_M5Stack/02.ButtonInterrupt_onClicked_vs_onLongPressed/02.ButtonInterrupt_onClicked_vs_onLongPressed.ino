/***********************************************
 * ตัวอย่าง ปุ่ม ควบคุมแบบ interrupt event
 *  วิธีใช้ onClicked(..) 
 *  ร่วมกับ onLongPressed(..)
 *
 *  เพื่อป้องกัน มาทำงาน onClicked(..)
 *  ให้กัน ด้วยการเช็ค BTN_XX.hasLongPressed()
 *
 ***********************************************/

#include <BlynkGOv3.h>

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // เมื่อปุ่มถูกกดจนปล่อย --> onClicked(..)
  BTN_A.onClicked([](ButtonISR* btn){
    // เช็คว่า ป้องกัน หากเคยมีการทำงาน LongPressed มาแล้วให้ return ออกไป
    if(BTN_A.hasLongPressed()) return; 

    Serial.println("[BTN_A] clicked");
  });

  // เมื่อปุ่มถูกกด แบบค้างนาน แต่ยังไม่ปล่อย --> onLongPressed(..)
  BTN_A.onLongPressed([](ButtonISR* btn){
    Serial.println("[BTN_A] long pressed");
  });

}

void loop() {
  BlynkGO.update();
}
