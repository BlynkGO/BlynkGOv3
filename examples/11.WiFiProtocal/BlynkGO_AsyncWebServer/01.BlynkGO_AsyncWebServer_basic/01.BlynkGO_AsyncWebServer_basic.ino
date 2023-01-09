/*************************************************************
 * ตัวอย่างนี้เป็นตัวอย่าง การตั้ง WebServer แบบ Asynchronous
 * WebServer แบบนี้จะดีกว่า WebServer ที่มากับ Arduino ESP32
 * คือ จะไม่มีการหยุดรอจนกว่า client กับ server สื่อสารกันสำเร็จ 
 * ทำให้เวลาระหว่างนั้น สามารถไปควบคุมอุปกรณ์รอบข้าง หรือทำอย่างอื่นๆได้ก่อน
 *  
 * ตัวอย่างจะมีการรับค่า input ผ่าน html  3 ช่อง input แบบง่ายๆ
 * แล้วนำมาแสดงบนหน้าจอ TFT
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

GWiFiManager wifi_manager;

namespace AsyncWEBSERVER {
  void begin();
  void end();
}

GContainer con_inputs;
  GLabel lb_input1(con_inputs);
  GLabel lb_input2(con_inputs);
  GLabel lb_input3(con_inputs);

GLabel lb_ipaddress;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // ออกแบบ graphic
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);
  lb_ipaddress.align(ALIGN_TOP,0,10);
  
  con_inputs.layout(LAYOUT_COL_L);          // จัด layout ให้ลูกๆ เรียงแบบ column ชิดซ้ายตรงกัน
  con_inputs.padding(10,10,10,10,5);        // จัดช่องไฟบน-ล่าง-ซ้าย-ขวา เป็น 10px ส่วนช่องไฟระหว่างลูกๆ ห่างกัน 5px
  con_inputs.font(prasanmit_25, TFT_BLACK); // จัดฟอนต์ให้ ลูกๆ เป็นฟอนต์ และสีตามต้องการ  
    lb_input1 = "INPUT1";
    lb_input2 = "INPUT2";
    lb_input3 = "INPUT3";
}

void loop() {
  BlynkGO.update();
}

// เมื่อ WiFi เชื่อมต่อสำเร็จ
WIFI_CONNECTED(){
  lb_ipaddress = "http://"+ WiFi.localIP().toString();
  // ให้ AsyncWEBSERVER เริ่มทำงาน
  AsyncWEBSERVER::begin();
}

// เมื่อ WiFi หลุดการเชื่อมต่อ
WIFI_DISCONNECTED(){
  lb_ipaddress = "WiFi ไม่มีการเชื่อมต่อ";
 // ให้ AsyncWEBSERVER จบการทำงาน
  AsyncWEBSERVER::end();
}

