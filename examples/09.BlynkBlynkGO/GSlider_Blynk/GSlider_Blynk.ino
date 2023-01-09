/********************************************************************
 * ตัวอย่าง การสร้าง Slider 
 * ที่สามารถควบคุมได้จาก TFT และ แอพ Blynk
 * --------------------------------------------
 * ที่แอพ Blynk ให้สร้าง Widget Slider 
 * โดยกำหนด virtual pin V2 
 * และมี ให้ช่วงระหว่าง 0-200 เพื่อให้ตรงกับบน TFT
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

GSlider slider(0,200);
// GSlider slider(0,200,V2, SLIDER_TYPE_NORMAL);  // แบบกำหนด Virtual Pin ตั้งแต่ประกาศ

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD);  // แบบเชื่อมต่อ Blynk Server มาตรฐาน
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD, BLYNK_HOST, BLYNK_PORT); // แบบเชื่อมต่อ Blynk Server กำหนดเอง
  
  wifi_manager.align(ALIGN_TOP_RIGHT, -10,5);
  blynk_manager.align(wifi_manager, ALIGN_LEFT, -10);

  slider.color(TFT_RED);
  slider.virtualPin(V2);    // กำหนด virtual pin สำหรับ blynk
}

void loop() {
  BlynkGO.update();
}

/* Sync V2 เมื่อ Blynk Connect แล้ว*/
BLYNK_CONNECTED(){
  Serial.println("[Blynk] Connected");
  BlynkGO.syncVirtual(V2);
}

BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] DisConnected");
}

/* กำหนดค่าให้ slider เมื่อทาง Blynk App มีการเปลี่ยนแปลง */
BLYNK_WRITE(V2){
  slider.value(param.asInt());
}

