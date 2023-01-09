/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ BlynkGO_AsyncLineNotify.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
 *   สร้างโดย TridentTD
 *   
 *   2.ไม่อนุญาต ให้แจกจ่าย สำเนา หรือแก้ไข โดยไม่ได้รับอนุญาต 
 *   
 *   3.สำหรับผู้ได้รับ ลิขสิทธิ์ สามารถนำไปใช้สร้าง firmware/rom 
 *   สำหรับ บอร์ด ESP32 ที่ระบุเท่านั้น เพื่อใช้ทางการค้าต่างๆได้
 *   โดยห้ามแจกจ่าย จำหน่าย ดัดแปลง แก้ไขใดๆ ในตัว source ไลบรารี่ออกไป
 *
 *   4.หากมีการนำไปใช้คอมไพล์ถือว่าได้ยอมรับ ตามเงื่อนไขลิขสิทธิ์ดังกล่าว
 *   เป็นที่เรียบร้อยแล้ว
 * 
 *********************************************************************
 */

#ifndef __BLYNKGO_ASYNC_LINENOTIFY_H__
#define __BLYNKGO_ASYNC_LINENOTIFY_H__

#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <SD.h>
#include <SPIFFS.h>
#include "../BlynkGO_AsyncTCP/BlynkGO_AsyncTCP.h"

class BlynkGO_AsyncLineNotify {
  public:
    BlynkGO_AsyncLineNotify();

    // กำหนด Default Line Token
    inline void  setToken(String token)           { _token = token; }
    inline void  setToken(const char* token)      { _token = token; }

    // LINE notify ด้วย ข้อความ
    void  notify(const char* message);
    void  notify(String message);
    void  notify(String token, String message);
    
    // LINE notify ด้วย ตัวเลข 
    void  notify(float value, uint8_t decimal = 2);
    void  notify(int value);
    void  notify(String token, float value, uint8_t decimal = 2);
    void  notify(String token, int value);
    

    // LINE notify ด้วย Sticker จาก https://devdocs.line.me/files/sticker_list.pdf
    void  notifySticker(String message, int StickerPackageID, int StickerID);
    void  notifySticker(int StickerPackageID, int StickerID);
    void  notifySticker(String token, String message, int StickerPackageID, int StickerID);

    // ส่ง รูปขึ้น LINE ด้วย url บนรูปบน Internet
    void  notifyPicture(String message, String picture_url);
    void  notifyPicture(String picture_url);
    void  notifyPicture(String token, String message, String picture_url);

    // ส่ง รูปขึ้น LINE ด้วย file รูปภาพ (jpg/png) ที่อยู่บน SD หรือ SPIFFS ของ ESP32
    void  notifyPicture(String message, fs::FS &fs, String path);
    void  notifyPicture(fs::FS &fs, String path);
    void  notifyPicture(String token, String message, fs::FS &fs, String path);

    void  notifyPicturePath(String filepath) {
        if( filepath.startsWith("sd://") ||
            filepath.startsWith("SD://")  )
        {
          filepath.replace("sd://", "/");
          filepath.replace("SD://", "/");
          this->notifyPicture(SD, filepath);
        }else
        if( filepath.startsWith("spiffs://") ||
            filepath.startsWith("SPIFFS://")  )
        {
          filepath.replace("spiffs://", "/");
          filepath.replace("SPIFFS://", "/");
          this->notifyPicture(SPIFFS, filepath);
        }
    }

    // ส่ง รูปขึ้น LINE ด้วย image data
    void  notifyPicture(String message, uint8_t* image_data, size_t image_size);
    void  notifyPicture(uint8_t* image_data, size_t image_size);
    void  notifyPicture(String token, String message, uint8_t* image_data, size_t image_size);
    
    bool  connected();
    void  connect();
    void  close();

    void  _notifyList();
    void  _notify(String message, int StickerPackageID=0, int StickerID=0, String picture_url="", fs::FS &fs=SPIFFS , String path="", uint8_t* image_data=NULL, size_t image_size=0);
    int   _httpCode;
  private:
    float   _version = 1.00;
    String  _token;
    AsyncClientSecure _clientSecure;
};

extern BlynkGO_AsyncLineNotify LINE;

#endif //__BLYNKGO_ASYNC_LINENOTIFY_H_
