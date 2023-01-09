#include <BlynkGOv3.h>

GButton btn("ทดสอบ");

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  btn.onClicked([](GWidget*widget){
    TWatch.MOTOR.once(200);				// API คำสั่งสำหรับให้ Vibrate-Motor ทำการสั่น กี่ ms
  });
}

void loop(){
  BlynkGO.update();  
}
