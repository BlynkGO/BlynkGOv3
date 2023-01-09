#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <Arduino.h>
#include <SD.h>
#include <SPIFFS.h>

namespace FILE_SYSTEM {
  void listDir(fs::FS &fs, const char * dirname, uint8_t levels=0);
  void createDir(fs::FS &fs, const char * path);
  void removeDir(fs::FS &fs, const char * path);
  void readFile(fs::FS &fs, const char * path);
  void writeFile(fs::FS &fs, const char * path, const char * message);
  void appendFile(fs::FS &fs, const char * path, const char * message);
  void renameFile(fs::FS &fs, const char * path1, const char * path2);
  void deleteFile(fs::FS &fs, const char * path);
}
#endif //  __FILE_SYSTEM_H__
