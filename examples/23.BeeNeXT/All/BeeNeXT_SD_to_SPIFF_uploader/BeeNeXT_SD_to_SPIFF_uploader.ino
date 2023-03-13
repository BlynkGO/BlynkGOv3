/**************************************
 * ตัวอย่าง upload ไฟล์จาก SD://spiffs/*.*  ไปไว้ที่ SPIFFS://*.*
 * หรือ download ไฟล์จาก SPIFFS://*.*   มาไว้ที่  SD://spiffs/*.*
 * 
 * โดย เปิดใช้ 
 *     #define SD_TO_SPIFFS
 *  หรือ 
 *     #define SPIFFS_TO_SD
 *  อย่างใดอย่างหนึ่ง
 *  
 **************************************/


#include "FileSystem.h"

// เลือกอย่างใดอย่างหนึ่ง
//#define SD_TO_SPIFFS
#define SPIFFS_TO_SD

void setup() {
  Serial.begin(115200); Serial.println();
  SPIFFS.begin();
  if (!SD.begin(5)) {
    Serial.println("[SD] failed");
    return;
  }else{
    Serial.println("[SD] inited");
  }

#if defined (SD_TO_SPIFFS)
  FILE_SYSTEM::listDir(SD, "/spiffs");
  File _folder = SD.open("/spiffs");  
#elif defined (SPIFFS_TO_SD)
  FILE_SYSTEM::listDir(SPIFFS, "/");
  File _folder = SPIFFS.open("/");
#endif

  File file = _folder.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      String filename = file.name();
#if defined (SD_TO_SPIFFS)
      filename.replace("/spiffs/", "");
      Serial.printf("[FileSystem] copy SD://spiffs/%s --> SPIFFS://%s\n", filename.c_str(), filename.c_str());
      File file_src  = SD.open(file.name());
      File file_dest = SPIFFS.open(file.name(), FILE_WRITE);
#elif defined(SPIFFS_TO_SD)
      Serial.printf("[FileSystem] copy SPIFFS:/%s --> SD://spiffs%s\n", filename.c_str(), filename.c_str());      
      File file_src  = SPIFFS.open(file.name());
      File file_dest = SD.open( String("/spiffs") + file.name(), FILE_WRITE);
#endif
      while (file_src.available()) {
        file_dest.write(file_src.read());
      }
      file_dest.close();
      file_src.close();
    }
    file = _folder.openNextFile();
  }
#if defined (SD_TO_SPIFFS)
  Serial.println("[SD] copy '/spiffs' --> SPIFFS drive ... Done!");
#elif defined(SPIFFS_TO_SD)
  Serial.println("[SPIFFS] copy '/' --> SD '/spiffs' ... Done!");
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
