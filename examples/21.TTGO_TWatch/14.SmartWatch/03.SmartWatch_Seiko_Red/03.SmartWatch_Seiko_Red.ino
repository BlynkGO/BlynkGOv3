/**************************************************************
 * 
 * BlynkGO SmartWatch : Seiko Red
 * 
 **************************************************************/

#include <BlynkGOv3.h>

IMG_DECLARE(img_seikored_bg);
IMG_DECLARE(img_seikored_hr);
IMG_DECLARE(img_seikored_min);
IMG_DECLARE(img_seikored_sec);

GImage SeikoRed_bg (img_seikored_bg);                 // หน้าปัดพื้นหลัง
GImage SeikoRed_hr (img_seikored_hr  , SeikoRed_bg);  // เช็มสั้น   มีเลเยอร์ SeikoRed เป็นเลเยอร์แม่
GImage SeikoRed_min(img_seikored_min , SeikoRed_bg);  // เข็มยาว  มีเลเยอร์ SeikoRed เป็นเลเยอร์แม่
GImage SeikoRed_sec(img_seikored_sec , SeikoRed_bg);  // เข็มวินาที มีเลเยอร์ SeikoRed เป็นเลเยอร์แม่
GLabel lb_wday(SeikoRed_bg);
GLabel lb_mday(SeikoRed_bg);
GTimer clock_timer;

String th_weekday[]    = { "อา", "จ", "อัง", "พ", "พฤ", "ศ", "ส" };
void SeikoRed_drawPin(uint8_t wday, uint8_t mday, uint8_t h, uint8_t m, uint8_t s);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  // GLabel อื่นๆที่เป็นเลเยอร์ลูก หากไม่ได้กำหนด จะใช้ตาม font นี้
  SeikoRed_bg .font(prasanmit_20, TFT_BLACK);
    lb_wday   .c_align(ALIGN_CENTER, 52, 3);
    lb_mday   .c_align(ALIGN_CENTER, 77, 3);
  
  SeikoRed_hr .align_center();    // วิตเจ็ต GImage เข็มยาว
  SeikoRed_min.align_center();    // วิตเจ็ต GImage เข็มสั้น
  SeikoRed_sec.align_center();    // วิตเจ็ต GImage เข็มวินาที

  clock_timer.setInterval(1000L, []() {
    // wday(), day(), hour(), minute(), second() จะตั้งเวลา ตาม RTC ของ T-Watch
    // และ หาก NTP ซิงคสำเร็จ RCT ของ T-Watch จะได้รับการปรับเวลาให้แม่นยำตาม NTP อัตโนมัติ
    SeikoRed_drawPin(wday(), day(), hour(), minute(), second());
  });
}
void loop() {
  BlynkGO.update();
}

// เปลี่ยน h, m, s ที่อ่านได้จาก ESP32 เป็นการหมุนมุมเข็มนาฬิกา
void SeikoRed_drawPin(uint8_t wday, uint8_t mday, uint8_t h, uint8_t m, uint8_t s){
  lb_wday = th_weekday[wday];
  lb_mday = String(mday);

  SeikoRed_sec  .angle( s*6);
  SeikoRed_min  .angle( (m*1.0 + s/60.0)*6 );
  SeikoRed_hr   .angle( (h*1.0 + m/60.0+ s/3600.0)*5*6 );
}

