/****************************************************
 * สำหรับ TTGO T-Watch2019 รุ่นที่มี GPS ( U-Blox )
 *  ก่อน compile ให้เข้าไปตั้งค่าใน blynkgo_config.h ให้เลือกบอร์ดเป็น
 *  
 *       #define TTGO_TWATCH_2019
 *  
 *  และเปิด ให้ ใช้ GPS ได้ด้วย
 *  
 *       #define BLYNKGO_USE_GPS           1 
 * 
 ****************************************************/

#include <BlynkGOv3.h>

GTimer timer_gps;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  timer_gps.setInterval(5000,[](){      // ตั้งเวลาให้ทำงานเป็นช่วงๆ ทุกๆ 5 วินาที

    if( ! TWatch.GPS.location.isValid() ){
      Serial.println("[GPS] please waiting...");
      return;
    }
    
    Serial.printf("[GPS] lat : %.6f ; long : %.6f\n", TWatch.GPS.location.lat(), TWatch.GPS.location.lng() );
    
    if( TWatch.GPS.date.isValid()){
      Serial.printf("[GPS] date : %02d/%02d/%02d\n", TWatch.GPS.date.day(), TWatch.GPS.date.month(), TWatch.GPS.date.year());
    }
    if( TWatch.GPS.time.isValid()){
        Serial.printf("[GPS] time : %02d:%02d:%02d:%02d\n", TWatch.GPS.time.hour(), TWatch.GPS.time.minute(), TWatch.GPS.time.second());
    }
    Serial.println();
  });
}

void loop() {
  BlynkGO.update();
}

