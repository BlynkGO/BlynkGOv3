#include <BlynkGOv3.h>

GTimer timer;
GTimer timer_once;

void my_function(void* param){
  int* pInt = (int*) param;   // cast ตัวแปรให้ตรงกับที่ส่งมา
  Serial.println((*pInt)++);
}

void my_function_once(void* param){
  int* pInt = (int*) param;   // cast ตัวแปรให้ตรงกับที่ส่งมา
  Serial.printf("[my_function_once] value = %d\n", *pInt);
}

void setup() {
  Serial.begin(115200);Serial.println();
  BlynkGO.begin();

  static int i=0;
  timer.setInterval(1000L, my_function, (void*)&i);        // ส่ง param &i ไปยัง my_function ด้วย และทำงานทุกๆวินาที

  static int j=10;
  timer_once.setOnce(3000L, my_function_once, (void*)&j);  // ส่ง param &j ไปยัง my_function_once ด้วย และทำงานแบบครั้งเดียวใน 3วินาทีถัดไป
}

void loop() {
  BlynkGO.update();
}
