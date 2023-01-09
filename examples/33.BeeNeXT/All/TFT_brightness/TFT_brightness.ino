/*****************************
 * ปรับความสว่างหน้าจอ สำหรับ 
 *   BlynkGO EMMA-II
 *****************************/
 
#include <BlynkGOv3.h>

GSlider slider;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  slider.range(10,255);
  slider.color(TFT_YELLOW);
  slider.onValueChanged([](GWidget*widget){
    static GTimer timer;
    timer.delay(50,[](){
      BlynkGO.brightness(slider.value());
    });
  });
  slider = BlynkGO.brightness();
}

void loop() {
  BlynkGO.update();
}

