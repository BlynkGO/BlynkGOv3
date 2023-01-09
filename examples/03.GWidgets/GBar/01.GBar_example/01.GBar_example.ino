#include <BlynkGOv3.h>

GBar  bar;              // ประกาศวิตเจ็ต GBar
//GBar  bar(0,1000);    // ประกาศวิตเจ็ต GBar พร้อมกำหนดช่วง 0-1000

GBar  bar2;

GBar  bar3;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  //--------------------------------------------------------------------
//  bar.size(30,160);                         // หากจะเปลี่ยนเป็นแนวตั้ง ให้กำหนดขนาด w < h
  bar.range(0,1000);                          // กำหนดช่วง 0-1000  หรือจะประกาศตั้งแต่ตอนประกาศวิตเจ็ตก็ได้
  bar.color(TFT_RED);                         // กำหนดสี
  bar.align(ALIGN_CENTER, 0, -60);            // จัดวางไว้กลางหน้าจอ โดยปรับเลื่อนขึ้นไป 60 pixel

  bar.onValueChanged([](GWidget*widget){      // หาก bar มีการกำหนดเปลี่ยนแปลงค่าเข้ามา
    Serial.printf("[Bar] %d\n", bar.value()); // จะมีการแสดงค่าของ bar ออกทาง Serial Monitor ด้วย
  });

  bar = 750;                                  // กำหนดค่าให้ bar  หรือจะใช้คำสั่ง bar.value(-75);  ก็ได้

  //--------------------------------------------------------------------
  bar2.range(-100,0);                         // กำหนดช่วง
  bar2.start_value(0);                        // ค่าเริ่มต้นของ bar โดยแท่ง bar จะวาดเริ่มจากค่านี้ไป
  bar2.color(TFT_BLUE);                       // กำหนดสี
  bar2.align(bar, ALIGN_BOTTOM,0, 30);        // จัดวางไว้ใต้ bar โดยปรับเลื่อนลงไปอีก 30 pixel

  bar2 = -28;                                 // กำหนดค่าให้ bar 
  
  //--------------------------------------------------------------------
  bar3.size(220,50);                          // กำหนดขนาด
  bar3.padding(10);                           // กำหนดระยะช่องไฟภายใน
  bar3.align(bar2, ALIGN_BOTTOM,0, 30);       // จัดวางไว้ใต้ bar2 โดยปรับเลื่อนลงไปอีก 30 pixel

  /* กำหนด style ปรับแต่งเอง  (must after created)    */
  /* GBar จะมี style อยู่ 2 แบบ คือ 
       style[BG]    สำหรับ สี่เหลี่ยมพื้นหลัง และ 
       style[INDIC] สำหรับ ในส่วนแสดง indicator ด้านบน */
  bar3.style[BG].corner_radius(15);           // รัศมีมุมโค้ง ของพื้นหลัง
  bar3.style[BG].border(3, TFT_RED);          // ขอบ ของพื้นหลัง
  bar3.style[BG].border_opa(255);             // ค่าความโปร่งใสของ ขอบ ของพื้นหลัง  0-255
  
  bar3.style[INDIC].color(TFT_NAVY);          // สี ของ indicator  หรือจะใช้ bar3.color(TFT_NAVY)  ก็ได้
  bar3.style[INDIC].corner_radius(10);        // รัศมีมุมโค้ง ของ indicator
  bar3.style[INDIC].border(0);                // ขอบ ของ indicator

  bar3 = 67;                                  // กำหนดค่าให้ bar 

  //--------------------------------------------------------------------
  /* ตั้งเวลาเพื่อจำลองกำหนดค่า */
//  static GTimer timer;
//  timer.setInterval(10,[](){
//    static int8_t step =1;
//    
//    int val = bar.value();
//    if(val+step > bar.value_max() || val+step < bar.value_min()){
//      step = -step;
//    }else{
//      bar = val+step;
//    }
//  });
}

void loop() {
  BlynkGO.update();
}

