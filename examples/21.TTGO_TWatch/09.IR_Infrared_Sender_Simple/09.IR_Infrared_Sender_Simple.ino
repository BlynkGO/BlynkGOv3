/*****************************************************************
 * ตัวอย่างนี้สำหรับ T-Watch รุ่น 2020
 * สำหรับควบคุมอุปกรณ์เครื่องใช้ไฟฟ้า ด้วย IR Infrared Sensor
 * 
 * ตัวอย่างนี้จะเป็นการควบคุมระดับแอร์ ระหว่าง 16-30 องศา
 * โดยการผนวก กราฟิก การสัมผัสปาดนิ้วขึ้นลงเพื่อปรับระดับแอร์ 
 * และ เซนเซอร์ IR Infrared ทำงานร่วมกัน
 * 
 * หมายเหตุ
 *   - ให้ลงไลบรารี่เพิ่มเติม Arduino_IRremote Version ล่าสุด 3.3.1 (@10/06/2021)
 *     ได้ที่  https://github.com/Arduino-IRremote/Arduino-IRremote
 *     
 *   - T-Watch รุ่น 2019 ไม่มี IR Sensor
 *   - T-Watch รุ่น 2020 v1 v2 v3 จะมี IR Sensor ที่ทำหน้าที่ ส่งสัญญาณอย่างเดียว     
 *   - การจะหาสัญญาณ IR สำหรับส่งไปยังอุปกรณ์เครื่องใช้ไฟฟ้า 
 *     ให้ หา โมดูล IR Sensor ที่มีทั้งตัวรับ (IR Reciever) และตัวส่ง (IR Emitter/ IR Sender) 
 *     ( โมดูล IR Sensor  https://miro.medium.com/max/810/1*jiXvn9_4S3IF0Ww9DFDeKw.jpeg )
 *     
 *     มาต่อกับ MCU อะไรก็ได้ แล้วทำการดักสัญญาณ IR ที่ปล่อยออกมาจาก รีโมทคอนโทรล ของอุปกรณ์ไฟฟ้าที่ต้องการ
 *     จากตัวอย่าง ReceiveDump ของไลบรารี่ Arduino_IRremote 
 *     
 *     จากนั้น ให้นำสัญญาณที่ได้ มากำหนดให้  IR Sender ของ T-Watch 2020
 *   
 *****************************************************************/

#include <BlynkGOv3.h>
#include <IRremote.h> // https://github.com/Arduino-IRremote/Arduino-IRremote

IMG_DECLARE(img_airconditioner);

GImage  air_conditioner(img_airconditioner);    // วิตเจ็ตรูป air_conditioner โดยโหลดรูปด้วย img_airconditioner
GCircle cir_air_level(air_conditioner);         // วิตเจ็ตวงกลม  ให้มี parent คือ วิตเจ็ตรูป  air_conditioner
GLabel  lb_air_level (cir_air_level);           // วิตเจ็ตข้อความ ให้มี parent คือ วิตเจ็ตวงกลม cir_air_level

uint8_t air_level = 24;

namespace AirConditioner { void OFF(); void level(uint8_t level); }

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  // กำหนดให้เป็น auto_sleepmode พร้อมตั้งเวลา กี่ ms ถ้าไม่มีการสัมผัส จะเข้าสู่ sleepmode ช่วยประหยัดพลังงาน
  //  TWatch.auto_sleepmode( true, 30000); 

  // เมื่อมีการสัมผัสหน้าจอ ในขณะที่หน้าจอดับอยู่ ให้สั่งเปิดหน้าจอ 
  //  TWatch.onTouch_Interrupt([](){      // เมื่อมีการสัมผัสหน้าจอ จาก interrupt เข้ามา
  //    if(TWatch.display_isOFF()){       // หากหน้าจอปิดอยู่
  //      TWatch.display_ON();
  //    }
  //  });

  air_conditioner.align(ALIGN_CENTER);
    cir_air_level.radius(25);
    cir_air_level.border(5, TFT_BLACK);
    cir_air_level.font(prasanmit_40, TFT_BLACK);  // ฟอนต์สำหรับ วิตเจ็ตลูกๆที่ไม่ได้กำหนดจะมาใช้ค่านี้
    cir_air_level.clickable(false);               // ไม่ให้รัยสัญญาณคลิกสัมผัสได้
    cir_air_level.align(ALIGN_TOP_RIGHT,-15,10);
      lb_air_level = String(air_level);

  air_conditioner.clickable(true);  // ให้รัยสัญญาณคลิกสัมผัสได้
  air_conditioner.gesture(true);    // เปิดให้คำสั่งปาดนิ้วได้
  air_conditioner.onGesture([](GWidget*widget){
    gesture_dir_t gesture_dir = air_conditioner.touch_gesture();
    uint8_t level = air_level;
    switch(gesture_dir){
      case GESTURE_DIR_TOP:     level++; break;
      case GESTURE_DIR_BOTTOM:  level--; break;
    }
    level = constrain(level, 16,30); // จำกัดให้อยู่ระหว่าง 16-30
    if( air_level != level) {
      air_level = level;
      AirConditioner::level(air_level); // ส่งคำสั่ง IR Infrered ออกไป ปรับระดับแอร์
      lb_air_level = String(air_level);
      TWatch.MOTOR.once(200);
    }
  });
}

void loop(){
  BlynkGO.update();
}
