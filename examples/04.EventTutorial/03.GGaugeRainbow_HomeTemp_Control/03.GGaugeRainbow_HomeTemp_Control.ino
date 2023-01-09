/**********************************************************************
 * ตัวอย่างนี้เป็นการประยุกต์ ใฃ้วิตเจ็ต GGaugeRainbow แบบเต็มวง (GAUGE_TYPE_FULL)
 * มาทำเป็นตัวตั้งอุณหภูมิ ไปพร้อมๆกับแสดงอุณหภูมิ โดยใช้ sensor SHT31
 * 
 * การตั้งอุณหภูมิ
 *   - หมุนปาดนิ้วเป็นวงกลม ที่ ขอบของตัวเกจจ์ 
 *       หมุนตามเข็ม = เพิ่มค่าทีละ 5 องศา
 *       หมุนทวนเข็ม = ลดค่าทีละ 5 องศา
 *   - ปาดนิ้วขึ้นลง
 *       ปาดนิ้วขึ้น = เพิ่มค่าทีละ 1 องศา
 *       ปาดนิ้วลง = ลดค่าทีละ 1 องศา
 * 
 * การแสดงอุณหภูมิปกติ
 *   - ตัวอย่างนี้ใช้ เซนเซอร์ SHT31
 *      ให้ติดตั้งไลบรารี่เพิ่มเติมให้เรียบร้อยก่อน คอมไพล์
 *       1. ไลบรารี่ Adafruit_BusIO โหลดไลบรารี่เพิ่มเติมได้ที่ https://github.com/adafruit/Adafruit_BusIO
 *       2. ไลบรารี่ Adafruit_SHT31 โหลดไลบรารี่เพิ่มเติมได้ที่ https://github.com/adafruit/Adafruit_SHT31
 *       
 *   - ให้ต่อ sensor SHT31 I2C ที่ขา 26 (SCL), 27 (SCA)
 *     ของ BlynkGO Board
 *     
 **********************************************************************/

#include <BlynkGOv3.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

#define TFT_MY_ORANGE     TFT_COLOR_HEX(0xfa3e00)
#define TFT_MY_BLUE       TFT_COLOR_HEX(0x002cbd)

FONT_DECLARE(BebasNeueBold_num_60);

GRect   app_hometemp_control;
GLabel  lb_title(app_hometemp_control);     // วิตเจ็ตข้อความ แสดง title
GCircle cir_outside(app_hometemp_control);  // วิตเจ็ตวงกลม ที่อยู่รอบนอก เมื่ออยู่ในโหมด HOME_TEMP_SETTING
GCircle cir_inside(app_hometemp_control);   // วิตเจ็ตวงกลม ที่อยู่ภายใน เมื่ออยู่ในโหมด HOME_TEMP_SETTING
GGaugeRainbow gauge(app_hometemp_control);  // วิตเจ็ตเกจจ์สีรุ้ง
GLabel  lb_unit(SYMBOL_DEGREE "C",gauge);   // วิตเจ็ตข้อความ แสดง หน่วย ให้เป็นเลเยอร์ลูกของ วิตเจ็ต gauge
GLabel  lb_status_updown(gauge);            // วตเจ็ตข้อความ แสดง สถานะเพิ่มหรือลด ให้เป็นเลเยอร์ลูกของ วิตเจ็ต gauge

float temp_setting_value = 18;              // อุณหภูมิ สำหรับตั้งค่า
float temp_normal_value  = 23;              // อุณหภูมิ (จำลอง) สำหรับแสดงค่าปัจจุบัน (ใช้จริงให้อ่านจาก sensor

Adafruit_SHT31 sht31 = Adafruit_SHT31();
GTimer         timer_sht31;

//------------------------------------
enum { HOME_TEMP_NORMAL, HOME_TEMP_SETTING };
typedef uint8_t home_temp_mode_t;
home_temp_mode_t home_temp_mode = HOME_TEMP_NORMAL;
GTimer  timer_mode_normal;
GTimer  timer_update_status_updown;

