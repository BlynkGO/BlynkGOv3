#include <BlynkGOv3.h>

GRoller roller;

String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                      "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                      "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();

  size_t th_month_num =  sizeof( th_month ) /sizeof(th_month[0]);

  roller.options(th_month, th_month_num);
  roller.align(ALIGN_CENTER);
  roller.onValueChanged([](GWidget* widget){
    Serial.print("Selected month: ");
    Serial.println(roller.selected() );
  });
}


void loop() {
  BlynkGO.update();
}

