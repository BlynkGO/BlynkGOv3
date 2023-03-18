/*********************************************************
 *  BeeNeXT2.8 <---> MCU 
 * 
 *   - ฝั่งจอ2.8นิ้ว เสียบที่ HMI Uart socket ข้าง usb
 *     
 * *******************************************************/

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

