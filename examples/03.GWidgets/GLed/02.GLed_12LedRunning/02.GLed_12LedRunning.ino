#include <BlynkGOv3.h>

GContainer cont_led;
  GLed led[12];
  size_t led_num = sizeof(led)/sizeof(led[0]);

GTimer timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // ออกแบบกราฟิก led ทั้ง 12 
  cont_led.opa(0);
  cont_led.padding(10);
  cont_led.layout(LAYOUT_ROW_M);
  for(int i=0; i< led_num; i++){
    led[i].size(20,20);
    led[i].color( TFT_COLOR_HSV( map(i,0, led_num-1,0,240),100,100) );
    led[i].parent(cont_led);
  }

  // ตั้งเวลาทำงาน ให้ led วิ่ง
  timer.setInterval(100, [](){
    static int8_t i=~0, step=1;  static bool state=LOW;
    step = (i==0)? 1 : (i== led_num-1)? -1 : step ;
    led[i=(state == LOW )? i + step : i].state(state=!state);
  });
}

void loop() {
  BlynkGO.update();
}

