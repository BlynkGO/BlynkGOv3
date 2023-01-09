#include <BlynkGOv3.h>

GLed    led(TFT_RED);           // สร้าง GLed สีแดง
GLabel  label(led);             // สร้าง GLabel สำหรับ led

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  led.size(80,80);              // กำหนดขนาด
  led.ON();                     // สถานะ led เป็น on

  label = "เปิด";                // ข้อความบน led เป็น "เปิด"

  led.onClicked([](GWidget *widget){
    led.toggle();
    label = (led.state())? "เปิด" : "ปิด";
  });

}

void loop() {
  BlynkGO.update();
}
