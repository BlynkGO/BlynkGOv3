/************************************************
 * 
 * สำหรับ BlynkGO Board 3.5 inch capacitive touch
 * หรือ BlynkGO Enterprise 5", 7"
 * หรือ TTGO-TWatch 2019 (รุ่นที่มีช่อง micro-sd)
 * 
 * ตัวอย่างสำหรับ list ไฟล์ทั้งหมดบน sd ออกมา
 * 
 ************************************************/

#include <BlynkGOv3.h>

void printDirectory(File dir, int numTabs);
File root;

GList list_sdfile;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

#if defined(BLYNKGO_ENTERPRISE)
  list_sdfile.size(400, 300);
#else
  list_sdfile.size(160, 120);
#endif

  root = SD.open("/");
  printDirectory(root, 0);

}

void loop() {
  BlynkGO.update();
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    list_sdfile.addItem(entry.name());  // เพิ่มชื่อไฟล์ เข้าในวิตเจ็ต list_sdfile
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
