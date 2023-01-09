/*******************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ WiFi
 * พร้อมแสดงเวลา NTP
 *
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

#define SSID          "----------------"
#define PASSWORD      "----------------"


GLabel labelWiFi, labelClock;

void setup() {
  Serial.begin(115200);Serial.println();
  BlynkGO.begin();

  labelClock.font(prasanmit_40);
  labelClock.color(TFT_GREEN);

  labelWiFi.align(ALIGN_TOP,0,15);
  labelWiFi = "WiFi connecting...";
      
  WiFi.begin( SSID, PASSWORD);
}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED(){
  labelWiFi = WiFi.localIP().toString();
}

NTP_SYNCED(){
  static GTimer timer;
  // ตั้งเวลาทุกๆ 1 วินาที ให้ ดึงค่า hour(), minute(), second() มาแสดงบน labelClock
  timer.setInterval(1000L, [](){
    labelClock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

