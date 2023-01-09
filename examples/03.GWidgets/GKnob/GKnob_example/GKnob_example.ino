#include <BlynkGOv3.h>

GKnob   knob;
GLabel  knob_label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  knob.color(TFT_RED);     /* กำหนดสี*/

  /* หากมีการเปลี่ยนแปลงค่าของ knob */
  knob.onValueChanged([](GWidget* widget){
    GKnob* pKnob = (GKnob*) widget;
    knob_label = StringX::printf("ปุ่มหมุน : %3d", pKnob->value()); // นำค่ามาแสดงที่ label ด้วย
  });

  knob_label = StringX::printf("ปุ่มหมุน : %3d", knob.value());
    knob_label.font(prasanmit_40);
    knob_label.position(30, 0);
  
}

void loop() {
  BlynkGO.update();
}

