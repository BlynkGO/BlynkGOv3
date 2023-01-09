/****************************************************************
 * ตัวอย่าง สำหรับดึง ค่า direction ของ T-Watch 
 * ด้วยเซนเซอร์ BMA423 ของ T-Watch
 * หลัง upload ให้เอียงจอไปทิศทางต่างๆ
 * 
 ****************************************************************/

#include <BlynkGOv3.h>

GLabel lb_direction;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  TWatch.BMA423.enableDirection(true);
  TWatch.BMA423.onDirection([](direction_t dir){
    switch(dir){
      case DIRECTION_TOP_EDGE:
        lb_direction = "บน";
        break;
      case DIRECTION_BOTTOM_EDGE:
        lb_direction = "ล่าง";
        break;
      case DIRECTION_LEFT_EDGE:
        lb_direction = "ซ้าย";
        break;
      case DIRECTION_RIGHT_EDGE:
        lb_direction = "ขวา";
        break;
      case DIRECTION_DISP_UP:
        lb_direction = "หงาย";
        break;
      case DIRECTION_DISP_DOWN:
        lb_direction = "คว่ำ";
        break;
    }
  });
}

void loop() {
  BlynkGO.update();
}


