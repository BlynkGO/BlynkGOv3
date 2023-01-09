/*******************************************************************
 *
 * ตัวอย่างปุ่มแบบกลม ด้วย สัญลักษณ์ 
 *  (ดูสัญญาณที่ใช้ได้จาก  https://bit.ly/3cERHoi )
 *
 *******************************************************************/

#include <BlynkGOv3.h>

/* ประกาศ Widget ต่างๆ */
GButton btn_next(SYMBOL_NEXT);      // ไม่ได้กำหนดประเภทปุ่มกด จะเป็นแบบ BUTTON_PUSH อัตโนมัติ  เป็นการสร้างวิตเจ็ตปุ่มกด ด้วย SYMBOL_NEXT
GButton btn_play(SYMBOL_PLAY);
GButton btn_prev(SYMBOL_PREV);
GLabel  info;

/* สถานะของปุ่ม btn_play */
enum {BTN_STOP, BTN_PLAY, BTN_PAUSE} btn_play_state = BTN_STOP;

/* ฟังก์ชั่น เมื่อ ปุ่มมีการถูก clicked เข้ามา */
void btn_onClicked(GWidget* widget) {
  /* ปุ่มที่มีการถูก clicked */
  GButton &btn = *(GButton*)widget; // ทำการ cast ให้กลายเป็น GButton แบบ reference
  
  /* หากปุ่มที่กด คือ ปุ่ม btn_next */
  if(btn == btn_next) {
    info = "NEXT Clicked";
  }
  /* หากปุ่มที่กด คือ ปุ่ม btn_prev */
  else if(btn == btn_prev) {
    info = "PREV Clicked";
  }
  /* หากปุ่มที่กด คือ ปุ่ม btn_play */
  else if(btn == btn_play) {
    switch( btn_play_state ) {
      case BTN_PLAY :
        info = "PLAY Clicked";
        btn_play = SYMBOL_PAUSE;    // เปลี่ยนสัญลักษณ์บนปุ่ม btn_play เป็น SYMBOL_PAUSE
        btn_play_state = BTN_PAUSE;
        break;
      case BTN_PAUSE :
        info = "PAUSE Clicked";
        btn_play = SYMBOL_STOP;     // เปลี่ยนสัญลักษณ์บนปุ่ม btn_play เป็น SYMBOL_STOP
        btn_play_state = BTN_STOP;
        break;
      case BTN_STOP :
        info = "STOP Clicked";
        btn_play = SYMBOL_PLAY;     // เปลี่ยนสัญลักษณ์บนปุ่ม btn_play เป็น SYMBOL_PLAY
        btn_play_state = BTN_PLAY;
        break;
    }
  }
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  btn_play.rounded(true);             // กำหนดให้เป็นปุ่มแบบโค้งมน
  btn_play.size(60,60);               // ขนาด 60x60
  btn_play.align(ALIGN_CENTER,0,40);  // จัดวางตำแหน่ง
  btn_play.onClicked(btn_onClicked);  // กำหนดฟังก์ชั่นเมื่อมีการกด clicked

  btn_next.rounded(true);
  btn_next.size(60,60);
  btn_next.align(btn_play, ALIGN_OUT_RIGHT_MID, 10, 0 );
  btn_next.onClicked(btn_onClicked);
  
  btn_prev.rounded(true);
  btn_prev.size(60,60);
  btn_prev.align(btn_play, ALIGN_OUT_LEFT_MID, -10, 0);
  btn_prev.onClicked(btn_onClicked);

  info = "Press any button";
  info.color(TFT_WHITE);
  info.font(prasanmit_40);
  info.align(ALIGN_CENTER,0, -40);

}

void loop() {
  BlynkGO.update();
}


