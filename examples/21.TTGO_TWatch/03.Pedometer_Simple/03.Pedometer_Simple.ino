/****************************************************************
 * ตัวอย่าง สำหรับดึงค่านับจำนวนก้าว ด้วยเซนเซอร์ BMA423 ของ T-Watch
 * 
 ****************************************************************/

#include <BlynkGOv3.h>

GLabel lb_step_counter;
GButton btn_step_reset;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  TWatch.BMA423.enableStepCounter(true);
  TWatch.BMA423.resetStepCounter();      // คำสั่งสำหรับ reset จำนวนก้าว
  TWatch.BMA423.onStepCounter([](int step_cnt){
    if(step_cnt > 0){
      lb_step_counter = String("จำนวนก้าว : ") + String(step_cnt);
    } 
  });

  lb_step_counter = "จำนวนก้าว : 0";
  lb_step_counter.align(ALIGN_CENTER,0, -40);
  btn_step_reset = "Reset";
  btn_step_reset.align(lb_step_counter, ALIGN_BOTTOM,0, 30);
  btn_step_reset.onClicked([](GWidget*widget){
    TWatch.BMA423.resetStepCounter();      // คำสั่งสำหรับ reset จำนวนก้าว
    lb_step_counter = "จำนวนก้าว : 0";
  });
}

void loop() {
  BlynkGO.update();
}


