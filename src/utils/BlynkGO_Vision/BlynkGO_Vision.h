#ifndef __BLYNKGO_VISION_H__
#define __BLYNKGO_VISION_H__

#include "../../config/blynkgo_config.h"

#if defined(BLYNKGO_BOARD_V1_1)

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <vector>
#include <SD.h>
#include <SPIFFS.h>

#define BLYNKGO_VISION_ERR_NO_KEY                 -1
#define BLYNKGO_VISION_ERR_FILE_CANT_OPEN         -2
#define BLYNKGO_VISION_ERR_ALLOC                  -3
#define BLYNKGO_VISION_ERR_CONNECTION             -4
#define BLYNKGO_VISION_ERR_REQ_TIMEOUT            -5
#define BLYNKGO_VISION_ERR_RES_NOT_HTTP_OK        -6
#define BLYNKGO_VISION_ERR_NOT_BLYNKGO_BOARD      -5559

enum {
  TEXT_DETECTION,
  OCR_TEXT_DETECTION,
  LOGO_DETECTION,
  LANDMARK_DETECTION,
};
typedef uint8_t detect_type_t;


typedef struct _bounding {
  uint16_t x;
  uint16_t y;
} bounding_t;

typedef struct _vision_text_detection {
  char description[512];    // ข้อความที่ detect ได้
  bounding_t *bounding;     // ตำแหน่งพิกัดกรอบ (ปกติ) 4 จุด
  size_t  bounding_num;     // จำนวนพิกัดกรอบ (ปกติ 4 จุดล้อมรอบข้อความ)
} text_annotation_t;

typedef struct _vision_logo_detection {
  char  description[512];    // ข้อความที่ detect ได้
  double score;
  bounding_t *bounding;     // ตำแหน่งพิกัดกรอบ (ปกติ) 4 จุด
  size_t  bounding_num;     // จำนวนพิกัดกรอบ (ปกติ 4 จุดล้อมรอบข้อความ)
} logo_annotation_t;

typedef struct _gps_t {
  double lat;
  double lon;
} gps_t;

typedef struct _vision_landmark_detection {
  char  description[512];    // ข้อความที่ detect ได้
  double score;
  bounding_t *bounding;     // ตำแหน่งพิกัดกรอบ (ปกติ) 4 จุด
  size_t  bounding_num;     // จำนวนพิกัดกรอบ (ปกติ 4 จุดล้อมรอบข้อความ)
  gps_t   gps;
} landmark_annotation_t;

typedef std::vector<text_annotation_t>      TextDetection_t;
typedef std::vector<logo_annotation_t>      LogoDetection_t;
typedef std::vector<landmark_annotation_t>  LandmarkDetection_t;

typedef union{
  LogoDetection_t *logo;
  TextDetection_t *text;
  LandmarkDetection_t *landmark;
} vision_detect_t;



typedef struct _vision_data_t {
  detect_type_t   type;
  vision_detect_t detect;
} vision_data_t;



class BlynkGO_Vision {
  public:
    BlynkGO_Vision();
    
    inline void setKey(String key)             { _vision_key = key; }
    inline void setTimeout(uint32_t timeout)   { _request_timeout = timeout; }

    int16_t text_detection(String pic_url, vision_data_t &data);
    int16_t text_detection(fs::FS &fs, String pic_path, vision_data_t &data);
    int16_t text_detection(uint8_t* img_data, size_t img_len, vision_data_t &data);
    
    int16_t logo_detection(String pic_url, vision_data_t &data);
    int16_t logo_detection(fs::FS &fs, String pic_path, vision_data_t &data);
    int16_t logo_detection(uint8_t* img_data, size_t img_len, vision_data_t &data);

    int16_t landmark_detection(fs::FS &fs, String pic_path, vision_data_t &data);
    int16_t landmark_detection(uint8_t* img_data, size_t img_len, vision_data_t &data);

    int16_t ocr_detection(fs::FS &fs, String pic_path, vision_data_t &data);
    int16_t ocr_detection(uint8_t* img_data, size_t img_len, vision_data_t &data);
    
    
    inline int httpCode()                      { return _http_code; }
  private:
    int16_t   _detection( detect_type_t detect_type, uint8_t* img_data, size_t img_len, vision_data_t &data);
    int16_t   _detection( detect_type_t detect_type, String img_url, vision_data_t &data);

    String    _vision_key = "";
    uint32_t  _request_timeout = 5000;
    int       _http_code;
};

extern BlynkGO_Vision VISION;

#endif // BLYNKGO_BOARD_V1_1

#endif //__BLYNKGO_VISION_H__
