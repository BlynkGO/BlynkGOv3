/*******************************************************************
 * Framework แบบ LeftDockMenu 
 *   - Framework นี้ จะคล้าย Framework แบบ DockMenu 
 *     แต่จะอยู่ด้านซ้ายมือ แทนด้านล่าง
 *   - DockMenu สามารถเรียกขึ้นมาได้โดยการกดปุ่มเมนูที่ซ้ายบน
 *     หรือ จะปาดนิ้วไปทางขวา ก็ได้ และสามารถซ่อนอัตโนมัติ 
 *     หากไม่ได้มีการ click เลือกใดๆ
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

// ประกาศภาพก่อนใช้  เตรียมรูป icon ขนาด 80x80 pixel
// ตามจำนวน SCREEN_NUM
IMG_DECLARE(ico_home);
IMG_DECLARE(ico_setting);
IMG_DECLARE(ico_wifi);
IMG_DECLARE(ico_clock);
IMG_DECLARE(ico_alarm);
IMG_DECLARE(ico_gauge);
IMG_DECLARE(ico_weather);
IMG_DECLARE(ico_power);

// วิตเจ็ตปุ่มด้วยรูปภาพ โดยให้เป็น สวิตซ์แบบ กดปิดปล่อยดับ (BUTTON_PUSH) ที่จะไปอยู่บน DockMenu
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

// กำหนด ไตเติ้ล ของแต่ละ screens
String  titles[SCREEN_NUM] = { "SmartHome", "Setting", "WiFi", "Clock", 
                              "Alarm", "Gauge", "Weather", "Power" };   

GRect   screens[SCREEN_NUM];    // screens ต่างๆ ตามจำนวนปุ่มกด

//--------------------------------------------------------------------
// User Widgets : วิตเจ็ตที่ต้องการเพิ่มเอง ที่จะจัดวางตามหน้า screens[i] ต่างๆ
GLabel  lb_childs[SCREEN_NUM];

//--------------------------------------------------------------------

void setup() {
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

void loop() {
  BlynkGO.update();
}


