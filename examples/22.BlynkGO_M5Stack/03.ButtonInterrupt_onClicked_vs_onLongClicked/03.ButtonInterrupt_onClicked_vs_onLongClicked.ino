/***********************************************
 * ตัวอย่าง ปุ่ม ควบคุมแบบ interrupt event
 *  ความแตกต่างระหว่าง onClicked(..) 
 *  และ  onLongClicked(..)
 ***********************************************/

#include <BlynkGOv3.h>

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();


  // เมื่อปุ่มถูกกดจนปล่อย --> onClicked(..)
  BTN_A.onClicked([](ButtonISR* btn){
    Serial.println("[BTN_A] clicked");
  });
  
  // เมื่อปุ่มถูกกด แบบค้างนาน จนปล่อย  --> onLongClicked(..) 
  // เมื่อทำงาน onLongClicked(..) แล้วจะไม่ทำงาน onClicked(..)
  BTN_A.onLongClicked([](ButtonISR* btn){
    Serial.println("[BTN_A] long clicked");
  });
  
}

void loop() {
  BlynkGO.update();
}

