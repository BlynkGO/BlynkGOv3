/***************************************************************
 * 
 * ตัวอย่าง GSlider แบบ ปรับเลื่อนค่า (ค่าเดียว) เพื่อให้ตั้งค่าอื่นๆต่อไป
 * 
 ***************************************************************/

#include <BlynkGOv3.h>

GSlider slider;
//GSlider slider(0,200);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

//  slider.size(20,200);  // หากจะเปลี่ยนเป็นแนวตั้ง ให้กำหนดขนาด wxh แทน
  slider.range(0,200);
  slider.color(TFT_RED);
  slider.onValueChanged([](GWidget* widget){
    Serial.println(slider.value());	// ดึงค่า มาแสดงบน Serial Monitor
  });

  slider = 140;  // กำหนดค่า หรือใช้ API  slider.value(140); ก็ได้
}

void loop() {
  BlynkGO.update();
}

