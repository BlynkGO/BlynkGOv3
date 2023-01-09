#include <BlynkGOv3.h>
#include <DHTesp.h>  // ให้เข้าไป download ไลบรารี่ DHTesp ได้ที่  https://github.com/beegee-tokyo/DHTesp

#define DHT_PIN   27              // กำหนดขา pin ที่ต่อ DHT
#define DHT_TYPE  DHTesp::DHT22   // กำหนดประเภท DHT

DHTesp dht;       // dht เซนเซอร์

GLabel label;     // วิตเจ็ต GLabel
GTimer timer;     // ตัวตั้งเวลาทำงานเป็นระยะๆ

void setup(){
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  dht.setup(DHT_PIN, DHT_TYPE);         // เริ่มต้นใช้งาน dht ด้วยการกำหนด ขา และประเภท ของ dht
  label.font(prasanmit_40,TFT_BLACK);   // กำหนด font และสี
  label.text_align(TEXT_ALIGN_RIGHT);   // เวลาข้อความมีหลายบรรทัดให้จัดข้อความขิดขวา
  
  timer.setInterval(2000L,[](){         // ตั้งเวลาทำงานทุกๆ 2 วินาที
    float temp  = dht.getTemperature();
    float humid = dht.getHumidity();

    if( isnan(temp) || isnan(humid) ) return; // หาก ค่า temp, humid อ่านได้ NAN ให้ออกไป

    // นำค่า temp, humid ที่ได้มาใส่ให้ วิตเจ็ตข้อความ เพื่อแสดงผลทางหน้าจอ 
    label = String("อุณหภูมิ : ") + String(temp,1) + SYMBOL_DEGREE + "C" + "\n" +
            String("ความชื้น : ") + String(humid,1) + "%";
  });
}

void loop(){
  BlynkGO.update();
}

