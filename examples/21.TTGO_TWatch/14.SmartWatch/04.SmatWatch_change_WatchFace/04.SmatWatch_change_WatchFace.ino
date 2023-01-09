/**************************************************************
 * 
 * BlynkGO SmartWatch : 
 *   ตัวอย่างนี้ จะใช้ วิตเจ็ตรูป ชุดเดียว แต่เปลี่ยน หน้าตา Watch Face ไปแบบต่างๆได้
 *   เมื่อมีการกดค้างที่รูปหน้าปัดพื้นหลัง
 * 
 **************************************************************/

#include <BlynkGOv3.h>

GImage  watch_bg;                 // หน้าปัดพื้นหลัง
GImage  watch_hr    (watch_bg);   // เข็มสั้น
GImage  watch_min   (watch_bg);   // เข็มยาว
GImage  watch_sec   (watch_bg);   // เข็มวินาที
GLabel  watch_date  (watch_bg);   // วันที
GLabel  watch_clock (watch_bg);   // เวลา
GLabel  watch_label[3] = { {watch_bg}, {watch_bg}, {watch_bg} }; // label เสริม

enum { WATCHFACE_CAPTAIN, WATCHFACE_AMAZFIT_VERGE, WATCHFACE_TAGHEUER_RED, WATCHFACE_MINIMALCOLORFUL, WATCHFACE_MAX };
int8_t watch_face = WATCHFACE_CAPTAIN;
GTimer clock_timer;

namespace CaptainWatch    { void design(); void update(); }
namespace AmazfitVerge    { void design(); void update(); }
namespace TAGHeuerRed     { void design(); void update(); }
namespace MinimalColorful { void design(); void update(); }

void watch_drawPin(uint8_t h, uint8_t m, uint8_t s) { 
  watch_hr .angle( (h * 1.0 + m / 60.0 + s / 3600.0) * 5 * 6 );
  watch_min.angle( (m * 1.0 + s / 60.0) * 6 );
  watch_sec.angle( s * 6);
}
void watch_hidden_all_label(){
  watch_date  .hidden(true);
  watch_clock .hidden(true);
  for(int i=0; i < sizeof(watch_label)/sizeof(watch_label[0]); i++)
    watch_label[i].hidden(true);
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  // กำหนดให้เป็น auto_sleepmode พร้อมตั้งเวลา กี่ ms ถ้าไม่มีการสัมผัส จะเข้าสู่ sleepmode ช่วยประหยัดพลังงาน
  //  TWatch.auto_sleepmode( true, 30000); 

  // เมื่อมีการสัมผัสหน้าจอ ในขณะที่หน้าจอดับอยู่ ให้สั่งเปิดหน้าจอ 
  //  TWatch.onTouch_Interrupt([](){      // เมื่อมีการสัมผัสหน้าจอ จาก interrupt เข้ามา
  //    if(TWatch.display_isOFF()){       // หากหน้าจอปิดอยู่
  //      TWatch.display_ON();
  //    }
  //  });

  watch_bg.clickable(true);                   // ให้วิตเจ็ตรูปหน้าปัดพื้นหลัง รับสัญญาณสัมผัสได้
  watch_bg.onLongPressed([](GWidget*widget){  // เมื่อมีการกดค้างยาว ที่รูปพื้นหลัง
    watch_face = (watch_face +1) % WATCHFACE_MAX;
    switch(watch_face){
      case WATCHFACE_CAPTAIN:         CaptainWatch::design();     break;
      case WATCHFACE_AMAZFIT_VERGE:   AmazfitVerge::design();     break;
      case WATCHFACE_TAGHEUER_RED:    TAGHeuerRed::design();      break;
      case WATCHFACE_MINIMALCOLORFUL: MinimalColorful::design();  break;
    }
  });

  CaptainWatch::design();
  clock_timer.setInterval(1000L, []() {
    switch(watch_face) {
      case WATCHFACE_CAPTAIN:         CaptainWatch::update();     break;
      case WATCHFACE_AMAZFIT_VERGE:   AmazfitVerge::update();     break;
      case WATCHFACE_TAGHEUER_RED:    TAGHeuerRed::update();      break;
      case WATCHFACE_MINIMALCOLORFUL: MinimalColorful::update();  break;
    }
  });
}

void loop() {
  BlynkGO.update();
}


