#include <BlynkGOv3.h>

GTimer timer;
GTimer timer_once;

void my_function(){
  static int i=0;
  Serial.println(i++);
}

void my_function_once(){
  Serial.println("function my_function_once() called");
}

void setup() {
  Serial.begin(115200);Serial.println();
  BlynkGO.begin();
  timer.setInterval(1000L, my_function);  // เรียกการทำงาน my_function ทุกๆ 1 วินาที
  timer_once.setOnce(3000L, my_function_once); // เรียกการทำงาน my_function_once ในอีก 3 วินาทีถัดไปขึ้นมาทำงาน
}

void loop() {
  BlynkGO.update();
}

