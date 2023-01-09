/**************************************************************
 * 
 * BlynkGO SmartWatch : Amafit Verge
 * 
 **************************************************************/

#include <BlynkGOv3.h>

IMG_DECLARE(img_amafit_verge_bg);
IMG_DECLARE(img_amafit_verge_hr);
IMG_DECLARE(img_amafit_verge_min);
IMG_DECLARE(img_amafit_verge_sec);

GImage  Amafit_Verge_bg (img_amafit_verge_bg);                       // หน้าปัดพื้นหลัง
GImage  Amafit_Verge_hr (img_amafit_verge_hr  , Amafit_Verge_bg);  // เข็มสั้น
GImage  Amafit_Verge_min(img_amafit_verge_min , Amafit_Verge_bg);  // เข็มยาว
GImage  Amafit_Verge_sec(img_amafit_verge_sec , Amafit_Verge_bg);  // เข็มวินาที

void Amafit_Verge_drawPin(uint8_t h /* 0-23 */, uint8_t m /* 0-59 */, uint8_t s /*0-59*/);
GTimer clock_timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  Amafit_Verge_bg .align_center();
  Amafit_Verge_hr .align_center();
  Amafit_Verge_min.align_center();
  Amafit_Verge_sec.align_center();

  clock_timer.setInterval(1000L, []() {
    // hour(), minute(), second() จะตั้งเวลา ตาม RTC ของ T-Watch
    // และ หาก NTP ซิงคสำเร็จ RCT ของ T-Watch จะได้รับการปรับเวลาให้แม่นยำตาม NTP อัตโนมัติ
    Amafit_Verge_drawPin(hour(), minute(), second()); //  ชม, นาที, วินาที   
  });
}

void loop() {
  BlynkGO.update();
}

void Amafit_Verge_drawPin(uint8_t h, uint8_t m, uint8_t s) { 
  Amafit_Verge_hr .angle( (h * 1.0 + m / 60.0 + s / 3600.0) * 5 * 6 );
  Amafit_Verge_min.angle( (m * 1.0 + s / 60.0) * 6 );
  Amafit_Verge_sec.angle( s * 6);
}

