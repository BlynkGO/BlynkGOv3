#include <BlynkGOv3.h>

GSwitch sw;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  sw.align(ALIGN_CENTER);
  sw.color(TFT_BLUE);
  sw.width(80);

  // ปรับแต่ง customize สไตล์ เอง  
  // ตัวอย่างแต่ง สไตล์เอง ให้ปุ่มทรงสี่เหลี่ยม แทน ปุ่มกลม
  sw.style[BG].corner_radius(5);        // style[BG] คือ สไตล์ ส่วนพื้นหลัง  - ปรับให้มีมุมโค้ง 5
  sw.style[BG].color(TFT_CYAN);         // สีของ style[BG] ซึ่งจะป็นสีขณะ sw สถานะ OFF
  sw.style[INDIC].corner_radius(5);     // style[INDIC] คือ สไตล์ ส่วนแสดงสี เวลาสถานะ ON  - ปรับให้มีมุมโค้ง 5
  sw.style[KNOB_ON].corner_radius(5);   // style[KNOB_ON] คือ สไตล์ ของ ปุ่ม เวลาสถานะ ON  - ปรับให้มีมุมโค้ง 5
  sw.style[KNOB_OFF].corner_radius(5);  // style[KNOB_OFF] คือ สไตล์ ของ ปุ่ม เวลาสถานะ ON - ปรับให้มีมุมโค้ง 5

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
