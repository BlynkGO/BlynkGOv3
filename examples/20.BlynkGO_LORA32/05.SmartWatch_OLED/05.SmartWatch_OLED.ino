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

FONT_DECLARE(prasanmit_15);

#define SSID            "-------------"
#define PASSWORD        "-------------"


GCircle cir_clock;
GLabel lb_dial_tick[12];
GLine  line_hour_pin(cir_clock);
GLine  line_minute_pin(cir_clock);
GLine  line_second_pin(cir_clock);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  WiFi.begin( SSID, PASSWORD);

  cir_clock.radius(32);
  cir_clock.opa(0);
  cir_clock.border(1,TFT_WHITE);

  Point c = cir_clock.centerpoint();   // จุดศูนย์กลางวงกลม เทียบภายใน cir_clock
  for(int i=0; i < 12; i++){
    // สร้างจุด ด้วยแขน 24pixel หมุนมุมไป i*30 องศา โดยมีมุมเริ่มต้น 270 องศา จากแกน x+ รอบจุด c แบบตามเข็มนาฬิกา 
    Point p = Point(24, i*30, 270, c, CLOCKWISE);  
    lb_dial_tick[i].parent(cir_clock);
    lb_dial_tick[i].center(p);               // กำหนดตำแหน่ง
    lb_dial_tick[i].font(prasanmit_15);
    lb_dial_tick[i] = String((i>0)? i : 12); // ใส่ข้อความให้ วิตเจ็ตข้อความ
  }

  line_hour_pin  .thickness(5, TFT_WHITE); line_hour_pin  .rounded(true);
  line_minute_pin.thickness(3, TFT_WHITE); line_minute_pin.rounded(true);
  line_second_pin.thickness(1, TFT_WHITE);

}

void loop() {
  BlynkGO.update();  
}


NTP_SYNCED(){
  static GTimer timer;
  // ตั้งเวลาทุกๆ 1 วินาที ให้ ดึงค่า hour(), minute(), second() มาเปลี่ยนเป็นมุม เพื่อลากเส้นทำเข็มนาฬิกา
  timer.setInterval(1000L, [](){
    // จุดศูนย์กลางวงกลม เทียบภายใน cir_clock
    Point c = cir_clock.centerpoint();

    // ลากเส้นจากจุดศ.ก. ไปยังจุดที่มี แขนยาว 12pixel ด้วยมุม (hour()%12)*30 องศา โดยมีมุมเริ่มต้น 270 องศา จากแกน x+ นับตามเข็มนาฬิกา 
    line_hour_pin   .setPoints(c, Point(12, (hour()  %12)*30, 270, c, CLOCKWISE)); 
    
    // ลากเส้นจากจุดศ.ก. ไปยังจุดที่มี แขนยาว 22pixel ด้วยมุม (minute()%60)*6 องศา โดยมีมุมเริ่มต้น 270 องศา จากแกน x+ นับตามเข็มนาฬิกา 
    line_minute_pin .setPoints(c, Point(22, (minute()%60)*6 , 270, c, CLOCKWISE));

    // ลากเส้นจากจุดศ.ก. ไปยังจุดที่มี แขนยาว 28pixel ด้วยมุม (second()%60)*6 องศา โดยมีมุมเริ่มต้น 270 องศา จากแกน x+ นับตามเข็มนาฬิกา 
    line_second_pin .setPoints(c, Point(28, (second()%60)*6 , 270, c, CLOCKWISE));
  });
}
