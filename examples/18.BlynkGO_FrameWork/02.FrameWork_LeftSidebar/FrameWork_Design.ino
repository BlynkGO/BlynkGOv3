//--------------------------------------------------------------------
// Framework Widgets
GRect rect_titlebar;
  GLabel        lb_title(titles[0], rect_titlebar);
  GWiFiManager  wifi_manager(rect_titlebar);
#ifndef TTGO_TWATCH
#if BLYNKGO_USE_BLYNK
  GBlynkManager blynk_manager;
#endif
  GLabel        lb_date(rect_titlebar);
  GLabel        lb_clock(rect_titlebar);
#endif

GRect rect_body;
  GPage page_left_sidebar(rect_body);
  GRect indic_selector;

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

    wifi_manager.align(ALIGN_TOP_RIGHT);

#ifndef TTGO_TWATCH    
#if BLYNKGO_USE_BLYNK
    blynk_manager.align(wifi_manager, ALIGN_LEFT, -5);
#endif
    lb_date.align(ALIGN_LEFT, 5);
  #if BLYNKGO_USE_BLYNK
    lb_clock.align(blynk_manager, ALIGN_LEFT, -5);
  #else
    lb_clock.align(wifi_manager, ALIGN_LEFT, -5);
  #endif
#endif

  //----------------------------------------------------
  // Body Design : Left-Sidebar & Screens[i]
  rect_body.size(GScreen.width(),GScreen.height()-rect_titlebar.height() );
  rect_body.align(ALIGN_BOTTOM);
  rect_body.opa(0);
  
  page_left_sidebar.size(84,rect_body.height());
  page_left_sidebar.bg.padding_top(2);                             // กำหนดช่องไฟด้านบนให้ เลเยอร์ bg ของ page เป็น 2
  page_left_sidebar.bg.padding_bottom(2);                          // กำหนดช่องไฟด้านล่างให้ เลเยอร์ bg ของ page เป็น 2
  page_left_sidebar.bg.padding_left(2);                            // กำหนดช่องไฟด้านซ้ายให้ เลเยอร์ bg ของ page เป็น 2
  page_left_sidebar.bg.padding_right(0);                           // กำหนดช่องไฟด้านขวาให้ เลเยอร์ bg ของ page เป็น 0
  page_left_sidebar.scrl.padding(0);                               // กำหนดช่องไฟทุกด้านให้ เลเยอร์ scrl ของ page เป็น 0
  page_left_sidebar.color(TFT_SILVER);
  page_left_sidebar.corner_radius(0);
  page_left_sidebar.scrollbar(SCROLLBAR_AUTOHIDE, false, true);    // scrollbar ให้เป็นแบบ autohide โดย แนวนอนไม่แสดง แนวตั้งให้แสดง
  page_left_sidebar.align(ALIGN_BOTTOM_LEFT);


  // ให้ปุ่มที่สร้างจากรูปภาพ มีแม่คือ page_left_sidebar พร้อมกำหนดคุณสมบัติการกด
  for(int i=0; i < SCREEN_NUM; i++){
    btns[i].parent(page_left_sidebar);                          // ให้เป็นลูกของ page_left_sidebar
    btns[i].page_glue(true);                                    // ให้ปุ่ม สามารถเลื่อนติดไปกับ page ด้วย (เวลาเลื่อนสัมผัสไม่ถูกสัญญาณ click ขวาง)
    btns[i].onClicked(btns_on_clicked);

    // สร้าง screens array แต่ละตัว ที่จะไปสัมพันธ์กับปุ่มกด    
    screens[i].size(GScreen.width()-page_left_sidebar.width(), page_left_sidebar.height());
    screens[i].parent(rect_body);
    screens[i].color(random(0xFFFF));
    screens[i].align(page_left_sidebar, ALIGN_RIGHT);
    screens[i].hidden(i>0);                                     // screens array ทุกตัว ให้ซ่อนลงไปก่อน ยกเว้น screens แรก
  }

  indic_selector.size(78,78);
  indic_selector.opa(0);
  indic_selector.border(2, TFT_NAVY);
  indic_selector.corner_radius(15);
  indic_selector.clickable(false);
  indic_selector.parent(btns[0]);
  
  page_left_sidebar.toForeground();                             // ย้ายมาเป็น layer บนสุด
}

void btns_on_clicked(GWidget*widget){
  int cur_id = (GImageButton*) widget - &btns[0];       // เช็คว่า ปุ่มที่ clicked มานั้น เป็น id ลำดับที่เท่าไรใน btns array
//  Serial.println(cur_id);
  indic_selector.parent(btns[cur_id]);
  for(int i=0; i < SCREEN_NUM; i++) {
    screens[i].hidden(i != cur_id );                // screens array ให้แสดงขึ้นมาเฉพาะ id ที่ตรงกับ ของปุ่มกด  ที่เหลือซ่อนลงไป
    lb_title = titles[cur_id];
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