// เช็ตโหมด เป็นโหมดแสดงอุณหภูมิปกติ HOME_TEMP_NORMAL หรือ แบบตั้งค่าอุณหภูมิ HOME_TEMP_SETTING
void set_mode(home_temp_mode_t mode );
// คืนค่าโหมดปัจจุบัน
home_temp_mode_t get_mode();
//------------------------------------

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  if (!sht31.begin(0x44)) {
    Serial.println("[SHT31] Couldn't find SHT31");
  }else{
    timer_sht31.setInterval( 1000L,[](){
      float t = sht31.readTemperature();
      float h = sht31.readHumidity();
      if( !isnan(t)){
        temp_normal_value = t;
        if( get_mode() == HOME_TEMP_NORMAL ){
          gauge = temp_normal_value;
        }
      }
    });
  }

  app_hometemp_control.size(GScreen);
  app_hometemp_control.color(TFT_BLACK);
  
  lb_title = "SmartHome Temperature";
  lb_title.font(prasanmit_40);
  lb_title.align(ALIGN_TOP,0,5);

  cir_outside.radius(125);
  cir_outside.clickable(false);
  cir_outside.opa(0);
  cir_outside.border(4, TFT_MY_ORANGE);
  cir_outside.align(lb_title, ALIGN_BOTTOM,0,10);
  cir_outside.hidden(true);  

  cir_inside.radius( 80);
  cir_inside.color(TFT_MY_ORANGE);
  cir_inside.align(cir_outside, ALIGN_CENTER);
  cir_inside.hidden(true);

  gauge.size(230,230);
  gauge.gauge_type(GAUGE_TYPE_FULL);
  gauge.color(TFT_NAVY, TFT_RED);
  gauge.font(BebasNeueBold_num_60);
  gauge.range(0, 50);
  gauge.label().align_center(0,-10);
    lb_unit.font(prasanmit_40);
    lb_unit.align( gauge.label(), ALIGN_BOTTOM,0,-10);

    lb_status_updown.font(prasanmit_40, TFT_SILVER);
    lb_status_updown.c_align(gauge.label(), ALIGN_CENTER, 55);
    timer_update_status_updown.setInterval(1000,[](){
      if( temp_normal_value > temp_setting_value ){
        lb_status_updown = SYMBOL_ARROW_DOWN;
        cir_outside.border_color(TFT_MY_BLUE);
        cir_inside.color(TFT_MY_BLUE);
      }else
      if( temp_normal_value < temp_setting_value ){
        lb_status_updown = SYMBOL_ARROW_UP;
        cir_outside.border_color(TFT_MY_ORANGE);
        cir_inside.color(TFT_MY_ORANGE);
      }else{
        lb_status_updown = "";
        cir_outside.border_color(TFT_MY_ORANGE);
        cir_inside.color(TFT_MY_ORANGE);
      }      

    });

  gauge = temp_normal_value;
  gauge.align(cir_outside, ALIGN_CENTER);
    
  gauge.clickable(true);    // เปิดให้รับสัญญาณ EVENT การกดสัมผัสได้
  gauge.gesture(true);      // เปิดให้รับสัญญาณ EVENT การปาดนิ้วได้
  gauge.event_cb([](GWidget*widget, event_t event){       // ดักสัญญาณ EVENT ต่างๆที่มาที่ gauge
    static Point    c     = gauge.centerpoint();
    static uint16_t cir_R = gauge.width()/2;              // รัศมี เกจจ์สีรุ้ง เป็น รัศมีวงนอก
    static uint16_t cir_r = cir_R - gauge.thickness();    // รัศมี เกจจ์สีรุ้ง ที่หักระยะ ความหนาของเกจจ์ เป็น รัศมีวงใน
    static Point p_start;                                 // จุดสัมผัสแรกสุด
    static float angle_start;                             // มุมของจุดสัมผัสแรกสุด
    Point p;
    float d, d_start;
    float angle;
    float angle_diff;
    
    // เมื่อมีการกดสัมผัสแรกเข้ามาที่วงกลม
    switch(event){
      case EVENT_PRESSED:                     // เมื่อเริ่มมีสัญญาณกดเข้ามา
        p_start     = gauge.touch_point();    // รับพิกัดจุดที่สัมผัส
        angle_start = p_start.angle_deg(c);   // เปลี่ยนเป็นมุมรอบจุด c ตามระนาบ xy หมุนทวนเข็มตามระบบ Math
        d_start     = p_start.distance(c);    // ระยะจากจุดสัมผัสแรก ไปยังจุดศก.
        timer_mode_normal.del();              // ยกเลิกตั้งเวลาที่เคยตั้งเดิม
        break;
      case EVENT_PRESSING:                    // เมื่อกำลังกดอยู่
        p = gauge.touch_point();              // รับพิกัดจุดที่สัมผัส
        d = p.distance(c);                    // หาระยะห่างระหว่างจุด p และ จุด c
        if( d > cir_R || d < cir_r ) return;  // หากจุดสัมผัสไม่ได้อยู่บนพื้นที่วงแหวน ให้คืนค่ากลับไปไม่ต้องทำอะไร
  
        angle       = p.angle_deg(c);         // มุมรอบจุด c ตามระนาบ xy หมุนทวนเข็มตามระบบ Math
        angle_diff  = ANGLE180( angle - angle_start); // ผลต่างมุม เทียบมุมเริ่มต้นสัมผัส โดยปรับมุมให้อยู่ระหว่างมากกว่า -180 องศา ไปจนถึง 180 องศา


        if( abs(angle_diff) >= 5 ) {              // หากมุมสัมผัสปัจจุบันที่บริเวณ เกจจ์ ต่างจากเมื่อแรกสัมผัสไป 5 องศา
          if( get_mode() == HOME_TEMP_NORMAL) {   // ให้เปลี่ยนจาก โหมด HOME_TEMP_NORMAL เป็นโหมด HOME_TEMP_SETTING
            set_mode(HOME_TEMP_SETTING);
          }

          if(angle_diff < 0 ) {
            // หากจุดที่สัมผัสล่าสุด มีผลต่างมุมติดลบถึง -5 องศา จากจุดเริ่มสัมผัส ให้เพิ่มค่า  โดยให้อยู่ภายในช่วงที่ value_min ถึง value_max
            gauge = constrain(temp_setting_value + 5, 10, gauge.value_max());
            temp_setting_value = gauge.value();
          }else{
            // หากจุดที่สัมผัสล่าสุด มีผลต่างมุม ถึง 5 องศา จากจุดเริ่มสัมผัส ให้ลดค่า  โดยให้อยู่ภายในช่วงที่ value_min ถึง value_max 
            gauge = constrain(temp_setting_value - 5, 10, gauge.value_max());
            temp_setting_value = gauge.value();
          }
          p_start = p;
          angle_start = p_start.angle_deg(c);
        }
        break;
      case EVENT_PRESS_LOST:                  // เมื่อหลุดจากการกดสัมผัส ออกนอกบริเวณวิตเจ็ต gauge
      case EVENT_RELEASED:                    // เมื่อมีการปล่อยจากการกดสัมผัส
        if( get_mode() == HOME_TEMP_SETTING) {
          set_mode(HOME_TEMP_NORMAL);         // คืนสถานะสู่โหมดปกติ HOME_TEMP_NORMAL
        }
        break;
      case EVENT_GESTURE:                     // เมื่อมีสัญญาณการปาดนิ้วเข้ามา
        if( d_start > cir_r ) return;         // หากจุดที่สัมผัสแรก อยู่ ภายนอก รัศมีวงใน ไม่ต้องทำอะไร
          
        if( get_mode() == HOME_TEMP_NORMAL){  // ให้เปลี่ยนจาก โหมด HOME_TEMP_NORMAL เป็นโหมด HOME_TEMP_SETTING
          set_mode(HOME_TEMP_SETTING);
        }

        switch(gauge.touch_gesture()){
          case GESTURE_DIR_TOP:               // เมื่อปาดน้ำขึ้น
            gauge = constrain(temp_setting_value + 1, 10, gauge.value_max());
            temp_setting_value = gauge.value();

            break;
          case GESTURE_DIR_BOTTOM:            // เมื่อปาดน้ำลง
            gauge = constrain(temp_setting_value - 1, 10, gauge.value_max());
            temp_setting_value = gauge.value();
            break;
        }
        break;
    }
  });

  app_hometemp_control.clickable(true);
  app_hometemp_control.gesture(true);
  app_hometemp_control.event_cb([](GWidget*widget, event_t event){
    switch(event){
      case EVENT_PRESSED:
        if( get_mode() == HOME_TEMP_NORMAL){  // ให้เปลี่ยนจาก โหมด HOME_TEMP_NORMAL เป็นโหมด HOME_TEMP_SETTING
          set_mode(HOME_TEMP_SETTING);
        }
        break;
      case EVENT_PRESS_LOST:                  // เมื่อหลุดจากการกดสัมผัส ออกนอกบริเวณวิตเจ็ต gauge
      case EVENT_RELEASED:                    // เมื่อมีการปล่อยจากการกดสัมผัส
        if( get_mode() == HOME_TEMP_SETTING) {
          set_mode(HOME_TEMP_NORMAL);         // คืนสถานะสู่โหมดปกติ HOME_TEMP_NORMAL
        }
        break;
      case EVENT_GESTURE:   
        if( get_mode() == HOME_TEMP_NORMAL){  // ให้เปลี่ยนจาก โหมด HOME_TEMP_NORMAL เป็นโหมด HOME_TEMP_SETTING
          set_mode(HOME_TEMP_SETTING);
        }
        switch(widget->touch_gesture()){
          case GESTURE_DIR_TOP:               // เมื่อปาดน้ำขึ้น
            gauge = constrain(temp_setting_value + 1, 10, gauge.value_max());
            temp_setting_value = gauge.value();
            break;
          case GESTURE_DIR_BOTTOM:            // เมื่อปาดน้ำลง
            gauge = constrain(temp_setting_value - 1, 10, gauge.value_max());
            temp_setting_value = gauge.value();
            break;
        }
        break;
    }
  });
}

