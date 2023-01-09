#include <BlynkGOv3.h>
#include "piece_bb.h"

#include <BlynkGOv3.h>

// ภาพ จากข้อมูลแบบ index 4bit (16สี) ขนาด 90x90 pixels
GImage image(piece_bb, 90, 90, CF_INDEXED_4BIT);  

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  image.create();
  image.dragable(true);
}

void loop() {
  BlynkGO.update();
}
