/*
 * ตัวอย่าง สร้างปุ่มกดแบบ วงกลม ด้วยสัญลักษณ์ อย่างรวดเร็ว
 * ดู รายการ สัญลักษณ์ ได้ที่ https://github.com/BlynkGO/BlynkGO_font ในโซนด้านล่างๆ
 */
 
#include <BlynkGOv3.h>

GButton btn;

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  btn = SYMBOL_SETTINGS;
  btn.round_design();               // ออกแบบสำเร็จรูปให้เป็น ปุ่มวงกลม
  btn.align(ALIGN_TOP_RIGHT,-5,5);  // วางไว้ด้านขวาบน ขยับเข้ามาทางซ้าย 5 pixel ลงมา 5 pixel
  btn.onClicked([](GWidget*widget){
    Serial.println("Button Clicked");    
  });
}

void loop(){
  BlynkGO.update();  
}
