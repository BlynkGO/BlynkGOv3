/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ BlynkGOv3.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
 *   สร้างโดย BlynkGO
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
 * [History]
 *   [V3.0.0 beta 1] @13/08/22
 *     - GWidget หลักๆ ทำงานได้
 *     - WiFi ทำงานได้
 *     - รองรับ Arduino ESP32 core 2.0.4 ล่าสุด
 *
 *   [V3.0.0 beta 2] @16/08/22
 *     - เพิ่ม BlynkGO Board v2.0P (ESP32S2 TFT3.5" capacitive)  (ยังไม่ทำงาน)
 * 
 *   [V3.0.0 beta 3] @22/10/22
 *     - เพิ่ม D32Pro + OLED
 *     - BeeNeXT 2.8R, 3.5R 
 *     - TFT Screen Capture  
 *     - OLED Screen Capture ( still not impliment )
 *
 *   [V3.0.0 beta 4] @28/02/23
 *     - เพิ่ม BlynkGO.autoNTP(true/false);
 *     - BlynkGO Enterprise 5" แบบ old และ jst
 *
 *   [V3.0.0 beta 5]
 *     - ปรับ แยก driver จอ/สัมผัส ออกมาเป็น static precompiled เพื่อทำให้ compile เร็วขึ้น
 *
 *   [V3.0.0 beta 6] @25/03/23 -29/03/23
 *     - เพิ่ม GCanvas และมีเพิ่ม GCanvas สามารถ drawQRCode(..) ได้ดว้ย
 *     - GCanvas มี API คำสั่ง getThermalPrinterBuffer(..) สำหรับสร้าง buffer ออก thermal printer ได้
 *     - เพิ่ม agfx core ด้วย
 *     - GObject เพิ่ม child_num_recursive() และ child_del_recursive() และ ระบบ widget_del() 
 *       ยังไม่ได้มีการปรับ del() ของแต่ละวิตเจ็ต
 *
 *   [V3.0.0 beta 7] @04/04/23-06/04/23
 *     - RTC ทำงานร่วมกับ Touch I2C ได้ (Wire1 ร่วม) หรือจะแยก ด้วย Wire ได้
 *     - fix img_decoder และ เพิ่ม jpg_data_to_img_dsc(..) สำหรับแปลง jpg data -> img_t
 *
 *   [V3.0.0 beta 8] @07/04/23
 *     - เพิ่ม BeeNeXT1.9" IPS (320x170)  SDK 3.0.8
 *
 *   [V3.0.0 beta 9] @07/04/23, 06/05/23
 *     - fix BeeNeXT3.5C SDK 3.0.8
 *     - fix BeeNeXT7.0IPS ให้ทำงานเฉพาะจออย่างเดียวไปก่อน สัมผัสยังไม่ได้ fix
 *     - fix backlight ของ BlynkGO Emma-II ให้ on off ได้ถูกต้อง
 *
 *   [V3.0.0 beta 10]
 *     - support BeeNeXT3.2C HMI Uart IPS  
 *     - support BeeNeXT7.0 HMI uart ให้สามารถทำงานสัมผัสไปได้ด้วย
 *     - เพิ่มคำสั่ง BlynkGO.centerpoint() จุดศูนย์กลางจอ
 * 
 *   [V3.0.0 beta 11] @ 19/07/23
 *     - support BeeNeXT4.3C , BeeNeXT4.3IPS
 *     - เพิ่มคำสั่ง BlynkGO.setTimestamp(time_t t) สำหรับกำหนด timestamp ปัจจุบันให้ BlynkGO
 *     - @22/07/23 เพิ่ม GTimer ให้ใช้คำสั่ง delay(...) แบบ ส่ง parameter เข้าไปใน timer ได้ด้วย
 *
 *********************************************************************
 */

#ifndef _BLYNKGO_V3_H__
#define _BLYNKGO_V3_H__

// #define BLYNKGO_VERSION             "3.0.0 beta11"

/** Major version number (X.x.x) */
#define BLYNKGO_VERSION_MAJOR   3
/** Minor version number (x.X.x) */
#define BLYNKGO_VERSION_MINOR   0
/** Patch version number (x.x.X) */
#define BLYNKGO_VERSION_PATCH   0

#define BLYNKGO_VERSION_TEXT    (String(BLYNKGO_VERSION_MAJOR)+"."+String(BLYNKGO_VERSION_MINOR)+"."+String(BLYNKGO_VERSION_PATCH))

/**
 * Macro to convert IDF version number into an integer
 *
 * To be used in comparisons, such as BLYNKGO_VERSION >= BLYNKGO_VERSION_VAL(3, 0, 0)
 */
#define BLYNKGO_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current IDF version, as an integer
 *
 * To be used in comparisons, such as BLYNKGO_VERSION >= BLYNKGO_VERSION_VAL(3, 0, 0)
 */
#define BLYNKGO_VERSION  BLYNKGO_VERSION_VAL( BLYNKGO_VERSION_MAJOR, \
                                              BLYNKGO_VERSION_MINOR, \
                                              BLYNKGO_VERSION_PATCH)

