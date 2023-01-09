/*****************************************************
 * ตัวอย่าง ส่ง Line Notify แบบไม่ต้องรอหยุดชะงัก
 *  ในระหว่างการเชื่อมต่อไปยัง Server ของ Line Notify
 *  APIs จะใช้ เหมือนกับ TridentTD_LineNotify ไปได้เลย
 *  
 *  ให้เปิดการใช้งาน ใน blynkgo_config.h
 *  
 *  #define BLYNKGO_USE_WIFI          1
 *  #define BLYNKGO_USE_LINENOTIFY    1
 *  
 *****************************************************/
 
 
#include <BlynkGOv3.h>

#define SSID            "--------------------------"
#define PASSWORD        "--------------------------"
#define LINE_TOKEN      "--------------------------"

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  WiFi.begin(SSID, PASSWORD);
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED(){
  Serial.println("[WiFi] connected");
  Serial.println("LINE Notify...");
  
  // ตัวอย่างส่งข้อความ
  LINE.notify("อุณหภูมิ เกินกำหนด");

  // ตัวอย่างส่งข้อมูล ตัวเลข
  LINE.notify(2342);          // จำนวนเต็ม
  LINE.notify(212.43434,5);   // จำนวนจริง แสดง 5 หลัก

  // เลือก Line Sticker ได้จาก https://devdocs.line.me/files/sticker_list.pdf
  LINE.notifySticker( 3, 240);
  LINE.notifySticker("Hello", 1, 2);

  // ตัวอย่างส่ง รูปภาพ ด้วย url
  LINE.notifyPicture("https://preview.ibb.co/j6G51n/capture25610417181915334.png");
  LINE.notifyPicture("จตุธาตุ","https://www.fotoaparat.cz/storage/pm/09/10/23/670915_a5351.jpg");
}