void loop() {
  BlynkGO.update();
}

// เช็ตโหมด เป็นโหมดแสดงอุณหภูมิปกติ HOME_TEMP_NORMAL หรือ แบบตั้งค่าอุณหภูมิ HOME_TEMP_SETTING
void set_mode(home_temp_mode_t mode ){
  
  if(mode == HOME_TEMP_SETTING){
    timer_mode_normal.del();                  // ยกเลิกตั้งเวลาที่เคยตั้งเดิม
    home_temp_mode = HOME_TEMP_SETTING;
    cir_outside.hidden(false);
    cir_inside.hidden(false);
    lb_title = "Temperature Setting";
    temp_normal_value = gauge.value();
    gauge = temp_setting_value;
  }else{
    timer_mode_normal.setOnce(2000L,[](){     // ตั้งเวลาอีก 2 วินาที ให้กลับสู่โหมดปกติ
      home_temp_mode = HOME_TEMP_NORMAL;
      cir_outside.hidden(true);
      cir_inside.hidden(true);
      lb_title = "SmartHome Temperature";
      temp_setting_value = gauge.value();
      gauge = temp_normal_value;
    });
  }
}

// คืนค่าโหมดปัจจุบัน
home_temp_mode_t get_mode(){
  return home_temp_mode;
}

