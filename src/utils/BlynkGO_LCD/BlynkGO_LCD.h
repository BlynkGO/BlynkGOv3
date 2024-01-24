/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ BlynkGO_LCD.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 *  [History]
 *   - Version 1.0.3 @16/09/66
 *     รองรับ  
 *       - BlynkGO Board v1.3
 *       - BlynkGO Emma-II (4MB/4MB - 16MB/8MB)
 *       - BlynkGO Enterprise 5",7"
 *       - BeeNeXT 1.9IPS
 *       - BeeNeXT 2.4R/C
 *       - BeeNeXT 2.8R
 *       - BeeNeXT 3.2IPS
 *       - BeeNeXT 3.5R/C
 *       - BeeNeXT 4.3R/C/IPS
 *       - BeeNeXT 5.0IPS
 *       - BeeNeXT 7.0IPS
 *   - Version 1.0.4 @29/10/66
 *       - BeeNeXT 5.0IPS ลบ noise
 *   - Version 1.0.5 @04/11/66
 *       - BeeNeXT 4.3IPS แก้ touch ให้ตรง
 *   - Version 1.0.6 @24/01/67
 *       - เพิ่ม BeeWave-S3 RGB8048 (800x480 Capacitive)
 * 
 *********************************************************************/

#ifndef __BLYNKGO_LCD_H__
#define __BLYNKGO_LCD_H__

/** Major version number (X.x.x) */
#define BLYNKGO_LCD_VERSION_MAJOR   1
/** Minor version number (x.X.x) */
#define BLYNKGO_LCD_VERSION_MINOR   0
/** Patch version number (x.x.X) */
#define BLYNKGO_LCD_VERSION_PATCH   6

#define BLYNKGO_LCD_VERSION_TEXT    (String("BlynkGO_LCD v.")+String(BLYNKGO_LCD_VERSION_MAJOR)+"."+String(BLYNKGO_LCD_VERSION_MINOR)+"."+String(BLYNKGO_LCD_VERSION_PATCH))

/**
 * Macro to convert IDF version number into an integer
 *
 * To be used in comparisons, such as BLYNKGO_LCD_VERSION >= BLYNKGO_LCD_VERSION_VAL(3, 0, 0)
 */
#define BLYNKGO_LCD_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current IDF version, as an integer
 *
 * To be used in comparisons, such as BLYNKGO_LCD_VERSION >= BLYNKGO_LCD_VERSION_VAL(3, 0, 0)
 */
#define BLYNKGO_LCD_VERSION  BLYNKGO_LCD_VERSION_VAL( BLYNKGO_LCD_VERSION_MAJOR, \
                                                      BLYNKGO_LCD_VERSION_MINOR, \
                                                      BLYNKGO_LCD_VERSION_PATCH)


#include <Arduino.h>
#include <sdkconfig.h>
// #include "config/blynkgo_config.h"
#include "../../config/blynkgo_config.h"

#define LGFX_USE_V1
#include "./LoveCat/lgfx/v1/platforms/esp32/Light_PWM.hpp"

#if !defined(BLYNKGO_OLED)

#if defined (TFT_4_3_INCH_TOUCH_485)
#include "."
class BlynkGO_LCD  {
  public:
    BlynkGO_LCD() { }
  private:
};

#else

#if defined(BLYNKGO_USE_AGFX) && (BLYNKGO_USE_AGFX==1)  // ใช้ AGFX 
/******************************************************
 * AGFX
 *****************************************************/
#include "./agfx/agfx.h"

static constexpr int _TFT_BLACK       = 0x0000;      /*   0,   0,   0 */
static constexpr int _TFT_WHITE       = 0xFFFF;      /* 255, 255, 255 */

#if defined(BEENEXT_1_9)
class BlynkGO_LCD : public Arduino_ST7789 {
  public:
    BlynkGO_LCD(): Arduino_ST7789(new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO), 
        TFT_RST, OFFSET_ROTATION, true/* IPS */, TFT_HEIGHT, TFT_WIDTH, 
        35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */) {}
    void begin(){
      Arduino_ST7789::begin();
      auto cfg = _light_instance.config();
        cfg.pin_bl      = TFT_BL;
        cfg.invert      = false;
        cfg.freq        = 12000; //44100; 
        cfg.pwm_channel = BACKLIGHT_CHANNEL;
      _light_instance.config(cfg);
      _light_instance.init(255);
    }

    inline void init()      { begin(); }
    inline void drawString(const char* str, int16_t x, int16_t y)   { this->setCursor(x, y); this->print(str); }
    inline void wakeup()                            { _light_instance.setBrightness(_last_brightness);}
    inline void sleep()                             { _light_instance.setBrightness(0); }
    inline void setBrightness(uint8_t brightness)   { _light_instance.setBrightness(brightness); _last_brightness = brightness; }
    inline uint8_t getBrightness()                  { return _last_brightness; }
  private:
    lgfx::Light_PWM     _light_instance;
    uint8_t             _last_brightness=255;
};

