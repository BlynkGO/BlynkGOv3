#include <BlynkGOv3.h>
#include <DHTesp.h>  // ให้เข้าไป download ไลบรารี่ DHTesp ได้ที่  https://github.com/beegee-tokyo/DHTesp

#define DHT_PIN   27              // กำหนดขา pin ที่ต่อ DHT
#define DHT_TYPE  DHTesp::DHT22   // กำหนดประเภท DHT

DHTesp dht;            // dht เซนเซอร์

GGauge gauge_temp;     // วิตเจ็ต GGauge
GGauge gauge_humid;    // วิตเจ็ต GGauge
GTimer timer;          // ตัวตั้งเวลาทำงานเป็นระยะๆ

void setup(){
  BlynkGO.begin();

  dht.setup(DHT_PIN, DHT_TYPE);         // เริ่มต้นใช้งาน dht ด้วยการกำหนด ขา และประเภท ของ dht

  gauge_temp.size(120,120);
  gauge_temp.title("อุณหภูมิ");
  gauge_temp.color(TFT_RED);
  gauge_temp.range(20,80);
  gauge_temp.decimal(1);
  gauge_temp.align(ALIGN_CENTER, -gauge_temp.width()/2);
  gauge_temp = 38.7;

  gauge_humid.size(120,120);
  gauge_humid.title("ความชื้น");
  gauge_humid.gauge_type(GAUGE_TYPE_FULL);
  gauge_humid.color(TFT_BLUE);
  gauge_humid.range(0,100);
  gauge_humid.decimal(1);
  gauge_humid.align(ALIGN_CENTER, gauge_humid.width()/2);
  gauge_humid.rounded(false);
  gauge_humid = 85.3;
  
  timer.setInterval(2000L,[](){         // ตั้งเวลาทำงานทุกๆ 2 วินาที
    float temp  = dht.getTemperature();
    float humid = dht.getHumidity();

    if(!isnan(temp))  gauge_temp  = temp;
    if(!isnan(humid)) gauge_humid = humid;
  });
}

void loop(){
  BlynkGO.update();
}

