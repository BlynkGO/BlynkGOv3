/****************************************************************
 *
 *           สำหรับ BlynkGO Board V1.x ให้ข้ามขั้นตอนนี้ 
 *
 ****************************************************************
 * Touch Calibration 
 *     สำหรับหน้าจอ TFT  2.4", 2.8", 3.2", 3.5", 4.0" 
 *     จอสัมผัสแบบ Resistive  ( Touch SPI XPT2046 )
 *
 *
 *  หมายเหตุ  หากมีปัญหาในการ calibrate ระบบสัมผัส ผิดพลาด
 *             กรุณาแจ้ง https://web.facebook.com/blynkgo.os.1
 *             หรือ email : blynkgo@gmail.com
 *
 *          สำหรับจอสัมผัสแบบ Capacitive ไม่ต้องทำการ calibrate
 *
 *****************************************************************/
#include <BlynkGOv3.h>

GButton button;                     // ประกาศตัวแปรวิตเจ็ตปุ่มกด

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.rotation(ROTATION_DEFAULT);
  BlynkGO.touch_calibrate();        // สั่งทำการ calibrate touch

  button = "ปุ่มกดทดสอบ";            // กำหนดข้อความให้ปุ่มกด
  button.position(20,30);           // กำหนดตำแหน่ง ซ้ายบนของปุ่มกด จากซ้ายบนของหน้าจอ
  button.onClicked([](GWidget *w){  // เมื่อการสัญญาณ การกด clicked เข้ามา
    static uint8_t count =0;
    Serial.println("ปุ่มกด");
    button = "กดทดสอบ " + String(++count);    // เปลี่ยนข้อความของปุ่มกด
    button.position(  random(10, BlynkGO.width() - button.width() -10),  
                      random(10, BlynkGO.height()- button.height()-10)  );
    if(count > 10) {
      button.hidden(true);                    // ซ่อนเลเยอร์ วิตเจ็ตปุ่มกด ออกไป
      
      static GRect screen;
      screen.color(TFT_GREEN, TFT_CYAN);
      screen.onPressing([](GWidget*widget){
        static GCircle cir_touch;
        static GTimer  timer;

        Point tp = widget->touch_point();
        if(tp.x <= 0 || tp.y <= 0 ) return;
        
        if(!cir_touch.isCreated()){
          cir_touch.radius(10);
          cir_touch.color(TFT_RED);
          cir_touch.shadow(10, TFT_RED);
          cir_touch.shadow_spread(10);
        }
        cir_touch.hidden(false);
        cir_touch.center(tp); 
        timer.delay(2000,[cir_touch](){
          cir_touch.hidden(true);
        });
      });
      
      static GLabel label;
      label = "แคลิเบรตสัมผัส\nเรียบร้อย!";       // กำหนดข้อความให้วิตเจ็ตข้อความ
      label.align(ALIGN_CENTER);              // จัดข้อความไว้กึ่งกลางหน้าจอ
      label.text_align(TEXT_ALIGN_CENTER);    // จัดเรียงชิดภายในข้อความแบบกึ่งกลาง
      label.color(TFT_BLACK);
    }
  });
}
void loop() {
  BlynkGO.update();
}
