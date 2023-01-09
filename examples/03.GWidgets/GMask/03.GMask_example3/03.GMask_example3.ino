#include <BlynkGOv3.h>

GMask   Mask(40,200);
GPage   page(Mask);
GCircle cir[10];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  Mask.line(255, Point(20,20), Point(20,180), 40, true );  

  page.size(40,200);
  page.bg.padding(0);
  page.scrl.padding(0);
  page.border(0);
  page.corner_radius(0);
  page.opa(0);
  page.scrollbar(SCROLLBAR_HIDE);
  
  for(int i=0;i < 10; i++){
    cir[i].parent(page);
    cir[i].size(40,40);
    cir[i].color(random(65536));
    cir[i].drag_parent(true);      // สำหรับ ส่ง สัญญาณ drag ไปให้ page เลือน scroll ได้ แต่ cirก็ยัง click ได้ด้วย
  }
}

void loop() {
  BlynkGO.update();
}

