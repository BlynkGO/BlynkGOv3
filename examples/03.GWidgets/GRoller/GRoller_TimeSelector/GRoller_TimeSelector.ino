#include <BlynkGOv3.h>

GRoller roller_hour;
GRoller roller_minute;
GRoller roller_second;

GLabel lbClock;

uint8_t hour   = 0;
uint8_t minute = 0;
uint8_t second = 0;


void roller_onValueChange(GWidget *widget){
  hour    = roller_hour   .selected().toInt();
  minute  = roller_minute .selected().toInt();
  second  = roller_second .selected().toInt();

  lbClock = StringX::printf("เวลา %02d:%02d:%02d", hour, minute, second);
}

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();
  lbClock  = StringX::printf("เวลา %02d:%02d:%02d", hour, minute, second);
  lbClock.font(prasanmit_35);
  lbClock.color(TFT_WHITE);
  
  roller_hour.options(0,23, true);      // สร้าง options จาก 0 ถึง 23 โดย true คือ ให้มี 0 นำหน้าด้วย
  roller_hour.width(70);                // กำหนดความกว้าง
  roller_hour.option_align(LABEL_ALIGN_CENTER); // กำหนดให้ option จัดข้อความแบบชิดกลาง ของ roller
  roller_hour.onValueChanged(roller_onValueChange);
  
  roller_minute.options(0,59, true);    // สร้าง options จาก 0 ถึง 59 โดย true คือ ให้มี 0 นำหน้าด้วย
  roller_minute.width(70);
  roller_minute.option_align(LABEL_ALIGN_CENTER);
  roller_minute.onValueChanged(roller_onValueChange);

  roller_second.options(0,59, true);    // สร้าง options จาก 0 ถึง 59 โดย true คือ ให้มี 0 นำหน้าด้วย
  roller_second.width(70);
  roller_second.option_align(LABEL_ALIGN_CENTER);
  roller_second.onValueChanged(roller_onValueChange);

  uint16_t w =  8 + roller_minute.width() + 8 + roller_second.width();
  lbClock      .align( ALIGN_IN_TOP_MID,0,10);        // จัดตำแหน่ง GLabel
  roller_hour  .align( lbDate      , ALIGN_BOTTOM  ,-w/2 , 10  );
  roller_minute.align( roller_hour  , ALIGN_RIGHT   , 8  , 0   ); 
  roller_second.align( roller_minute, ALIGN_RIGHT   , 8  , 0   );
}


void loop() {
  BlynkGO.update();
}


