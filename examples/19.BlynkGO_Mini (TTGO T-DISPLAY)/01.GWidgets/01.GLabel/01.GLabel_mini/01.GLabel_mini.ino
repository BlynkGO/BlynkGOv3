#include <BlynkGOv3.h>

// ให้ประกาศ resource ฟอนต์ไทยขนาดเล็ก ก่อนนำไปใช้ใน วิตเจ็ตต่างๆ
// หมายเหตุ สำหรับ BlynkGO font อื่นๆ เข้าไปโหลดได้ที่ https://github.com/BlynkGO/BlynkGO_font
FONT_DECLARE(prasanmit_15);  

// ตัวแปรวิตเจ็ต GLabel
GLabel label;
GLabel label2;

void setup(){
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  label = "ต้อนรับสู่ BlynkGO";
  label.color(TFT_BLACK);                       // กำหนดสีข้อความ
  label.align(ALIGN_CENTER,0,0);                // ไว้กลางจอ

  label2 = "ข้อความจิ๋ว";
  label2.font(prasanmit_15, TFT_RED);           // กำหนด font พร้อมสี
  label2.align(label, ALIGN_TOP_RIGHT,-20,10);  // วาง label2 ไว้เหนือ label บนขวา และปรับเลื่อนซ้าย 20 ลงล่าง 10
}

void loop(){
  BlynkGO.update();
}

