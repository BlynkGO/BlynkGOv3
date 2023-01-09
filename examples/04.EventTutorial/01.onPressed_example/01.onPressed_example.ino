/*
 * ตัวอย่าง ดักจับสัญญาณกดสัมผัส ด้วย onPressed(...) และ อื่นๆ
 * 
 * API    onPressed(...)    : สำหรับแรกสัมผัส
 *        onPressing(...)   : สำหรับกำลังสัมผัส
 *        onReleased(...)   : สำหรับปล่อยการสัมผัส
 *        onClicked(...)    : สำหรับมีการ clicked
 *        onLongPressed(...) : สำหรับมีการกดค้างยาว
 *        onLongPressedRepeat(...) : สำหรับมีการกดค้างยาวแล้วค้างยาวซ้ำๆๆๆ
 *        onValueChanged(...) : (สำหรับบางวิตเจ็ต) เมื่อมีการเปลี่ยนแปลงค่าเกิดขึ้น 
 *        
 * ทุกวิตเจ็ต จะสามารถดักรับสัญญาณการกดต่างๆ ได้หมด
 * หาก วิตเจ็ตไหนไม่ต้องการให้รับสัญญาณการกดที่เข้ามา ให้เปิด หรือปิด ด้วย
 * API คำสั่ง clickable(true) : เปิดรับสัญญาณกดได้ด้วย หรือ clickable(false) ปิดรับสัญญาณการกด
 * สัญญาณการกด หากถูกปิดไป วิตเจ็ตนั้น จะถูกข้ามไป ส่งให้วิตเจ็ตที่อยู่ layer ชั้นถัดๆลงไปเป็นลำดับแทน
 *
 */

#include <BlynkGOv3.h>

GRect rect_blue;
GRect rect_red;
GRect rect_green;
GLabel label;

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  rect_blue.size(120,80);
  rect_blue.color(TFT_BLUE);
  rect_blue.align(ALIGN_TOP,0,10);
  rect_blue.onPressed(rect_onPressed);
  
  rect_red.size(rect_blue);  // ให้มีขนาดเดียวกับ rect_blue;
  rect_red.color(TFT_RED);
  rect_red.align(ALIGN_LEFT,10,0);
  rect_red.onPressed(rect_onPressed);

  rect_green.size(rect_blue);  // ให้มีขนาดเดียวกับ rect_blue;
  rect_green.color(TFT_GREEN);
  rect_green.align(ALIGN_RIGHT,-10,0);
  rect_green.onPressed(rect_onPressed);

  label = "กดที่สี่เหลี่ยม";
  label.font(prasanmit_40, TFT_WHITE);
  label.align(ALIGN_BOTTOM, 0, -10);
}

void loop(){
  BlynkGO.update();
}

void rect_onPressed(GWidget* widget){
  if(rect_blue == widget ) {
    label = "Rect Blue Pressed";
  }else
  if(rect_red == widget ) {
    label = "Rect Red Pressed";
  }else
  if(rect_green == widget ) {
    label = "Rect Green Pressed";
  }
}


