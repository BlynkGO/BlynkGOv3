/**********************************************************************
 *
 *  ก่อนใช้งาน
 *  เข้าไปที่ BlynkGOv3/src/config/blynkgo_config.h
 *    เปลี่ยนบอร์ดเป็น
 *       #define  BLYNKO_LORA32
 *
 *    และที่ ArduinoIDE เลือกบอร์ดเป็น BlynkGO Lora32
 *
 **********************************************************************/

/**********************************************************************
 * ตัวอย่างนี้ มีการใช้ WiFi และ NTP ในการ compile
 * ให้กำหนด เปิด การ compile ด้วย WiFi และ NTP ได้ที่
 * ไฟล์ BlynkGOv3/src/config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 **********************************************************************/


#include <BlynkGOv3.h>

#define SSID            "-------------"
#define PASSWORD        "-------------"

#if defined(BLYNKGO_BOARD_V1_1)
  #define TANK_W            150
  #define TANK_H            220
  #define TANK_BORDER       5
  #define TANK_R            20
  #define TANK_LEFT_PADDING 50
  #define TANK_WATER_COLOR  TFT_CYAN

  #define LABEL_FONT        prasanmit_40
  #define LABEL_PADDING     60
  #define LB_WIFI_OFFSET_X  5
  #define LB_WIFI_OFFSET_Y  -5
  
#elif defined(BLYNKGO_MINI)  
  #define TANK_W            70
  #define TANK_H            100
  #define TANK_BORDER       3
  #define TANK_R            15
  #define TANK_LEFT_PADDING 50

  #define TANK_WATER_COLOR  TFT_CYAN
  #define LABEL_FONT        prasanmit_30
  #define LABEL_PADDING     12
  #define LB_WIFI_OFFSET_X  5
  #define LB_WIFI_OFFSET_Y  -5

#elif defined(BLYNKGO_LORA32)
  #define TANK_W            40
  #define TANK_H            55
  #define TANK_BORDER       1
  #define TANK_R            8
  #define TANK_LEFT_PADDING 24
  #define TANK_WATER_COLOR  TFT_WHITE

  #define LABEL_FONT        prasanmit_20
  #define LABEL_PADDING     4
  #define LB_WIFI_OFFSET_X  0
  #define LB_WIFI_OFFSET_Y  -8
#endif

GLabel  lb_wifi_connected(SYMBOL_WIFI);

GRect tank;
  GRect tank_water(tank);
GLabel lb_water;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  WiFi.begin(SSID, PASSWORD);

  
  tank.size(TANK_W,TANK_H);
  tank.border(TANK_BORDER, TFT_WHITE);
  tank.opa(0);
  tank.corner_radius(TANK_R);
  tank.corner_mask(true);
  tank.align(ALIGN_LEFT,TANK_LEFT_PADDING);

    tank_water.size(tank.width()-2*tank.border(), tank.height()-2*tank.border());
    tank_water.color(TANK_WATER_COLOR); //TFT_WHITE);
    tank_water.align(ALIGN_BOTTOM,0,-tank.border());

    lb_water.font(LABEL_FONT);
    lb_water.align(tank, ALIGN_RIGHT,LABEL_PADDING);
    lb_water.text_line_space(-10);
    lb_water.text_align(TEXT_ALIGN_CENTER);

    int8_t water_level = 80;
    tank_water.height( map(water_level,0,100, 0, tank.height()-2*tank.border() ));

    static GTimer tank_timer;
    tank_timer.setInterval(100,[](){
      static int8_t water_level;
      static int8_t step = 1;
      
      tank_water.height( map(water_level,0,100, 0, tank.height()-2*tank.border() ));

      lb_water = StringX::printf("น้ำประปา\n %d L",  map(water_level,0,100, 0,2000));

      water_level += step;
      if( water_level > 100) { water_level = 100; step = -1; }
      else if( water_level <0) {water_level=0; step = 1; }
   }); 

  lb_wifi_connected.font(LABEL_FONT);
  lb_wifi_connected.align(ALIGN_TOP_LEFT,LB_WIFI_OFFSET_X,LB_WIFI_OFFSET_Y);
  lb_wifi_connected.hidden(true);
}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED(){
  lb_wifi_connected.hidden(false);
}

WIFI_DISCONNECTED(){
  lb_wifi_connected.hidden(true);
}
