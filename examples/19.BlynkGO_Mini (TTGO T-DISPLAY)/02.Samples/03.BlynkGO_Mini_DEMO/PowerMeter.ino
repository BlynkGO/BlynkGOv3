IMG_DECLARE(img_powermeter);

GImage powermeter_mitsu(img_powermeter);
GRect  rect_powermeter(powermeter_mitsu);
GLabel lb_powermeter_val(rect_powermeter);
float power_val = 432.8;

GLabel lb_power_info("Power (kWh)");
GLabel lb_power_val;
GLabel lb_current_info("Current (A)");
GLabel lb_current_val;
float  current_val = 31.4;

void PowerMeter_create(GWidget&parent){
  //--------------------------------
  // ส่วน รูปมิเตอร์
  powermeter_mitsu.parent(parent);
  powermeter_mitsu.align(ALIGN_LEFT,4);
  rect_powermeter.size(66,19);
  rect_powermeter.align(ALIGN_TOP, 0, 35);
  rect_powermeter.color(TFT_BLACK);
  rect_powermeter.font(prasanmit_20, TFT_WHITE);
  rect_powermeter.letter_space(3);

  lb_powermeter_val.align(ALIGN_RIGHT,-4);
  lb_powermeter_val.colorful(true);

  //--------------------------------
  // ด้านขวา 
  lb_power_info.parent(parent);
  lb_power_info.font(prasanmit_20, TFT_GREEN);
  lb_power_info.align(powermeter_mitsu, ALIGN_RIGHT_TOP, 20,-5);

  lb_power_val.parent(parent);
  lb_power_val.align( ALIGN_TOP_RIGHT, -20, lb_power_info.bottomY()-12);
  lb_power_val.font(prasanmit_35, TFT_WHITE);
  

  lb_current_info.parent(parent);
  lb_current_info.linkStyle(lb_power_info);
  lb_current_info.align(powermeter_mitsu, ALIGN_RIGHT_TOP, 20, lb_power_val.bottomY()-20);

  lb_current_val.parent(parent);
  lb_current_val.align( ALIGN_TOP_RIGHT, -20, lb_current_info.bottomY()-12);
  lb_current_val.linkStyle(lb_power_val);
  lb_current_val = String(current_val, 1);

  PowerMeter_value(432.7);

}

void PowerMeter_value(float val) {
  power_val = val; //mod_f(val,10000);
  lb_powermeter_val = StringX::printf("%04d.#ff0000 %d#", (int) power_val , (int)(power_val*10)%10);  
  lb_power_val = String(power_val,1);
}
