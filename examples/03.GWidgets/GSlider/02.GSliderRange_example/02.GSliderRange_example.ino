/***************************************************************
 * 
 * ตัวอย่าง GSlider แบบ range โดยเลื่อนเปลี่ยน ค่าบน ค่าล่าง ของ range ได้
 * 
 ***************************************************************/

#include <BlynkGOv3.h>

GSlider slider(0,200, SLIDER_TYPE_RANGE);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

//  slider.size(20,200);      // หากจะเปลี่ยนเป็นแนวตั้ง ให้กำหนดขนาด w < h
  slider.color(TFT_RED);
  slider.onValueChanged([](GWidget* widget){
    Serial.printf("Slider Range : %d - %d\n", slider.lower_value(), slider.upper_value());
  });

  slider.upper_value(150);    // กำหนดค่าบน เริ่มต้นด้วย 150
  slider.lower_value(40);     // กำหนดค่าล่าง เริ่มต้นด้วย 40
}

void loop() {
  BlynkGO.update();
}

