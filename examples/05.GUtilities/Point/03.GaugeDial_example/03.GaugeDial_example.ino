/*
 * ตัวอย่างวาดแป้นนาฬิกา ที่มีตัวเลข 1-12 
 */

#include <BlynkGOv3.h>

float     gauge_angle = 225;    // พื้นที่มุมทั้งหมดของหน้าปัด 
const int gauge_scale = 10;     // แบ่งออกเป็น 10 ส่วน scale เท่าๆกัน

GLabel label[gauge_scale + 1];
GLine  line[gauge_scale + 1];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  Point c = GScreen.center();   // จุดศูนย์กลางจอ TFT
  for(int i=0; i < gauge_scale + 1; i++){
    Point p1 = Point( 100, (gauge_angle/gauge_scale)*i , 90 + (360- gauge_angle)/2, c, CLOCKWISE);
    Point p2 = Point( 90, (gauge_angle/gauge_scale)*i , 90 + (360- gauge_angle)/2, c, CLOCKWISE);
    Point p3 = Point( 70, (gauge_angle/gauge_scale)*i , 90 + (360- gauge_angle)/2, c, CLOCKWISE);

    line[i].setPoints(p1,p2); line[i].thickness(5);  line[i].color(TFT_BLACK);
    label[i] = String(i);     label[i].center(p3);   label[i].color(TFT_BLACK);
  }
}

void loop() {
  BlynkGO.update();
}
