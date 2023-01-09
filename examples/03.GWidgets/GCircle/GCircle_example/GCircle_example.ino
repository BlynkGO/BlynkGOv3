#include <BlynkGOv3.h>

GCircle circle;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  circle.radius(80);
  circle.color(TFT_RED);

  circle.border(10);
  circle.border_color(TFT_BLUE);  
}
  
void loop() {
  BlynkGO.update();
}

