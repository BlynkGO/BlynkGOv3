/***********************************************************
 * ตัวอย่างนาฬิกา สำหรับจอขนาด 3.5 นิ้วขึ้นไป
 *   โดยมีการใช้สีระบบ HSV ในการกำหนดสีแต่ละ ขีดของหน้าปัดนาฬิกา
 *   ด้วยเวลาจาก RTC DS3231, DS1307, PCF8523, PCF8563
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

IMG_DECLARE(img_amazfit_watchface);
FONT_DECLARE(veganova_black_num_35);
FONT_DECLARE(veganova_black_num_50);

GImage amazfit(img_amazfit_watchface);
  GLabel lb_hour  ("00", amazfit);
  GLabel lb_minute("00", amazfit);
  GLabel lb_second("00", amazfit);
  
void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // ออกแบบ Graphic
  amazfit.align_center();
    lb_hour  .font (veganova_black_num_50);
    lb_hour  .align(ALIGN_TOP_RIGHT, 110-amazfit.width(),131);
    lb_minute.font (veganova_black_num_50);
    lb_minute.align(lb_hour, ALIGN_RIGHT, 8);
    lb_second.font (veganova_black_num_35, TFT_COLOR_HEX(0xAAAAAA));
    lb_second.align(lb_minute, ALIGN_RIGHT,7,3);

  // ตั้งเวลาทุกๆ วินาที ดึงเวลาจากระบบที่ได้รับการซิงค์จาก RTC
  // ด้วย hour(), minute(), second()
  static GTimer timer_clock;
  timer_clock.setInterval(1000,[](){
    lb_hour  .raw_text(StringX::printf("%02d" , hour()));
    lb_minute.raw_text(StringX::printf("%02d" , minute()));
    lb_second.raw_text(StringX::printf("%02d" , second()));
  });
}

void loop() {
  BlynkGO.update();
}
