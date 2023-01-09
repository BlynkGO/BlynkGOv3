
/*************************************************
 * วิธีแสดงรูปภาพ
 * 1. แปลงรูป png หรือ jpg หรือ bmp เป็น c file 
 *    ด้วย Tool เครื่องมือ BlynkGO_ImgConv (ติดต่อ admin)
 *    ด้วยคำสั่ง 
 *      1.1 สำหรับรูปทั่วไป 
 *         BlynkGO_ImgConv  <รูป>  <ชื่อ>
 *
 *      1.2 สำหรับรูปที่มีพื้นที่โปร่งใส
 *         BlynkGO_ImgConv  <รูป>  <ชื่อ>  t
 *
 * 2. ก่อนใช้งาน ให้ประกาศด้วย IMG_DECLARE(ชื่อรูป) ก่อนเสมอ
 * 
 *************************************************/

#include <BlynkGOv3.h>

IMG_DECLARE(image_test);              // ประกาศก่อนใช้งานรูปภาพแบบ c file

GImage myPicture;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  myPicture = image_test;
  BlynkGO.update();
  delay(2000);
}

uint16_t deg = 0;
uint16_t zm = 256;
int8_t   zm_step = -1;

void loop() {
  BlynkGO.update();

  myPicture.angle(deg);
  myPicture.zoom(zm);

  deg = ANGLE360(deg+6);
  zm += zm_step; 
  if(zm <  25) { zm_step =  1; }
  if(zm > 150) { zm_step = -1; }
}
