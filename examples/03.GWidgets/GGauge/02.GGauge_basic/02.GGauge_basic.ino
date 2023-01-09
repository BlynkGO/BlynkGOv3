#include <BlynkGOv3.h>

GGauge gauge;

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  gauge.title("อุณหภูมิ");
  gauge.color(TFT_RED);                           // กำหนดสี
  gauge.decimal(2);                               // ให้แสดงทศนิยมกี่ตำแหน่ง (0, 1,2 3)
  gauge.thickness(10);                            // กำหนดความหนาของ gauge
  gauge.range(10,200);                            // กำหนด range ให้ gauge
  gauge.range_font(prasanmit_25, TFT_ORANGE);     // กำหนด font และ สีให้ range
//  gauge.range_color(TFT_ORANGE);                // ปรับสี range
//  gauge.range_hidden(true);                     // หากต้องการให้ range ซ่อนหรือไม่

  gauge = 123.54;
}

void loop(){
  BlynkGO.update();  
}

