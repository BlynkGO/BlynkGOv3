/**************************************************************
 * 
 * BlynkGO SmartWatch : Captain America
 * 
 **************************************************************/

#include <BlynkGOv3.h>

IMG_DECLARE(img_captainamerica_bg);
IMG_DECLARE(img_captainamerica_hr);
IMG_DECLARE(img_captainamerica_min);
IMG_DECLARE(img_captainamerica_sec);

GImage  captainAmerica_bg (img_captainamerica_bg);                       // หน้าปัดพื้นหลัง
GImage  captainAmerica_hr (img_captainamerica_hr  , captainAmerica_bg);  // เข็มสั้น
GImage  captainAmerica_min(img_captainamerica_min , captainAmerica_bg);  // เข็มยาว
GImage  captainAmerica_sec(img_captainamerica_sec , captainAmerica_bg);  // เข็มวินาที

void CaptainAmerica_drawPin(uint8_t h /* 0-23 */, uint8_t m /* 0-59 */, uint8_t s /*0-59*/);
GTimer clock_timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  captainAmerica_bg .align_center();
  captainAmerica_hr .align_center();
  captainAmerica_min.align_center();
  captainAmerica_sec.align_center();

  clock_timer.setInterval(1000L, []() {
    // hour(), minute(), second() จะตั้งเวลา ตาม RTC ของ T-Watch
    // และ หาก NTP ซิงคสำเร็จ RCT ของ T-Watch จะได้รับการปรับเวลาให้แม่นยำตาม NTP อัตโนมัติ
    CaptainAmerica_drawPin(hour(), minute(), second()); //  ชม, นาที, วินาที   
  });
}

void loop() {
  BlynkGO.update();
}

void CaptainAmerica_drawPin(uint8_t h, uint8_t m, uint8_t s) { 
  captainAmerica_hr .angle( (h * 1.0 + m / 60.0 + s / 3600.0) * 5 * 6 );
  captainAmerica_min.angle( (m * 1.0 + s / 60.0) * 6 );
  captainAmerica_sec.angle( s * 6);
}

