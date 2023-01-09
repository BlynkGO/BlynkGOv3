#include <BlynkGOv3.h>

GArc arc(80,25,TFT_BLUE);             // กำหนด รัศมี, ความหนา, สี
GTimer timer_arc;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  arc.direction(ARC_CLOCKWISE);       // มุม แบบตามเข็ม  ARC_CLOCKWISE : ตามเข็ม หรือ ARC_ANTICLOCKWISE : ทวนเข็ม
//  arc.rounded(true);                // ให้ปลายมน
//  arc.radius(80);                   // กำหนดรัศมี
//  arc.thickness(25);                // กำหนดความหนา
//  arc.color(TFT_BLUE);              // กำหนดสี
  arc.angle_offset(150);              // กำหนดมุม offset จากแกน x + 

  timer_arc.setInterval(100,[](){
    arc.angle( 0, random(240));  // กำหนดมุม มุมตั้งต้น และ มุมสิ้นสุด จาก มุม offset
  });
}

void loop() {
  BlynkGO.update();
  
}

