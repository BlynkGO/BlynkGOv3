#include <BlynkGOv3.h>

GColorPicker color_picker;
GLabel       color_lb;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  color_picker.type(TYPE_DISK);
  color_picker.onValueChanged([](GWidget* widget){
    color_lb = StringX::printf("ค่าสี : 0x%04x", color_picker.color() );
  });

  color_lb.color(TFT_WHITE);
  color_lb.align(ALIGN_IN_TOP_MID);
  color_lb = StringX::printf("ค่าสี : 0x%04x", color_picker.color() );
}

void loop() {
  BlynkGO.update();
}

