/*********************************
 * ตัวอย่างสำหรับ ทดสอบ สัมผัสหน้าจอ
 *   หน้าจอจะขึ้นมาเป็นจอสีฟ้า 
 *   ให้สัมผัสไปทั่วๆหน้าจอ จะมีจุดปรากฏตามที่สัมผัส
 ********************************/

#include <BlynkGOv3.h>

GRect rect;
GCircle dot;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  rect.size(GScreen); 
  rect.color(TFT_BLUE);
  rect.onPressing([](GWidget*widget){
    Point p = rect.touch_point();
    POINT_PRINTLN(p);
    dot.center(p);
  });
  dot.color(TFT_YELLOW);
  dot.radius(10);
  dot.center(0,0);
}

void loop() {
  BlynkGO.update();
}

