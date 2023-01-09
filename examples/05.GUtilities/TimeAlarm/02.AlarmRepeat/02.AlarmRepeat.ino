/*******************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ WiFi
 * พร้อมแสดงเวลา NTP
 * และมีการตั้งเวลา Alarm ทำงานซ้ำๆ ทุกๆวัน/ทุกๆวันในสัปดาห์ ตามเวลาที่กำหนด
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


  // ตั้งปลุกแบบซ้ำ ทุกๆวัน ด้วย เวลาที่ต้องการ
  BlynkGO.alarmRepeat(11,25,0,[](){
    Serial.printf("[Alarm Repeat] %02d:%02d:%02d\n", hour(), minute(), second());
    // หากจะให้ตั้งเวลาให้ทำอะไรให้เพิ่มตรงนี้ ....

  });

  // ตั้งปลุกแบบซ้ำ ทุกๆวันในสัปดาห์ ด้วย เวลาที่ต้องการ
  BlynkGO.alarmRepeat(ALARM_WEDNESDAY, 12,30,0,[](){
    Serial.printf("[Alarm Repeat] %s %02d:%02d:%02d\n", dayStr(weekday()), hour(), minute(), second());
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

