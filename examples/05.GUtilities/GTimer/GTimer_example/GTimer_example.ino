#include <BlynkGOv3.h>

GTimer timer;
GTimer timer_delay;

void setup() {
  Serial.begin(115200);Serial.println();
  BlynkGO.begin();

  // ตั้งเวลา timer ทำงาน ทุกๆ 1 วินาที
  timer.setInterval(1000, [](){
    static int i=0;
    Serial.println(i++);
  });  

  // ตั้งเวลา timer_delay ให้ ในอีก 3 วินาทีถัดไป ค่อยทำงาน
  timer_delay.delay(3000, [](){
    Serial.println("function my_function_once() called");
  });
}

void loop() {
  BlynkGO.update();
}