#elif defined(BEENEXT_4_3C) ||  defined(BEENEXT_4_3IPS) ||  defined(BEENEXT_5_0IPS) ||  defined(BEENEXT_7_0IPS) ||  defined(BEEWAVE_S3_RGB8048)

#if defined(TOUCH_GT911_TAMC) || defined(TOUCH_GT911)
#include "./LoveCat/lgfx/v1/touch/TAMC_GT911/TAMC_GT911.h"
#endif

class BlynkGO_LCD : public Arduino_RPi_DPI_RGBPanel {
  public:
// #if defined(BEENEXT_5_0IPS) || defined(BEENEXT_4_3C) ||  defined(BEENEXT_7_0IPS)
    BlynkGO_LCD() : Arduino_RPi_DPI_RGBPanel( new Arduino_ESP32RGBPanel(
        GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
        TFT_HENABLE /* DE */, TFT_VSYNC /* VSYNC */, TFT_HSYNC /* HSYNC */, TFT_PCLK /* PCLK */,
        TFT_R0 /* R0 */, TFT_R1 /* R1 */, TFT_R2 /* R2 */, TFT_R3 /* R3 */, TFT_R4 /* R4 */,
        TFT_G0 /* G0 */, TFT_G1 /* G1 */, TFT_G2 /* G2 */, TFT_G3 /* G3 */, TFT_G4 /* G4 */, TFT_G5 /* G5 */,
        TFT_B0 /* B0 */, TFT_B1 /* B1 */, TFT_B2 /* B2 */, TFT_B3 /* B3 */, TFT_B4 /* B4 */ ),
      TFT_WIDTH  /* width */ ,  TFT_HSYNC_POLARITY /* hsync_polarity */, TFT_HSYNC_FRONT_PORCH /* hsync_front_porch */, TFT_HSYNC_PULSE_WIDTH /* hsync_pulse_width */, TFT_HSYNC_BACK_PORCH /* hsync_back_porch */,
      TFT_HEIGHT /* height */,  TFT_VSYNC_POLARITY /* vsync_polarity */, TFT_VSYNC_FRONT_PORCH /* vsync_front_porch */, TFT_VSYNC_PULSE_WIDTH /* vsync_pulse_width */, TFT_VSYNC_BACK_PORCH /* vsync_back_porch */,
      TFT_PCLK_IDLE_HIGH /* pclk_active_neg */, TFT_PREFER_SPEED /* prefer_speed */, true /* auto_flush */)  {}

    void begin(){
      Arduino_RPi_DPI_RGBPanel::begin();
      
      auto cfg = _light_instance.config();
        cfg.pin_bl      = TFT_BL;
        cfg.invert      = false;
        cfg.freq        = 12000; //44100; 
        cfg.pwm_channel = BACKLIGHT_CHANNEL;
      _light_instance.config(cfg);
      // _light_instance.init(255);
      _light_instance.init(0);  // ระมัดระวังจะมองไม่เห็นได้ ให้มาเปิดตรงนี้

#if defined(TOUCH_GT911_TAMC) || defined(TOUCH_GT911)
      if(_ts==NULL) { _ts = new TAMC_GT911(TOUCH_I2C_SDA, TOUCH_I2C_SCL, TOUCH_INT, TOUCH_RST, TFT_WIDTH, TFT_HEIGHT); }
      if(_ts)       { _ts->begin(); }
#endif
      this->setRotation(255);
    }

    inline void init()      { begin(); }

#if defined(TOUCH_GT911_TAMC) || defined(TOUCH_GT911)
  #if defined(BEENEXT_4_3IPS) 
    template <typename T>
    uint_fast8_t getTouch(T *x, T *y)
    {
      if(_ts == NULL) return 0;
      auto ret = _ts->getTouch(x,y);
      *x = map( constrain(*x,0,480), 0,480,0,800);
      *y = map( constrain(*y,0,240), 0,240,0,480);
      return ret;
    }
  #else
    template <typename T>
    uint_fast8_t getTouch(T *x, T *y)
    {
      return (_ts==NULL)? 0 : _ts->getTouch(x,y);
    }
  #endif
#endif

    #if defined(BEENEXT_7_0IPS)
    void setRotation(uint8_t r){
      Arduino_RPi_DPI_RGBPanel::setRotation(r);
      if(_ts) { 
        _ts->setRotation((r+3)%4);
      }
    }
    #endif
    
    int16_t _windows_x, _windows_y, _windows_w, _windows_h;
    int16_t _xx,_yy, _cx, _cy;

