/*********************************************************
 *  BeeNeXT3.5 <---> MCU 
 * 
 *   - ฝั่งจอ3.5นิ้ว เสียบที่ socket ด้านบนขวา
 *     โดยต่อ 3 สาย คือ สายสีดำ,เหลือง,น้ำเงิน ยกเว้นสายสีแดง
 *     และ usb เสียบไฟเลี้ยง
 *     
 * *******************************************************/

#include <BlynkGOv3.h>
#include "00_resource.h"

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BeeNeXT.enable(true);
  GRAPHIC::design();
}

void loop() {
  BlynkGO.update();
}


