#include <BlynkGOv3.h>

GCircle   dial;             /* วงกลมหน้าปัด */
GTriangle triangle(dial);   /* เข็มสามเหลี่ยม */
GCircle   dot(dial);        /* วงกลมตรงกลางหน้าปัด */
Point     pivot;            /* จุดหมุน */
GTimer    timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  dial.radius(110);
  dial.color(TFT_WHITE);
  dial.border(10);
  dial.border_color(TFT_BLUE);

  pivot = Point(dial.radius(), dial.radius());  // จุดหมุนให้เป็นจุดศก.วงกลม

  // พิกัดจุดของ สามเหลี่ยม (จุดจะเทียบจากภายใน parent ของสามเหลี่ยม)
  Point p[3];
    p[0] = pivot+ Point( 0, -90 );  /*         /\ p[0]     */
    p[1] = p[0] + Point( 5, 120 );  /*        /  \         */
    p[2] = p[0] + Point(-5, 120 );  /*  p[2] /____\ p[1]   */

  triangle.setPoints(p);
  triangle.color(TFT_RED);

  // สำหรับหมุนสามเหลี่ยม 
  triangle.direction(CLOCKWISE); // ให้หมุนแบบตามเข็มนาฬิกา
  triangle.angle_offset(0);      // มุมเริ่มต้น ที่ 0 องศา

  dot.radius(15);
  dot.color(TFT_BLACK);

  // ตั้งเวลาทุกๆ 1วินาทีให้ ทำการหมุน triangle ด้วยองศาที่กำหนดรอบจุดหมุน pivot
  timer.setInterval(1000L, [](){
    static int16_t angle = 0;
    triangle.angle(angle, pivot);
    angle = (angle+6)%360;
  });
  
}

void loop() {
  BlynkGO.update();
}


