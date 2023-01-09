//--------------------------------------------------------------------
// Framework Widgets
//--------------------------------------------------------------------
GRect rect_titlebar;  // rect_titlebar มี wifi_manager, lb_title, btn_dock เป็นเลเยอร์ลูก
  GWiFiManager  wifi_manager(rect_titlebar);
  GLabel        lb_title(titles[0], rect_titlebar);
  GButton       btn_dock(SYMBOL_LIST,rect_titlebar);
#ifndef TTGO_TWATCH
  GLabel        lb_date(rect_titlebar);
  GLabel        lb_clock(rect_titlebar);
#if BLYNKGO_USE_BLYNK
  GBlynkManager blynk_manager;
#endif
#endif

  
GRect rect_body;      // rect_body มี page_dockmenu และ screens[i] ต่างๆ เป็นเลเยอร์ลูก
  GPage page_dockmenu(rect_body);

  GAnimation  anim_page_dockmenu;
  GTimer      timer_anim;

//--------------------------------------------------------------------
// Framework Design
//--------------------------------------------------------------------
void framework_design(){

  //----------------------------------------------------
  // Title Bar Design
  rect_titlebar.width(GScreen);
  rect_titlebar.color(TFT_BLACK);
  rect_titlebar.font(prasanmit_30, TFT_SILVER);
  rect_titlebar.align(ALIGN_TOP);
  
    lb_title.align(ALIGN_CENTER);
    lb_title.font(prasanmit_40,TFT_WHITE);

  rect_titlebar.height(lb_title);
  // Serial.printf("[TitleBar] Height : %d\n", rect_titlebar.height());

    wifi_manager.align(ALIGN_TOP_LEFT,5,0);
    
    btn_dock.size(50,50);
    btn_dock.opa(0);                            // ปรับค่าโปร่งใส 0 เวลาปกติ
    btn_dock.rounded(true); 
    btn_dock.padding(0); 
    btn_dock.border(0);
    btn_dock.style[PRESSED].color( TFT_WHITE);
    btn_dock.style[PRESSED].opa(32);            // ปรับค่าให้โปร่งใส 32 เวลากด
    btn_dock.align(ALIGN_TOP_RIGHT,-5,0);
    btn_dock.onClicked([](GWidget*w){
      page_dockmenu_init_show_or_hide();
    });

#ifndef TTGO_TWATCH    
    lb_date.align(wifi_manager, ALIGN_RIGHT, 5);

#if BLYNKGO_USE_BLYNK
    blynk_manager.align(btn_dock, ALIGN_LEFT, -5);
    lb_clock.align(blynk_manager, ALIGN_LEFT, -5);
#else
    lb_clock.align(btn_dock, ALIGN_LEFT, -5);
#endif

#endif
  
  //----------------------------------------------------
  // Body Design : DockMenu & Screens[i]
  rect_body.size(GScreen.width(),GScreen.height()-rect_titlebar.height() );
  rect_body.align(ALIGN_BOTTOM);
  rect_body.opa(0);
  rect_body.gesture(true);
  rect_body.onGesture([](GWidget*w){
    gesture_dir_t gesture_dir = w->touch_gesture();
    Serial.println(gesture_dir);
    switch(gesture_dir){
      case GESTURE_DIR_TOP:
        if( page_dockmenu.posY() == rect_body.height() ){
          anim_page_dockmenu_show();
        }
        break;
      case GESTURE_DIR_BOTTOM:
        if( page_dockmenu.posY() == rect_body.height() - page_dockmenu.height() ){
          anim_page_dockmenu_hide();
        }
        break;
    }
  });
  rect_body.onPressed([](GWidget*w){
    if( page_dockmenu.posY() == rect_body.height() - page_dockmenu.height() ){
      anim_page_dockmenu_hide();
    }
  });

  // กำหนดคุณสมบัติของ วิตเจ็ต GPage ที่จะทำเป็น Side Bar
  page_dockmenu.size( GScreen.width(),84);
  page_dockmenu.corner_radius(0);
  page_dockmenu.layout(LAYOUT_ROW_M);
  page_dockmenu.bg.padding(0);                                 // กำหนดช่องไฟให้ เลเยอร์ bg ของ page เป็น 0
  page_dockmenu.scrl.padding(0);                               // กำหนดช่องไฟให้ เลเยอร์ scrl ของ page เป็น 0
  page_dockmenu.color(TFT_SILVER);
  page_dockmenu.bg.opa(150);
  page_dockmenu.scrl.opa(0);
  page_dockmenu.scrollbar(SCROLLBAR_AUTOHIDE, true, false);    // scrollbar ให้เป็นแบบ autohide โดย แนวนอนแสดง แนวตั้งไม่ให้แสดง
  page_dockmenu.position(0,rect_body.height());
  page_dockmenu.event_cb([](GWidget*w, event_t event){
    switch(event){
      case EVENT_PRESSING:
        timer_anim.del();
        Serial.println("[Page] Pressing");
        break;
      case EVENT_PRESS_LOST:
      case EVENT_RELEASED:
        timer_anim.setOnce(3000L,[](){
          if( page_dockmenu.posY() == rect_body.height() - page_dockmenu.height()) {
            anim_page_dockmenu_hide();
          }
        });
        Serial.println("[Page] Released");
        break;
    }
  });
  
  // ให้ปุ่มที่สร้างจากรูปภาพ มีแม่คือ page_dockmenu พร้อมกำหนดคุณสมบัติการกด
  for(int i=0; i < SCREEN_NUM; i++){
    btns[i].parent(page_dockmenu);                             // ให้เป็นลูกของ page_dockmenu
    btns[i].page_glue(true);                                  // ให้ปุ่ม สามารถเลื่อนติดไปกับ page ด้วย (เวลาเลื่อนสัมผัสไม่ถูกสัญญาณ click ขวาง)
    btns[i].event_parent(true);                               // ส่ง event ไปยัง แม่คือ page ด้วย
    btns[i].onClicked([](GWidget*widget){                     // เมื่อมีการ click มาที่ปุ่ม
      int btns_id = (GImageButton*) widget - &btns[0];        // เช็คว่า ปุ่มที่ clicked มานั้น เป็นลำดับที่เท่าไรใน btns array
      Serial.println(btns_id);
      lb_title = titles[btns_id];
      for(int i=0; i < SCREEN_NUM; i++)
        screens[i].hidden(i != btns_id );                      // screens array ให้แสดงขึ้นมาเฉพาะ id ที่ตรงกับ ของปุ่มกด  ที่เหลือซ่อนลงไป

      anim_page_dockmenu_hide();
    });

    // สร้าง screens array แต่ละตัว ที่จะไปสัมพันธ์กับปุ่มกด
    screens[i].parent(rect_body);
    screens[i].size(rect_body);
    screens[i].color(random(0xFFFF));
    screens[i].event_parent(true);
    screens[i].align(ALIGN_BOTTOM);
    screens[i].hidden(i>0);                                    // screens array ทุกตัว ให้ซ่อนลงไปก่อน ยกเว้น screens แรก
  }
}

