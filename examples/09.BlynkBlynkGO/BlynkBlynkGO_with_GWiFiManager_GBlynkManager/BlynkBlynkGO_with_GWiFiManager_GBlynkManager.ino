/********************************************************************************
 * ตัวอย่าง การเชื่อมต่อ WiFi ด้วย วิตเจ็ต GWiFiManager
 *  และ เชื่อมต่อ Blynk ด้วย วิตเจ็ต GBlynkManager
 *  
 * ------------------------------------------------------------------
 *  วิธีการติดตั้ง
 *  1. ให้ copy ไฟล์ ในโฟลเดอร์ BlynkGOv3/BlynkBlynkGO ให้นำโฟลเดอร์ src ไปวางไว้ที่ 
 *     โฟลเดอร์ไลบรารี่ Blynk หากมีไฟล์ซ้ำให้ copy ทับ
 *  2. เปิด BlynkGOv3/src/config/blynkgo_config.h
 *     ตั้ง ให้ 
 *          #define BLYNKGO_USE_WIFI      1
 *          #define BLYNKGO_USE_NTP       1
 *          #define BLYNKGO_USE_BLYNK     1
 *          
 *        และ ในตอนล่างจะมีให้เลือกได้ โดย ภาษา ที่ต้องการ ภาษาใดภาษาหนึ่ง
 *  3. ฝั่งแอพ Blynk ให้สร้าง เกจจ์ สำหรับรับค่า AQI จาก ESP32 ไปแสดง ด้วย VirtualPin V2
 * 
 ********************************************************************************/

#include <BlynkGOv3.h>

GWiFiManager  wifi_manager;
GBlynkManager blynk_manager;
GAqi          aqi;
GTimer        aqi_timer;

void aqi_set_and_display(){
  int pm25 = random(20,360);                      // สุ่มเลข pm2.5
  aqi.pm25(pm25);                                 // กำหนดค่า pm2.5 ให้ วิตเจ็ต
  BlynkGO.virtualWrite(V2, aqi.getAQI(AQI_US));   // ส่งค่า AQI แบบ US ไปให้ ทางฝั่งแอพ Blynk ด้วย V2
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5); 
  wifi_manager.font(prasanmit_40);
  blynk_manager.align(wifi_manager, ALIGN_LEFT,-5);

  aqi.aqi_type(AQI_US);

  aqi_set_and_display();
  aqi_timer.setInterval( 2*60000L, aqi_set_and_display);
  
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
BLYNK_CONNECTED(){
  Serial.println("[Blynk] Connected");
}

// มีหรือไม่ก็ได้
BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] Disconnected");
}

