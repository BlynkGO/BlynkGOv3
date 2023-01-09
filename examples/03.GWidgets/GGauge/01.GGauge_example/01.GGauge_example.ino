#include <BlynkGOv3.h>

GGauge gauge( "อุณหภูมิ (°C)", TFT_COLOR(255, 100, 100) );

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // กำหนดขนาด ได้ 3 แบบ (ตามขนาดจอด้วย)
  // gauge.size(320,320);  gauge.size(240,240); gauge.size(120,120); 

  gauge.range(20,80);           // กำหนดช่วงให้ gauge
  gauge.decimal(1);             // กำหนดทศนิยม 0 หรือ 1 หรือ 2 ตำแหน่ง
  // gauge.gauge_type(GAUGE_TYPE_FULL);  // เปลี่ยนประเภท gauge ได้ 2 แบบ คือ GAUGE_TYPE_ARC หรือ GAUGE_TYPE_FULL
  // gauge.rounded(false);               // กำหนดปลาย gauge ให้โค้งหรือแบบตรงๆ
}

float step = 0.2;
float temperature = 20;

void loop() {
  BlynkGO.update();
  gauge = temperature;          // ตัวอย่างกำหนดค่าให้ gauge
  // Serial.println(gauge.value());   // ดึงค่าจาก gauge มาแสดงออก Serial Monitro
  
  temperature += step;
  if(temperature > 80 )     { temperature = 80; step = -step; }
  else if(temperature < 20) { temperature = 20; step = -step; }
}
