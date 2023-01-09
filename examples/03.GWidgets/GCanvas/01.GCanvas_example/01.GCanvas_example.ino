/* 
 *  Canvas แปลว่า ผืนผ้าที่ใช้ในการวาดรูปทางศิลปะ
 *  GCanvas เป็น วิตเจตสำหรับการวาดรูปไปบนพื้นวิตเจต 
 *  แบบ ไม่มีการเปลี่ยนแปลงภายหลัง 
 *  เปรียบเสมือนเป็นผืนผ้าวาดรูปเพื่อใช้ร่วมกับ วิตเจ็ตอื่นๆ
 */

#include <BlynkGOv3.h>

GCanvas canvas;  // จะมีขนาดเท่า GScreen เลย
// GCanvas canvas(480,320);  // กำหนดขนาดเอง


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  canvas.fillScreen(TFT_WHITE);
  canvas.drawRect(Point(10,20), 100,68, TFT_BLUE);
  
}

void loop() {
  BlynkGO.update();
}

