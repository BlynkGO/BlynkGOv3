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

// #define BLYNKGO_KEY           "----------------"
// #define SSID                  "----------------"
// #define PASSWORD              "----------------"

// #define BLYNK_AUTH            "----------------"
// #define BLYNK_HOST            "blynk.jpnet.co.th"   // Blynk Server แบบกำหนดเอง  ( ที่ blynk.jpnet.co.th ให้ energy 10ล้าน )
// #define BLYNK_PORT            8080                  // Blynk Port   แบบกำหนดเอง

#define BLYNK_BRIDGE_AUTH     "----------------"    // Blynk Auth ที่ 2 ที่จะทำ Bridge

GWiFiManager wifi_manager;
GBlynkManager blynk_manager;

GLabel lb_wifi_state;
GLabel lb_blynk_state;

WidgetBridge bridge1(V1);
BlynkTimer timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
//  BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD);   // แบบเชื่อมต่อ Blynk Server มาตรฐาน
//  BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD, BLYNK_HOST, BLYNK_PORT); // แบบเชื่อมต่อ Blynk Server กำหนดเอง

  wifi_manager.align(ALIGN_TOP_RIGHT, -10,5);
  blynk_manager.align(wifi_manager, ALIGN_LEFT, -10);

  lb_wifi_state .align(ALIGN_TOP,0,5);                      // วางไว้ชิดบนกลาง ลงมา 5 pixel
  lb_blynk_state.align(lb_wifi_state, ALIGN_BOTTOM);        // วางไว้ใต้ lb_wifi_state

  timer.setInterval(1000L, blynkAnotherDevice);
}

void loop() {
  BlynkGO.update();
  timer.run();
}


static bool value = true;
void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
{
  // Send value to another device
  if (value) {
    bridge1.digitalWrite(9, HIGH);  // Digital Pin 9 on the second board will be set HIGH
    bridge1.virtualWrite(V5, 1);    // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.

    /////////////////////////////////////////////////////////////////////////////////////////
    // Keep in mind that when performing virtualWrite with Bridge,
    // second board will need to process the incoming command.
    // It can be done by using this handler on the second board:
    //
    //    BLYNK_WRITE(V5){
    //    int pinData = param.asInt(); // pinData variable will store value that came via Bridge
    //    }
    //
    /////////////////////////////////////////////////////////////////////////////////////////
  } else {
    bridge1.digitalWrite(9, LOW); // Digital Pin 9 on the second board will be set LOW
    bridge1.virtualWrite(V5, 0); // Sends 0 value to BLYNK_WRITE(V5) handler on receiving side.
  }
  // Toggle value
  value = !value;
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

// เมื่อ Blynk เชื่อมต่อสำเร็จ (หากใช้ Bridge ให้มีส่วนนี้)
BLYNK_CONNECTED() {
  lb_blynk_state = "[Blynk] Connected";     // แสดงออกจอ TFT
  Serial.println(lb_blynk_state);           // แสดงออก Serial Monitor
  bridge1.setAuthToken(BLYNK_BRIDGE_AUTH);  // หลังการเชื่อมต่อ Blynk ให้ กำหนดวาง Auth Token ของ Blynk ค่าที่ 2,... ที่นี่
}

// เมื่อ Blynk หลุดการเชื่อมต่อ  (ใส่หรือไม่ก็ได้)
BLYNK_DISCONNECTED() {
  lb_blynk_state = "[Blynk] Disconnected";  // แสดงออกจอ TFT
  Serial.println(lb_blynk_state);           // แสดงออก Serial Monitor
}

