/**********************************************************************
 *
 *  ก่อนใช้งาน
 *  เข้าไปที่ BlynkGOv3/src/config/blynkgo_config.h
 *    เปลี่ยนบอร์ดเป็น
 *       #define  BLYNKO_LORA32
 *
 *    และที่ ArduinoIDE เลือกบอร์ดเป็น BlynkGO Lora32
 *
 **********************************************************************/

/**********************************************************************
 * ตัวอย่างนี้ มีการใช้ WiFi และ NTP ในการ compile
 * ให้กำหนด เปิด การ compile ด้วย WiFi และ NTP ได้ที่
 * ไฟล์ BlynkGOv3/src/config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 **********************************************************************/


#include <BlynkGOv3.h>

#define SSID            "-------------"
#define PASSWORD        "-------------"

FONT_DECLARE(LedDotMatrix_25);

GLabel  lb_clock;
GLabel  lb_wifi_connected(SYMBOL_WIFI);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  WiFi.begin( SSID, PASSWORD);

  lb_wifi_connected.font(prasanmit_20);
  lb_wifi_connected.align(ALIGN_TOP_LEFT,0,-8);
  lb_wifi_connected.hidden(true);

  lb_clock.font(LedDotMatrix_25);
}

void loop() {
  BlynkGO.update();  
}


WIFI_CONNECTED(){
  lb_wifi_connected.hidden(false);
}

WIFI_DISCONNECTED(){
  lb_wifi_connected.hidden(true);
}


NTP_SYNCED(){
  static GTimer timer;
  // ตั้งเวลาทุกๆ 1 วินาที ให้ ดึงค่า hour(), minute(), second() มาแสดงบน labelClock
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}




