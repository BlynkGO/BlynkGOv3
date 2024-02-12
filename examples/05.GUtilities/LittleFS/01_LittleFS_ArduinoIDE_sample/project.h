#pragma once

// #include <BlynkGOv3.h>
#include <LittleFS.h>
#include <time.h>

namespace LITTLEFS {
  void begin();  
  bool format();
  void listDir(const char * dirname, uint8_t levels);
  void createDir(const char * path);
  void removeDir(const char * path);
  void readFile(const char * path);
  void writeFile(const char * path, const char * message);
  void appendFile(const char * path, const char * message);
  void renameFile(const char * path1, const char * path2);
  void deleteFile(const char * path);
  void writeFile2(const char * path, const char * message);
  void deleteFile2(const char * path);
  void testFileIO(const char * path);
}
