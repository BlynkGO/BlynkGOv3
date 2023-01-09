/********************************************************************
 * ตัวอย่าง การสร้าง ปุ่มสวิทซ์ ด้วย GSwitch
 * ที่สามารถควบคุมได้จาก TFT และ แอพ Blynk
 * --------------------------------------------
 * ที่แอพ Blynk ให้สร้าง Widget Button
 * โดยกำหนด virtual pin V1 
 * ------------------------------------------------------------------
 * หมายเหตุ 
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

// #define BLYNKGO_KEY   "----------------"
// #define SSID          "----------------"
// #define PASSWORD      "----------------"

// #define BLYNK_AUTH    "----------------"
// #define BLYNK_HOST    "blynk.jpnet.co.th"   // Blynk Server แบบกำหนดเอง  ( ที่ blynk.jpnet.co.th ให้ energy 10ล้าน )
// #define BLYNK_PORT    8080                  // Blynk Port   แบบกำหนดเอง

GWiFiManager wifi_manager;
GBlynkManager blynk_manager;

GSwitch sw;
// GSwitch sw(V1);  // แบบกำหนด Virtual Pin ตั้งแต่ประกาศ โดยไม่ต้องกำหนด sw.virtualPin(V1) อีก

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD);  // แบบเชื่อมต่อ Blynk Server มาตรฐาน
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD, BLYNK_HOST, BLYNK_PORT); // แบบเชื่อมต่อ Blynk Server กำหนดเอง
  
  wifi_manager.align(ALIGN_TOP_RIGHT, -10,5);
  blynk_manager.align(wifi_manager, ALIGN_LEFT, -10);

  sw.align(ALIGN_CENTER);

  /* หากมีการเปลี่ยนแปลง sw ไม่ว่าจากแอพ blynk หรือ กดจอ TFT
     จะมีการเปิด/ปิดไฟ ที่ ขา gpio ที่กำหนด 
     และมีการ update กันระหว่างแอพ blynk อัตโนมัติ
     ด้วย virtual pin ที่กำหนด */
  sw.gpio(LED_BUILTIN);   // กำหนดขา gpio สำหรับต่อกับ relay
  sw.active_high(false);  // กำหนด gpio เมื่อ ON ให้ pinMode เป็น LOW ( default จะเป็น HIGH)
  sw.virtualPin(V1);      // กำหนด virtual pin สำหรับ blynk หรือประกาศตั้งแต่ส่วนต้นก็ได้
  
  sw.ON();      // สั่งเปิด
//  sw.OFF();   // สั่งปิด
  sw.onValueChanged([](GWidget* widget){
    Serial.printf("Switch (from TFT): %d\n", sw.state());
  });

}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED(){
  Serial.print("[WiFi] Connected. IP : ");
  Serial.println(WiFi.localIP());
}

WIFI_DISCONNECTED(){
  Serial.print("[WiFi] DisConnected");
}

BLYNK_CONNECTED() {
  Serial.println("[Blynk] Connected");
  BlynkGO.syncAll();
}

BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] DisConnected");
}

BLYNK_WRITE(V1){    // เมื่อ แอพ blynk มีการกดปุ่มเข้ามาทาง virtual pin V1
  sw.state(param.asInt());  // ให้ทำการ update สถานะ ของ sw3d ด้วย (ซึ่งจะอัพเดตขา gpio ด้วยในตัว)
  Serial.printf("Switch (from Blynk App) : %d\n", sw.state());
}

