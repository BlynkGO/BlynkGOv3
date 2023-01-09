#include <BlynkGOv3.h>

GMask   Mask(40,200);   // ตัว mask
GRect   rect(Mask);   // rect ที่จะมีการถูก mask

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // สร้าง mask ด้วย เส้นตรง alpha 255  หนา 40 แบบปลายหมด
  Mask.line(255, Point(20,20), Point(20,180), 40, true );  
  Mask.dragable(true);      // ให้ mask มีการลากเลื่อนได้

  rect.color(TFT_BLUE);   // rect ที่จะถูก mask มีสีฟ้า
  rect.clickable(false);  // rect นี้ ไม่ให้ click ได้

}

void loop() {
  BlynkGO.update();
}

