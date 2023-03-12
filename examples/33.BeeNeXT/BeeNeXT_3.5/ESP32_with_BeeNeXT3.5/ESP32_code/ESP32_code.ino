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
