/***************************************************************
 * ก่อนใช้งาน ร่วมกับแอพ Blynk
 *  1. ให้ copy ไฟล์ ในโฟลเดอร์ BlynkGOv3/BlynkBlynkGO ให้นำโฟลเดอร์ src ไปวางไว้ที่ 
 *     โฟลเดอร์ไลบรารี่ Blynk หากมีไฟล์ซ้ำให้ copy ทับ
 *
 *  2. ให้กำหนด เปิด การใช้งาน Blynk ให้ BlynkGO ได้ที่
 *     ไฟล์ config/blynkgo_config.h
 *     โดยกำหนดเป็น
 *       #define BLYNKGO_USE_WIFI           1
 *       #define BLYNKGO_USE_NTP            1
 *       #define BLYNKGO_USE_BLYNK          1
 *
 **************************************************************/

#include <BlynkGOv3.h>

#define BLYNK_AUTH    "----------------"
//#define BLYNK_HOST    "blynk.jpnet.co.th"   // Blynk Server แบบกำหนดเอง  ( ที่ blynk.jpnet.co.th ให้ energy 10ล้าน )
//#define BLYNK_PORT    8080                  // Blynk Port   แบบกำหนดเอง


GWiFiManager wifi_manager;
GLabel lb_wifi_state;
GLabel lb_blynk_state;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.configBlynk(BLYNK_AUTH); // แบบเชื่อมต่อ Blynk Server มาตรฐาน
  // BlynkGO.configBlynk(BLYNK_AUTH, BLYNK_HOST, BLYNK_PORT); // แบบเชื่อมต่อ Blynk Server กำหนดเอง
  
  wifi_manager  .align(ALIGN_TOP_RIGHT,-5);                 // วางไว้ชิดบนขวา เลื่อนซ้าย 5 pixel
  lb_wifi_state .align(ALIGN_TOP,0,5);                      // วางไว้ชิดบนกลาง ลงมา 5 pixel
  lb_blynk_state.align(lb_wifi_state, ALIGN_BOTTOM);        // วางไว้ใต้ lb_wifi_state
}

void loop() {
  BlynkGO.update();
}

// เมื่อ WiFi เชื่อมต่อสำเร็จ  (ใส่หรือไม่ก็ได้)
WIFI_CONNECTED() {
  lb_wifi_state = "[WiFi] Connected. IP : " + WiFi.localIP().toString();    // แสดงออกจอ TFT
  Serial.println( lb_wifi_state );          // แสดงออก Serial Monitor
}

// เมื่อ WiFi หลุดการเชื่อมต่อ  (ใส่หรือไม่ก็ได้)
WIFI_DISCONNECTED(){
  lb_wifi_state = "[WiFi] Disconnected.";   // แสดงออกจอ TFT
  Serial.println(lb_wifi_state);            // แสดงออก Serial Monitor
}

// เมื่อ Blynk เชื่อมต่อสำเร็จ  (ใส่หรือไม่ก็ได้)
BLYNK_CONNECTED() {
  lb_blynk_state = "[Blynk] Connected";     // แสดงออกจอ TFT
  Serial.println(lb_blynk_state);           // แสดงออก Serial Monitor
}

// เมื่อ Blynk หลุดการเชื่อมต่อ  (ใส่หรือไม่ก็ได้)
BLYNK_DISCONNECTED() {
  lb_blynk_state = "[Blynk] Disconnected";  // แสดงออกจอ TFT
  Serial.println(lb_blynk_state);           // แสดงออก Serial Monitor
}