#include <Arduino.h>
#include "config/blynkgo_config.h"
#include "extra/GLVGL.h"
#include "widgets/GWidgets.h"
#include "objx/TridentTD_AQI.h"
#include "utils/TimeAlarms/TimeAlarms.h"
#include "utils/lvgl-dev-7.0/src/lv_draw/TridentUnicode.h"
#include "utils/TridentTD_ESP32NVS/TridentTD_ESP32NVS.h"  // ESP32 NVS

#if defined(BEENEXT) || BLYNKGO_USE_BEENEXT
// #include "utils/BeeNeXT/BeeNeXT.h"
#include <BeeNeXT.h>
#endif

// #if defined(BLYNKGO_USE_SOFTWARESERIAL) && (CONFIG_IDF_TARGET_ESP32S3==0)
// #include "utils/BeeNeXT/libs/SoftwareSerial/SoftwareSerial.h"
// #endif

#if BLYNKGO_USE_NTP
  #include "time.h"
#endif

#if defined (LAOS_TEXT)
FONT_DECLARE(saysettha_15);
FONT_DECLARE(saysettha_25);
#endif

#if defined (VIETNAM_TEXT)
FONT_DECLARE(arial_vietnam_20);
FONT_DECLARE(arial_vietnam_30);
#endif

#if BLYNKGO_USE_LORA
#include "utils/LoRa/LoRa.h"
#endif

#if defined(BLYNKGO_OLED)
  #if defined(BLYNKGO_OLED_SSD1306)
    #include "utils/ESP32_oled/SSD1306Wire.h"  //https://github.com/ThingPulse/esp8266-oled-ssd1306
  #elif defined(BLYNKGO_OLED_SH1106)
    #include "utils/ESP32_oled/SH1106Wire.h"
  #endif
#else
  #include "utils/BlynkGO_LCD/BlynkGO_LCD.h"
  extern BlynkGO_LCD lcd;
#endif

#if BLYNKGO_USE_SD          /* SD instance can use */
  #if defined SD_MMC_MODE
    #include "SD_MMC.h"
  #else
    #include "SD.h"
  #endif
  using namespace fs;
#endif //BLYNKGO_USE_SD

#if BLYNKGO_USE_BLYNK
  #include <WiFiClient.h>
  #include <BlynkBlynkGO.h>

  extern BlynkWifi Blynk;
  #include <BlynkWidgets.h>

  #define BLYNK_VPIN        request.pin
#endif

#if BLYNKGO_USE_GDATABASE
  #include "utils/TridentTD_SQLite32/TridentTD_SQLite32.h"
#endif

#if defined (TTGO_TWATCH)
  #include "utils/PCF8563/pcf8563.h"                // RTC PCF8563
  #include "utils/BlynkGO_BMA423/BlynkGO_BMA423.h"  // Accel. Sensor
  #include "utils/AXP202/axp20x.h"
#endif // TTGO_TWATCH

