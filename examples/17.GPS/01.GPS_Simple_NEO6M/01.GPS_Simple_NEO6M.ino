/****************************************************
 * ก่อน compile ให้เข้าไปตั้งค่าใน blynkgo_config.h 
 * ให้ เปิดใช้ GPS เป็นดังนี้
 *  
 *   #define BLYNKGO_USE_GPS           1 
 * 
 ****************************************************/

#include <BlynkGOv3.h>

#define GPS_RX        22
#define GPS_TX        25

IMG_DECLARE(img_background);

namespace GPS_NEO6M {
  void    begin(uint8_t rx, uint8_t tx);
  void    onGPS(uint32_t interval, void(*fn)(void));
  double  lat(), lon();
  int8_t  day(), month(), hour(), minute(), second(), centisecond();
  int16_t year();
}

GImage bg(img_background);
GLabel lb_gps;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(); 

  bg.align_center();
  lb_gps.align(ALIGN_TOP_LEFT, 300, 230);
  lb_gps.font(prasanmit_35, TFT_WHITE);
  
  GPS_NEO6M::begin( GPS_RX, GPS_TX );  
  GPS_NEO6M::onGPS( 5000, [](){        // เช็คตำแหน่งทุกๆ 5 วินาที
    // แสดงตำแหน่ง GPS ขึ้นจอ TFT
    lb_gps = StringX::printf("LAT   : %.6f\nLONG : %.6f", GPS_NEO6M::lat(), GPS_NEO6M::lon());

    // หากจะดึงเวลา จาก ดาวเทียมมาใช้ สามารถเรียกใช้คำสั่ง เวลาดังนี้ได้เลย
    // GPS_NEO6M::day() ,  GPS_NEO6M::month(),  GPS_NEO6M::year()
    // GPS_NEO6M::hour(),  GPS_NEO6M::minute(), GPS_NEO6M::second(),  GPS_NEO6M::centisecond()

  });
}

void loop() {
  BlynkGO.update();
}

