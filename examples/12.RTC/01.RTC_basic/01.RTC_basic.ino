/********************************************************************
 * ตัวอย่าง โมดูลนาฬิกา RTC DS3231 , DS1307, PCF8523, PCF8563
 *   
 * วิธีการต่อสาย แบบ ใช้ I2C ร่วมกับ Touch I2C
 *    - BlynkGO Board V1.1-1.3
 *        ต่อ RTC SDA --> ขา 26 (I2C SDA)
 *        ต่อ RTC SCL --> ขา 27 (I2C SCL)
 *    - BlynkGO Emma-II
 *        ต่อ RTC SDA --> ขา 18 (I2C SDA)
 *        ต่อ RTC SCL --> ขา 19 (I2C SCL)
 * 
 *  หมายเหตุ 
 *     หาก ต้องการกำหนดขา I2C แยกจาก Touch I2C
 *     ให้กำหนด ขาที่ต้องการได้ ใน blynkgo_config.h เช่น
 *        #define RTC_I2C_SDA        25
 *        #define RTC_I2C_SCL        4
 *
 *******************************************************************
 * วิธีการใช้
 *   1. ให้ตั้งเวลา RTC ให้ตรงก่อน โดยรอบแรกจะตั้งเวลาให้แม่นยำ
 *      ด้วยเวลาจาก NTP internet
 *      
 *      ก่อน compile ที่ไฟล์ blynkgo_config.h ให้ตั้ง
 *         #define  BLYNKGO_USE_WIFI            1
 *         #define  BLYNKGO_USE_NTP             1
 *
 *      ส่วน RTC เปิดใช้ตาม RTC chip ที่ใช้  (อย่างใดอย่างหนึ่ง)
 *         #define  BLYNKGO_USE_RTC_DS323X      1
 *      หรือ
 *         #define  BLYNKGO_USE_RTC_DS1307      1
 *      หรือ
 *         #define  BLYNKGO_USE_RTC_PCF8523     1
 *      หรือ
 *         #define  BLYNKGO_USE_RTC_PCF8563     1
 *
 *      จากนั้น save แล้ว upload โคดนี้
 *      เมื่อ wifi และ NTP ซิงค์แล้ว เวลาจาก NTP internet
 *      จะมีการมาจูนให้ RTC มีความแม่นยำตามเวลา internet ให้อัตโนมัติ
 *      
 *      
 *   2. รอบถัดๆ สามารถปิด WiFi และ NTP โดยกำหนดใน blynkgo_config.h เป็น
 *
 *         #define  BLYNKGO_USE_WIFI            0
 *         #define  BLYNKGO_USE_NTP             0
 *      
 *       เหลือเพียง RTC ที่ใช้ เช่น
 *         #define  BLYNKGO_USE_RTC_DS323X      1
 *      
 *      แม้ไม่ใช้ WiFi internet 
 *      ก็ยังสามารถใช้ API คำสั่ง
 *      hour(), minute(), second(), day(), month(), year(), th_year(), wday(), timestamp()
 *      ซึ่งเป็นเวลาของระบบ ณ เวลาปัจจุบัน ที่ได้มีการซิงค์เวลาจาก RTC
 *      โดย RTC ก็ได้มีจูนให้ความแม่นยำเรียบร้อยจาก NTP ที่ได้ซิงค์ในรอบแรกแล้ว
 * 
 ********************************************************************/

#include <BlynkGOv3.h>

#if BLYNKGO_USE_WIFI && BLYNKGO_USE_NTP
GWiFiManager wifi_manager;
#endif

GLabel lb_clock;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
    
#if BLYNKGO_USE_WIFI && BLYNKGO_USE_NTP
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);
#endif

  lb_clock.font(prasanmit_40, TFT_WHITE);
  static GTimer timer_clock;
  timer_clock.setInterval(1000,[](){
    lb_clock = StringX::printf("%02d:%02d:%02d\n", hour(), minute(), second());
  });
}

void loop() {
  BlynkGO.update();
}

