#include <BlynkGOv3.h>
#include <PMS.h>        // download library at https://github.com/fu-hsi/PMS

#define PMS_RX2     22
#define PMS_TX2     25
#define pms_serial  Serial2

PMS pms(pms_serial);
PMS::DATA pms_data;   //pm1.0 = pms_data.PM_AE_UG_1_0 (ug/m3); pm2.5 = pms_data.PM_AE_UG_2_5 (ug/m3); pm10.0 = pms_data.PM_AE_UG_10_0 (ug/m3)

GAqi   aqi;
GTimer aqi_timer;

void setup() {
  Serial.begin(115200);
  pms_serial.begin(9600, SERIAL_8N1, PMS_RX2, PMS_TX2);

  BlynkGO.begin();
  
  aqi.aqi_type(AQI_TH);

  aqi_timer.setInterval(10L, [](){    // ตั้งเวลาในการวนอ่านข้อมูล เซนเซอร์ PMS
    if (pms.read(pms_data)){  
      static uint32_t timer;
      if( timer <= millis() ){
        timer = millis() + 5000;      // ตั้งเวลาในการแสดงผลออกจอ
        aqi.pm25(pms_data.PM_AE_UG_2_5);
        Serial.println( "[PM2.5] PM2.5 : "+ String((int) aqi.pm25()) + " μg/m3 --> Thai AQI : " + String((int)aqi.getAQI(AQI_TH)));
      }
    }
  });
}

void loop() {
  BlynkGO.update();
}

