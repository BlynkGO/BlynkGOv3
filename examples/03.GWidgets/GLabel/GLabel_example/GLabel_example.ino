#include <BlynkGOv3.h>

GLabel label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  label = "สวัสดีครับ";

  // font : prasanmit_20, prasanmit_25, prasanmit_30,prasanmit_35, prasanmit_40  สามารถเลือกเปลี่ยนใช้ได้เลย
  // download ฟอนต์เพิ่มเติมได้ที่ https://github.com/BlynkGO/BlynkGO_font
  label.font(prasanmit_40);
  label.color(TFT_BLUE);
}

void loop() {
  BlynkGO.update();
}

