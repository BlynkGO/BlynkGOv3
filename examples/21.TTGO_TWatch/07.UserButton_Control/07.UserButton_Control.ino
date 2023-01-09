/*****************************************************************
 * 
 * ตัวอย่างนี้สำหรับ T-Watch รุ่น 2019 
 * สำหรับควบคุม ปุ่มกด User-Button (GPIO 36) 
 * 
 * หมายเหตุ
 *    T-Watch รุ่น 2020 ไม่มีปุ่มนี้
 * 
 *****************************************************************/

#include <BlynkGOv3.h>

GLabel label;

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  label = "Hello T-Watch";
  
  // กด clicked ที่ปุ่ม User-Button ของ T-Watch ให้ เปิดปิดหน้าจอ
  TWatch.BTN.onClicked([](Button2 &btn){
    Serial.println("User Button Clicked");
    (TWatch.display_isOFF())? TWatch.display_ON() : TWatch.display_OFF();    
  });

  // กด double-clicked ที่ปุ่ม User-Button ของ T-Watch ให้ restart
  TWatch.BTN.onDoubleClicked([](Button2 &btn){
    Serial.println("User Button Double-Clicked");
    ESP.restart();
  });
}

void loop(){
  BlynkGO.update();  
}

