#include <BlynkGOv3.h>

GSliderH slider_h;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  slider_h.size(240,60);
  slider_h.range(0,100);
  slider_h.title("อุณหภูมิ");
  slider_h.unit("°C");

  slider_h = 38;
}

void loop() {
  BlynkGO.update();
}

