/*************************************************************
 * ตัวอย่างนี้เป็นตัวอย่าง การตั้ง WebSocket Server แบบ Asynchronous
 *
 * ตัวอย่างจะมี GSwitch3D บนหน้าจอ TFT
 * ซึ่งสามารถสั่งเปิดปิด realtime ได้จากทั้งบนหน้าจอ TFT
 * และ ผ่าน WebSocket Server ได้
 * 
 * ก่อนการ compile
 *   1. ติดตั้งไลบรารี่ ArduionJson version 6.xx ให้เรียบร้อย
 *      ผ่านทาง ArduinoIDE เมนู Sketch > Include Library > Manage Libraries...
 *   2. โปรเจคนี้มีการใช้ WiFi ให้เปิดใช้งาน WiFi ด้วย ที่
 *      blynkgo_config.h โดยกำหนดให้
 *
 *        #define BLYNKGO_USE_WIFI     1
 * 
 * วิธีการใช้งาน
 *   1. ทำการ Upload โคดนี้
 *   2. ตั้งค่า WiFi ผ่านวิตเจ็ต GWiFiManager ทางหน้าจอ TFT
 *   3. เมื่อเชื่อมต่อสำเร็จ จะปรากฏ ip address ของ WebServer
 *      ให้นำไปเปิดบน Browser
 *
 *** หมายเหตุ ***
 *   หากสร้างโปรเจคใหม่ หรือ  save ไปเป็นอีกโปรเจค
 *   ให้ สำเนา src ของตัวอย่างไปวางรวมในโฟลเดอร์ของโปรเจคใหม่ด้วย
 *
 *************************************************************/

#include <BlynkGOv3.h>

namespace AsyncWEBSOCKET {
  void begin();
  void end();
  void update();
  void notifyClients(bool sw_state);
}

GWiFiManager wifi_manager;
GSwitch3D  sw3d;
GLabel lb_ipaddress;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);
  lb_ipaddress.align(ALIGN_TOP,0,10);
  
  sw3d.gpio(25);      // ขา pin สำหรับ ต่อ relay
  sw3d.onValueChanged([](GWidget*widget){ // เมื่อ User กดปุ่มเปลี่ยนสถานะ sw3d ที่หน้าจอ TFT
    // ส่งสถานะ ปุ่มกดของ sw3d ไปยัง client ของ AsyncWEBSOCKET ด้วย
    AsyncWEBSOCKET::notifyClients(sw3d.isON());
  });
}

void loop() {
  BlynkGO.update(); 
  AsyncWEBSOCKET::update();
}

// เมื่อ WiFi มีการเชื่อมต่อ
WIFI_CONNECTED(){
  lb_ipaddress = "http://"+ WiFi.localIP().toString();
  // เริ่มทำงาน AsyncWEBSOCKET
  AsyncWEBSOCKET::begin();
}

// เมื่อ WiFi หลุดการเชื่อมต่อ
WIFI_DISCONNECTED(){
  lb_ipaddress = "WiFi ไม่มีการเชื่อมต่อ";
  // หยุดทำงาน AsyncWEBSOCKET
  AsyncWEBSOCKET::end();
}

