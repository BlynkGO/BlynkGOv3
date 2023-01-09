/*********************************************************************
 * 
 * Framework แบบ LeftSidebar
 *   - มีแถบข้างด้านซ้ายมือ ให้ user เลือกเปลี่ยน screen
 *   - มี WiFiManager, BlynkManager 
 *   - ผู้สร้างโปรเจค สามารถเพิ่ม resource รูปสำหรับทำ ปุ่มกด ที่ sidebar สำหรับเปลียน screen ได้
 *   - ผู้สร้างโปรเจค สามารถกำหนดจัดวาง วิตเจ็ตลูกๆไปตามแต่ละ screen ต่างๆ
 *
 * ---------------------------------------
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 * หากจะใช้ Blynk Manager ร่วมด้วย
 * ให้กำหนด เปิด ใช้งาน Blynk ด้วย โดยกำหนดเป็น
 *     # define BLYNKGO_USE_BLYNK        1
 * 
 *********************************************************************/

#include <BlynkGOv3.h>

//--------------------------------------------------------------------
// User Config
#define SCREEN_NUM      8     // กำหนดว่ามีทั้งหมดกี่ screen

// ประกาศภาพก่อนใช้  โดยจัดเตรียมรูป icon ขนาด 80x80 pixel
IMG_DECLARE(ico_home);
IMG_DECLARE(ico_setting);
IMG_DECLARE(ico_wifi);
IMG_DECLARE(ico_clock);
IMG_DECLARE(ico_alarm);
IMG_DECLARE(ico_gauge);
IMG_DECLARE(ico_weather);
IMG_DECLARE(ico_power);

String titles[SCREEN_NUM] = { "SmartHome", "Setting", "WiFi", "Clock", 
                              "Alarm", "Gauge", "Weather", "Power" };   // กำหนด ไตเติ้ล ของแต่ละ screen

// วิตเจ็ตปุ่มด้วยรูปภาพ โดยให้เป็น สวิตซ์แบบ กดปิดปล่อยดับ (BUTTON_PUSH) ที่จะไปอยู่บน Left-Sidebar
GImageButton btns[SCREEN_NUM] = {
  {BUTTON_PUSH, ico_home},        
  {BUTTON_PUSH, ico_setting}, 
  {BUTTON_PUSH, ico_wifi}, 
  {BUTTON_PUSH, ico_clock}, 
  {BUTTON_PUSH, ico_alarm},  
  {BUTTON_PUSH, ico_gauge}, 
  {BUTTON_PUSH, ico_weather}, 
  {BUTTON_PUSH, ico_power}
};

GRect screens[SCREEN_NUM];

//--------------------------------------------------------------------
// User Widgets
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

