/*  BeeNeXT3.5 <---> MCU                  */
/*  ผ่านทาง Serial2 (35,22) baudrate 9600  */

#include <BlynkGOv3.h>
#include "00_resource.h"

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  GRAPHIC::design();
}

void loop() {
  BlynkGO.update();
}

BEENEXT_DATA(){
  if(BeeNeXT.key() == "MY_VALUE"){        // หากฝั่ง NodeMCU ส่งมาด้วย key "MY_VALUE"
    GRAPHIC::setValue( BeeNeXT.toInt() ); // ค่า value จาก NodeMCU ที่ส่งมา เปลี่ยนเป็น int แล้วกำหนดให้ GRAPHIC ไปใช้งาน
  }
}

