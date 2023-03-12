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
    BeeNeXT.send("MY_VALUE", c );  // ฝั่ง MCU ส่งค่า c ไปยัง ฝั่ง จอ BeeNeXT 3.5" ด้วย key "MY_VALUE" & value คือ ค่า c
  });
}

void loop() {
  BeeNeXT.update();
}

BEENEXT_DATA(){ // รับข้อมูลจาก BeeNeXT3.5 เพื่อมาเปิดปิด led  
  if(BeeNeXT.key() == "LED"){ // หากฝั่งจอ BeeNeXT3.5" ส่งมาด้วย key "LED"
    digitalWrite(LED, BeeNeXT.toBool()? TURN_ON : TURN_OFF ); // นำค่า value มาเปลี่ยนเป็นตัวแปร bool แล้วไปกำหนด led 
  }
}
