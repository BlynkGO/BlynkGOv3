/****************************************************************
 * ตัวอย่างนี้เป็นการแสดงเวลาจาก RTC ของ T-Watch ออก วิตเจ็ตข้อความ
 * โดย เมื่อเวลาได้รับการซิงค์จาก NTP แล้ว
 * RTC ของ T-Watch จะมีการ ปรับค่าให้แม่นยำตาม NTP ให้อัตโนมัติ
 * 
 * ตัวอย่างนี้จะไม่ทำการดึงเวลาจาก NTP มาใช้ตรงๆ
 * แต่จะใช้เวลาจาก RTC เป็นหลัก 
 * เนื่องจาก หากไม่มี internet 
 * เวลา จะยังคงสามารถแสดงได้ปกติ โดยดึงจาก RTC ของ T-Watch
 * 
 * ---------------------------------------
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 * 
 ****************************************************************/

#include <BlynkGOv3.h>

GWiFiManager wifi_manager;
GLabel lb_datetime;
GTimer rct_timer;
const char* th_weekday[] = {"อา.","จ.", "อัง.", "พ.", "พฤ.", "ศ.", "ส." };

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-5);
  lb_datetime.align(ALIGN_CENTER);
  lb_datetime.text_align(TEXT_ALIGN_CENTER);
  
  rct_timer.setInterval(1000L,[](){
    // เวลาที่ดึงจาก RTC ของ T-Watch มาแสดงที่วิตเจ็ตข้อความ
    // RTC_Date rct_date = TWatch.RTC.getDateTime(); // ดึงวันที่เวลาจาก RTC ของ T-Watch
    // lb_datetime = StringX::printf("%s%02d/%02d/%04d\n%02d:%02d:%02d", 
    //                 th_weekday[rct_date.weekday], rct_date.day, rct_date.month, rct_date.year,
    //                 rct_date.hour, rct_date.minute, rct_date.second);

    // หรือจะดึงจาก day(), month(), year(), hour(), minute(), second(), wday() ก็ได้
   lb_datetime = StringX::printf("%s%02d/%02d/%04d\n%02d:%02d:%02d", 
                   th_weekday[wday()], day(), month(), th_year(),
                   hour(), minute(), second());
  });
}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED(){
  Serial.println("[WiFi] Connected.");
}

