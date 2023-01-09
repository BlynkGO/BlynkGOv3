#include <BlynkGOv3.h>

GSwitch3D Switch3D(TFT_BLUE);             // สร้าง GSwitch3D พร้อมกำหนดสี
GLabel    label = "ห้องนั่งเล่น";

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  Switch3D.size(80,120);
  label.align(Switch3D, ALIGN_BOTTOM);
  
  // เมื่อ user กดเปลี่ยนแปลงเข้ามาที่ Switch3D
  Switch3D.onValueChanged([](GWidget* widget){
    if( Switch3D.isON()){     // หากสถานะเป็น ON
      Serial.println("Switch3D : Turn ON"); 
    }else{
      Serial.println("Switch3D : Turn OFF"); 
    }
  });
}
  
void loop() {
  BlynkGO.update();
}
