#include <BlynkGOv3.h>

GLabel label;
GSwitch3D sw3d;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  sw3d.color(TFT_BLUE);
  label = "Hello T-Watch";
  label.align(sw3d, ALIGN_BOTTOM);
  
}

void loop() {
  BlynkGO.update();
}


