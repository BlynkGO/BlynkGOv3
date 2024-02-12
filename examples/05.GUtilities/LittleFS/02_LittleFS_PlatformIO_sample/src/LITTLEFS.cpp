#include <LittleFS.h>
#include <time.h>
#include "project.h"

#define FORMAT_LITTLEFS_IF_FAILED true

void LITTLEFS::begin() {
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
}

bool LITTLEFS::format(){
  return LittleFS.format();
}

void LITTLEFS::listDir(const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = LittleFS.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");

      Serial.print(file.name());
      time_t t = file.getLastWrite();
      struct tm * tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, ( tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);

      if (levels) {
        listDir(file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");

      Serial.print(file.size());
      time_t t = file.getLastWrite();
      struct tm * tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, ( tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour , tmstruct->tm_min, tmstruct->tm_sec);
    }
    file = root.openNextFile();
  }
}

void LITTLEFS::createDir(const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (LittleFS.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void LITTLEFS::removeDir(const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (LittleFS.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void LITTLEFS::readFile(const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = LittleFS.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void LITTLEFS::writeFile(const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void LITTLEFS::appendFile(const char * path, const char * message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = LittleFS.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void LITTLEFS::renameFile(const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (LittleFS.rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void LITTLEFS::deleteFile(const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (LittleFS.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

// SPIFFS-like write and delete file

// See: https://github.com/esp8266/Arduino/blob/master/libraries/LittleFS/src/LittleFS.cpp#L60
void LITTLEFS::writeFile2(const char * path, const char * message) {
  if (!LittleFS.exists(path)) {
    if (strchr(path, '/')) {
      Serial.printf("Create missing folders of: %s\r\n", path);
      char *pathStr = strdup(path);
      if (pathStr) {
        char *ptr = strchr(pathStr, '/');
        while (ptr) {
          *ptr = 0;
          LittleFS.mkdir(pathStr);
          *ptr = '/';
          ptr = strchr(ptr + 1, '/');
        }
      }
      free(pathStr);
    }
  }

  Serial.printf("Writing file to: %s\r\n", path);
  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

// See:  https://github.com/esp8266/Arduino/blob/master/libraries/LittleFS/src/LittleFS.h#L149
void LITTLEFS::deleteFile2(const char * path) {
  Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

  if (LittleFS.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }

  char *pathStr = strdup(path);
  if (pathStr) {
    char *ptr = strrchr(pathStr, '/');
    if (ptr) {
      Serial.printf("Removing all empty folders on path: %s\r\n", path);
    }
    while (ptr) {
      *ptr = 0;
      LittleFS.rmdir(pathStr);
      ptr = strrchr(pathStr, '/');
    }
    free(pathStr);
  }
}

void LITTLEFS::testFileIO(const char * path) {
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  size_t len = 0;
  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }

  size_t i;
  Serial.print("- writing" );
  uint32_t start = millis();
  for (i = 0; i < 2048; i++) {
    if ((i & 0x001F) == 0x001F) {
      Serial.print(".");
    }
    file.write(buf, 512);
  }
  Serial.println("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
  file.close();

  file = LittleFS.open(path);
  start = millis();
  end = start;
  i = 0;
  if (file && !file.isDirectory()) {
    len = file.size();
    size_t flen = len;
    start = millis();
    Serial.print("- reading" );
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      if ((i++ & 0x001F) == 0x001F) {
        Serial.print(".");
      }
      len -= toRead;
    }
    Serial.println("");
    end = millis() - start;
    Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
    file.close();
  } else {
    Serial.println("- failed to open file for reading");
  }
}
