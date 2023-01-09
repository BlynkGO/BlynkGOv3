#include <BlynkGOv3.h>

GLabel label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  BTN1.gpio(15);    // คำสั่งเปลี่ยนขา GPIO เป็นขาอื่นเอง (ปกติ GPIO 35)
  BTN2.gpio(17);    // คำสั่งเปลี่ยนขา GPIO เป็นขาอื่นเอง (ปกติ GPIO 0)


  BTN1.onPressed([](ButtonISR* btn){
    Serial.println("[BTN1] Pressed");
    label = "[BTN1] Pressed";
  });

  BTN1.onReleased([](ButtonISR* btn){
    Serial.println("[BTN1] Released");
    label = "[BTN1] Released";
  });

  BTN2.onLongPressed([](Button2 &btn){
    Serial.println("[BTN2] Long Pressed");    
    label = "[BTN2] Long Pressed";
  });

//  BTN1.reset();     // คำสั่งสำหรับ ล้าง funtion ในการควบคุมการกดต่างๆ ที่เคยกำหนด
//  BTN2.reset();

}

void loop() {
  BlynkGO.update();
}

