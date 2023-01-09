#include <BlynkGOv3.h>

GSliderV slider_v;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  slider_v.size(80,240);
  slider_v.range(0,100);
  slider_v.title("อุณหภูมิ");
  slider_v.unit("°C");

  slider_v = 38;
}

void loop() {
  BlynkGO.update();
}

