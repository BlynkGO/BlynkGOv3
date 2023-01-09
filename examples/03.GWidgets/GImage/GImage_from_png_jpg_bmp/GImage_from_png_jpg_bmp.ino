#include <BlynkGOv3.h>

GImage myPicture;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // กำหนดรูปภาพ
  // หากจาก SD ให้ขึ้นต้นด้วย "SD://...."
  // หากจาก SPIFFS ให้ขึ้นต้นด้วย "SPIFFS://...."
  myPicture = "SD://TridentTD/pictures/aero_blue.png";   
}

void loop() {
  BlynkGO.update();
}

