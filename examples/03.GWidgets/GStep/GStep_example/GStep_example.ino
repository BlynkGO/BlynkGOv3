#include <BlynkGOv3.h>

GStep step1(-4000,4000);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  step1 = 0;   // กำหนดค่า

  // กดปุ่ม + หรือ - (กดค้างจะเพิ่มความเร็วอัตโนมัติ)
  // เมื่อมีการเปลี่ยนแปลงค่า จะเรียกมายังที่นี่
  step1.onValueChanged([](GWidget* widget ) {
    GStep* pStep = (GStep*) widget;
    Serial.printf("Step : %d\n", (int) pStep->value());
  });
}

void loop() {
  BlynkGO.update();
}
