#include <BlynkGOv3.h>

GSlider slider_brightness(2,255);
//uint8_t bright;
void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // กำหนดให้เป็น auto_sleepmode พร้อมทั้งเวลา กี่ ms ถ้าไม่มีการสัมผัส จะเข้าสู่ sleepmode ช่วยประหยัดพลังงาน
  // TWatch.auto_sleepmode( true, 10000); 

  // เมื่อมีการสัมผัสหน้าจอ ในขณะที่หน้าจอดับอยู่ ให้สั่งเปิดหน้าจอ 
  //  TWatch.onTouch_Interrupt([](){            // เมื่อมีการสัมผัสหน้าจอ จาก interrupt เข้ามา
  //    if(TWatch.display_isOFF()){             // หากหน้าจอปิดอยู่
  //      TWatch.display_ON();
  //    }
  //  });
    
  slider_brightness.color(TFT_YELLOW);
  slider_brightness.width(200);

  // ตั้งค่าระดับความสว่าง ให้ slider_brightness จาก ระดับความสว่างของ BlynkGO ปัจจุบัน
  // false ในพารามิเตอร์สอง คือ ไม่ต้องแสดง animation
  slider_brightness.value(BlynkGO.brightness(), false);
  
  slider_brightness.onValueChanged([](GWidget* widget){
    TWatch.enableInterrupt(IRQ_TOUCH, false);       // ปิด Touch Interrupt ชั่วคราว ป้องกันการชนกันของ Touch Interrupt กับ การบันทึกลง flashMem ในคำสั่ง API brightness(...)
    BlynkGO.brightness(slider_brightness.value());  // มีการจำค่าลง flashMem ให้อัตโนมัติ หากมี reboot ก็จะกลับมาความสว่างระดับเดิมได้
    TWatch.enableInterrupt(IRQ_TOUCH, true);        // เปิด Touch Interrupt อีกครั้ง
  });

}

void loop(){
  BlynkGO.update();
}


