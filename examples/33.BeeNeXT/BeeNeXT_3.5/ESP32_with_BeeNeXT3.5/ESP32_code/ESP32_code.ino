/*  ESP32 HardSerial2(16,17) <---> BeeNeXT3.5 */

#include <BeeNeXT.h>

#define LED               5
#define TURN_ON           HIGH
#define TURN_OFF          LOW

#define BEENEXT_HARD_RX2  16
#define BEENEXT_HARD_TX2  17

void setup() {
  Serial.begin(115200); Serial.println();
  Serial2.begin(9600, SERIAL_8N1, BEENEXT_HARD_RX2, BEENEXT_HARD_RX2);
  BeeNeXT.begin(&Serial2);

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
