/*
 * ตัวอย่างวาดแป้นนาฬิกา ที่มีตัวเลข 1-12 
 */

#include <BlynkGOv3.h>

GLabel label[12];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  Point c = GScreen.center();   // จุดศูนย์กลางจอ TFT
  for(int i=0; i < 12; i++){
    //จุด p สร้างจาก แขน 90pixels หมุนไปมุม i*30 โดยเริ่มต้นที่มุม 270 รอบจุด c นับมุมแบบตามเข็ม
    Point p = Point(90, i*30, 270, c, CLOCKWISE);  
    label[i].center(p);               // กำหนดตำแหน่ง
    label[i] = String((i>0)? i : 12); // ใส่ข้อความให้ วิตเจ็ตข้อความ
  }
}

void loop() {
  BlynkGO.update();
}

