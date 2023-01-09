/********************************************************************************
 * ตัวอย่าง การเชื่อมต่อ WiFi ด้วย วิตเจ็ต GWiFiManager
 *  และ เชื่อมต่อ Blynk ด้วย วิตเจ็ต GBlynkManager
 *  
 * ------------------------------------------------------------------
 *  วิธีการติดตั้ง
 *     1. ให้ copy ไฟล์ ในโฟลเดอร์ BlynkGOv3/blynk/src  นี้ไปวางไว้ที่ 
 *        โฟลเดอร์ไลบรารี่ Blynk หากมีไฟล์ซ้ำให้ copy ทับ
 *     2. เปิด BlynkGOv3/src/config/blynkgo_config.h
 *        ตั้ง ให้ 
 *          #define BLYNKGO_USE_WIFI      1
 *          #define BLYNKGO_USE_NTP       1
 *          #define BLYNKGO_USE_BLYNK     1
 * 
 ********************************************************************************/

#include <BlynkGOv3.h>

GWiFiManager  wifi_manager;
GBlynkManager blynk_manager;
GLabel        lb_clock;

void wifi_manager_customize();
void blynk_manager_customize();

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5); 
  wifi_manager.font(prasanmit_40);
  blynk_manager.align(wifi_manager, ALIGN_LEFT,-5);

  lb_clock.font(prasanmit_40);
  lb_clock.color(TFT_GREEN);
  lb_clock.align(ALIGN_CENTER);
}

void loop() {
  BlynkGO.update();
}

// มีหรือไม่ก็ได้
WIFI_CONNECTED(){
  Serial.println("[WiFi] Connected");
  Serial.println(WiFi.localIP());
}

// มีหรือไม่ก็ได้
NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

// มีหรือไม่ก็ได้
BLYNK_CONNECTED(){
  Serial.println("[Blynk] Connected");
}

// มีหรือไม่ก็ได้
BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] Disconnected");
}
