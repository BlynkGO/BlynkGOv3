/*****************************************************
 * ก่อน compile กำหนด ใน blynkgo_config.h ให้เรียบร้อย
 * และ ที่ ArduionIDE 
 *   - ให้เลือก BlynkGO Board V1.3
 *   - เลือก Partition Scheme ไปที่ BlynkGO 3MB
 *
 * ตัวอย่างนี้ เป็นการควบคุมปุ่ม BTN_A, BTN_B, BTN_C 
 * แบบ interrupt event
 *   
 *****************************************************/

#include <BlynkGOv3.h>

GLabel label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  label = "Hello M5Stack";


  // ปุ่มกด A, B, C ทำงานแบบ Interrupt Service Routine (ISR) หรือที่เรียกว่า Interrupt
  BTN_A.onClicked([](ButtonISR* btn){
    Serial.println("[BTN A] clicked");
  });
  
  BTN_B.onClicked([](ButtonISR* btn){
    Serial.println("[BTN B] clicked");
  });

  BTN_C.onClicked([](ButtonISR* btn){
    Serial.println("[BTN C] clicked");
  });

  // BTN_A.onPressed(...)           เริ่มกด
  // BTN_A.onPressing(...)          เมื่อกดค้าง
  // BTN_A.onReleased(...)          เมื่อปล่อยการกด
  // BTN_A.onLongPressed(...)       เมื่อกดค้างยาว
  // BTN_A.onLongPressedRepeat(..)  เมื่อมีการกดค้างยาวซ้ำ ๆ ๆ อีกครั้ง
  // BTN_A.onClicked(..)            เมื่อตั้งแต่กดจนถึงปล่อย จะมาทำงานที่ onClicked นี้
  // BTN_A.onLongClicked(..)        เมื่อตั้งแต่กดจนถึงปล่อย แบบค้างยาว จะมาทำงานที่ onLongClicked นี้ โดยไม่ทำงาน onClicked(..)

  // BTN_A.reset();                 หากจะล้าง ที่เคยกำหนดไปเดิมทั้งหมด

}

void loop() {
  BlynkGO.update();
}

