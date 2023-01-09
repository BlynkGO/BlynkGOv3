//----------------------------------------------------
// Framework Widgets
GRect        rect_titlebar;
  GLabel        lb_title(titles[0], rect_titlebar);
  GLabel        lb_date(rect_titlebar);
  GLabel        lb_clock(rect_titlebar);
  GWiFiManager  wifi_manager(rect_titlebar);
  GBlynkManager blynk_manager(rect_titlebar);
  
  
void framework_design(){
  BlynkGO.fillScreen(TFT_COLOR(50,50,50));
  
  //----------------------------------------------------
  // Title Bar Design
  rect_titlebar.width(GScreen);   // ให้ ความกว้าง เท่ากับ หน้าจอหลัก 
  rect_titlebar.color(TFT_BLACK);
  rect_titlebar.font(prasanmit_30, TFT_SILVER);
  rect_titlebar.align(ALIGN_TOP);
  
    lb_title.align(ALIGN_CENTER);
    lb_title.font(prasanmit_40,TFT_WHITE);

  rect_titlebar.height(lb_title); // ให้ ความสูง เท่ากับ lb_title

    wifi_manager.align(ALIGN_TOP_RIGHT);
    blynk_manager.align(wifi_manager, ALIGN_LEFT,-5);
    lb_clock.align(blynk_manager, ALIGN_LEFT,-5);
    lb_date.align(ALIGN_LEFT, 5);

  //----------------------------------------------------
  // Body Design : multiscreen
  screens.size(GScreen.width(), GScreen.height() - rect_titlebar.height());
  screens.addScreen(SCREEN_NUM);
  screens.align(ALIGN_BOTTOM);
  screens.onValueChanged([](GWidget*w){ 
    lb_title = titles[screens.current_screen_id()];
  });  
}

const char* th_weekday[] = {"อา.","จ.", "อัง.", "พ.", "พฤ.", "ศ.", "ส." };
NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_date  = StringX::printf("%s%02d/%02d", th_weekday[wday()], day(), month());
    lb_clock = StringX::printf("%02d:%02d", hour(), minute());

  });
}

