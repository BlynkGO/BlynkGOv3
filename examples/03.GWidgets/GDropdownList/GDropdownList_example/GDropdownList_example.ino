#include <BlynkGOv3.h>

GDropdownList ddlist;

String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                      "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                      "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  ddlist.size(160,60);
  ddlist.height_open(200);
  ddlist.options(th_month, 12);
  ddlist.font(prasanmit_40);
  ddlist.align(ALIGN_TOP,0,20);
  
  ddlist.onValueChanged([](GWidget*widget){
    Serial.println(ddlist.selected());
  });
}

void loop() {
  BlynkGO.update();
}

