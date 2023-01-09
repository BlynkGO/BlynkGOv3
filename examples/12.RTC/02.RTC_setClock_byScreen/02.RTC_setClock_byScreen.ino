/******************************************************
 * ตัวอย่างตั้งเวลา RTC DS3231, DS1307, PCF8523, PCF8563 ผ่านหน้าจอ 
 *    - ปาดขึ้น/ลง เพื่อปรับเวลา
 *    - กดค้างเพื่อ reset ให้เป็น '00'
 *      (วินาที กดค้าง ได้อย่างเดียว)
 *
 * -----------------------------------------------
 * วิธีใช้
 *    ก่อน compile ให้ตั้งใน blynkgo_config.h 
 *    เปิดใช้งาน RTC ตาม RTC chip ที่ใช้  (อย่างใดอย่างหนึ่ง)
 *         #define  BLYNKGO_USE_RTC_DS323X      1
 *      หรือ
 *         #define  BLYNKGO_USE_RTC_DS1307      1
 *      หรือ
 *         #define  BLYNKGO_USE_RTC_PCF8523     1
 *      หรือ
 *         #define  BLYNKGO_USE_RTC_PCF8563     1
 * 
 ******************************************************/

#include <BlynkGOv3.h>

FONT_DECLARE(AftikaBold_60);

GContainer  cont_clock;
  GContainer  cont_hour(cont_clock);
    GLabel lb_hour("00", cont_hour);
  GContainer  cont_minute(cont_clock);
    GLabel lb_minute("00", cont_minute);
  GContainer  cont_second(cont_clock);
    GLabel lb_second("00", cont_second);
  GLabel lb_colon[2]= {{":",cont_clock},{":",cont_clock}};

GLabel lb_clock_setting = "ตั้งเวลา RTC ผ่านหน้าจอ";
GLabel lb_info = "- ปาดขึ้น/ลง เพื่อปรับตั้งค่าเวลา\n- กดค้างเพื่อ reset เป็น '00'";

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  cont_clock.layout(LAYOUT_ROW_M);
  cont_clock.padding(0,0,0,0,2);
  cont_clock.opa(0);
    cont_hour.width(100);
    cont_hour.border(1, TFT_DARKGRAY);
    cont_hour.color(TFT_BLUE, TFT_CYAN);
    cont_hour.corner_radius(20);
    cont_hour.font(AftikaBold_60, TFT_BLACK);
    cont_hour.layout(LAYOUT_CENTER);
    cont_hour.clickable(true);
    cont_hour.gesture(true);
    cont_hour.onLongPressedRepeat([](GWidget*widget){
      static int8_t count = 0;
      count++;
      if(count == 3){
        count = 0;
        BlynkGO.RTC.set(day(), month(),year(), 0, minute(), second());
        lb_hour  .raw_text(StringX::printf("%02d", hour()));
      }
    });
    cont_hour.onGesture([](GWidget*widget){
      gesture_dir_t gesture = widget->touch_gesture();                 // รูปแบบการปาดนิ้ว      
      int8_t h = hour();
      switch(gesture){
        case GESTURE_DIR_TOP:
          h = (h+1<24)? h+1 : 0;
          BlynkGO.RTC.set( day(), month(), year(), h, minute(), second());
          lb_hour = StringX::printf("%02d", hour());
          break;     // ปาดขึ้นข้างบน
        case GESTURE_DIR_BOTTOM:
          h = (h-1>=0)? h-1 : 23;
          BlynkGO.RTC.set( day(), month(), year(), h, minute(), second());        
          lb_hour = StringX::printf("%02d", hour());
          break;     // ปาดลงข้างล่าง
      }
    });
      lb_hour.create();

    lb_colon[0].font(AftikaBold_60, TFT_WHITE);
    
    cont_minute.width(100);
    cont_minute.border(1, TFT_DARKGRAY);
    cont_minute.color(TFT_BLUE, TFT_CYAN);
    cont_minute.corner_radius(20);
    cont_minute.font(AftikaBold_60, TFT_BLACK);
    cont_minute.layout(LAYOUT_CENTER);
    cont_minute.clickable(true);
    cont_minute.gesture(true);
    cont_minute.onLongPressedRepeat([](GWidget*widget){
      static int8_t count = 0;
      count++;
      if(count == 3){
        count = 0;
        BlynkGO.RTC.set(day(), month(),year(), hour(), 0, second());
        lb_minute.raw_text(StringX::printf("%02d", minute()));
      }
    });
    cont_minute.onGesture([](GWidget*widget){
      gesture_dir_t gesture = widget->touch_gesture();                 // รูปแบบการปาดนิ้ว      
      int8_t m = minute();
      switch(gesture){
        case GESTURE_DIR_TOP:
          m = (m+1<60)? m+1 : 0;
          BlynkGO.RTC.set(day(), month(),year(), hour(), m, second());
          lb_minute.raw_text(StringX::printf("%02d", minute()));
          break;     // ปาดขึ้นข้างบน
        case GESTURE_DIR_BOTTOM:
          m = (m-1>=0)? m-1 : 59;
          BlynkGO.RTC.set(day(), month(),year(), hour(), m, second());
          lb_minute.raw_text(StringX::printf("%02d", minute()));
          break;     // ปาดลงข้างล่าง
      }
    });
      lb_minute.create();

    lb_colon[1].font(AftikaBold_60, TFT_WHITE);

    cont_second.width(100);
    cont_second.border(1, TFT_DARKGRAY);
    cont_second.color(TFT_BLUE, TFT_CYAN);
    cont_second.corner_radius(20);
    cont_second.font(AftikaBold_60, TFT_BLACK);
    cont_second.layout(LAYOUT_CENTER);
    cont_second.clickable(true);
    cont_second.onLongPressedRepeat([](GWidget*widget){
      static int8_t count = 0;
      count++;
      if(count == 3){
        count = 0;
        BlynkGO.RTC.set(day(), month(),year(), hour(), minute(), 0);
        lb_second.raw_text(StringX::printf("%02d", second()));
      }
    });
      lb_second.create();

  lb_clock_setting.font(prasanmit_40);
  lb_clock_setting.align(cont_clock, ALIGN_TOP,0,-30);
  lb_info.line_space(-5);
  lb_info.align(cont_clock, ALIGN_BOTTOM,0,30);
  
  //------------------------------------------------------
  static GTimer timer_clock;
  timer_clock.setInterval(1000,[](){
    lb_hour   = StringX::printf("%02d", hour());
    lb_minute = StringX::printf("%02d", minute());
    lb_second = StringX::printf("%02d", second());
  });
}

void loop() {
  BlynkGO.update();
}


