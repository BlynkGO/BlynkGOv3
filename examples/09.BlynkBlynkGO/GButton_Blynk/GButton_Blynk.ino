/********************************************************************
 * ตัวอย่าง การสร้าง Styled Button 
 * สามารถกดปุ่มแสดงได้พร้อมกันทั้ง TFT และ แอพ Blynk
 * ------------------------------------------------------------------
 * ที่แอพ Blynk ให้สร้าง Widget Styled Button 2 ปุ่ม
 *  ปุ่มแรก สีเหลือง กำหนด ปุ่มแบบ push (กดติดปล่อยดับ) และ มี virtual pin V7
 *  ปุ่มสอง สีเขียว  กำหนด ปุ่มแบบ switch (กดติดกดดับ) และ มี virtual pin V8
 *
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

GButton button_push   ( BUTTON_PUSH   , "ปุ่มกด");      // ปุ่มกด แบบ กดติดปล่อยดับ
GButton button_switch ( BUTTON_SWITCH , "สวิตซ์");      // ปุ่มกด แบบ กดติดกดดับ
//GButton button_switch( BUTTON_SWITCH, "สวิตซ์", V8);  // แบบกำหนด Virtual Pin รวมไปด้วยตั้งแต่ประกาศ โดยไม่ต้องกำหนด API virtualPin(..) ภายหลังซ้ำ

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD);  // แบบเชื่อมต่อ Blynk Server มาตรฐาน
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD, BLYNK_HOST, BLYNK_PORT); // แบบเชื่อมต่อ Blynk Server กำหนดเอง
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);
  
  wifi_manager.align(ALIGN_TOP_RIGHT, -10,5);
  blynk_manager.align(wifi_manager, ALIGN_LEFT, -10);

  // ปุ่มกด แบบ กดติดปล่อยดับ
  button_push.gpio(39);               // กำหนดขา GPIO ที่ใช้ต่อกับ relay
  button_push.virtualPin(V7);         // กำหนด virtual pin สำหรับเชื่อมต่อข้อมูลกับ แอพ Blynk
  button_push.rounded(true);          // ปุ่มแบบโค้งมน
  button_push.text_color(TFT_BLACK);  // กำหนดสีข้อความ
  button_push.color(TFT_ORANGE);      // กำหนดสีปุ่ม
  button_push.style[RELEASED] .color(TFT_DARKYELLOWBLYNK);  // ปรับแต่ง style เอง สำหรับ ปุ่มขณะ ปล่อยดับ (สีพื้น)
  button_push.style[RELEASED] .font_color(TFT_WHITE);       // ปรับแต่ง style เอง สำหรับ ปุ่มขณะ ปล่อยดับ (สีฟอนต์)
  button_push.style[PRESSED]  .color(TFT_YELLOWBLYNK);      // ปรับแต่ง style เอง สำหรับ ปุ่มขณะ กดติด   (สีพื้น)
  button_push.align(ALIGN_CENTER, 0,-50);                   // จัดวางตำแหน่างกลางจอ เลื่อนขึ้นบน 50 pixel

  // ปุ่มกด แบบ กดติดกดดับ
  button_switch.mode(BUTTON_MODE_TOGGLE);
  button_switch.gpio(38);
  button_switch.virtualPin(V8);
  button_switch.rounded(true);
  button_switch.text_color(TFT_BLACK);
  button_switch.color(TFT_GREEN);
  button_switch.style[RELEASED]       .color(TFT_DARKGREENBLYNK); // ปรับแต่ง style เอง สำหรับ ปุ่มขณะ ปิด ON (สีพื้น)
  button_switch.style[RELEASED]       .font_color(TFT_WHITE);     // ปรับแต่ง style เอง สำหรับ ปุ่มขณะ ปิด ON (สีฟอนต์)
  button_switch.style[TOGGLE_RELEASED].color(TFT_GREENBLYNK);     // ปรับแต่ง style เอง สำหรับ ปุ่มขณะ เปิด OFF (สีพื้น)
  button_switch.align(ALIGN_CENTER, 0, 50);                       // จัดวางตำแหน่างกลางจอ เลื่อนลง 50 pixel
}

void loop() {
  BlynkGO.update();
}

BLYNK_CONNECTED() {
  Serial.println("[Blynk] Connected");
  BlynkGO.syncAll();    // ซิงค์ สถานะทั้งหมดกับทาง Blynk Sever
}

BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] DisConnected");
}

/* แอพ Blynk สั่งกดปุ่ม มีการ อิพเดตมาให้ buttom_push ของ BlynkGO ด้วย */
BLYNK_WRITE(V7){
  button_push.state(param.asInt());
}

/* แอพ Blynk สั่งกดปุ่ม มีการ อิพเดตมาให้ button_switch ของ BlynkGO ด้วย */
BLYNK_WRITE(V8){
  button_switch.state(param.asInt());
}


