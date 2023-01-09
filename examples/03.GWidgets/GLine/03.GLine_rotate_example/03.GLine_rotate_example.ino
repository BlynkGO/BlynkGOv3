#include <BlynkGOv3.h>

GLabel label[12];   // ทำตัวเลขบนหน้าปัดนาฬิกา
GLine  line;        // ทำเข็มนาฬิกา
Point  c;           // จุดศูนย์กลาง
GTimer timer;       // ตัวตั้งเวลา

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLACK);

  c = GScreen.center();   // จุดศูนย์กลางจอ TFT
  for(int i=0; i < 12; i++){
    //จุด p สร้างจาก แขน 90pixels หมุนไปมุม i*30 โดยเริ่มต้นที่มุม 270 รอบจุด c นับมุมแบบตามเข็ม
    Point p = Point(90, i*30, 270, c, CLOCKWISE);  
    label[i].center(p);               // กำหนดตำแหน่ง
    label[i] = String((i>0)? i : 12); // ใส่ข้อความให้ วิตเจ็ตข้อความ
  }

  line.color(TFT_RED);
  line.thickness(20);
  line.rounded(true);     // ปลายมน หรือไม่

  timer.setInterval(1000L,[](){
    static uint8_t sec = 0;
    // จากเส้น ด้วยรัศมีแขน 80 pixels ด้วยมุม sec*6 โดยมีมุมเริ่มต้นที่ 270 
    // มีจุดหมุนคือ GScreen.center() จุดกลางจอ และหมุนแบบตามเข็มนาฬิกา
    line.setPoints(80, sec*6, 270, c, CLOCKWISE);
    sec = (sec+1) %60;
  });
}

void loop() {
  BlynkGO.update();
}



