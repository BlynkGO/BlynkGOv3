/*  ArduinoUNO SoftSerial(8,9) <---> BeeNeXT3.5 */

#include <BeeNeXT.h>

#define LED               13
#define TURN_ON           HIGH
#define TURN_OFF          LOW

#define BEENEXT_SOFT_RX   8
#define BEENEXT_SOFT_TX   9

void setup() {
  Serial.begin(115200); Serial.println();
  BeeNeXT.begin(SOFT_RX, SOFT_TX);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, TURN_OFF);

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    static int c;
    Serial.println(++c);
    BeeNeXT.send("lb_text", c );  // ฝั่ง NodeMCU ส่งค่า c ไปยัง ฝั่ง จอ BeeNeXT 3.5" ด้วย key "lb_text" & value คือ ค่า c
  });
}

void loop() {
  BeeNeXT.update();
}

// SERIAL_DATA() {
//   Serial.print("[SERIAL_DATA] get : ");
//   Serial.println(BeeNeXT.data()); 
// }

BEENEXT_DATA(){ // รับได้แล้วทาง SoftSerial จาก 3.5 HMI Uart Port เพื่อมาเปิดปิด led 
  Serial.print("[BEENEXT_DATA] get : ");
  Serial.println(BeeNeXT.data());
  
  if(BeeNeXT.key() == "LED"){ // หากฝั่งจอ BeeNeXT3.5" ส่งมาด้วย key "LED"
    digitalWrite(LED, BeeNeXT.toBool()? TURN_ON : TURN_OFF ); // นำค่า value มาเปลี่ยนเป็นตัวแปร bool แล้วไปกำหนด led 
  }
}