//#if defined(TTGO_TWATCH_2020_V1) || defined(TTGO_TWATCH_2020_V3)
#if BLYNKGO_USE_AUDIO
  #include "utils/ESP32-audioI2S/Audio.h"
#endif
//#endif

#if defined(TTGO_TWATCH_2020_V3)
#if BLYNKGO_USE_MIC
  #include <driver/i2s.h>
#endif
#endif

#if BLYNKGO_USE_BUTTON_ISR || defined (BLYNKGO_MINI) || defined (TTGO_TWATCH_2019) || defined (BLYNKGO_LORA32) || defined(BEENEXT_1_9) || defined (BLYNKGO_M5STACK)
  // #include "utils/Button2/Button2.h"
  #include "utils/BlynkGO_ButtonISR/BlynkGO_ButtonISR.h"
#endif

#if BLYNKGO_USE_GPS
  #include "utils/TinyGPSPlus/TinyGPS++.h"
#endif


#if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563
  #include "utils/RTC/RTClib.h"
#endif // #if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563


#if BLYNKGO_USE_WIFI
  #include "WiFi.h"
  #include "utils/BlynkGO_AsyncTCP/BlynkGO_AsyncTCP.h"
  // #include "utils/AsyncHttpClient/AsyncHttpClient.h"
#if BLYNKGO_USE_LINENOTIFY
  #include "utils/BlynkGO_AsyncLineNotify/BlynkGO_AsyncLineNotify.h"
#endif

#if BLYNKO_USE_ASYNC_MQTT32
  #include "utils/BlynkGO_AsyncMQTT32/BlynkGO_AsyncMQTT32.h"
#endif

#if defined(BLYNKGO_BOARD_V1_3) && defined(BLYNKGO_CAMERA_AI)
  #include "utils/BlynkGO_Vision/BlynkGO_Vision.h"
#endif

#if BLYNKGO_USE_NTP
  #if BLYNKGO_USE_MQTT
    #if BLYNKGO_USE_QRPAYMENT
      #include "utils/BlynkGO_GBPrimePay/BlynkGO_GBPrimePay.h"
    #endif
  #endif
#endif // BLYNKGO_USE_NTP && BLYNKGO_USE_MQTT && BLYNKGO_USE_GBPRIMEPAY



#define BlynkGO_HTTPClient      AsyncHttpClient

typedef void (*WiFiConnectedCb)(void);
typedef void (*WiFiDisconnectedCb)(void);
typedef void (*SoftAPStartCb)(void);
typedef void (*SoftAPStopCb)(void);
typedef void (*SoftAPStaConnectedCb)(void);
typedef void (*SoftAPStaDisconnectedCb)(void);
typedef void (*NTPSyncedCb)(void);

#ifndef SMARTWIFI
#define SMARTWIFI
  #define WIFI_CONNECTED()    void WiFiOnConnected()
  #define WIFI_DISCONNECTED() void WiFiOnDisconnected()
  #define SOFTAP_START()      void SoftAPOnStart()
  #define SOFTAP_STOP()       void SoftAPOnStop()
  #define SOFTAP_STA_CONNECTED()  void SoftAPOnStaConnected()
  #define SOFTAP_STA_DISCONNECTED()  void SoftAPOnStaDisconnected()
#if BLYNKGO_USE_NTP
  #define NTP_SYNCED()        void NTPOnSynced()
#endif // BLYNKGO_USE_NTP
#endif // SMARTWIFI

  #ifdef __cplusplus
  extern "C" {
  #endif
    void WiFiNoOpCbk();
    WIFI_CONNECTED();
    WIFI_DISCONNECTED();
    SOFTAP_START();
    SOFTAP_STOP();
    SOFTAP_STA_CONNECTED();
    SOFTAP_STA_DISCONNECTED();
#if BLYNKGO_USE_NTP
    NTP_SYNCED();
#endif // BLYNKGO_USE_NTP
  #ifdef __cplusplus
  }
  #endif
#endif  //BLYNKGO_USE_WIFI

#define BEENEXT_DISABLE     false

typedef void(*fn_last_touch_cb_t)(void);

