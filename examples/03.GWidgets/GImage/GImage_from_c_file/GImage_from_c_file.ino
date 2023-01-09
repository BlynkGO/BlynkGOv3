
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

IMG_DECLARE(image_test);              // c file ต้องประกาศก่อนใช้งาน

GImage myPicture;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  myPicture = image_test;             // กำหนดรูปภาพแบบ c file
}

void loop() {
  BlynkGO.update();
}

