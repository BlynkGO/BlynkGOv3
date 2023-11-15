
namespace SD_CARD {
  void listDir(const char * dirname, uint8_t levels);
}

void SD_CARD::listDir(const char * dirname, uint8_t levels){
  if(!BlynkGO.sd_mount()) {
    Serial.println("[SD] not mount");
    return;
  }
  
  Serial.printf("[SD] Listing directory: %s\n", dirname);

  File root = SD.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        SD_CARD::listDir(file.path(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
