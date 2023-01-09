#include <BlynkGOv3.h>

// ตัวแปรวิตเจ็ต GLabel
GLabel label = "ข้อความวิ่ง";

void setup(){
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  // กำหนดให้อักษรวิ่ง ในระยะความกว้าง เท่ากว้างหน้าจอ ด้วยความเร็ว 40 pixel ต่อวินาที
  label.rolling(true, GScreen.width(), 40);
  label.font(prasanmit_40,TFT_BLACK);

}

void loop(){
  BlynkGO.update();
}


