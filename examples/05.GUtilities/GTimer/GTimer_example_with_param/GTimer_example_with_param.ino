#include <BlynkGOv3.h>

GTimer timer;
GTimer timer_delay;

void setup() {
  Serial.begin(115200);Serial.println();
  BlynkGO.begin();

  static int i=0;
  // ตั้งเวลา timer ให้ทำงานเป็นช่วงๆ ห่างกันทุกๆ 1 วินาที
  timer.setInterval(1000,[](void* param){
    int* pInt = (int*) param;   // cast ตัวแปรให้ตรงกับที่ส่งมา
    Serial.println((*pInt)++);
  }, (void*)&i); // ส่ง &i เข้าไปที่ parameter ของ callback function สำหรับ timer ด้วย
  
  static int j=10;
  // ตั้งเวลา timer_delay ให้เริ่มทำงานใน 3วินาทีถัดไป
  timer_delay.delay(3000L, [](void* param){
    int* pInt = (int*) param;   // cast ตัวแปรให้ตรงกับที่ส่งมา
    Serial.printf("[my_function_once] value = %d\n", *pInt);
  }, (void*)&j);  // ส่ง &j เข้าไปที่ parameter ของ callback function สำหรับ timer_delay ด้วย

}

void loop() {
  BlynkGO.update();
}