    inline void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h)  { 
      _windows_x  = x; _windows_y  = y; _windows_w = w; _windows_h = h; _xx=0; _yy=0; _cx = _windows_x + _xx; _cy = _windows_y + _yy;
    }
    void writeColor( uint16_t color, uint16_t num) {
        if(_xx >= _windows_w -1 && _yy >= _windows_h-1) return;
        
        Arduino_RPi_DPI_RGBPanel::writePixel( _cx, _cy, color);
        if( ++_xx >= _windows_w) {_xx = _xx - _windows_w; _yy++; }
        _cx = constrain(_windows_x + _xx ,0 , width()); 
        _cy = constrain(_windows_y + _yy ,0 , height());
        if( num-1 > 0 ) writeColor(color, num-1);
    }

    inline void drawString(const char* str, int16_t x, int16_t y)   { this->setCursor(x, y); this->print(str); }
    inline void wakeup()                            { _light_instance.setBrightness(_last_brightness);}
    inline void sleep()                             { _light_instance.setBrightness(0); }
    inline void setBrightness(uint8_t brightness)   { _light_instance.setBrightness(brightness); _last_brightness = brightness; }
    inline uint8_t getBrightness()                  { return _last_brightness; }

#if defined(TOUCH_GT911_TAMC) || defined(TOUCH_GT911)  // สำหรับ AGFX TOUCH_GT911 จะมองเป็น TOUCH_GT911_TAMC ทั้งหมด
    TAMC_GT911 *_ts = NULL;
#endif

  private:
    lgfx::Light_PWM     _light_instance;
    uint8_t             _last_brightness=255;
};

#endif // agfx (BeeNeXT 4.3,5.0,7.0)


#else // ใช้ LoveCat
/******************************************************
 * LoveCat
 *****************************************************/

#include "./LoveCat/LovyanGFX.hpp"
// #include "./LoveCat/lgfx/v1/panel/Panel_Device.hpp"
#include "./LoveCat/lgfx/v1/Touch.hpp"

class BlynkGO_LCD : public lgfx::LGFX_Device {
  private:
#if defined(ILI9488_DRIVER)
    lgfx::Panel_ILI9488 _panel_instance;
#elif defined(ST7796_DRIVER)
    lgfx::Panel_ST7796  _panel_instance;
#elif defined(RA8875_DRIVER)
    lgfx::Panel_RA8875  _panel_instance;
#elif defined(ILI9341_DRIVER)
    lgfx::Panel_ILI9341 _panel_instance;
#elif defined(ST7789_DRIVER)
    lgfx::Panel_ST7789  _panel_instance;
#elif defined(RGB_DRIVER) && defined (CONFIG_IDF_TARGET_ESP32S3)
    lgfx::Panel_RGB     _panel_instance;
#elif defined(GC9A01_DRIVER)
    lgfx::Panel_GC9A01 _panel_instance;
#endif

#if defined(TOUCH_GSL1680F)
    lgfx::Touch_GSL1680F_800x480 _touch_instance;
#elif defined(TOUCH_FT5206) || defined(TOUCH_FT6236) || defined(TOUCH_FT6336U)
    lgfx::Touch_FT5x06  _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436
#elif defined(TOUCH_XPT2046)
  #if defined(BEENEXT_4_3)
    lgfx::Touch_XPT2046_neo _touch_instance;
  #else
    lgfx::Touch_XPT2046 _touch_instance;
  #endif
#elif defined(TOUCH_GT911)
    lgfx::Touch_GT911   _touch_instance;
#elif defined(TOUCH_GT911_TAMC)
    lgfx::Touch_GT911_TAMC  _touch_instance;  
#elif defined(TOUCH_CST820)
    lgfx::Touch_CST820  _touch_instance;
#endif


#if TFT_BL != -1
    lgfx::Light_PWM     _light_instance;
#endif

  #if defined  (TFT_PARALLEL16)
    lgfx::Bus_Parallel16  _bus_instance; 
  #elif defined(TFT_PARALLEL8)
    lgfx::Bus_Parallel8   _bus_instance;
  #elif defined(TFT_RGB_BUS)
    lgfx::Bus_RGB         _bus_instance;
  #else
    lgfx::Bus_SPI         _bus_instance;
  #endif


