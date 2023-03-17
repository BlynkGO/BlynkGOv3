GGaugeRainbow gauge_temp;
GGaugeRainbow gauge_humid;
GLabel  lb_temp_info;
GLabel  lb_humid_info;
GLabel  lb_beenext_connection;
GButton btn(BUTTON_SWITCH, "OFF");

void GRAPHIC::design(){
  gauge_temp.size(140,140);
  gauge_temp.color(TFT_YELLOW,TFT_RED);
  gauge_temp.range(0,60);
  gauge_temp.decimal(2);
  gauge_temp.align(ALIGN_CENTER, -gauge_temp.width()/2-10, 20);

  lb_temp_info = "อุณหภูมิ";
  lb_temp_info.font(prasanmit_20, TFT_SILVER);
  lb_temp_info.align(gauge_temp, ALIGN_IN_BOTTOM, 0,-10);

  gauge_humid.size(gauge_temp);
  gauge_humid.color(TFT_CYAN, TFT_BLUE);
  gauge_humid.range(0,100);
  gauge_humid.decimal(2);
  gauge_humid.align(gauge_temp, ALIGN_RIGHT,20);

  lb_humid_info = "ความชื้น";
  lb_humid_info.font(prasanmit_20, TFT_SILVER);
  lb_humid_info.align(gauge_humid, ALIGN_IN_BOTTOM, 0,-15);

  lb_beenext_connection.font(prasanmit_25,TFT_SILVER);
  lb_beenext_connection.align(ALIGN_BOTTOM_RIGHT,-5);
  
  btn.size(60,60);
  btn.corner_radius(40);
  btn.color(TFT_RED);
  btn.align(ALIGN_TOP_RIGHT,-10,10);
  btn.onValueChanged([](GWidget*widget){
    if(btn.isON()){
      btn = "ON";
      btn.color(TFT_GREEN);
    }else{
      btn = "OFF";
      btn.color(TFT_RED);
    }    
    Serial.println(btn.isON());
    BeeNeXT.send("LED", btn.isON());  // ส่งค่าไปยัง ฝั่ง MCU ด้วย key "LED" และ value คือ สถานะของ btn ว่า ON (true) หรือ OFF (false)
  });
}

void GRAPHIC::BeeNeXT_Connected(){
  lb_beenext_connection = "[BeeNeXT <==> MCU] connected";
  lb_beenext_connection.font(prasanmit_20,TFT_SILVER);
  BeeNeXT.send("LED", btn.isON());
}

void GRAPHIC::BeeNeXT_Disconnected(){
  lb_beenext_connection.font(prasanmit_20,TFT_RED);
  lb_beenext_connection = "[BeeNeXT <==> MCU] disconnected";
}

void GRAPHIC::setTemp(float temp){
  gauge_temp = temp;
}

void GRAPHIC::setHumid(float humid){
  gauge_humid = humid;
}


