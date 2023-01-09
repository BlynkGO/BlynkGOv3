/***********************************************************
 * ตัวอย่างนาฬิกา สำหรับจอขนาด 3.5 นิ้วขึ้นไป
 *   โดยมีการใช้สีระบบ HSV ในการกำหนดสีแต่ละ ขีดของหน้าปัดนาฬิกา 
 *   ตั้งค่า WiFi ผ่านทาง GWiFiManager
 *   
 *   ให้เปิดใช้ WiFi, NTP ที่ blynkgo_config.h โดยกำหนด
 *   #define BLYNKGO_USE_WIFI     1
 *   #define BLYNKGO_USE_NTP      1
 *   
 ***********************************************************/

#include <BlynkGOv3.h>

GWiFiManager wifi_manager;

GRect my_Watch;
  GCircle cir_60_ticks[60];
  GContainer cont_clock(my_Watch);
    GLabel lb_hour  ("00", cont_clock);
    GLabel lb_colon (":" , cont_clock);
    GLabel lb_minute("00", cont_clock);
  
void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);

  my_Watch.size(280,280);                       // กำหนดขนาด กว้างx สูง
  my_Watch.opa(30);                             // โปร่งใส 30 จากทั้งหมด 255
  my_Watch.dragable(true);
  for(int i=0;i < 60; i++){
    cir_60_ticks[i].parent(my_Watch);           // ให้มี layer แม่เป็น my_Watch
    cir_60_ticks[i].radius(2);

    // กำหนดสี ด้วยการใช้สีจากระบบสี HSV
    // ระบบสี HSV --> Hue 0-360 deg; Saturation 0-100; Value 0-100
    cir_60_ticks[i].color( TFT_COLOR_HSV(i*6,100,100) ); 

    // สร้างจุด ที่เกิดจากรัศมี 150 ทำมุม i*6 โดยมีมุมเริ่มต้น 270 รอบจุดกึ่งกลาง my_Watch แบบตามเข็ม
    Point p = Point(120, i*6, 270, my_Watch.centerpoint(), CLOCKWISE);
    cir_60_ticks[i].center(p);
  }
  
  cont_clock.layout(LAYOUT_ROW_M);
  cont_clock.padding(0,0,0,0,5);
  cont_clock.opa(0);
    lb_hour.font(BebasNeueBold_num_45);
    lb_colon.font(BebasNeueBold_num_45);
    lb_minute.font(BebasNeueBold_num_45);
}

void loop() {
  BlynkGO.update();
}

NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000,[](){
    static uint8_t s;
    cir_60_ticks[s].radius(2);                        // cir_60_ticks ณ s เดิม   ให้ รัศมีเป็น 2
    cir_60_ticks[s = second()].radius(7);             // cir_60_ticks ณ s ปัจจุบัน ให้ รัศมีเป็น 7

    uint16_t angle_hour = esp32_mod(hour(),12)* 30;   // 1 ชม. = 30 องศา
    uint16_t angle_minute = minute()*6;               // 1 นาที = 6 องศา
    lb_hour  .raw_text( StringX::printf("%02d", hour())   );  lb_hour  .color( TFT_COLOR_HSV( angle_hour   ,100,100));
    lb_minute.raw_text( StringX::printf("%02d", minute()) );  lb_minute.color( TFT_COLOR_HSV( angle_minute ,100,100));    
  });
}


