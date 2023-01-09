/********************************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ WiFi
 * พร้อมแสดงเวลา NTP
 * และมีการตั้งเวลา Alarm ทำงานทุกๆ กี่นาที ที่กำหนด แบบลงตัว
 * เช่น ทุกๆ  5 นาที  จะไป alarm ที่ เวลา  XX:00 --> XX:05 --> XX:10 --> ....
 *     ทุกๆ 20 นาที  จะไป alarm ที่ เวลา  XX:00 --> XX:20 --> XX:40 --> ....
 * -----------------------------------------------------------------
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 *******************************************************************/

#include <BlynkGOv3.h>

#define ALARM_EVERY_PERIOD        5  // minutes

GWiFiManager  wifi_manager;
GLabel        lb_clock;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,0);
  lb_clock.align(wifi_manager, ALIGN_LEFT,-10);

}

void loop() {
  BlynkGO.update();
}

void alarm_every(){

  time_t cur_alarm   = secs_of_day(hour(), minute(), second());
  time_t next_alarm  =  ( int( cur_alarm / (ALARM_EVERY_PERIOD*60) ) + 1 ) * ALARM_EVERY_PERIOD*60 ;

  BlynkGO.alarmOnce(next_alarm, alarm_every);

  if( cur_alarm % (ALARM_EVERY_PERIOD*60)==0 ) {
    Serial.printf("[Alarm] %02d:%02d:%02d\n", hour(cur_alarm), minute(cur_alarm), second(cur_alarm));

    // หากจะให้ตั้งเวลาให้ทำอะไรให้เพิ่มตรงนี้ ....

  }else{
    Serial.printf("[Next Alarm] %02d:%02d:%02d\n", hour(next_alarm), minute(next_alarm), second(next_alarm));
  }
  

}

NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });

  static bool first_synced = true;
  if(first_synced){
    alarm_every();
    first_synced = false;
  }
}

