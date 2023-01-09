
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
GLabel label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  myPicture = image_test;
  label     = "ภาพปกติ";

  label.align(ALIGN_IN_BOTTOM_MID);
}
  
void loop() {
  BlynkGO.update();
  delay(2000);
  
  myPicture.filter(TFT_GREEN);        // ใส่ filter สีเขียว
  label = "ใส่ filter สีเขียว";          // เปลี่ยนข้อความเดิมเป็นข้อความใหม่
  
  BlynkGO.update();                    // อัพเดต แสดงผลล่าสุด
  delay(2000);

  myPicture.reset();                  // คืนค่าสู่รูปปกติ
  myPicture.opa(80);                  // ความทึบของภาพ 0 : โปร่งใส , 255 : ภาพทึบปกติ
  label = "ปรับความโปร่งใสเป็น 80";     // เปลี่ยนข้อความเดิมเป็นข้อความใหม่
  
  BlynkGO.update();                    // อัพเดต แสดงผลล่าสุด
  delay(2000);

  myPicture.zoom(40);                 // ซูมรูป
  label = "Zoom 40%";                // เปลี่ยนข้อความเดิมเป็นข้อความใหม่
  
  BlynkGO.update();                    // อัพเดต แสดงผลล่าสุด
  delay(2000);

  myPicture.reset(); 
  myPicture.angle(120);               // หมุน (องศา) ตามเข็มจาก 12 นาฬิกา
  label = "หมุน 120 องศา";            // เปลี่ยนข้อความเดิมเป็นข้อความใหม่
  
  BlynkGO.update();                    // อัพเดต แสดงผลล่าสุด
  delay(2000);

  myPicture.reset();                  // คืนค่าสู่รูปปกติ
  label = "ภาพปกติ";                  // เปลี่ยนข้อความเดิมเป็นข้อความใหม่
}

