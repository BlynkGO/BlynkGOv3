/****************************************************************
 * ตัวอย่าง สำหรับดึง ค่า Acceleration (ค่าเกี่ยวกับเซนเซอร์ความเร่ง ค่าต่างๆ)
 * ด้วยเซนเซอร์ BMA423 ของ T-Watch
 * 
 * หมายเหตุ
 *   ค่า roll & pitch คืออะไร :  https://i.stack.imgur.com/8IuOw.png
 *  
 ****************************************************************/

#include <BlynkGOv3.h>

GLabel lb_accel[5];
GTimer accel_timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  for(int i=0; i < 5; i++){
    lb_accel[i].font(prasanmit_20);

    (i==0)? lb_accel[i].align(ALIGN_TOP_LEFT, 20,40) :
            lb_accel[i].align(lb_accel[i-1], ALIGN_BOTTOM_LEFT);
  }
  
  TWatch.BMA423.enableAcceleration(true);

  // แบบตั้งเวลาทุกๆ 2 วินาที ทำการดึงค่า Acceleration จากเซนเซอร์ BMA423 ของ T-Watch
  // มาแสดง ที่วิตเจ็ตข้อความ
  accel_timer.setInterval(2000,[](){
    acceleration_t accel = TWatch.BMA423.getAcceleration();
    lb_accel[0] = StringX::printf("Accel (lsb)  = (%d, %d, %d)"       , accel.x, accel.y, accel.z);
    lb_accel[1] = StringX::printf("Accel (m/s2) = (%.2f, %.2f, %.2f)" , accel.x_ms2, accel.y_ms2, accel.z_ms2);
    lb_accel[2] = StringX::printf("x (roll, pitch) = (%.2f, %.2f)"    , accel.roll_x, accel.pitch_x); // ค่า roll & pitch คืออะไร :  https://i.stack.imgur.com/8IuOw.png
    lb_accel[3] = StringX::printf("y (roll, pitch) = (%.2f, %.2f)"    , accel.roll_y, accel.pitch_y);
    lb_accel[4] = StringX::printf("z (roll, pitch) = (%.2f, %.2f)"    , accel.roll_z, accel.pitch_z);
  });

  // แบบนี้จะอ่านออกมาได้ไว real-time 
  // เหมาะสำหรับนำไปคำนวณบางอย่างต่างหากเอง ที่ต้องการความไวสูง
//  TWatch.BMA423.onAcceleration([](acceleration_t accel){
//    Serial.printf("Accel  = (%d, %d, %d) ; ", accel.x, accel.y, accel.z);
//    Serial.printf("x (roll, pitch) = (%.2f, %.2f) ;", accel.roll_x, accel.pitch_x);
//    Serial.printf("y (roll, pitch) = (%.2f, %.2f) ;", accel.roll_y, accel.pitch_y);
//    Serial.printf("z (roll, pitch) = (%.2f, %.2f)\n", accel.roll_z, accel.pitch_z);
//  });

}

void loop() {
  BlynkGO.update();
}
