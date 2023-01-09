/*******************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ WiFi
 * พร้อมแสดงเวลา NTP
 * โดย NTP จะมีการ sync ซ้ำอัตโนมัติ 
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

GLabel labelWiFI;
GLabel labelClock;
GTimer timer;

void setup() {
  Serial.begin(115200);Serial.println();
  BlynkGO.begin();

  labelWiFI.align(ALIGN_TOP,0,15);
  
  labelWiFI = "Hello BlynkGO WiFi Start!";

  labelClock = "";
  labelClock.font(prasanmit_40,TFT_GREEN);

  WiFi.begin( SSID, PASSWORD);
}

void loop() {
  BlynkGO.update();
}

// เมื่อ WiFi เชื่อมต่อสำเร็จ จะมาเรียกส่วนนี้ (มีหรือไม่ก็ได้)
WIFI_CONNECTED(){
  labelWiFI = String("[WiFi] Connected.\nIP : ") +  WiFi.localIP().toString();
  Serial.println(labelWiFI);
}

// เมื่อ WiFi หลุดการเชื่อมต่อ จะมาเรียกส่วนนี้ (มีหรือไม่ก็ได้)
WIFI_DISCONNECTED(){
  labelWiFI = "[WiFi] Disconnected.";
  Serial.println(labelWiFI);
}

// เมื่อ NTP ซิงค์เวลาสำเร็จ จะมาเรียกส่วนนี้ (มีหรือไม่ก็ได้)
NTP_SYNCED(){
  timer.setInterval(1000L, [](){
    labelClock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}
