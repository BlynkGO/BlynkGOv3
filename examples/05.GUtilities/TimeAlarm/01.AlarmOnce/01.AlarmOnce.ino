/*******************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ WiFi
 * พร้อมแสดงเวลา NTP
 * และมีการตั้งเวลา Alarm ทำงานเพียง ครั้งเดียว ตามเวลาที่กำหนด
 * ---------------------------------------
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 ******************************************************/


#include <BlynkGOv3.h>

GWiFiManager  wifi_manager;
GLabel        lb_clock;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,0);
  lb_clock.align(wifi_manager, ALIGN_LEFT,-10);


  // ตั้งปลุกแบบครั้งเดียว ด้วย เวลาที่ต้องการ
  BlynkGO.alarmOnce(11,25,0,[](){
    Serial.printf("[Alarm Once] %02d:%02d:%02d\n", hour(), minute(), second());
    // หากจะให้ตั้งเวลาให้ทำอะไรให้เพิ่มตรงนี้ ....

  });

  // ตั้งปลุกแบบครั้งเดียว ด้วย วันในสัปดาห์ และ เวลาที่ต้องการ
  BlynkGO.alarmOnce(ALARM_WEDNESDAY, 12,30,0,[](){
    Serial.printf("[Alarm Once] %s %02d:%02d:%02d\n", dayStr(weekday()), hour(), minute(), second());
    // หากจะให้ตั้งเวลาให้ทำอะไรให้เพิ่มตรงนี้ ....
    
  });
}

void loop() {
  BlynkGO.update();
}


NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

