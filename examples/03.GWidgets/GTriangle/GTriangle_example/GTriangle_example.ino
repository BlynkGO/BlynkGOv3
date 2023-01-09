#include <BlynkGOv3.h>

GTriangle triangle;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  Point p[3]; 
    p[0] = Point(40, 120);
    p[1] = Point(300, 40);
    p[2] = Point(100, 220);

  triangle.setPoints(p);
  triangle.color(TFT_RED);


  // สำหรับหมุนสามเหลี่ยม 
  // triangle.direction(CLOCKWISE); // ให้หมุนแบบตามเข็มนาฬิกา
  // triangle.angle_offset(150);    // มุมเริ่มต้น ที่ 150 องศา นับจากแกน x+
  //
  // Point pivot = Point(RES_WIDTH/2, RES_HEIGHT/2);
  // triangle.angle(50, pivot);     // หมุนสามเหลี่ยมไป 50 องศา จากมุมเริ่มต้น angle_offset(..) รอบจุดหมุน (pivot)

}

void loop() {
  BlynkGO.update();
}