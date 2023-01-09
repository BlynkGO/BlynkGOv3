/****************************************************************************
 *  ตัวอย่าง resource รูป ที่แปลง เป็น .bin (binary file)
 *   แล้วนำไปวางบน SD card หรือ วางบน SPIFFS
 *   ช่วยในการลดพื้นที่ drom ของ ESP32 ไม่ให้บวมล้น
 *   (หากใช้เป็น resource รูปแบบ .c file)
 *
 *  วิธีแปลงรูปแบบ bin file (.bin)
 *  --------------------------
 *   1. ติดตั้ง BlynkGO_ImgConv  version 1.6 ขึ้นไป
 *   2. เรียกใช้คำสั่ง
 *      2.1 ภาพแบบธรรมดา
 *           ตัวอย่าง
 *             > blynkgo_imgconv picture.png  pic_name  bin565
 *      2.2 ภาพแบบโปร่งใส
 *           ตัวอย่าง
 *             > blynkgo_imgconv picture.png  pic_name  t  bin565
 *
 *  ก่อน compile
 *  -------------
 *   1. วาง resource รูปแบบ .bin ไว้ใน subfolder ของโปรเจคที่สร้าง ( กด Ctrl+K )
 *      ด้วย ชือ ซับโฟลเดอร์ data
 *   2. ปิด Serial Monitor หากเปิดอยู่
 *   3. ทำการ upload โฟลเดอร์ data ของ โปรเจคที่เตรียมไว้ ขึ้นบน SPIFFS
 *      ด้วยเมนู Tools > ESP32 Sketch Data Upload
 *
 *      (เมนูนี้ติดตั้ง อ่านจากที่  https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/ )
 *
 ***************************************************************************/

#include <BlynkGOv3.h>

GImage image;

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // image = "SD://spiderman.bin";    // โหลดรูป binary file แบบ bin565 ที่วางอยู่บน SD
  image = "SPIFFS://spiderman.bin";   // โหลดรูป binary file แบบ bin565 ที่วางอยู่บน SPIFFS

}

void loop(){
  BlynkGO.update();
}

