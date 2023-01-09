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
//  gauge.critical_value( 60 );             // ค่าเริ่มต้นโซนที่เป็นค่าอันตราย
//  gauge.critical_color( TFT_RED);         // สีโซนค่าอันตราย

  color_t needle_color[] = { TFT_BLUE, TFT_ORANGE, TFT_VIOLET };   // สีของเข็ม
  gauge.needle_color( needle_color, 3 );                           // ให้มีเข็ม 3 เข็ม ตามสีที่กำหนด

  gauge.needle[0] = 10;                        // กำหนดค่าให้เข็มที่ 0 ด้วย 10
  gauge.needle[1] = 20;                        // กำหนดค่าให้เข็มที่ 1 ด้วย 20
  gauge.needle[2] = 30;                        // กำหนดค่าให้เข็มที่ 2 ด้วย 30
  
  // แสดงค่าออก Serial Monitor
  Serial.println( gauge.needle[0] );
  Serial.println( gauge.needle[1] );
  Serial.println( gauge.needle[2] );
}

void loop() {
  BlynkGO.update();
}


