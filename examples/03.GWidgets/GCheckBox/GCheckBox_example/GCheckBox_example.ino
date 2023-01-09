#include <BlynkGOv3.h>

GCheckBox cb;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);
  
  cb = "นี่คือ เช็คบ็อกซ์";        // กำหนดข้อความของ checkbox
  cb.position(20,100);        // กำหนดตำแหน่ง
  cb.color(TFT_BLUE);         // กำหนดสี
  cb.font(prasanmit_30);      // กำหนด font
  cb.checked(true);           // กำหนดสถานะให้ checkbox
  cb.onValueChanged([](GWidget* widget){ // หากมีการเปลียนแปลงสถานะ
     Serial.print("checkbox status : ");
     Serial.println( (cb.checked())? "Checked" : "Unchecked"); 
  });
}

void loop() {
  BlynkGO.update();
}

