#include <BlynkGOv3.h>

GCircle dot_center(5, TFT_WHITE);
GCircle dot0(5, TFT_GREEN);
GCircle dot1(5, TFT_BLUE);
GCircle dot2(5, TFT_RED);
GCircle dot3(5, TFT_YELLOW);

GLabel label  = "center";
GLabel label0 = "dot0";
GLabel label1 = "dot1";
GLabel label2 = "dot2";
GLabel label3 = "dot3";

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // จุดศูนย์กลาง จอ TFT
  Point c = GScreen.center();                   

  // หมุนแขน 80 pixels ไป 45 องศา รอบจุด c แบบตามเข็ม
  Point p0 = Point(80, 45, c, CLOCKWISE);

  // หมุนแขน 80 pixels ไป 120 องศา จากมุม offset โดยมี มุม offset เริ่มที่ 90 องศา
  // หมุนรอบจุด c แบบทวนเข็มนาฬิกา
  Point p1 = Point(80, 120, 90, c, ANTICLOCKWISE); 

  // หมุนจุด p1 ไป 60 องศา รอบจุด c แบบทวนเข็มนาฬิกา
  Point p2 = p1.rotate( 60, c, ANTICLOCKWISE);

  // หมุนจุด p2 ไป 210 องศา รอบจุด c แบบตามเข็มนาฬิกา
  Point p3 = p2.rotate( 210, c, CLOCKWISE);

  dot_center.center(c);
  dot0.center(p0);
  dot1.center(p1);
  dot2.center(p2);
  dot3.center(p3);

  // วางข้อความไว้ใต้ dot ต่างๆ
  label .align(dot_center, ALIGN_BOTTOM,0,-5);
  label0.align(dot0, ALIGN_BOTTOM,0,-5);
  label1.align(dot1, ALIGN_BOTTOM,0,-5);
  label2.align(dot2, ALIGN_BOTTOM,0,-5);
  label3.align(dot3, ALIGN_BOTTOM,0,-5);

  //--------------------------------
  Serial.printf( "p0 angle (clockwise) from c      : %d\n", (int) p0.angle_deg(c, CLOCKWISE) );
  Serial.printf( "p0 angle (anti clockwise) from c : %d\n", (int) p0.angle_deg(c, ANTICLOCKWISE) );
  Serial.printf( "p0 distance from c               : %f\n",       p0.distance(c) );

}

void loop() {
  BlynkGO.update();
}


