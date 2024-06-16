#include <BlynkGOv3.h>

GRoller roller_day;
GRoller roller_month;
GRoller roller_year;

                  
String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                      "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                      "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

GLabel lbDate;

String dd = "1";
String mm = "มกราคม";
String yy = "2510";

void roller_onValueChange(GWidget *widget){
  dd  = roller_day.selected();
  mm   = roller_month.selected();
  yy  = roller_year.selected();
  
  lbDate = dd + " " + mm + " " + yy;
}

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();

  lbDate  = dd + " " + mm + " " + yy;     // สร้าง GLabel โดยการนำ String มาต่อกันได้เลย
  lbDate.font(prasanmit_35);                  // เปลี่ยน font
  lbDate.color(TFT_WHITE);
  
  roller_day.options(1,31);                   // กำหนด options ให้ roller_day ด้วยตัวเลข ตั้งแต่ 1 ถึง 31
  roller_day.width(35);
  roller_day.onValueChanged(roller_onValueChange);
  
  size_t th_month_num =  sizeof( th_month ) /sizeof(th_month[0]);
  roller_month.options(th_month, th_month_num); // กำหนด options ให้ roller_month ด้วย String Array รายชื่อเดือน
  roller_month.onValueChanged(roller_onValueChange);

  roller_year.options(2510,2563);             // กำหนด options ให้ roller_year ด้วยตัวเลข ตั้งแต่ 2510 ถึง 2563
  roller_year.onValueChanged(roller_onValueChange);

  uint16_t w =  10 + roller_month.width() + 10 + roller_year.width();
  lbDate      .align( ALIGN_IN_TOP_MID,0,10);        // จัดตำแหน่ง GLabel
  roller_day  .align( lbDate      , ALIGN_BOTTOM  ,-w/2 , 10  );
  roller_month.align( roller_day  , ALIGN_RIGHT   , 10  , 0   ); 
  roller_year .align( roller_month, ALIGN_RIGHT   , 10  , 0   );
}

void loop() {
  BlynkGO.update();
}

