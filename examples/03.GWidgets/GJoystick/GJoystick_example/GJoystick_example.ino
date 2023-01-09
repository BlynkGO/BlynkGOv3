#include <BlynkGOv3.h>

GJoystick joystick;    // ประกาศตัวแปร GJoystick
GLabel    joy_x;
GLabel    joy_y;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  GScreen.font(prasanmit_25);  // กำหนดที่ parent, child ที่ไม่ได้กำหนดจะมาใช้จาก parent

  joystick.size(160,160);
  joystick.color(TFT_RED);
  joystick.border(5);

  joystick.onValueChanged([](GWidget* widget){
    GJoystick *pJoy = (GJoystick*) widget;
    joy_x = StringX::printf("x : %4d", pJoy->x() );
    joy_y = StringX::printf("y : %4d", pJoy->y() );
  });

  joy_x = StringX::printf("x : %4d", joystick.x());
  joy_y = StringX::printf("y : %4d", joystick.y());
    joy_x.position(GScreen.width()-80,0);
    joy_y.align(joy_x, ALIGN_OUT_BOTTOM_LEFT, 0, -10);
}

void loop() {
  BlynkGO.update();
}

