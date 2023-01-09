#include <BlynkGOv3.h>

GLabel label("สวัสดีครับ");

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  label.editable(true);     // กดคลิก จะสามารถแก้ไขข้อความได้
  label.rolling(true, BlynkGO.width()-10); // ทำให้เป็นข้อความวิ่ง และ กำหนดความกว้าง
  label.body_draw(true);    // ให้แสดง body พื้นหลังได้ด้วย
  label.body_color(TFT_RED, TFT_ORANGE);  // กำหนดสี body พื้นหลัง
  
}

void loop() {
  BlynkGO.update();
}
