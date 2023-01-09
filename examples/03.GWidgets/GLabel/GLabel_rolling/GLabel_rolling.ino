#include <BlynkGOv3.h>

GLabel label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  label = "ข้อความวิ่ง....";
  
  // กำหนดให้อักษรวิ่ง ในระยะความกว้าง 200 pixel ด้วยความเร็ว 40 pixel ต่อวินาที
  label.rolling(true, 200, 40);  
  label.font(prasanmit_40);               // กำหนด font
  label.color(TFT_BLACK);                 // กำหนดสีของ label
  label.body_draw(true);                  // ให้วาดพื้นหลังข้อความด้วย
  label.body_color(TFT_RED, TFT_ORANGE);  // กำหนดสีพื้นหลังข้อความ
}

void loop() {
  BlynkGO.update();
}

