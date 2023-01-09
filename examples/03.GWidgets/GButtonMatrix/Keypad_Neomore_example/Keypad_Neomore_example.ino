#include <BlynkGOv3.h>

IMG_DECLARE(img_background);

GImage background(img_background);                            // วิตเจ็ตรูป สำหรับ background
GLabel lb_speed;  GTextArea ta_speed; GLabel lb_spd_unit;     // วิตเจ็ตข้อความ และ วิตเจ็ตสำหรับใส่ข้อความ
GLabel lb_volt;   GTextArea ta_volt;  GLabel lb_volt_unit;    // วิตเจ็ตข้อความ และ วิตเจ็ตสำหรับใส่ข้อความ

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  background.align(ALIGN_CENTER);
  
  lb_speed      = "ความเร็ว : ";
  ta_speed      = "0";
  lb_spd_unit   = "cm/s";

  lb_speed.align(ALIGN_TOP_LEFT, 60, 50);
  lb_speed.color(TFT_BLACK);
  ta_speed.width(180);
  ta_speed.align( lb_speed, ALIGN_RIGHT);
  ta_speed.cursor_type(CURSOR_NONE);        // ไม่ต้องแสดง cursor
  ta_speed.default_keyboard(false);         // ไม่ใช้ default keyboard
  ta_speed.onClicked([](GWidget*w){
    keypad(ta_speed);                       // หากมีการกด clicked มา ให้เรียก keypad ทำงาน
  });
  lb_spd_unit.align(ta_speed, ALIGN_RIGHT, 10);
  lb_spd_unit.color(TFT_BLACK);
  
  lb_volt       = "แรงดัน : ";
  ta_volt       = "0";
  lb_volt_unit  = "volt";

  lb_volt.align(lb_speed, ALIGN_BOTTOM_LEFT, 0, 40);
  lb_volt.color(TFT_BLACK);
  ta_volt.width(180);
  ta_volt.align( ta_speed, ALIGN_BOTTOM_LEFT, 0, 30);
  ta_volt.cursor_type(CURSOR_NONE);         // ไม่ต้องแสดง cursor
  ta_volt.default_keyboard(false);          // ไม่ใช้ default keyboard
  ta_volt.onClicked([](GWidget*w){
    keypad(ta_volt);                        // หากมีการกด clicked มา ให้เรียก keypad ทำงาน
  });
  lb_volt_unit.align(ta_volt, ALIGN_RIGHT, 10);
  lb_volt_unit.color(TFT_BLACK);

  //---------------------------------------------
  // ดึงค่าไปใช้งาน เมื่อมีการเปลี่ยนแปลงค่า
  ta_speed.onValueChanged([](GWidget*w){
    float speed = ta_speed.toFloat();
    Serial.printf( "Speed : %.2f\n", speed);
  });
  // ดึงค่าไปใช้งาน เมื่อมีการเปลี่ยนแปลงค่า
  ta_volt.onValueChanged([](GWidget*w){
    float volt = ta_volt.toFloat();
    Serial.printf( "Volt : %.2f\n", volt);
  });
}

void loop() {
  BlynkGO.update();
}
