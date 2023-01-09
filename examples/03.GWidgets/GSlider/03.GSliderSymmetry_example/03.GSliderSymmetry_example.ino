/***************************************************************
 * 
 * ตัวอย่าง GSlider แบบ ปรับเลื่อนค่า (ค่าเดียว) แบบสมมาตร
 * เหมาะสำหรับการเลือกตั้งค่าแบบมีค่า เกินกว่า 0 และ น้อยกว่า 0
 * 
 ***************************************************************/

#include <BlynkGOv3.h>

GSlider slider(-200,200, SLIDER_TYPE_SYM);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  //slider.size(20,200);  // หากจะเปลี่ยนเป็นแนวตั้ง ให้กำหนดขนาด wxh แทน
  slider.color(TFT_RED);
  slider.onValueChanged([](GWidget* widget){
    Serial.println(slider.value());	// ดึงค่า มาแสดงบน Serial Monitor
  });

  slider = 140;  // กำหนดค่า หรือใช้ API  slider.value(140); ก็ได้
}

void loop() {
  BlynkGO.update();
}

