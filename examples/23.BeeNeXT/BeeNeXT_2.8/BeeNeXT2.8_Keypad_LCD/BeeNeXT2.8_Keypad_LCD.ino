#include <BlynkGOv3.h>

IMG_DECLARE(img_keypad);
FONT_DECLARE(BebasNeueBold_num_45);

GImage image_keypad(img_keypad);
  GRect rect_monitor(image_keypad);
    GTextArea ta_keypad(rect_monitor);

  GRect rect_keypad_mode(image_keypad);
    GLabel lb_keypad_mode(rect_keypad_mode);
    enum { KEYPAD_MODE_NUM, EYPAD_MODE_TEXT };
    uint8_t keypad_mode = KEYPAD_MODE_NUM;
  
  GButtonMatrix btnm (image_keypad);
  const char * btnm_map[] = { "1", "2", "3", SYMBOL_ARROW_LEFT, "\n",
                              "4", "5", "6", SYMBOL_UP, "\n",
                              "7", "8", "9", SYMBOL_DOWN, "\n",
                              SYMBOL_LEFT, "0", SYMBOL_RIGHT, SYMBOL_ENTER, ""};
  // enum ให้ตั้งชื่อเลียน และไล่ลำดับตาม btnm_map ข้างบน
  enum {KEY_1, KEY_2, KEY_3, KEY_BACKSPACE,
        KEY_4, KEY_5, KEY_6, KEY_UP,
        KEY_7, KEY_8, KEY_9, KEY_DOWN,
        KEY_LEFT, KEY_0, KEY_RIGHT, KEY_ENTER };

int value= 1235;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.rotation(ROTATION_VERTICAL);
  image_keypad.align_center();

  rect_keypad_mode.size(60,30);
  rect_keypad_mode.align(ALIGN_TOP_RIGHT, 320-288,-45);
  rect_keypad_mode.color(TFT_BLACK);
  rect_keypad_mode.font( prasanmit_25, TFT_WHITE);
    lb_keypad_mode = "123";

  rect_monitor.size(200,48);
  rect_monitor.align(ALIGN_TOP, 0, 20-3);
  rect_monitor.opa(0);
    ta_keypad.opa(0);
    ta_keypad.border(0);
    ta_keypad.corner_radius(0);
    ta_keypad.width(rect_monitor);
    ta_keypad.text_align(TEXT_ALIGN_CENTER);
    ta_keypad.font(BebasNeueBold_num_45, TFT_BLACK);
    ta_keypad.default_keyboard(false);        // ไม่ใช้ default keybard
    ta_keypad.cursor_type(CURSOR_NONE);       // ไม่ต้องแสดง cursor
    ta_keypad.onClicked([](GWidget*widget){   // หากมีการ clicked มาที่ ta_keypad
      ta_keypad.cursor_type(CURSOR_LINE);     // ให้แสดง cursor แบบ line
      static GTimer timer_ta;
      timer_ta.delay(5000,[](){               // อีก 5 วินาทีถัดไป
        ta_keypad.cursor_pos(CURSOR_LAST);    // ให้ย้าย cursor ไปไว้หลังสุด
        ta_keypad.cursor_type(CURSOR_NONE);   // และให้ไม่ต้องแสดง cursor
      });
    });
    ta_keypad.text(String(value));            // กำหนด ข้อความ ให้ ta_keypad

  btnm.map(btnm_map);                         // กำหนด map ของปุ่มให้ ButtonMatrix
  btnm.size(190,190);                         // กำหนดขนาด
  btnm.padding(0,0,0,0,6);                    // กำหนดช่องไฟระหว่างปุ่ม
  btnm.border(0);                             // กำหนดขอบให้ปุ่ม
  btnm.radius(5);                             // กำหนดรัศมีมุมของปุ่ม
  btnm.align(ALIGN_BOTTOM,0,-25);

  btnm.style[BTNM_BG] .opa(0);                // style ของ พื้นหลัง btnm ให้พื้นหลังโปร่งใส
  btnm.style[BTNM_BG] .border(0);             // style ของ พื้นหลัง btnm ไม่ต้องมีขอบ
  btnm.style[RELEASED].opa(0);                // style ของ ปุ่ม btnm เมื่อปล่อย ให้โปร่งใส
  btnm.style[RELEASED].text_opa(0);           // style ของ ปุ่ม btnm เมื่อปล่อย ข้อความบนปุ่ม โปร่งใส
  btnm.style[PRESSED] .color(TFT_BLACK);      // style ของ ปุ่ม btnm เมื่อกด ให้สีเป็นสีดำ
  btnm.style[PRESSED] .opa(180);              // style ของ ปุ่ม btnm เมื่อกด ให้โปร่งใส 180 จาก 255 ระดับ
  btnm.style[PRESSED] .text_opa(0);           // style ของ ปุ่ม btmn เมื่อกด ให้ข้อความบนปุ่มโปร่งใส

  // เมื่อ User มีการกดปุ่ม btnm เปลี่ยนค่า เข้ามา
  btnm.onValueChanged([](GWidget* widget) { 
    uint16_t btnm_btn_id = btnm.pressed_button_id();
    if(btnm_btn_id == 0xFFFF) return;
//    Serial.println(btnm_btn_id);
    switch(btnm_btn_id){
      case KEY_0:         ta_keypad.add('0');       break;
      case KEY_1:         ta_keypad.add('1');       break;
      case KEY_2:         ta_keypad.add('2');       break;
      case KEY_3:         ta_keypad.add('3');       break;
      case KEY_4:         ta_keypad.add('4');       break;
      case KEY_5:         ta_keypad.add('5');       break;
      case KEY_6:         ta_keypad.add('6');       break;
      case KEY_7:         ta_keypad.add('7');       break;
      case KEY_8:         ta_keypad.add('8');       break;
      case KEY_9:         ta_keypad.add('9');       break;
      case KEY_BACKSPACE: ta_keypad.backspace();    break;
      case KEY_LEFT:      ta_keypad.cursor_left();  break;
      case KEY_RIGHT:     ta_keypad.cursor_right(); break;
      case KEY_UP:        break;
      case KEY_DOWN:      break;
      case KEY_ENTER:     break;
    }

    if(ta_keypad.text().length() == 0 ){
      ta_keypad.cursor_type(CURSOR_NONE);     // ไม่ต้องแสดง cursor
    }else{
      ta_keypad.cursor_type(CURSOR_LINE);     // ให้แสดง cursor แบบ line
      ta_keypad.event_send(EVENT_CLICKED);    // ส่งจดหมายไปใน ta_keyapd ให้จำลองว่าเหมือนมีการ clicked เข้ามา
    }
  });

}

void loop() {
  BlynkGO.update();
}
