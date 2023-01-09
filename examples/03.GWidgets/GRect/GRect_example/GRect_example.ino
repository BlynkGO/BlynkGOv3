/***********************************************
 * ตัวอย่างสร้าง วิตเจ็ตสี่เหลี่ยม GRect และ สี่เหลี่ยมแบบมุมโค้ง
 *   มีการดักสัญญาณ clicked ที่ สี่เหลี่ยม
 *   ให้ทดสอบกด clicked ที่พื้นที่ สี่เหลี่ยม
 *   วิตเจ็ตข้อความ GLabel จะย้ายไปเป็น layer ลูก
 *   ของวิตเจ็ตสี่เหลี่ยมที่ clicked เลือกนั้น
 *   
 ***********************************************/

#include <BlynkGOv3.h>

GRect rect;
GRect round_rect;

GLabel label = "Clicked";

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  rect.size(150,80);
  rect.color(TFT_BLUE);
  rect.opa(180);                // ปรับค่าโปร่งใส
  rect.border(10, TFT_WHITE);
  rect.align(ALIGN_CENTER, 0 , -rect.height()/2 - 20);
  rect.onClicked(myClicked);

  round_rect.size(rect);        // ให้มีขนาดเท่ากับ rect
  round_rect.color(TFT_RED);
  round_rect.corner_radius(20); // รัศมีโค้งตามมุมทั้ง 4
  round_rect.align( rect, ALIGN_BOTTOM, 0, 40);
  round_rect.onClicked(myClicked);
}

void loop(){
  BlynkGO.update();
}

void myClicked(GWidget*widget){
  label.parent(widget);
}

