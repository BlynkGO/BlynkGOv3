#include <BlynkGOv3.h>

GPreload preload;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  preload.color(TFT_RED);
  
  // preload.arc_length(60);
  // preload.spin_time(2000);
  // preload_type :PRELOAD_TYPE_SPINNING_ARC, PRELOAD_TYPE_FILLSPIN_ARC, PRELOAD_TYPE_CONSTANT_ARC 
  // preload.preload_type(PRELOAD_TYPE_CONSTANT_ARC);
  // preload.direction(CLOCKWISE);  // CLOCKWISE, ANTICLOCKWISE

}

void loop() {
  BlynkGO.update();
}
