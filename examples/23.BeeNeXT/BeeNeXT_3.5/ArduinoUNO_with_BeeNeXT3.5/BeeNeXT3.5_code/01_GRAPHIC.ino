
GLabel  lb_text;
GButton btn(BUTTON_SWITCH, "OFF");

void GRAPHIC::design(){
  lb_text = "Hello สวัสดีครับ";

  btn.color(TFT_RED);
  btn.align(ALIGN_TOP,0,20);
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

void GRAPHIC::setValue(int value){
  lb_text = value;
}
