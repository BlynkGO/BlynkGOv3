#include <BlynkGOv3.h>

GSwitch sw;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  sw.align(ALIGN_CENTER);
  sw.color(TFT_RED);
  sw.ON();      // สั่งเปิด
//  sw.OFF();   // สั่งปิด

  // เมื่อมีการเปลี่ยนแปลงสถานะ
  sw.onValueChanged([](GWidget* widget){
    Serial.printf("Switch : %d\n", sw.state());
  });
  
}

void loop() {
  BlynkGO.update();
}


