#include <BlynkGOv3.h>

GLevel level;
GLabel label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED,TFT_ORANGE);

//  level.range(0,100);   // กำหนด range
  level = 70;             // กำหนดค่าให้ level
  level.color(TFT_RED);
  level.onValueChanged([](GWidget* widget){
    Serial.printf("Level Value : %d\n",level.value());
    label = level.value();
    label.c_align(level, ALIGN_RIGHT, 50,0);
  });

  label = level.value();
  label.c_align(level, ALIGN_RIGHT, 50,0);

}


void loop() {
  BlynkGO.update();
}


