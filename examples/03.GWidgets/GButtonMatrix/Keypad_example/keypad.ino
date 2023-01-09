/***************************************************
 * Keypad Example
 * 
 * เลือกภาษา ได้จาก config/blynkgo_config.h
 * select language in config/blynkgp_config.h
 * 
 ***************************************************/

#if defined (ENG_TEXT)
  #define KEY_OK        "OK"
  #define KEY_CLEAR     "Clear"
  #define KEY_DEL       "Del"
#elif defined (LAOS_TEXT)
  #define KEY_OK        "ຕົກ​ລົງ"
  #define KEY_CLEAR     "ລ້າງ"
  #define KEY_DEL       "ລົບ"
#elif defined (VIETNAM_TEXT)
  #define KEY_OK        "đồng ý"
  #define KEY_CLEAR     "mới"
  #define KEY_DEL       "Del"
#else
  #define KEY_OK        "ตกลง"
  #define KEY_CLEAR     "ล้าง"
  #define KEY_DEL       "ลบ"
#endif

const char * keypad_map[] = { "1", "2", "3", "\n",
                              "4", "5", "6", "\n",
                              "7", "8", "9", "\n",
                              ".", "0", KEY_DEL, ""};

IMG_DECLARE(img_background);

GTextArea    *_ta=NULL;
GImage        keypad_bg(img_background);          // วิตเจ็ต GImage
GTextArea     keypad_ta(keypad_bg);               // วิตเจ็ต GTextArea สำหรับกรอกข้อความ
GButtonMatrix keypad_btnm(keypad_map, keypad_bg); // วิตเจ็ต GButtonMatrix และกำหนด key map
GButton       keypad_ok(KEY_OK,keypad_bg);           // วิตเจ็ต GButton ปุ่ม OK
GButton       keypad_clear(KEY_CLEAR,keypad_bg);     // วิตเจ็ต GButton ปุ่ม Clear

void keypad(GTextArea& textarea){
  _ta = &textarea;
  
  if(!keypad_btnm.isCreated()) {
    keypad_bg.create();
    
    keypad_ta.width(200);
    keypad_ta.align(ALIGN_TOP_RIGHT,-20,20);
    keypad_ta.default_keyboard(false);
  
    keypad_btnm.size(240,320);           // size  width x height
    keypad_btnm.padding(8);              // ช่องไฟระหว่างปุ่มกดภายใน keypad
    keypad_btnm.opa(0);                  // พื้นหลังปุ่มกด ไม่ให้แสดงสีใดๆ ให้โปร่งใส
    keypad_btnm.align(ALIGN_LEFT, 10);
  
    // customize keypad's style
    keypad_btnm.style[KEYPAD_BG].border(0);
    keypad_btnm.style[RELEASED].color(TFT_BLUE, TFT_CYAN);
    keypad_btnm.style[PRESSED].color(TFT_NAVY);
    keypad_btnm.font(prasanmit_40,TFT_WHITE);
  
    keypad_btnm.onValueChanged([](GWidget*w){
      String key = keypad_btnm.active_button();
      if(key=="") return;
      ( key == KEY_DEL)?  keypad_ta.backspace() : keypad_ta.add(key);
    });
  
    keypad_ok.width(150);  
    // customize keypad_ok's style
    keypad_ok.style[RELEASED].color(TFT_BLUE, TFT_CYAN);
    keypad_ok.style[PRESSED].color(TFT_NAVY);
    keypad_ok.font(prasanmit_40, TFT_WHITE);
    keypad_ok.align(keypad_ta, ALIGN_BOTTOM, 0, 30);
    keypad_ok.onClicked([](GWidget*w){
      Serial.println( keypad_ta.text() );
      if(_ta != NULL){
        _ta->text( keypad_ta.text() );
        _ta->cursor_pos(CURSOR_LAST);
        keypad_bg.hidden(true);
      }
    });
  
    keypad_clear.width(150);
    // customize keypad_clear's style
    keypad_clear.style[RELEASED].color(TFT_RED,TFT_ORANGE);
    keypad_clear.style[PRESSED].color(TFT_DARKRED);
    keypad_clear.font(prasanmit_40, TFT_WHITE);
    keypad_clear.align(keypad_ok, ALIGN_BOTTOM, 0, 30);
    keypad_clear.onClicked([](GWidget*w){
      keypad_ta = "";
    });
  }


  keypad_ta.text(_ta->text());
  keypad_ta.cursor_pos(CURSOR_LAST);

  keypad_bg.hidden(false);
  keypad_bg.toForeground();
}


