void multiscreen_btn_ctrl(){
  BTN1.reset();
  BTN2.reset();

  // ปุ่ม 1 หากผู้ใช้ทำการ clicked
  BTN1.onPressed([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() < multiscreen.num()-1) {
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
    }else{
      multiscreen.current_screen( 0, false);
    }

    // อีก 10วินาที หากไม่มีการกดให้กลับไปให้ keylock ขึ้นมาทำงาน
    timer_keylock.setOnce(KEYLOCK_SAVER_INTERVAL, [](){ app_keylock.hidden(false); app_keylock_btn_ctrl(); }); 
  });  

  //--------------------------------------------
  // ปุ่ม 2 หากผู้ใช้ทำการ pressed
  BTN2.onPressed([](ButtonISR* btn) {
    uint8_t screen_id = multiscreen.current_screen_id();
    if( screen_id >=1 && screen_id <=3 )
      sw3d[screen_id -1].toggle();

    // อีก 10วินาที หากไม่มีการกดให้กลับไปให้ keylock ขึ้นมาทำงาน
    timer_keylock.setOnce(KEYLOCK_SAVER_INTERVAL, [](){ app_keylock.hidden(false); app_keylock_btn_ctrl(); });
  });
}

// ทำการ random key ต่างๆ ใหม่
void app_keylock_random_key(){
  for(int i=0; i < KEYLOCK_NUM; i++){
    key[i] = random(10);
  }
  
}

// ตัวปรับให้กลับสู่สถานะ ไม่ได้ edit ใน app_password
void app_keylock_clear_edit_mode(){
  keylock_edit_mode = false;
  for(int i=0; i < KEYLOCK_NUM; i++){
    lb_key[i] = String("*");
    lb_key[i].border(3  , TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));
  }
  lb_ok.border(3  , TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));
  lb_ok.refreshStyle();
}

// ตัวแปลงค่า key array เป็นค่า password 
int key2password(uint8_t *key, size_t len){
  if(len == 0 || key== NULL) return 0;
  int password = 0;
  for(int i=0; i < len; i++){
    password = password*10 + key[i];
  }
  Serial.println(password);
  return password;
}

// ตัวควบคุมปุ่มกด สำหรับ app_keylock
void app_keylock_btn_ctrl(){
  btn_ctrl_reset();  // clear ที่เคยกำหนด ปุ่มว่าจะให้ไปทำอะไรๆ ต่อ

  // reset 
  key_id = 0; app_keylock_clear_edit_mode();

  BTN1.onPressed([](ButtonISR* btn) {
    if(keylock_edit_mode) {
      key_id = (key_id+1)% (KEYLOCK_NUM+1);  
    }else{
      keylock_edit_mode = true;
      if(key_id == KEYLOCK_NUM) key_id = 0;
    }
    
    for(int i=0; i < KEYLOCK_NUM; i++){
      lb_key[i] = (i==key_id)? String(key[i]) : String("*");
      lb_key[i].border(3  , (i==key_id)? TFT_GREEN : TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));
    }
    Serial.println(key_id);
    lb_ok.border(3  , (key_id== KEYLOCK_NUM)? TFT_GREEN : TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));
    lb_ok.refreshStyle();

    timer_keylock.setOnce(5000L, app_keylock_clear_edit_mode);
  });
  //--------------------------------------------
  BTN2.onPressed([](ButtonISR* btn) {
    if(keylock_edit_mode) {
      if(key_id < KEYLOCK_NUM) { 
        key[key_id] = (key[key_id]+1)%10;
        lb_key[key_id] = String(key[key_id]);
      }else
      if(key_id == KEYLOCK_NUM) {
        if( key2password( key, KEYLOCK_NUM) == KEYLOCK_PASSWORD ) {
          app_keylock.hidden(true);
          app_keylock_random_key();  // สลับ key เป็ฯค่าอื่นๆ ป้องกันรอบถัดๆไป

          multiscreen_btn_ctrl();    // เปลี่ยนให้ ปุ่มควบคุมสำหรับ multiscreen

          // อีก 10วินาที หากไม่มีการกดให้กลับไปให้ keylock ขึ้นมาทำงาน
          timer_keylock.setOnce(KEYLOCK_SAVER_INTERVAL, [](){ app_keylock.hidden(false); app_keylock_btn_ctrl(); });
        }else{
          msg_box.hidden(false);
          
          timer_msg_box.setOnce(2000, [](){ msg_box.hidden(true);});
        }
      }
    }else{
      keylock_edit_mode = true;
      if(key_id == KEYLOCK_NUM) key_id = 0;
      for(int i=0; i < KEYLOCK_NUM; i++){
        lb_key[i] = (i==key_id)? String(key[i]) : String("*");
        lb_key[i].border(3  , (i==key_id)? TFT_GREEN : TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));
      }
      lb_ok.border(3  , (key_id== KEYLOCK_NUM)? TFT_GREEN : TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));
      lb_ok.refreshStyle();
    }
    timer_keylock.setOnce(5000L,app_keylock_clear_edit_mode);
  });
}