  public:
    BlynkGO_LCD(void)
    {
#if defined(RGB_DRIVER)  && defined (CONFIG_IDF_TARGET_ESP32S3)
      {
        auto panel_cfg            = _panel_instance.config();
        auto panel_cfg2           = _panel_instance.config_detail();
        
        panel_cfg.memory_width    = panel_cfg.panel_width   = TFT_WIDTH;
        panel_cfg.memory_height   = panel_cfg.panel_height  = TFT_HEIGHT;
        panel_cfg.offset_x        = panel_cfg.offset_y      = 0;
        panel_cfg.offset_rotation  = OFFSET_ROTATION;
        panel_cfg2.use_psram      = 1;
        
        _panel_instance.config(panel_cfg);
        _panel_instance.config_detail(panel_cfg2);
      }

      {
        auto bus_cfg = _bus_instance.config();
        bus_cfg.panel             = &_panel_instance;

        bus_cfg.pin_d0            = TFT_B0;  // B0
        bus_cfg.pin_d1            = TFT_B1;  // B1
        bus_cfg.pin_d2            = TFT_B2;  // B2
        bus_cfg.pin_d3            = TFT_B3;  // B3
        bus_cfg.pin_d4            = TFT_B4;  // B4
        bus_cfg.pin_d5            = TFT_G0;  // G0
        bus_cfg.pin_d6            = TFT_G1;  // G1
        bus_cfg.pin_d7            = TFT_G2;  // G2
        bus_cfg.pin_d8            = TFT_G3;  // G3
        bus_cfg.pin_d9            = TFT_G4;  // G4
        bus_cfg.pin_d10           = TFT_G5;  // G5
        bus_cfg.pin_d11           = TFT_R0;  // R0
        bus_cfg.pin_d12           = TFT_R1;  // R1
        bus_cfg.pin_d13           = TFT_R2;  // R2
        bus_cfg.pin_d14           = TFT_R3;  // R3
        bus_cfg.pin_d15           = TFT_R4;  // R4

        bus_cfg.pin_henable       = TFT_HENABLE;
        bus_cfg.pin_vsync         = TFT_VSYNC;
        bus_cfg.pin_hsync         = TFT_HSYNC;
        bus_cfg.pin_pclk          = TFT_PCLK;
        bus_cfg.freq_write        = 16000000;

        bus_cfg.hsync_polarity    = TFT_HSYNC_POLARITY;//0;
        bus_cfg.hsync_front_porch = TFT_HSYNC_FRONT_PORCH;//8;
        bus_cfg.hsync_pulse_width = TFT_HSYNC_PULSE_WIDTH;//4;
        bus_cfg.hsync_back_porch  = TFT_HSYNC_BACK_PORCH;//16;
        bus_cfg.vsync_polarity    = TFT_VSYNC_POLARITY;//0;
        bus_cfg.vsync_front_porch = TFT_VSYNC_FRONT_PORCH;//4;
        bus_cfg.vsync_pulse_width = TFT_VSYNC_PULSE_WIDTH;//4;
        bus_cfg.vsync_back_porch  = TFT_VSYNC_BACK_PORCH;//4;
        bus_cfg.pclk_idle_high    = TFT_PCLK_IDLE_HIGH;//1;

        _bus_instance.config(bus_cfg);
        _panel_instance.setBus(&_bus_instance);
      }

#else // กรณีไม่ใช่ RGB_DRIVER && !defined (CONFIG_IDF_TARGET_ESP32S3)

      {
        auto bus_cfg = _bus_instance.config();
  #if defined(ILI9488_DRIVER)
    #if defined(TFT_PARALLEL16)
      #if defined(BLYNKGO_BOARD_S3_PARALLEL)
        bus_cfg.port        = 0;  // 使用するI2Sポートを選択 (0 or 1)
        bus_cfg.freq_write  = 20000000; // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
      #else
        bus_cfg.i2s_port    = I2S_NUM_0;  // 使用するI2Sポートを選択 (0 or 1) (ESP32のI2S LCDモードを使用します)
        bus_cfg.freq_write  = 16000000; // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
      #endif
        bus_cfg.pin_wr      = TFT_PIN_WR;           // WR を接続しているピン番号
        bus_cfg.pin_rd      = TFT_PIN_RD;           // RD を接続しているピン番号
        bus_cfg.pin_rs      = TFT_PIN_RS;           // RS(D/C)を接続しているピン番号

        bus_cfg.pin_d0      = TFT_PIN_D0;
        bus_cfg.pin_d1      = TFT_PIN_D1;
        bus_cfg.pin_d2      = TFT_PIN_D2;
        bus_cfg.pin_d3      = TFT_PIN_D3;
        bus_cfg.pin_d4      = TFT_PIN_D4;
        bus_cfg.pin_d5      = TFT_PIN_D5;
        bus_cfg.pin_d6      = TFT_PIN_D6;
        bus_cfg.pin_d7      = TFT_PIN_D7;
        bus_cfg.pin_d8      = TFT_PIN_D8;
        bus_cfg.pin_d9      = TFT_PIN_D9;
        bus_cfg.pin_d10     = TFT_PIN_D10;
        bus_cfg.pin_d11     = TFT_PIN_D11;
        bus_cfg.pin_d12     = TFT_PIN_D12;
        bus_cfg.pin_d13     = TFT_PIN_D13;
        bus_cfg.pin_d14     = TFT_PIN_D14;
        bus_cfg.pin_d15     = TFT_PIN_D15;
    #else
        bus_cfg.spi_host    = TFT_SPI_HOST_TYPE; //SPI_HOST_TYPE; //SPI2_HOST;
        bus_cfg.spi_mode    = 0;             // SPI通信モードを設定 (0 ~ 3)
        bus_cfg.freq_write  = 40000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
        bus_cfg.freq_read   = 16000000;    // 受信時のSPIクロック
        bus_cfg.spi_3wire   = false;       //true;        // 受信をMOSIピンで行う場合はtrueを設定
        bus_cfg.use_lock    = true;        // トランザクションロックを使用する場合はtrueを設定
        bus_cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
        bus_cfg.pin_mosi    = TFT_MOSI;
        bus_cfg.pin_miso    = TFT_MISO;
        bus_cfg.pin_sclk    = TFT_SCLK;
        bus_cfg.pin_dc      = TFT_DC; 
    #endif
  #elif defined(ST7789_DRIVER)
        bus_cfg.spi_host    = TFT_SPI_HOST_TYPE;  // ใน blynkgo_system_config.h
        bus_cfg.spi_mode    = 0;             // SPI通信モードを設定 (0 ~ 3)
        bus_cfg.freq_write  = 40000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
        bus_cfg.freq_read   = 16000000;    // 受信時のSPIクロック
        bus_cfg.spi_3wire   = false;//true;        // 受信をMOSIピンで行う場合はtrueを設定
        bus_cfg.use_lock    = true;        // トランザクションロックを使用する場合はtrueを設定
        bus_cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
        bus_cfg.pin_mosi    = TFT_MOSI;
        bus_cfg.pin_miso    = TFT_MISO;
        bus_cfg.pin_sclk    = TFT_SCLK;
        bus_cfg.pin_dc      = TFT_DC; 
  #elif defined(ST7796_DRIVER)
    #if defined(TFT_PARALLEL8)
//        bus_cfg.i2s_port    = I2S_NUM_0;  // 使用するI2Sポートを選択 (0 or 1) (ESP32のI2S LCDモードを使用します)
        bus_cfg.freq_write  = 16000000; // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
        bus_cfg.pin_wr      = TFT_PIN_WR;           // WR を接続しているピン番号
        bus_cfg.pin_rd      = TFT_PIN_RD;           // RD を接続しているピン番号
        bus_cfg.pin_rs      = TFT_PIN_RS;           // RS(D/C)を接続しているピン番号

        bus_cfg.pin_d0      = TFT_PIN_D0;
        bus_cfg.pin_d1      = TFT_PIN_D1;
        bus_cfg.pin_d2      = TFT_PIN_D2;
        bus_cfg.pin_d3      = TFT_PIN_D3;
        bus_cfg.pin_d4      = TFT_PIN_D4;
        bus_cfg.pin_d5      = TFT_PIN_D5;
        bus_cfg.pin_d6      = TFT_PIN_D6;
        bus_cfg.pin_d7      = TFT_PIN_D7;
    #else
        bus_cfg.spi_host    = TFT_SPI_HOST_TYPE; //VSPI_HOST; //SPI2_HOST;
        bus_cfg.spi_mode    = 0;             // SPI通信モードを設定 (0 ~ 3)
        bus_cfg.freq_write  = 40000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
        bus_cfg.freq_read   = 16000000;    // 受信時のSPIクロック
        bus_cfg.spi_3wire   = false;//true;        // 受信をMOSIピンで行う場合はtrueを設定
        bus_cfg.use_lock    = true;        // トランザクションロックを使用する場合はtrueを設定
        bus_cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
        bus_cfg.pin_mosi    = TFT_MOSI;
        bus_cfg.pin_miso    = TFT_MISO;
        bus_cfg.pin_sclk    = TFT_SCLK;
        bus_cfg.pin_dc      = TFT_DC; 
    #endif
  #elif defined(RA8875_DRIVER)
        bus_cfg.spi_host    = VSPI_HOST;  // VSPI_HOST : OK , HSPI_HOST : OK
        bus_cfg.spi_mode    = 0;          // (0 ~ 3); 0 or 3 ; 0 : X, 1 : X , 2 : X, 3 : OK
        bus_cfg.freq_write  = 40000000;
        bus_cfg.freq_read   = 5000000;    // 5000000 : OK , 16000000 : OK
        bus_cfg.pin_sclk    = TFT_SCLK;
        bus_cfg.pin_mosi    = TFT_MOSI;
        bus_cfg.pin_miso    = TFT_MISO;
        bus_cfg.spi_3wire   = TFT_SPI_3WIRE;
  #elif defined(ILI9341_DRIVER)
        bus_cfg.spi_host = SPI_HOST_TYPE; //VSPI_HOST;     // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
        // ※ ESP-IDFバージョンアップに伴い、VSPI_HOST , HSPI_HOSTの記述は非推奨になるため、エラーが出る場合は代わりにSPI2_HOST , SPI3_HOSTを使用してください。
        bus_cfg.spi_mode = 0;             // SPI通信モードを設定 (0 ~ 3)
        bus_cfg.freq_write = 40000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
        bus_cfg.freq_read  = 16000000;    // 受信時のSPIクロック
        bus_cfg.spi_3wire  = false;       // false จะได้จับภาพได้ 受信をMOSIピンで行う場合はtrueを設定
        bus_cfg.use_lock   = true;        // トランザクションロックを使用する場合はtrueを設定
        bus_cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
        // ※ ESP-IDFバージョンアップに伴い、DMAチャンネルはSPI_DMA_CH_AUTO(自動設定)が推奨になりました。1ch,2chの指定は非推奨になります。
        bus_cfg.pin_sclk  = TFT_SCLK;            // SPIのSCLKピン番号を設定
        bus_cfg.pin_mosi  = TFT_MOSI;            // SPIのMOSIピン番号を設定
        bus_cfg.pin_miso  = TFT_MISO;            // SPIのMISOピン番号を設定 (-1 = disable)
        bus_cfg.pin_dc    = TFT_DC;            // SPIのD/Cピン番号を設定  (-1 = disable)
  #elif defined(GC9A01_DRIVER)
        // SPIバスの設定
        bus_cfg.spi_host    = SPI_HOST_TYPE; // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
        // ※ ESP-IDFバージョンアップに伴い、VSPI_HOST , HSPI_HOSTの記述は非推奨になるため、エラーが出る場合は代わりにSPI2_HOST , SPI3_HOSTを使用してください。
        bus_cfg.spi_mode    = 0;                  // SPI通信モードを設定 (0 ~ 3)
        bus_cfg.freq_write  = 80000000;         // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
        bus_cfg.freq_read   = 20000000;          // 接收时的SPI时钟
        bus_cfg.spi_3wire   = true;              // 受信をMOSIピンで行う場合はtrueを設定
        bus_cfg.use_lock    = true;               // 使用事务锁时设置为 true
        bus_cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定 (0=DMA不使用 / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=自動設定)
        // ※ ESP-IDFバージョンアップに伴い、DMAチャンネルはSPI_DMA_CH_AUTO(自動設定)が推奨になりました。1ch,2chの指定は非推奨になります。
        bus_cfg.pin_sclk    = TFT_SCLK;            // SPIのSCLKピン番号を設定
        bus_cfg.pin_mosi    = TFT_MOSI;            // SPIのMOSIピン番号を設定
        bus_cfg.pin_miso    = TFT_MISO;            // SPIのMISOピン番号を設定 (-1 = disable)
        bus_cfg.pin_dc      = TFT_DC;            // SPIのD/Cピン番号を設定  (-1 = disable)
  #endif

        _bus_instance.config(bus_cfg);
        _panel_instance.setBus(&_bus_instance);
      }

      {
        auto panel_cfg = _panel_instance.config();
  #if defined(ILI9488_DRIVER) || defined(ST7796_DRIVER) || defined(ILI9341_DRIVER) || defined(ST7789_DRIVER) || defined(GC9A01_DRIVER)
        panel_cfg.pin_cs           = TFT_CS; //14;
        panel_cfg.pin_rst          = TFT_RST;
        panel_cfg.pin_busy         = TFT_BUSY;

        panel_cfg.memory_width     = TFT_WIDTH;  // 実際に表示可能な幅
        panel_cfg.memory_height    = TFT_HEIGHT;  // 実際に表示可能な高さ
        panel_cfg.panel_width      = TFT_WIDTH;  // 実際に表示可能な幅
        panel_cfg.panel_height     = TFT_HEIGHT;  // 実際に表示可能な高さ
        panel_cfg.offset_x         =     0;  // パネルのX方向オフセット量
        panel_cfg.offset_y         =     0;  // パネルのY方向オフセット量
        panel_cfg.offset_rotation  = OFFSET_ROTATION;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
        panel_cfg.dummy_read_pixel =     8;  // ピクセル読出し前のダミーリードのビット数
        panel_cfg.dummy_read_bits  =     1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
        panel_cfg.readable         =  true;  // データ読出しが可能な場合 trueに設定
        panel_cfg.invert           = TFT_INVERT;  // パネルの明暗が反転してしまう場合 trueに設定
        panel_cfg.rgb_order        = false;  // パネルの赤と青が入れ替わってしまう場合 trueに設定
    #if defined(TFT_PARALLEL16)
        panel_cfg.dlen_16bit       = true;  // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
    #else
        panel_cfg.dlen_16bit       = false;  // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合 trueに設定
    #endif
        panel_cfg.bus_shared       =  true;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

  #elif defined(RA8875_DRIVER)
        panel_cfg.pin_cs            = TFT_CS;
        panel_cfg.pin_rst           = TFT_RST;
        panel_cfg.pin_busy          = TFT_BUSY;
        panel_cfg.panel_width       = TFT_WIDTH;   // 実際に表示可能な幅
        panel_cfg.panel_height      = TFT_HEIGHT;  // 実際に表示可能な高さ

        panel_cfg.dummy_read_pixel  = TFT_DUMMY_READ_PIXEL;  // 8 or 16 OK
        panel_cfg.dummy_read_bits   = TFT_DUMMY_READ_BITS;   // 0 or 1 OK
        panel_cfg.readable          = true;
  #endif
        _panel_instance.config(panel_cfg);
      }

#endif // #if not defined(RGB_DRIVER)

#if defined(TOUCH_GT911) || defined(TOUCH_GT911_TAMC)
   { // タッチスクリーン制御の設定を行います。（必要なければ削除）
      auto touch_cfg = _touch_instance.config();
      touch_cfg.i2c_port   = TOUCH_I2C_PORT;         // 使用するI2Cを選択 (0 or 1)
      touch_cfg.i2c_addr   = TOUCH_I2C_ADDR;         // I2Cデバイスアドレス番号
      touch_cfg.pin_sda    = TOUCH_I2C_SDA;          // SDAが接続されているピン番号
      touch_cfg.pin_scl    = TOUCH_I2C_SCL;          // SCLが接続されているピン番号
      touch_cfg.pin_int    = TOUCH_INT;              // INTが接続されているピン番号
//      touch_cfg.pin_rst    = TOUCH_RST;            // เพิ่มบรรทัดนี้ touch สัมผัสไม่ได้
  #if defined(TOUCH_GT911_TAMC)
      touch_cfg.pin_rst    = TOUCH_RST;
  #endif
      touch_cfg.freq       = 400000;                 // I2Cクロックを設定

      touch_cfg.x_min      = 0;
      touch_cfg.x_max      = TFT_WIDTH -1;//480-1;
      touch_cfg.y_min      = 0;
      touch_cfg.y_max      = TFT_HEIGHT-1; //272-1;

      touch_cfg.bus_shared = false;//true;                   // 画面と共通のバスを使用している場合 trueを設定
      touch_cfg.offset_rotation = OFFSET_ROTATION;//+3;//0;                 // 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

      _touch_instance.config(touch_cfg);
      _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
    }
#elif defined(TOUCH_GSL1680F) || defined(TOUCH_FT5206) || defined(TOUCH_FT6236) || defined(TOUCH_FT6336U)
      { // タッチスクリーン制御の設定を行います。（必要なければ削除）
        auto touch_cfg = _touch_instance.config();
    // #if defined(TOUCH_GT911)
    //     touch_cfg.x_min      = 0;
    //     touch_cfg.x_max      = 480-1;
    //     touch_cfg.y_min      = 0;
    //     touch_cfg.y_max      = 272-1;
    // #else
        touch_cfg.x_min      = 0;
        touch_cfg.x_max      = TFT_WIDTH-1;
        touch_cfg.y_min      = 0;
        touch_cfg.y_max      = TFT_HEIGHT-1;
    // #endif
        touch_cfg.pin_int    = TOUCH_INT;              // INTが接続されているピン番号
        touch_cfg.bus_shared = true;                   // 画面と共通のバスを使用している場合 trueを設定
        touch_cfg.offset_rotation = 0;                 // 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

        touch_cfg.i2c_port   = TOUCH_I2C_PORT;         // 使用するI2Cを選択 (0 or 1)
        touch_cfg.i2c_addr   = TOUCH_I2C_ADDR;         // I2Cデバイスアドレス番号
        touch_cfg.pin_sda    = TOUCH_I2C_SDA;          // SDAが接続されているピン番号
        touch_cfg.pin_scl    = TOUCH_I2C_SCL;          // SCLが接続されているピン番号
        touch_cfg.freq       = 400000;                 // I2Cクロックを設定

        _touch_instance.config(touch_cfg);
        _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
      }
#elif defined(TOUCH_CST820)
      {
        auto touch_cfg = _touch_instance.config();
        touch_cfg.x_min      = 0;
        touch_cfg.x_max      = TFT_WIDTH-1;
        touch_cfg.y_min      = 0;
        touch_cfg.y_max      = TFT_HEIGHT-1;

        touch_cfg.pin_int    = TOUCH_INT;              // INTが接続されているピン番号
        touch_cfg.pin_rst    = TOUCH_RST;
        touch_cfg.bus_shared = true;                   // 画面と共通のバスを使用している場合 trueを設定
        touch_cfg.offset_rotation = 0;                 // 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

        touch_cfg.i2c_port   = TOUCH_I2C_PORT;         // 使用するI2Cを選択 (0 or 1)
        touch_cfg.i2c_addr   = TOUCH_I2C_ADDR;         // I2Cデバイスアドレス番号
        touch_cfg.pin_sda    = TOUCH_I2C_SDA;          // SDAが接続されているピン番号
        touch_cfg.pin_scl    = TOUCH_I2C_SCL;          // SCLが接続されているピン番号

        _touch_instance.config(touch_cfg);
        _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
      }
#elif defined(TOUCH_XPT2046)
      { // タッチスクリーン制御の設定を行います。（必要なければ削除）
        auto touch_cfg = _touch_instance.config();
    #if defined (BEENEXT_4_3)
        touch_cfg.x_min           = 100;//4000;//0;    // タッチスクリーンから得られる最小のX値(生の値)
        touch_cfg.x_max           = 4000;//100;//TFT_HEIGHT-1;  // タッチスクリーンから得られる最大のX値(生の値)
        touch_cfg.y_min           = 100;//0;    // タッチスクリーンから得られる最小のY値(生の値)
        touch_cfg.y_max           = 4000; //TFT_WIDTH-1;  // タッチスクリーンから得られる最大のY値(生の値)
    #else
        touch_cfg.x_min           = 0;    // タッチスクリーンから得られる最小のX値(生の値)
        touch_cfg.x_max           = TFT_HEIGHT-1;  // タッチスクリーンから得られる最大のX値(生の値)
        touch_cfg.y_min           = 0;    // タッチスクリーンから得られる最小のY値(生の値)
        touch_cfg.y_max           = TFT_WIDTH-1;  // タッチスクリーンから得られる最大のY値(生の値)
    #endif
        
        touch_cfg.pin_int    = TOUCH_INT;   // INTが接続されているピン番号
        touch_cfg.bus_shared = true; // 画面と共通のバスを使用している場合 trueを設定

        // SPI接続の場合
        touch_cfg.spi_host = TOUCH_HOST_TYPE; //HSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
        touch_cfg.offset_rotation = 0;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定
        touch_cfg.freq     = 1000000;     // SPIクロックを設定
        touch_cfg.pin_sclk = TOUCH_SCLK;     // SCLKが接続されているピン番号
        touch_cfg.pin_mosi = TOUCH_MOSI;     // MOSIが接続されているピン番号
        touch_cfg.pin_miso = TOUCH_MISO;     // MISOが接続されているピン番号
        touch_cfg.pin_cs   = TOUCH_CS;     //   CSが接続されているピン番号

        _touch_instance.config(touch_cfg);
        _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
      }
#endif // #elif defined(TOUCH_XPT2046)

#if TFT_BL != -1
      { // バックライト制御の設定を行います。（必要なければ削除）
        auto cfg = _light_instance.config();    // バックライト設定用の構造体を取得します。

        cfg.pin_bl      = TFT_BL;               // バックライトが接続されているピン番号
    #if defined(RGB_DRIVER)
    #else
#if defined(TFT_BL_INVERT)
        cfg.invert      = TFT_BL_INVERT;             // バックライトの輝度を反転させる場合 true
#else
        cfg.invert      = true;//false;                // バックライトの輝度を反転させる場合 true
#endif
        cfg.freq        = 12000; //44100;                // バックライトのPWM周波数
        cfg.pwm_channel = BACKLIGHT_CHANNEL;    // 使用するPWMのチャンネル番号  : ch15
    #endif
        _light_instance.config(cfg);
        _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
      }
#endif // #if TFT_BL != -1

    setPanel(&_panel_instance);
  }

#if defined(TOUCH_GT911) || defined(BEENEXT_4_3) // fix getTouch(..) for GT911 ให้ถูกต้อง
    template <typename T>
    uint_fast8_t getTouch(T *x, T *y, uint_fast8_t index = 0)
    {
      if (index >= 8) return 0;
      lgfx::v1::touch_point_t tp[8];
      auto count = LGFX_Device::getTouch(tp, index + 1);
      if (index >= count) return 0;
        switch(panel()->getRotation()){
          case 0:  // w : 800 h: 480
            if (x) *x = tp[index].x;
            if (y) *y = tp[index].y;
            break;
          case 1:  // w : 480  h: 800
            if (x) *x = tp[index].y;
            if (y) *y = height() - tp[index].x;
            break;
          case 2:
            if (x) *x = width()  - tp[index].x;
            if (y) *y = height() - tp[index].y;
            break;
          case 3:
            if (x) *x = width() - tp[index].y;
            if (y) *y = tp[index].x;
            break;
          case 4:
            if (x) *x = tp[index].x;
            if (y) *y = height() - tp[index].y;
            break;
          case 5:  // w : 480  h: 800
            if (x) *x = tp[index].y;
            if (y) *y = tp[index].x;
            break;
          case 6: // w : 800 h: 480
            if (x) *x = width()  - tp[index].x;
            if (y) *y = tp[index].y;
            break;
          case 7: // w : 480  h: 800
            if (x) *x = width()  - tp[index].y;
            if (y) *y = height() - tp[index].x;
            break;
        }
      return count;
    }
#endif //#if defined(TOUCH_GT911) || defined(BEENEXT_4_3) 
};


#endif //#if defined(BLYNKGO_USE_AGFX) && (BLYNKGO_USE_AGFX==1) 
#endif// #if defined (TFT_4_3_INCH_TOUCH_485)
#endif// #if !defined(BLYNKGO_OLED)


#endif // __BLYNKGO_LCD_H__
