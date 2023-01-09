#include <BlynkGOv3.h>

GGaugeRainbow gauge[8];
GLabel lb_unit(SYMBOL_DEGREE "C", gauge[7]);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  gauge[0].size(60,60);
  gauge[0].color(TFT_YELLOW, TFT_RED);
  gauge[0].align(ALIGN_TOP_RIGHT,-10,10);

  gauge[1].size(60,60);
  gauge[1].gauge_type(GAUGE_TYPE_FULL);
  gauge[1].color(TFT_RED, TFT_VIOLET);
  gauge[1].align(gauge[0], ALIGN_BOTTOM,0,10);

  gauge[2].size(80,80);
  gauge[2].color(TFT_GREEN, TFT_NAVY);
  gauge[2].align(gauge[1], ALIGN_BOTTOM,0,10);

  gauge[3].size(80,80);
  gauge[3].gauge_type(GAUGE_TYPE_FULL);
  gauge[3].color(TFT_BLUE, TFT_PINK);
  gauge[3].align(gauge[2], ALIGN_BOTTOM);

  gauge[4].size(100,100);
  gauge[4].gauge_type(GAUGE_TYPE_FULL);
  gauge[4].decimal(1);
  gauge[4].align(ALIGN_TOP_LEFT,20);
  
  gauge[5].size(100,100);
  gauge[5].color(TFT_BROWN, TFT_VIOLET);
  gauge[5].thickness(25);
  gauge[5].align(gauge[4], ALIGN_RIGHT,30);

  gauge[6].size(100,100);
  gauge[6].color(TFT_SILVER, TFT_BLUE);
  gauge[6].align(gauge[5], ALIGN_RIGHT,30);

  gauge[7].size(220,220);
  gauge[7].gauge_type(GAUGE_TYPE_FULL);
  gauge[7].decimal(2);
  gauge[7].c_align( ALIGN_BOTTOM,-40,-110);
  gauge[7].label().align_center(0,-10);
  gauge[7].dragable(true);

    // วิธีวาง วิตเจ็ตข้อความ เพื่อแสดงหน่วย เพิ่มเติม
    lb_unit.font(prasanmit_40);
    lb_unit.align( gauge[7].label(), ALIGN_BOTTOM,0,-10);

  for(int i=0; i< 8;i++){
    gauge[i] = random(100);
  }
  gauge[4] = random(1000)/10.0;
  gauge[7] = random(10000)/100.0;
  
}

void loop() {
  BlynkGO.update();
}

