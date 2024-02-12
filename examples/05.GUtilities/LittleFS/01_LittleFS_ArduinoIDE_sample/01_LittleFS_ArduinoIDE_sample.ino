/*
 * ให้กำหนดใน blynkgo_config.h เปิดใช้ LittleFS 
 * โดยกำหนด
 *    #define BLYNKGO_USE_LITTLEFS     1
 * 
 */

#include <BlynkGOv3.h>
#include "project.h"

GLabel label;

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();

  label = "Hello";

  LITTLEFS::listDir("/", 0);
  LITTLEFS::createDir("/mydir");
  LITTLEFS::writeFile("/mydir/hello2.txt", "Hello2");
  LITTLEFS::readFile("/mydir/hello2.txt");
  LITTLEFS::deleteFile("/mydir/hello2.txt");
  LITTLEFS::removeDir("/mydir");
  LITTLEFS::listDir("/", 0);
}

void loop() {
  BlynkGO.update();
}