class BlynkGOv3 {
  public:
    BlynkGOv3();

    /*****************************************************************
     * API begin(..) สำหรับ เริ่มต้น BlynkGO
     *   parameter:
     *     [String] blynkgo_key : รหัส BLYNKGO_KEY ที่ลงทะเบียน
     *****************************************************************/
    void     begin(uint64_t blynkgo_key=0);
    void     begin(String blynkgo_key);
    /*****************************************************************
     * API update() สำหรับให้ BlynkGO ทำงาน ให้มีเสมอโดยวางไว้ใน loop()
     *****************************************************************/
    void     update();

    /*****************************************************************
     * API rotation(..) และ rotation() สำหรับกำหนด/ คืนค่าการหมุนทิศทางหน้าจอ
     *   parameter:
     *     [uint8_t] r   : ค่า 0-3 สำหรับใช้หมุนทิศทางหน้าจอ
     *****************************************************************/
#if !defined(BLYNKGO_OLED)
    void     rotation(uint8_t r); // r : 0..3
    uint8_t  rotation();
#endif

    /*****************************************************************
     * API hasSD()  สำหรับเช็คว่ามี SD พร้อมใช้งานอยู่หรือไม่ (ใส่ตั้งแต่แรก)
     *****************************************************************/
    inline bool hasSD()               { return _has_sd;  }

#if defined (TOUCH_XPT2046)
    /****************************************************************
     * API touch_calibrate() สำหรับ calibrate ระบบสัมผัสหน้าจอ แบบ resistive
     ****************************************************************/
    void     touch_calibrate();
#endif

    fn_last_touch_cb_t  _fn_last_touch = NULL;
    inline void last_touch_cb(fn_last_touch_cb_t fn)     { _fn_last_touch = fn; }

#if defined(BLYNKGO_OLED)
    /*****************************************************************
     * API oled_threshold()     สำหรับกำหนด/เรียกดู ค่า threadhold สำหรับ จอ OLED
     *                          หากเกิน ให้แสดงเป็น สีขาว หากต่ำกว่าแสดงเม็ดสีเป็นดำ
     *                          default : 128  (ยิ่งค่าน้อย ขาวจะกว้างมากขึ้น)
     *****************************************************************/
    void    oled_threshold(uint8_t threshold);
    uint8_t oled_threshold();
#endif

    /*****************************************************************
     * API fillScreen(..) สำหรับกำหนดสีพื้นหลังจอ 
     *   parameter:
     *     [color_t] color : ค่าสีที่ 1
     *     [color_t] gradian_color : ค่าสีที่ 2 (option) หากต้องการทำเฉดสีพื้นหลัง
     *****************************************************************/
    __attribute__ ((always_inline)) inline void     fillScreen(color_t color)                           { GScreen.color(color); }
    __attribute__ ((always_inline)) inline void     fillScreen(color_t color, color_t gradian_color)    { GScreen.color(color, gradian_color);}

    /*****************************************************************
     * API width() และ height() สำหรับ คืนค่าขนาดหน้าจอ กว้าง ยาว 
     *****************************************************************/
    __attribute__ ((always_inline)) inline uint16_t width()                                             { return GScreen.width();  }
    __attribute__ ((always_inline)) inline uint16_t height()                                            { return GScreen.height(); }

    __attribute__ ((always_inline)) inline Point    centerpoint()                                       { return GScreen.centerpoint(); }
 
    /*****************************************************************
     * API  font_load(String font_path) สำหรับโหลด binary font จาก SD หรือ SPIFFS
     *   parameter:
     *     [String] font_path : ตำแหน่ง font  ตัวอย่าง "SD://font.bin" หรือ "SPIFFS://font.bin"
     *   return:
     *      คืนค่า เป็น font_t pointer ของ font ที่โหลด
     *
     * API  font_free(font_t font)  สำหรับคืน mem ของ font pointer ที่โหลดจาก binary font
     *   parameter:
     *     [font_t* ]  font : font pointer ของฟอนต์ที่โหลดจาก binary font
     *
     *****************************************************************/
    font_t* font_load(String font_path);
    void    font_free(font_t* font);