void anim_page_dockmenu_hide(){
  page_dockmenu.toForeground();
  anim_page_dockmenu.init(page_dockmenu, ANIM_Y, rect_body.height()-page_dockmenu.height(),rect_body.height(),1000);
  anim_page_dockmenu.path_type(ANIM_PATH_EASE_IN);
  anim_page_dockmenu.create();
}

void anim_page_dockmenu_show(){

  page_dockmenu.toForeground();
  anim_page_dockmenu.init(page_dockmenu, ANIM_Y, rect_body.height(), rect_body.height()-page_dockmenu.height(),1000);
  anim_page_dockmenu.path_type(ANIM_PATH_EASE_IN);
  anim_page_dockmenu.ready_cb([](GAnimation*a){    
    timer_anim.setOnce(3000L,[](){
      if( page_dockmenu.posY() == rect_body.height() - page_dockmenu.height()) {
        anim_page_dockmenu_hide();
      }
    });
  });
  anim_page_dockmenu.create();
}

void page_dockmenu_init_show_or_hide(){
  if( page_dockmenu.posY() == rect_body.height() ) {
    anim_page_dockmenu_show();
  }
  else 
  if( page_dockmenu.posY() == rect_body.height() - page_dockmenu.height() ){
    anim_page_dockmenu_hide();
  }
}

#ifndef TTGO_TWATCH
const char* th_weekday[] = {"อา.","จ.", "อัง.", "พ.", "พฤ.", "ศ.", "ส." };
NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_date  = StringX::printf("%s%02d/%02d", th_weekday[wday()], day(), month());
    lb_clock = StringX::printf("%02d:%02d", hour(), minute());
  });
}
#endif
