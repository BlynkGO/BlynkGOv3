#include <BlynkGOv3.h>

GBar    bar;              // ประกาศวิตเจ็ต GBar

GLabel  lb_mark[3];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

//  bar.size(30,200);     // หากจะเปลี่ยนเป็นแนวตั้ง ให้กำหนดขนาด w < h
  bar.symmetric(true);    // กำหนด bar แบบ สมมาตร โดยช่วงต้องกำหนดให้มีค่าน้อยสุด เป็นค่าติดลบ และค่ามากสุดเป็นค่าบวก
  bar.range(-100,100);    // กำหนดช่วง -100 ถึง 100
  bar.color(TFT_VIOLET);  // กำหนดสี

  bar.onValueChanged([](GWidget*widget){      // หาก bar มีการกำหนดเปลี่ยนแปลงค่าเข้ามา
    Serial.printf("[Bar] %d\n", bar.value()); // จะมีการแสดงค่าของ bar ออกทาง Serial Monitor ด้วย
  });

  if( bar.width() > bar.height() ) {
    lb_mark[0] = -100;  lb_mark[0].c_align(bar, ALIGN_TOP_LEFT  , 0,-15);
    lb_mark[1] = 0;     lb_mark[1].c_align(bar, ALIGN_TOP       , 0,-15);
    lb_mark[2] = 100;   lb_mark[2].c_align(bar, ALIGN_TOP_RIGHT , 0,-15);
  }else{
    lb_mark[0] = 100;   lb_mark[0].align(bar, ALIGN_RIGHT_TOP   , 5, 0);
    lb_mark[1] = 0;     lb_mark[1].align(bar, ALIGN_RIGHT       , 5, 0);
    lb_mark[2] = -100;  lb_mark[2].align(bar, ALIGN_RIGHT_BOTTOM, 5, 0);
  }
  
  bar = -60;              // กำหนดค่าให้ bar

  /* ตั้งเวลาเพื่อจำลองกำหนดค่า */
//  static GTimer timer;
//  timer.setInterval(10,[](){
//    static int8_t step =1;
//    int val = bar.value();
//    if(val+step > bar.value_max() || val+step < bar.value_min()){
//      step = -step;
//    }else{
//      bar = val+step;   // กำหนดค่าให้ bar
//    }
//  });
}

void loop() {
  BlynkGO.update();
}

