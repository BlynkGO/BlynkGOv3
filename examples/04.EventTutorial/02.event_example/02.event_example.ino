/*
 * ตัวอย่าง ดักจับสัญญาณกดสัมผัส ด้วย event_cb
 * โดย เมื่อมีการสัมผัสเข้ามาในพื้นที่บริเวณวงแหวนของวงกลม
 * หากลากนิ้วสัมผัสหมุนตามเข็มจะให้เพิ่มค่า
 * หากลากนิ้วสัมผัสหมุนทวนเข็มจะให้ลดค่า
 */

#include <BlynkGOv3.h>

FONT_DECLARE(BebasNeueBold_num_50); // ประกาศฟอนต์เสริมก่อนใช้เสมอ

GCircle circle;           // วิตเจ็ตวงกลม
GLabel  label(circle);    // วิตเจ็ตข้อความ เป็น layer ลูกของวิตเจ็ตวงกลม
int16_t value_max = 100;  // ค่ามากสุด
int16_t value_min = 0;    // ค่าน้อยสุด
int16_t value     = 20;   // ค่าปัจจุบัน

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  circle.radius(120);               // วงกลมรัศมี 120
  circle.opa(0);                    // วงกลมสีโปร่งใส
  circle.border(50, TFT_BLUE);      // วงกลมมีขอบหนา 50 สีฟ้า
  circle.event_cb(circle_event_cb); // กำหนด ฟังกชั่น event_cb สำหรับ ดักจับสัญญาณสัมผัส

  label = String(value)+ SYMBOL_DEGREE;         // ข้อความแสดงค่าวางไว้บนวงกลม (กำหนดเป็น layer ลูกของวงกลม)
  label.font(BebasNeueBold_num_50, TFT_BLACK);  // กำหนดฟอนต์พร้อมสี
  
}

void loop() {
  BlynkGO.update();
}

void circle_event_cb(GWidget* widget, event_t event){
  static Point    c     = Point(circle.width()/2, circle.height()/2 );  // จุดศูนย์กลางวงกลม
  static uint16_t cir_R = circle.radius();                              // รัศมีวงกลมนอก
  static uint16_t cir_r = circle.radius() - circle.border();            // รัศมีวงกลมใน
  static uint8_t step   = 1;
  static Point start_p;         // จุดสัมผัสแรกสุด
  static float start_angle;     // มุมของจุดสัมผัสแรกสุด
  Point p;
  float d;
  float angle;
  float angle_diff;

  // เมื่อมีการกดสัมผัสแรกเข้ามาที่วงกลม
  switch(event){
    case EVENT_PRESSED:
      start_p     = circle.touch_point();   // รับพิกัดจุดที่สัมผัส
      start_angle = start_p.angle_deg(c);   // เปลี่ยนเป็นมุมรอบจุด c ตามระนาบ xy หมุนทวนเข็มตามระบบ Math
      step = 1;
      break;
    case EVENT_PRESSING:
      p = circle.touch_point();             // รับพิกัดจุดที่สัมผัส
      d = p.distance(c);                    // หาระยะห่างระหว่างจุด p และ จุด c
      if( d > cir_R || d < cir_r ) return;  // หากจุดสัมผัสไม่ได้อยู่บนพื้นที่วงแหวน ให้คืนค่ากลับไปไม่ต้องทำอะไร

      angle       = p.angle_deg(c);         // มุมรอบจุด c ตามระนาบ xy หมุนทวนเข็มตามระบบ Math
      angle_diff  = ANGLE180( angle - start_angle); // ผลต่างมุม เทียบมุมเริ่มต้นสัมผัส โดยปรับมุมให้อยู่ระหว่างมากกว่า -180 องศา ไปจนถึง 180 องศา
      
      if( abs(angle_diff) >= 5 ) {
        if(angle_diff < 0 ) {
          // หากจุดที่สัมผัสล่าสุด มีผลต่างมุมติดลบถึง -5 องศา จากจุดเริ่มสัมผัส ให้เพิ่มค่า  โดยให้อยู่ภายในช่วงที่ value_min ถึง value_max
          value += step;
          value = constrain(value, value_min, value_max);
          label = String(value)+ SYMBOL_DEGREE;
        }else{
          // หากจุดที่สัมผัสล่าสุด มีผลต่างมุม ถึง 5 องศา จากจุดเริ่มสัมผัส ให้ลดค่า  โดยให้อยู่ภายในช่วงที่ value_min ถึง value_max 
          value -= step;
          value = constrain(value, value_min, value_max);                       
          label = String(value)+ SYMBOL_DEGREE;
        }
        start_p = p;
        start_angle = start_p.angle_deg(c);
      }

      if(value== value_min || value==value_max) step = 1;
      break;    
    case EVENT_RELEASED:            // เมื่อมีการปล่อยการกด
      break;
    case EVENT_CLICKED:             // เมื่อมีการ clicked
      break;
    case EVENT_SHORT_CLICKED:       // เมื่อมีการ clicked แบบสั้น
      break;
    case EVENT_LONG_PRESSED:        // เมื่อมีการ กดค้างยาว
      step ++;
      break;
    case EVENT_LONG_PRESSED_REPEAT: // เมื่อมีการกดค้างยาวแล้ววนค้างยาวซ้ำๆๆๆ
      step ++;
      break;
    case EVENT_PRESS_LOST:          // เมื่อสัมผัสที่กำลังสัมผัสอยู่ลากออกนอกพื้นที่ของวิตเจ็ต
      break;
  }
}