    /*****************************************************************
     * API brightness(..) และ brightness() สำหรับปรับระดับความสว่างหน้าจอ/ คืนค่าระดับความสว่างหน้าจอ
     *   parameter:
     *     [uint8_t] level : ค่าระดับความสว่าง 0-255(สว่างสุด)
     *     [bool   ] save  : ให้มีการบันทึกค่าของ flash memory ด้วยหรือไม่
     *****************************************************************/
    void     brightness(uint8_t level, bool save=true);  // level : 0..255
    uint8_t  brightness();                               // ถ้า TFT_BL==-1 จะคืนค่า 255 ตลอด

    /*****************************************************************
     * API backlight_on()  สำหรับเปิดไฟ led หน้าจอ กลับคืนมา
     * API backlight_off() สำหรับปิดไฟ led หน้าจอ
     *****************************************************************/
#if !defined(BLYNKGO_OLED)
    // __attribute__ ((always_inline)) inline void wakeup()                          { lcd.setBrightness( this->flashMem_Int("BRIGHTNESS") ); _is_backlight_on = true;  }
    // __attribute__ ((always_inline)) inline void sleep()                           { lcd.setBrightness(0);                                  _is_backlight_on = false; }
    __attribute__ ((always_inline)) inline void wakeup()                          { lcd.wakeup(); _is_backlight_on = true;}
    __attribute__ ((always_inline)) inline void sleep()                           { lcd.sleep();  _is_backlight_on = false;}
    __attribute__ ((always_inline)) inline void backlight_ON()                    { this->wakeup(); }
    __attribute__ ((always_inline)) inline void backlight_OFF()                   { this->sleep();  }

    __attribute__ ((always_inline)) inline bool backlight_isON()                  { return _is_backlight_on;  }    // ถ้า TFT_BL==-1 จะคืนค่า true ตลอด
    __attribute__ ((always_inline)) inline bool backlight_isOFF()                 { return !_is_backlight_on; }
    __attribute__ ((always_inline)) inline bool isSleep()                         { return !_is_backlight_on; }
#endif

#if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_2) || defined(BEENEXT_3_2C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)
    void     ledRGB(uint8_t r, uint8_t g, uint8_t b);
    float    readLDR();  // 0-100;
#endif

#if BLYNKGO_USE_SNAPSHOT
    /*****************************************************************
     * API capture(..) สำหรับจับภาพหน้าจอบันทึกลง SD หรือ SPIFFS
     *   parameter:
     *     [const char*] filename  : ชื่อไฟล์สำหรับเซฟภาพหน้าจอ
     *         หากไม่ใส่ระบบจะ กำหนดค่าให้อัตโนมัติ
     *         การใส่ชื่อใส่ได้ 2 แบบ คือ   "SD://ตำแหน่งfolder/ชื่อไฟล์.png"
     *                          หรือ  "SPIFFS://ชื่อไฟล์.png"
     *   return:
     *      คืนค่า เป็น boolean >>> true : สำเร็จ ; false : ไม่สำเร็จ
     *****************************************************************/
    bool     capture(const char* filename=NULL);
    inline  bool capture(String filename)       { return capture(filename.c_str()); }
#endif


