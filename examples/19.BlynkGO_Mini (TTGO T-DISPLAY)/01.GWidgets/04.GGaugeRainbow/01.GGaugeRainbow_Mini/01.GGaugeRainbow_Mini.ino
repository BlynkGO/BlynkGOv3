#include <BlynkGOv3.h>

GGaugeRainbow gauge[2];       // สร้าง GGaugeRainbow 2 ชุด
GLabel        lb_unit[2];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  gauge[0].size(100,100);
  gauge[0].color(TFT_PINK, TFT_VIOLET);
  gauge[0].range(0,100);
  gauge[0].decimal(1);
  gauge[0].align(ALIGN_LEFT, 10);  
  gauge[0] = 94.6;

    lb_unit[0] = SYMBOL_DEGREE "C";
    lb_unit[0].font(prasanmit_20);
    // วางไว้ใต้ ข้อความที่แสดงค่าของวิตเจ็ต GGaugeRainbow โดยเลื่อนขึืนไป 15pixel
    lb_unit[0].align( gauge[0].label(), ALIGN_BOTTOM,0,-15);     

  gauge[1].size( gauge[0] );
  gauge[1].gauge_type(GAUGE_TYPE_FULL);
  gauge[1].align(ALIGN_RIGHT,-10);
  gauge[1] = 98;

    lb_unit[1] = "%";
    lb_unit[1].font(prasanmit_20);
    // วางไว้ใต้ ข้อความที่แสดงค่าของวิตเจ็ต GGaugeRainbow โดยเลื่อนขึืนไป 15pixel
    lb_unit[1].align( gauge[1].label(), ALIGN_BOTTOM,0,-15);    

}

void loop() {
  BlynkGO.update();
}





