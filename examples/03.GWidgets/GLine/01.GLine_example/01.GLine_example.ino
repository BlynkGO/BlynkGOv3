#include <BlynkGOv3.h>

GLine line;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  Point p1 = Point(200,20);
  Point p2 = Point(40,180);
  line.setPoints(p1,p2);
  line.color(TFT_RED);
  line.thickness(10);
//  line.rounded(true); // ปลายมน หรือไม่

}

void loop() {
  BlynkGO.update();
}


