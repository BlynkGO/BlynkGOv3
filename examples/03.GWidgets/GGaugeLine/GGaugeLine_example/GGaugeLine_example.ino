#include <BlynkGOv3.h>

GGaugeLine gauge;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

//  gauge.size(240,240);                    // กำหนดขนาด
//  gauge.range(0,100);                     // กำหนด ระยะค่าของ gauge
//  gauge.scale(270, 20+1, 10+1);           // กำหนดขีดบอกระดับบน gauge ให้กินพื้นที่มุม 270 องศา โดยมีขีดเส้นบอกระดับ 20+1 ค่า (รวมค่าเริ่มต้น) มีข้อความบอกระดับ 10+1 ค่า (รวมค่าเริ่มต้น)
  gauge.scale_color(TFT_DARKGRAY, TFT_RED); // กำหนดสีของเส้นบอกระดับ
  gauge.scale_width(5);                     // ความหนาของเส้นบอกระดับ
//  gauge.scale_length(10);                 // ความยาวของเส้นบอกระดับ
//  gauge.scale_padding(8);                 // ช่องไฟระหว่างเส้นบอกระดับ กับ ข้อความบอกระดับ
//  gauge.font(prasanmit_40,TFT_WHITE);     // กำหนดฟอนต์และสีอักษรของ ค่าบอกระดับ
//  gauge.critical_value( 60 );             // ค่าเริ่มต้นโซนที่เป็นค่าอันตราย
//  gauge.critical_color( TFT_RED);         // สีโซนค่าอันตราย
  gauge.circle_middle_color(TFT_GREEN);     // สี วงกลมหมุดเข็ม ตรงกลาง
  gauge.circle_middle_radius(50);           // รัศมี วงกลมหมุดเข็ม ตรงกลาง
  gauge.needle_color( TFT_BLUE );           // กำหนดสีเข็ม

  gauge = 35;                               // กำหนดค่าให้เข็ม
  Serial.println( gauge.value() );          // แสดงค่าปัจจุบันของ gauge ออก Serial Monitor
}

void loop() {
  BlynkGO.update();
}
