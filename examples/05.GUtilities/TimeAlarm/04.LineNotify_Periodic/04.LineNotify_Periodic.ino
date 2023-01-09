/********************************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ WiFi พร้อมแสดงเวลา NTP
 * และ มีการตั้งเวลาส่ง Line Notify ภายในช่วงเวลาที่กำหนด
 * โดยมีการส่ง Line Notify ทุกๆ กี่นาที ที่กำหนด แบบลงตัว
 * เช่น ทุกๆ  5 นาที  จะไป alarm ที่ เวลา  XX:00 --> XX:05 --> XX:10 --> ....
 *     ทุกๆ 20 นาที  จะไป alarm ที่ เวลา  XX:00 --> XX:20 --> XX:40 --> ....
 * -----------------------------------------------------------------
 * ก่อนใช้งาน WiFi และ NTP และ LineNotify
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *     #define BLYNKGO_USE_LINENOTIFY    1
 *******************************************************************/

#include <BlynkGOv3.h>

#define LINE_TOKEN                "--------------------"    // Line Token
#define LINE_START                "07:00"                   // เวลาเริ่มส่งเพื่อส่ง Line Notify
#define LINE_STOP                 "17:30"                   // เวลาสิ้นสุดเพื่อส่ง Line Notify
#define LINE_EVERY_PERIOD         5                         // ส่ง Line Notify ทุกๆ กี่นาที (minutes)

GWiFiManager  wifi_manager;
GLabel        lb_clock;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,0);
  lb_clock.align(wifi_manager, ALIGN_LEFT,-10);

  LINE.setToken(LINE_TOKEN);
}

void loop() {
  BlynkGO.update();
}

void line_alarm_every(){
  time_t cur_alarm   = secs_of_day(hour(), minute(), second());
  time_t next_alarm  =  ( int( cur_alarm / (LINE_EVERY_PERIOD*60) ) + 1 ) * LINE_EVERY_PERIOD*60 ;

  int h_start = String(LINE_START).substring(0,2).toInt();
  int m_start = String(LINE_START).substring(3).toInt();
  int h_stop  = String(LINE_STOP).substring(0,2).toInt();
  int m_stop  = String(LINE_STOP).substring(3).toInt();

  time_t line_start_alarm  = secs_of_day( h_start, m_start, 0);
  time_t line_stop_alarm   = secs_of_day( h_stop, m_stop, 0);

  if( next_alarm < line_start_alarm - 10 || next_alarm > line_stop_alarm + 10 )  next_alarm = line_start_alarm;
  
  BlynkGO.alarmOnce(next_alarm, line_alarm_every);

  if( cur_alarm % (LINE_EVERY_PERIOD*60)==0 ) {
    Serial.printf("[Alarm] %02d:%02d:%02d\n", hour(cur_alarm), minute(cur_alarm), second(cur_alarm));

    /**************************************
     * หากจะให้ตั้งเวลาให้ทำอะไรให้เพิ่มตรงนี้ ....
     **************************************/
    // จำลองสุ่มค่า อุณหภูมิ และความชื้น  (หากทดลองจริงให้อ่านค่าจาก sensor ที่ใช้)
    float temp = random(2000,3000)/100.0; 
    float humid = random(2000,6000)/100.0;

    // สร้างข้อความที่จะส่ง LINE Notify
    String message = StringX::printf("\n[%02d:%02d:%02d]\nTemp : %.2f\nHumid : %.2f", 
                          hour(cur_alarm), minute(cur_alarm), second(cur_alarm), temp, humid);
    LINE.notify(message);
    
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
    line_alarm_every();
    first_synced = false;
  }
}
