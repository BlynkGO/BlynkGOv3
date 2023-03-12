/*  BeeNeXT3.5 <---> ESP32 HardSerial2(16,17) */
/*  ผ่านทาง Serial2 (35,22) baudrate 9600       */

#include <BlynkGOv3.h>

GLabel lb_text;
GButton btn(BUTTON_SWITCH, "OFF");

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

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
    BeeNeXT.send("LED", btn.isON());  // ส่ง BeeNeXT ได้แล้ว  ทาง 3.5" Uart Port ( HMI HardSerial)
  });
}

void loop() {
  BlynkGO.update();
}

BEENEXT_DATA(){
  Serial.print("[BEENEXT_DATA] get : ");
  Serial.println(BeeNeXT.data());

  if(BeeNeXT.key() == "lb_text"){ // หากฝั่ง NodeMCU ส่งมาด้วย key "lb_text"
    lb_text = BeeNeXT.value();    // ให้นำ value ที่ส่งมา ไปกำหนดให้วิตเจ็ต lb_text
  }
}

// SERIAL_DATA() {
//   Serial.print("[SERIAL_DATA] get : ");
//   Serial.println(BeeNeXT.data()); 
// }
