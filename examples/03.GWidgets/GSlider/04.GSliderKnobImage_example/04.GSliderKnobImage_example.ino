/***************************************************************
 * 
 * ตัวอย่าง GSlider โดยมีการกำหนดรูปในปุ่มเลื่อนๆ
 * 
 ***************************************************************/

#include <BlynkGOv3.h>

FONT_DECLARE(prasanmit_60);                   // ประกาศฟอนต์ก่อนเรียกใช้
IMG_DECLARE(ico_ventilation_fan);             // ประกาศรูป c file ก่อนเรียกใช้

GSlider slider(0,50, SLIDER_TYPE_RANGE);      // ประกาศวิตเจ็ตสไลเดอร์ กำหนดค่าต่ำสุด สูงสุด และกำหนด ให้เป็นประเภทปรับ 2 ค่าซ้ายขวา
GLabel  lb_range;                             // ประกาศวิตเจ็ตข้อความ

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  slider.knob_image(ico_ventilation_fan);     // กำหนดรูปให้ปุ่มเลื่อน
  slider.width(300);                          // กำหนดความกว้าง
  slider.color(TFT_BLUE, TFT_RED);            // กำหนดสี ด้วย เฉดสี 2 สี
  slider.onValueChanged([](GWidget* widget){  // เมื่อ slider มีการเลื่อนเปลี่ยนค่าเข้ามา
    lb_range = StringX::printf("%d" SYMBOL_DEGREE " - %d" SYMBOL_DEGREE,
                                    slider.left_value(),slider.right_value() );
  });

  slider.right_value(35);                     // กำหนดค่าทางขวา
  slider.left_value(15);                      // กำหนดค่าทางซ้าย

  lb_range.font(prasanmit_60, TFT_WHITE);     // กำหนด font และสีให้วิตเจ็ตข้อความ
  lb_range.align(slider, ALIGN_TOP, 0, -40);  // กำหนดตำแหน่งให้วางเหนือ slider ขึ้นไปอีก 40 pixel
  
  // ให้ส่ง event ออกไปจำลองว่ามีการเปลี่อนค่า เพื่อให้ไปเรียก onValueChanged(..) ที่กำหนดข้างต้นขึ้นมาทำงาน
  slider.event_send(EVENT_VALUE_CHANGED);     
}

void loop() {
  BlynkGO.update();
}

