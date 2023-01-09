#include <BlynkGOv3.h>

GStep step1(-100,100);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  step1 = 0;          // กำหนดค่า
  step1.decimal(1);   // ให้แสดงทศนิยมกี่ตำแหน่ง  0,1,2,3  (default =0)
  step1.step(0.1);    // ปรับค่า step สำหรับเปลี่ยนค่าทีละ ที่ต้องการ เช่น 1, 0.1, 0.01, 0.001 เป็นต้น

  // กดปุ่ม + หรือ - (กดค้างจะเพิ่มความเร็วอัตโนมัติ)
  // เมื่อมีการเปลี่ยนแปลงค่า จะเรียกมายังที่นี่
  step1.onValueChanged([](GWidget* widget ) {
    GStep* pStep = (GStep*) widget;
    Serial.printf("Step : %.1f\n", pStep->value()); // แสดงค่า ออก Serial Monitor ด้วยทศนิยม 1 ตำแหน่ง
  });
}

void loop() {
  BlynkGO.update();
}
