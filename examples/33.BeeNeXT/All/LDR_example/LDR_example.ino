#include <BlynkGOv3.h>

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  static GTimer timer;
  timer.setInterval(1000,[](){
    Serial.println( analogRead(LDR_PIN) );
  });
}

void loop() {
  BlynkGO.update();
}

