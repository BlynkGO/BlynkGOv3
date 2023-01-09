#include <BlynkGOv3.h>

// สร้างรูปจาก sd
GImage background_img	("SD://BlynkGO/images/aero_blue.png");
GIcon  calculator_icon("SD://BlynkGO/icons/png60x60/calculator.png", "เครื่องคิดเลข");

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  background_img.align_center();
  // calculator_icon.dragable(true);
  calculator_icon.onClicked([](GWidget* widget){
    Serial.println("Icon clicked");
  });
}

void loop() {
  BlynkGO.update();
}

