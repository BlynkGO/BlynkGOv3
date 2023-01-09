/*********************************************************************
 * 
 * Framework แบบ multiscreen
 *   - มี WiFiManager, BlynkManager 
 *   - ผู้สร้างโปรเจค สามารถกำหนดจำนวน screen และ title ของแต่ละ screen
 *   - ผู้สร้างโปรเจค สามารถกำหนดจัดวาง วิตเจ็ตลูกๆไปตามแต่ละหน้า screen ต่างๆ
 *
 * ---------------------------------------
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *     #define BLYNKGO_USE_BLYNK         1
 *
 *********************************************************************/

#include <BlynkGOv3.h>

//--------------------------------------------------------------------
// User Config
#define SCREEN_NUM      4     // กำหนดว่ามีทั้งหมดกี่ screen
String titles[SCREEN_NUM] = { "SmartHome", "Sensors", "Gauge", "About" };   // กำหนด ไตเติ้ล ของแต่ละ screen

//--------------------------------------------------------------------

GMultiScreen  screens;

//--------------------------------------------------------------------
// User Widgets
// เพิ่มวิตเจ็ต ต่างๆ ตรงนี้
GLabel        lb_childs[SCREEN_NUM];

//--------------------------------------------------------------------
void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  framework_design();

  //----------------------------------------------------
  // User Design
  // ให้ออกแบบวาง วิตเจ็ตไปตาม screen ต่างๆในส่วนนี้
  // โดยใช้ API คำสั่ง parent(screens[หน้า])  ที่ต้องการ
  
  for(int i=0; i < SCREEN_NUM; i++){
    lb_childs[i].parent(screens[i]);
    lb_childs[i] = "Screen " + String(i+1);
  }
  
}

void loop(){
  BlynkGO.update();
}