    /*****************************************************************
     * API flashMem(..) สำหรับ เก็บข้อมูลแบบ key->value 
     *                  โดยจัดเก็บบน flash memory ของ ESP32
     *   parameter:
     *     [String] key   : ชื่อ key ที่ใช้จัดเก็บบน flash memory
     *     [  ... ] value : ค่า ที่ต้องการจัดเก็บไว้บน flash memory
     *****************************************************************/
    __attribute__ ((always_inline)) inline bool flashMem(String key, uint8_t value)                       { return NVS.setInt(key,(uint8_t)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, int16_t value)                       { return NVS.setInt(key,(int16_t)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, uint16_t value)                      { return NVS.setInt(key,(uint16_t)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, int32_t value)                       { return NVS.setInt(key,(int32_t)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, uint32_t value)                      { return NVS.setInt(key,(uint32_t)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, int64_t value)                       { return NVS.setInt(key,(int64_t)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, uint64_t value)                      { return NVS.setInt(key,(uint64_t)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, float value)                         { return NVS.setFloat(key,(float)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, String value)                        { return NVS.setString(key,(String)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, const char* value)                   { return NVS.setCharArray(key,(const char*)value); }
    __attribute__ ((always_inline)) inline bool flashMem(String key, void* object, size_t length)         { return NVS.setObject(key, (void*) object, length); }

    /*****************************************************************
     * API flashMem_Int(..) , flashMem_Float(..), flashMem_String(..), 
     *     flashMem_CharArray(..), flashMem_Object(..)
     *   คำสั่ง สำหรับ ดึง value ของ key ที่จัดเก็บบน flash memory ของ ESP32
     *
     *   parameter:
     *     [String] key   : ชื่อ key ที่ใช้จัดเก็บบน flash memory
     *   return:
     *     คืนค่าเป็น value ตามประเภท API นั้นๆ
     *****************************************************************/
    __attribute__ ((always_inline)) inline int64_t flashMem_Int(String key)                               { return NVS.getInt(key);    }
    __attribute__ ((always_inline)) inline float   flashMem_Float(String key)                             { return NVS.getFloat(key);  }
    __attribute__ ((always_inline)) inline String  flashMem_String(String key)                            { return NVS.getString(key); }
    __attribute__ ((always_inline)) inline char*   flashMem_CharArray(String key, char* rev_data)         { return NVS.getCharArray(key, (char*) rev_data ); }
    __attribute__ ((always_inline)) inline void*   flashMem_Object(String key, void* blob)                { return NVS.getObject(key, (void*) blob );        }  // แบบมี alloc อยู่ภายใน
    __attribute__ ((always_inline)) inline bool    flashMem_Object(String key, void* blob, size_t length) { return NVS.getObject(key, (void*) blob, length ); }  // แบบสร้าง variable ภายนอกแล้วส่ง address มารับ
    __attribute__ ((always_inline)) inline bool    flashMem_Array(String key, void* blob, size_t length)  { return NVS.getObject(key, (void*) blob, length ); } 

    /*****************************************************************
     * API flashMem_exists(..) สำหรับเช็คว่า key ที่ต้องการมีอยู่บน flash memory ของ ESP32 หรือไม่
     *   parameter:
     *     [String] key   : ชื่อ key ที่ใช้จัดเก็บบน flash memory
     *   return:
     *     คืนค่าเป็น boolean >>> true : มี key นี้อยู่ ; false : ไม่มี key นี้อยู่
     *****************************************************************/
    __attribute__ ((always_inline)) inline bool    flashMem_exists(String key)                            { return NVS.exists(key); }

    /*****************************************************************
     * API flashMem_erase(..) สำหรับลบ key ที่ต้องการมีอยู่บน flash memory ของ ESP32 ออก
     *   parameter:
     *     [String] key   : ชื่อ key ที่ใช้จัดเก็บบน flash memory
     *   return:
     *     คืนค่าเป็น boolean >>> true : ลบสำเร็จ ; false : ไม่สำเร็จ
     *****************************************************************/
    __attribute__ ((always_inline)) inline bool    flashMem_erase(String key)                             { return NVS.erase(key); }
    bool  flashMem_eraseAll();

    /*****************************************************************
     * API setTimestamp(...) สำหรับกำหนด timestamp ปัจจุบันให้ ระบบ BlynkGO
     *****************************************************************/
    void setTimestamp(time_t timestamp);

    
#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK
    /*****************************************************************
     * API onWiFiConnected(..)      สำหรับ กำหนด callback function เมื่อ WiFi เชื่อมต่อสำเร็จ
     * API onWiFiDisconnected(..)   สำหรับ กำหนด callback function เมื่อ WiFi ถูกตัดการเชื่อมต่อเป็นที่เรียบร้อย
     * API onSoftAPStart(..)        สำหรับ กำหนด callback function เมื่อ WiFi SoftAP เริ่มทำงาน
     * API onSoftAPStop(..)         สำหรับ กำหนด callback function เมื่อ WiFi SoftAP ปิดทำงาน
     * API onWiFiEvent(..)          สำหรับ กำหนด WiFi Event function จัดการเอง
     *****************************************************************/
    void     onWiFiConnected    (WiFiConnectedCb cbConnected);
    void     onWiFiDisconnected (WiFiDisconnectedCb cbDisconnected);
    void     onSoftAPStart      (SoftAPStartCb cbSoftAPStart);
    void     onSoftAPStop       (SoftAPStopCb  cbSoftAPStop);
    void     onSoftAPStaConnected(SoftAPStaConnectedCb cbSoftAPStaConnected);
    void     onSoftAPStaDisconnected(SoftAPStaDisconnectedCb cbSoftAPStaDisconnected);
    void     onWiFiEvent        (WiFiEventCb cbEvent);

#if BLYNKGO_USE_NTP
    /*****************************************************************
     * API onNTPSynced(..)          สำหรับ กำหนด callback function เมื่อ NTP Synced เวลาสำเร็จ
     *****************************************************************/
    void     onNTPSynced        (NTPSyncedCb cbNtpSynced);

    /*****************************************************************
     * API getLocalTime(..)         สำหรับ ดึงเวลาของระบบมาที่ตัวแปร struct tm คืนค่าหาก ntp syned เวลาสำเร็จ
     *****************************************************************/
    bool     getLocalTime(struct tm &timeinfo);

    /*****************************************************************
     * API isNTPSynced(..)          สำหรับเช็คว่า ntp synced เวลาสำเร็จหรือยัง
     *****************************************************************/
    bool     isNTPSynced();

    /*****************************************************************
     * API alarmRepeat(..)          สำหรับตั้งเวลาปลุกให้ทำงานตามเวลาที่ต้องการ ทุกๆวัน
     *****************************************************************/
    void     alarmRepeat(const int H, const int M, const int S, OnTick_t onTickHandler);
    void     alarmRepeat(time_t secs_of_day, OnTick_t onTickHandler);

    /*****************************************************************
     * API alarmRepeat(..)          สำหรับตั้งเวลาปลุกให้ทำงานตาม วันในสัปดาห์และเวลาที่ต้องการ ทุกๆสัปดาห์
     *    parameter:
     *        alarm_dow_t :  ALARM_SUNDAY, ALARM_MONDAY, ALARM_TUESDAY, ALARM_WEDNESDAY, ALARM_THURSDAY, ALARM_FRIDAY, ALARM_SATURDAY
     *****************************************************************/
    void     alarmRepeat(const alarm_dow_t DOW, const int H, const int M, const int S, OnTick_t onTickHandler);

    /*****************************************************************
     * API alarmOnce(..)           สำหรับตั้งเวลาปลุกให้ทำงานตามเวลาที่ต้องการ เฉพาะครั้งเดียว
     *****************************************************************/
    void     alarmOnce(const int H, const int M, const int S, OnTick_t onTickHandler);
    void     alarmOnce(time_t secs_of_day, OnTick_t onTickHandler, bool absolute_timestamp=false);

    /*****************************************************************
     * API alarmRepeat(..)        สำหรับตั้งเวลาปลุกให้ทำงานตาม วันในสัปดาห์และเวลาที่ต้องการ เฉพาะครั้งเดียว
     *    parameter:
     *        alarm_dow_t :  ALARM_SUNDAY, ALARM_MONDAY, ALARM_TUESDAY, ALARM_WEDNESDAY, ALARM_THURSDAY, ALARM_FRIDAY, ALARM_SATURDAY
     *****************************************************************/
    void     alarmOnce(const alarm_dow_t DOW, const int H, const int M, const int S, OnTick_t onTickHandler);

    /*****************************************************************
     * API timezone(..)          สำหรับกำหนด timezone สำหรับ sync เวลา NTP
     *****************************************************************/
    void     timezone(float tz);  // ei 7
    float    timezone();

    /*****************************************************************
     * API daylight_saving(..)   สำหรับกำหนด daylight saving สำหรับ sync เวลา NTP
     *****************************************************************/
    void     daylight_saving(float daylight);
    float    daylight_saving();

    /*****************************************************************
     * API syncNTP()            สำหรับสั่งให้ทำการ sync เวลา NTP (โดย WiFi ได้เชื่อมอยู่)
     *****************************************************************/
    void     syncNTP();

    inline void     autoNTP(bool mode)        { _auto_ntp_synced = mode; }
    inline bool     autoNTP()                 { return _auto_ntp_synced; }



#endif //BLYNKGO_USE_NTP
#endif //#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK


#if BLYNKGO_USE_BLYNK
    void begin(String blynkgo_key, String blynk_auth, String ssid, String pass, 
                    String domain = BLYNK_DEFAULT_DOMAIN,
                    uint16_t port = BLYNK_DEFAULT_PORT);

    void begin(String blynk_auth, String ssid, String pass, 
                    String domain = BLYNK_DEFAULT_DOMAIN,
                    uint16_t port = BLYNK_DEFAULT_PORT);

    void configBlynk(String blynk_auth, String domain = BLYNK_DEFAULT_DOMAIN, uint16_t port = BLYNK_DEFAULT_PORT);
    void enableBlynk(bool enable);

    template <typename... Args>
    inline void virtualWrite(int pin, Args... values) { Blynk.virtualWrite(pin, values...); }
    template <typename... Args>
    inline void syncVirtual(Args... pins)             { Blynk.syncVirtual(pins...); }
    inline void syncAll()                             { Blynk.syncAll(); }
#endif


#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK
    WiFiEventCb         wifi_event_cb         = NULL;
    WiFiConnectedCb     wifi_connected_cb     = NULL;
    WiFiDisconnectedCb  wifi_disconnected_cb  = NULL;
    SoftAPStartCb       softab_start_cb       = NULL;
    SoftAPStopCb        softab_stop_cb        = NULL;
    SoftAPStaConnectedCb softab_sta_connected_cb = NULL;
    SoftAPStaDisconnectedCb softab_sta_disconnected_cb = NULL;
#if BLYNKGO_USE_NTP
    NTPSyncedCb         ntp_synced_cb         = NULL;
#endif // if BLYNKGO_USE_NTP
#endif //#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK

#if BLYNKGO_USE_RTC_DS323X
    RTC_DS3231  RTC;
#elif BLYNKGO_USE_RTC_DS1307
    RTC_DS1307  RTC;
#elif BLYNKGO_USE_RTC_PCF8523
    RTC_PCF8523 RTC;
#elif BLYNKGO_USE_RTC_PCF8563
    RTC_PCF8563 RTC;
#endif //BLYNKGO_USE_RTC_PCF8563

    void   regist(String blynkgo_key);
    void   regist(uint64_t blynkgo_key);
    inline String version()       { return _version; }

    static void blynkgo_alarm_handler();
    void hw_lcd_init();
#if BLYNKGO_USE_SD
    void hw_sd_init();
#endif //BLYNKGO_USE_SD

  private:
    String _version = BLYNKGO_VERSION_TEXT;
    uint8_t _rotation;
    bool _is_backlight_on = true;
    bool _has_sd = false;
    bool _auto_ntp_synced = true;
    // bool _is_blynkgo_system_inited = false;

    void blynkgo_system_init();
    void blynkgo_logo();
};

extern BlynkGOv3 BlynkGO;

uint32_t last_touch();

#if defined(BLYNKGO_MINI)
  extern ButtonISR BTN1;
  extern ButtonISR BTN2;
#elif defined(BLYNKGO_M5STACK)
  extern ButtonISR BTN_A;
  extern ButtonISR BTN_B;
  extern ButtonISR BTN_C;
#elif defined(BLYNKGO_LORA32) || defined(BEENEXT_1_9)  || BLYNKGO_USE_BUTTON_ISR
  extern ButtonISR BTN;
#endif

#endif //_BLYNKGO_V3_H__
