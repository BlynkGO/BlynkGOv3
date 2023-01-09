#include <BlynkGOv3.h>

GLine line;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  Points points = { Point(10,50), Point(80,20), Point(300,120), Point(150,200), Point(100, 80) };
  
  line.setPoints( points );
  line.color(TFT_RED);
  line.thickness(10);
  line.rounded(true);     // ปลายมน หรือไม่

}

void loop() {
  BlynkGO.update();
}

