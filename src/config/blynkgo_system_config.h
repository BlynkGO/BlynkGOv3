/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ blynkgo_system_config.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 *********************************************************************
 */

#ifndef __BLYNKGO_SYSTEM_CONFIG_H__
#define __BLYNKGO_SYSTEM_CONFIG_H__

#define BLYNKGO_V3

#if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563
  #define BLYNKGO_USE_RTC           1
#else
  #define BLYNKGO_USE_RTC           0
#endif

//-------------------------------------------------
/* System Config */
#define BLYNKGO_SPIFFS_DRIVE      "F:/"
#define BLYNKGO_SD_DRIVE          "S:/"
#define CALIBRATION_FILE          "/TouchCalData1"

#define LED_R_CHANNEL             ((uint8_t)12)
#define LED_G_CHANNEL             ((uint8_t)13)
#define LED_B_CHANNEL             ((uint8_t)14)

#if defined (CONFIG_IDF_TARGET_ESP32S3)
#define BACKLIGHT_CHANNEL         ((uint8_t)7)  // ((uint8_t)1)
#else
#define BACKLIGHT_CHANNEL         ((uint8_t)15)  // ((uint8_t)1)
#endif

#ifndef BLYNKGO_USE_FS
#define BLYNKGO_USE_FS            1
#endif

#ifndef BLYNKGO_USE_KBD
#define BLYNKGO_USE_KBD           1
#endif

#ifndef BLYNKGO_USE_MANAGER
#define BLYNKGO_USE_MANAGER       0
#endif

#ifndef BLYNKGO_USE_SNAPSHOT
#define BLYNKGO_USE_SNAPSHOT      1
#endif

#if BLYNKGO_USE_MANAGER || BLYNKGO_USE_BLYNK
#ifdef BLYNKGO_USE_WIFI
#undef BLYNKGO_USE_WIFI
#define BLYNKGO_USE_WIFI          1
#endif  // BLYNKGO_USE_WIFI
#endif  // BLYNKGO_USE_MANAGER or BLYNKGO_USE_BLYNK

#if BLYNKGO_USE_NTP
#define NTP_HOSTNAME1             "time.nist.gov"
#define NTP_HOSTNAME2             "pool.ntp.org" //
#define NTP_HOSTNAME3             "asia.pool.ntp.org" // "time.navy.mi.th"
#define NTP_TIMEZONE              7
#define NTP_DAYLIGHT_SAVING       0
#define NTP_INTERVAL_SYNC         1  // (minutes) ซิงค์เวลากับ NTP Server ทุกๆกี่นาที
#endif

#if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563
#define BLYNKGO_USE_RTC           1
#else
#define BLYNKGO_USE_RTC           0
#endif

#define TFT_BACKLIGHT_ON          HIGH



//------------------------------------------------
#if defined BLYNKGO_BOARD_V1_3
  #define BLYNKGO_BOARD_NAME      "BlynkGO Board V1.3 + TFT 3.5\" Capacitive with Camera Module"
  #define ROTATE_TYPE             3
  #define OFFSET_ROTATION         0
  #define BLYNKGO_USE_SD          1

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_TYPE or VSPI_TYPE;
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 3.5
  #define ILI9488_DRIVER     // TFT 3.5"
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  33
  #define TFT_RST                 26 //-1
  #define TFT_BUSY                -1
  #define TFT_BL                  -1  
  #define TFT_INVERT              false

  // SD
  #define SD_HSPI_MODE
  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

  // capacitive touch i2c
  #define TOUCH_FT6236
  #define TOUCH_I2C_ADDR          0x38
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_SDA           26
  #define TOUCH_I2C_SCL           27
  #define TOUCH_INT               -1 //38


  // Camera
  #define PWDN_GPIO_NUM           -1
  #define RESET_GPIO_NUM          -1
  #define XCLK_GPIO_NUM           32
  #define SIOD_GPIO_NUM           26
  #define SIOC_GPIO_NUM           27

  #define Y9_GPIO_NUM             35
  #define Y8_GPIO_NUM             34
  #define Y7_GPIO_NUM             39
  #define Y6_GPIO_NUM             36
  #define Y5_GPIO_NUM             21
  #define Y4_GPIO_NUM             19
  #define Y3_GPIO_NUM             18
  #define Y2_GPIO_NUM             5
  #define VSYNC_GPIO_NUM          25
  #define HREF_GPIO_NUM           23
  #define PCLK_GPIO_NUM           22

  #define DAC_IIS_BCK             22
  #define DAC_IIS_WS              25   // LRC = WS = WSEL
  #define DAC_IIS_DOUT            23
  #define DAC_IIS_DIN             -1

#elif defined BLYNKGO_BOARD_OLED_SSD1306
  #define BLYNKGO_BOARD_NAME      "BlynkGO Board  + OLED (SSD1306)"
  #define BLYNKGO_OLED_SSD1306
  #define BLYNKGO_OLED

  #define BLYNKGO_USE_SD          1
  #define SD_HSPI_MODE

  // HSPI MISO12, MOSI13, SCK14, SS15;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_TYPE or VSPI_TYPE;
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // SD
  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

#elif defined BLYNKGO_BOARD_OLED_SH1106
  #define BLYNKGO_BOARD_NAME      "BlynkGO Board  + OLED (SH1106)"
  #define BLYNKGO_OLED_SH1106
  #define BLYNKGO_OLED

  #define BLYNKGO_USE_SD          1
  #define SD_HSPI_MODE

  // HSPI MISO12, MOSI13, SCK14, SS15;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_TYPE or VSPI_TYPE;
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // SD
  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

#endif

//------------------------------------------------
#if defined BLYNKGO_BOARD_V2

  #define BLYNKGO_BOARD_NAME      "BlynkGO Board V2.0P + TFT 3.5\" Capacitive (ESP32S2)"
  #define BLYNKGO_BASIC
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         1
  #define BLYNKGO_USE_SD          1

  // TFT 3.5E
  #define ILI9488_DRIVER     // TFT 3.5"  
  #define TFT_PARALLEL16
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define TFT_CS                  37  // OK
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  45    // OK
  #define TFT_INVERT              false

  #define TFT_PIN_WR              35
  #define TFT_PIN_RD              34
  #define TFT_PIN_RS              36
  
  #define TFT_PIN_D0              33
  #define TFT_PIN_D1              21
  #define TFT_PIN_D2              14
  #define TFT_PIN_D3              13
  #define TFT_PIN_D4              12
  #define TFT_PIN_D5              11
  #define TFT_PIN_D6              10
  #define TFT_PIN_D7              9
  #define TFT_PIN_D8              3
  #define TFT_PIN_D9              8
  #define TFT_PIN_D10             16
  #define TFT_PIN_D11             15
  #define TFT_PIN_D12             7
  #define TFT_PIN_D13             6
  #define TFT_PIN_D14             5
  #define TFT_PIN_D15             4

  // SD
  // HSPI MISO12, MOSI13, SCK14, SS15;
//  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_TYPE or VSPI_TYPE;
  #define SD_VSPI_MODE
  #define SD_MISO                 41
  #define SD_MOSI                 2
  #define SD_SCLK                 42
  #define SD_CS                   1       // SD Chip Select

  // capacitive touch i2c
  #define TOUCH_FT6236
  #define TOUCH_I2C_ADDR          0x38
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_SDA           38
  #define TOUCH_I2C_SCL           39
  #define TOUCH_INT               40
#endif

#if defined BLYNKGO_BOARD_S3_PARALLEL
  #define BLYNKGO_BOARD_NAME      "BlynkGO Board S3 Parallel TFT 3.5\" Capacitive"
  // #define BLYNKGO_BASIC
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         1
  #define BLYNKGO_USE_SD          1

  // TFT 3.5
  #define ILI9488_DRIVER     // TFT 3.5"  
  #define TFT_PARALLEL16
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define DOUBLE_FULL_BUFFER      1
  #define TFT_CS                  -1    // OK
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  45    // OK
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false

  #define TFT_PIN_WR              35
  #define TFT_PIN_RD              48
  #define TFT_PIN_RS              36
  
  #define TFT_PIN_D0              47
  #define TFT_PIN_D1              21
  #define TFT_PIN_D2              14
  #define TFT_PIN_D3              13
  #define TFT_PIN_D4              12
  #define TFT_PIN_D5              11
  #define TFT_PIN_D6              10
  #define TFT_PIN_D7              9
  #define TFT_PIN_D8              3
  #define TFT_PIN_D9              8
  #define TFT_PIN_D10             16
  #define TFT_PIN_D11             15
  #define TFT_PIN_D12             7
  #define TFT_PIN_D13             6
  #define TFT_PIN_D14             5
  #define TFT_PIN_D15             4

  // capacitive touch i2c
  #define TOUCH_FT6236
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x38
  #define TOUCH_I2C_SDA           38
  #define TOUCH_I2C_SCL           39
  #define TOUCH_INT               -1 //38

  #define SD_FSPI_MODE
  #define SD_CS                   1        // SD Chip Select
  #define SD_MOSI                 2
  #define SD_SCLK                 42
  #define SD_MISO                 41

#endif

//------------------------------------------------
#if defined BLYNKGO_EMMA_II
  #if defined EMMA16MB
    #define BLYNKGO_BOARD_NAME      "BlynkGO EMMA II (TFT 3.5 Capacitive Flash 16MB PSRAM 8MB)"
  #else
    #define BLYNKGO_BOARD_NAME      "BlynkGO EMMA II (TFT 3.5 Capacitive Flash 4MB PSRAM 4MB)"
  #endif
  // #define BLYNKGO_BASIC
  #define ROTATE_TYPE             1
  #define OFFSET_ROTATION         0
  #define BLYNKGO_USE_SD          0
  #define SD_HSPI_MODE


  // HSPI MISO12, MOSI13, SCK14, SS15;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_TYPE or VSPI_TYPE;
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 3.5
  #define ST7796_DRIVER
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                -1      // SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15      // Chip select control pin
  #define TFT_DC                  21      // Data Command control pin
  #define TFT_RST                 22      // TFT reset pin
  #define TFT_BUSY                -1
  #define TFT_BL                  23      // TFT backlight  pin
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false

  // capacitive touch i2c
  #define TOUCH_FT6236
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x38
  #define TOUCH_I2C_SDA           18
  #define TOUCH_I2C_SCL           19
  #define TOUCH_INT               -1 //38

#elif defined BLYNKGO_EMMA_III
  #define BLYNKGO_BOARD_NAME      "BlynkGO EMMA III (TFT 3.5 Capacitive ESP32S3 Parallel)"
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         1
  #define BLYNKGO_USE_SD          1

  // TFT 3.5
  #define ST7796_DRIVER     // TFT 3.5"  
  #define TFT_PARALLEL8
  #define TFT_IPS
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define TFT_CS                  -1  // not defined
  #define TFT_DC                  0
  #define TFT_RST                 4
  #define TFT_BUSY                -1
  #define TFT_BL                  45
  #define TFT_INVERT              true

  #define TFT_PIN_WR              47  // Write clock
  #define TFT_PIN_RD              -1  // not defined
  #define TFT_PIN_RS              0   // [???] Command/Data selectio
  #define TFT_PIN_TE              48  // [???] Frame sync
  
  #define TFT_PIN_D0              9
  #define TFT_PIN_D1              46
  #define TFT_PIN_D2              3
  #define TFT_PIN_D3              8
  #define TFT_PIN_D4              18
  #define TFT_PIN_D5              17
  #define TFT_PIN_D6              16
  #define TFT_PIN_D7              15

  // SD
  // HSPI MISO12, MOSI13, SCK14, SS15;
//  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_TYPE or VSPI_TYPE;
  #define SD_VSPI_MODE
  #define SD_MISO                 41
  #define SD_MOSI                 2
  #define SD_SCLK                 42
  #define SD_CS                   1       // SD Chip Select

  // capacitive touch i2c  // 
  #define TOUCH_FT6336U
  #define TOUCH_I2C_ADDR          0x38
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_SDA           6
  #define TOUCH_I2C_SCL           5
  #define TOUCH_I2C_RESET         4    // ???
  #define TOUCH_INT               7
#endif
// #endif

//------------------------------------------------
#if defined BEENEXT_1_9    // 170*320(Pixel)
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 1.9\" ESP32S3"

  #define ST7789_DRIVER
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         1
  #define ROTATE_TYPE             1
  #define BLYNKGO_USE_SD          0
  #define BLYNKGO_USE_AGFX        1

  #define SPI_HOST_TYPE           SPI2_HOST  // spi_host_device_t  ใน spi_types.h --> SPI1_HOST=0 or SPI2_HOST=1 ( FSPI=0 or HSPI=1  ESP32S3   ดูใน esp32_hal_spi.h ; )
  #define SPI_MOSI                13
  #define SPI_SCLK                12
  #define SPI_MISO                -1

  // TFT
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              170
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  10      // Chip select control pin
  #define TFT_DC                  11      // Data Command control pin
  #define TFT_RST                 1
  #define TFT_BUSY                -1
  #define TFT_BL                  14       // TFT backlight  pin
  #define TFT_INVERT              true

//------------------------------------------------
#elif defined BEENEXT_2_4
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 2.4\" Touch (resistive)"
  #define BLYNKGO_BASIC
  #define BEENEXT

  #define OFFSET_ROTATION         1
  // #define TOUCH_OFFSET_ROTATION   6
  #define ROTATE_TYPE             0

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 2.4"
  #define ILI9341_DRIVER      // TFT 2.4"
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              320
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  27 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1

  
  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select

  #define TOUCH_XPT2046
  #define TOUCH_HOST_TYPE         SPI_HOST_TYPE
  #define TOUCH_INT               36
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                33

  #define LDR_PIN                 34
  #define LDR_RESOLUTION          12
  #define SPEAKER_PIN             26

  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

#elif defined BEENEXT_2_4C
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 2.4\" Touch (capacitive)"  // ยังไม่ได้ impliment
  #define BLYNKGO_BASIC
  #define BEENEXT

  #define OFFSET_ROTATION         1
  #define ROTATE_TYPE             0

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 2.4"
  #define ILI9341_DRIVER      // TFT 2.4"
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              320
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  27 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1

  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select

  // capacitive touch i2c
  #define TOUCH_CST820
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x15
  #define TOUCH_I2C_SDA           33
  #define TOUCH_I2C_SCL           32
  #define TOUCH_INT               -1    // 21 , -1 not used
  #define TOUCH_RST               25
  #define TOUCH_BUS_SHARED        false

  #define LDR_PIN                 34
  #define LDR_RESOLUTION          12
  #define SPEAKER_PIN             26

  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

#elif defined BEENEXT_2_8
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 2.8\" Touch (resistive)"
  #define BLYNKGO_BASIC
  #define BEENEXT
  // #define TFT_2_8_INCH_TOUCH
  #define OFFSET_ROTATION         0
  #define ROTATE_TYPE             3

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 2.8"
  #define ILI9341_DRIVER      // TFT 2.8"
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              320
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  21 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1

  
  #define BLYNKGO_USE_SD          1   // conflict with TFT
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select

  #define TOUCH_XPT2046
  #define TOUCH_HOST_TYPE         -1
  #define TOUCH_INT               36
  #define TOUCH_MOSI              32
  #define TOUCH_MISO              39
  #define TOUCH_SCLK              25
  #define TOUCH_CS                33

  #define LDR_PIN                 34
  #define LDR_RESOLUTION          12
  #define SPEAKER_PIN             26

  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

#elif defined BEENEXT_2_8C
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 2.8\" Touch (capacitive)"  // ยังไม่ได้ impliment
  #define BLYNKGO_BASIC
  #define OFFSET_ROTATION         0
  #define ROTATE_TYPE             3
  #define BEENEXT
  // #define TFT_2_8_INCH_TOUCH
  #define OFFSET_ROTATION         0
  #define ROTATE_TYPE             3

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 2.8"
  #define ILI9341_DRIVER      // TFT 2.8"
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              320
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  21 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1

  
  #define BLYNKGO_USE_SD          1   // conflict with TFT
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select


  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

#elif defined BEENEXT_3_2
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 3.2\" Touch (resistive)"  // ยังไม่ได้ impliment
  #define BLYNKGO_BASIC
  #define OFFSET_ROTATION         0
  #define ROTATE_TYPE             3
  #define BEENEXT
  // #define TFT_3_2_INCH_TOUCH

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 3.2"
  #define ST7789_DRIVER      // TFT 3.2"
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              320
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 35
  #define TFT_BUSY                0//-1
  #define TFT_BL                  27 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              true  // กลับสี
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1

  
  #define BLYNKGO_USE_SD          1 
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select

  #define TOUCH_XPT2046
  #define TOUCH_HOST_TYPE         SPI_HOST_TYPE
  #define TOUCH_INT               36
  #define TOUCH_MOSI              SPI_MOSI // 13 <-- 32
  #define TOUCH_MISO              SPI_MISO // 12 <-- 39
  #define TOUCH_SCLK              SPI_SCLK // 14 <-- 25
  #define TOUCH_CS                33


  #define LDR_PIN                 34
  #define LDR_RESOLUTION          12
  #define SPEAKER_PIN             26

  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

#elif defined BEENEXT_3_2C
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 3.2\" Touch (capacitive)"  // ยังไม่ได้ impliment
  #define BLYNKGO_BASIC
  #define OFFSET_ROTATION         0
  #define ROTATE_TYPE             3
  #define BEENEXT

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 3.2"
  #define ST7789_DRIVER      // TFT 3.2"
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              320
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 35
  #define TFT_BUSY                -1
  #define TFT_BL                  27 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              true  // กลับสี
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1

  
  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select

  // capacitive touch i2c
  #define TOUCH_GT911_TAMC
  // #define TOUCH_GT911
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x5D  // 0x14
  #define TOUCH_I2C_SDA           33
  #define TOUCH_I2C_SCL           32
  #define TOUCH_INT               -1    // not used
  #define TOUCH_RST               25
  #define TOUCH_BUS_SHARED        false

  #define LDR_PIN                 34
  #define LDR_RESOLUTION          12
  #define SPEAKER_PIN             26

  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

//------------------------------------------------
#elif defined BEENEXT_3_5
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 3.5\" Touch (resistive)"
  #define BLYNKGO_BASIC
  #define BEENEXT
  // #define TFT_3_5_INCH_TOUCH
  #define OFFSET_ROTATION         0
  #define ROTATE_TYPE             3

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 3.5"
  #define ST7796_DRIVER           // TFT 3.5"
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  27 // 21 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1
  
  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select

  #define TOUCH_XPT2046
  #define TOUCH_HOST_TYPE         SPI_HOST_TYPE
  #define TOUCH_INT               36
  #define TOUCH_MOSI              SPI_MOSI // 13 <-- 32
  #define TOUCH_MISO              SPI_MISO // 12 <-- 39
  #define TOUCH_SCLK              SPI_SCLK // 14 <-- 25
  #define TOUCH_CS                33

  #define LDR_PIN                 34
  #define LDR_RESOLUTION          12
  #define SPEAKER_PIN             26

  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

#elif defined BEENEXT_3_5C
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 3.5\" Touch (capacitive)"
  #define BLYNKGO_BASIC
  #define BEENEXT
  #define OFFSET_ROTATION         0
  #define ROTATE_TYPE             3

  // HSPI MISO12, MOSI13, SCK14, SS15;
  // VSPI MISO19, MOSI23, SCK18, SS5;
  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_HOST or VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                13
  #define SPI_SCLK                14

  // TFT 3.5"
  #define ST7796_DRIVER           // TFT 3.5"
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define TFT_SPI_HOST_TYPE       SPI_HOST_TYPE
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15
  #define TFT_DC                  2
  #define TFT_RST                 -1
  #define TFT_BUSY                -1
  #define TFT_BL                  27 // 2.8 : 21 
  #define TFT_BL_INVERT           false
  #define TFT_INVERT              false
  #define TFT_SPI_3WIRE           false
  #define TFT_DUMMY_READ_PIXEL    8
  #define TFT_DUMMY_READ_BITS     1
  
  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE
  #define SD_SCLK                 18
  #define SD_MISO                 19
  #define SD_MOSI                 23
  #define SD_CS                   5       // SD Chip Select

  // capacitive touch i2c
  #define TOUCH_GT911_TAMC        // ใช้ได้
  // #define TOUCH_GT911           // ไม่ทำงาน
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x5D  // 0x14
  #define TOUCH_I2C_SDA           33
  #define TOUCH_I2C_SCL           32
  #define TOUCH_INT               21    // not used
  #define TOUCH_RST               25
  #define TOUCH_BUS_SHARED        false

  #define LDR_PIN                 34
  #define LDR_RESOLUTION          12
  #define SPEAKER_PIN             26

  #define LED_R                   4
  #define LED_G                   16
  #define LED_B                   17

#elif defined BEENEXT_4_3
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 4.3\" Touch (resistive)"
  #define BEENEXT
  #define ROTATE_TYPE             0 //3
  #define OFFSET_ROTATION         0
  #define BLYNKGO_USE_SD          1

  #define RGB_DRIVER
  #define TFT_RGB_BUS
  #define TFT_WIDTH               480
  #define TFT_HEIGHT              272
  #define TFT_BL                  2
  // #define TFT_B0                  8
  // #define TFT_B1                  3
  // #define TFT_B2                  46
  // #define TFT_B3                  9
  // #define TFT_B4                  1
  // #define TFT_G0                  5
  // #define TFT_G1                  6
  // #define TFT_G2                  7
  // #define TFT_G3                  15
  // #define TFT_G4                  16
  // #define TFT_G5                  4
  // #define TFT_R0                  45
  // #define TFT_R1                  48
  // #define TFT_R2                  47
  // #define TFT_R3                  21
  // #define TFT_R4                  14

  // #define TFT_HENABLE             40
  // #define TFT_VSYNC               41
  // #define TFT_HSYNC               39
  // #define TFT_PCLK                42

  #define TFT_B0                  GPIO_NUM_8
  #define TFT_B1                  GPIO_NUM_3
  #define TFT_B2                  GPIO_NUM_46
  #define TFT_B3                  GPIO_NUM_9
  #define TFT_B4                  GPIO_NUM_1
  #define TFT_G0                  GPIO_NUM_5
  #define TFT_G1                  GPIO_NUM_6
  #define TFT_G2                  GPIO_NUM_7
  #define TFT_G3                  GPIO_NUM_15
  #define TFT_G4                  GPIO_NUM_16
  #define TFT_G5                  GPIO_NUM_4
  #define TFT_R0                  GPIO_NUM_45
  #define TFT_R1                  GPIO_NUM_48
  #define TFT_R2                  GPIO_NUM_47
  #define TFT_R3                  GPIO_NUM_21
  #define TFT_R4                  GPIO_NUM_14

  #define TFT_HENABLE             GPIO_NUM_40
  #define TFT_VSYNC               GPIO_NUM_41
  #define TFT_HSYNC               GPIO_NUM_39
  #define TFT_PCLK                GPIO_NUM_42

  #define TFT_HSYNC_POLARITY      0
  #define TFT_HSYNC_FRONT_PORCH   8
  #define TFT_HSYNC_PULSE_WIDTH   4
  #define TFT_HSYNC_BACK_PORCH    16
  #define TFT_VSYNC_POLARITY      0
  #define TFT_VSYNC_FRONT_PORCH   4
  #define TFT_VSYNC_PULSE_WIDTH   4
  #define TFT_VSYNC_BACK_PORCH    4
  #define TFT_PCLK_IDLE_HIGH      1

  #define SD_FSPI_MODE
  #define SD_CS                   10       // SD Chip Select
  #define SD_MOSI                 11
  #define SD_SCLK                 12
  #define SD_MISO                 13

  #define TOUCH_XPT2046
  #define TOUCH_HOST_TYPE         SPI2_HOST // SPI0_HOST, SPI1_HOST, SPI2_HOST,-1 //FSPI_HOST
  #define TOUCH_CS                38
  #define TOUCH_MOSI              11
  #define TOUCH_SCLK              12
  #define TOUCH_MISO              13
  #define TOUCH_INT               -1 // -1 not use :(18)

#elif defined BEENEXT_4_3C
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 4.3\" Touch (capacitive)"
  #define BEENEXT
  #define ROTATE_TYPE             0 //3
  #define OFFSET_ROTATION         0
  #define BLYNKGO_USE_SD          1
  #define BLYNKGO_USE_AGFX        1

  #define RGB_DRIVER
  #define TFT_RGB_BUS
  #define TFT_WIDTH               480
  #define TFT_HEIGHT              272
  #define TFT_BL                  2
  #define TFT_B0                  GPIO_NUM_8
  #define TFT_B1                  GPIO_NUM_3
  #define TFT_B2                  GPIO_NUM_46
  #define TFT_B3                  GPIO_NUM_9
  #define TFT_B4                  GPIO_NUM_1
  #define TFT_G0                  GPIO_NUM_5
  #define TFT_G1                  GPIO_NUM_6
  #define TFT_G2                  GPIO_NUM_7
  #define TFT_G3                  GPIO_NUM_15
  #define TFT_G4                  GPIO_NUM_16
  #define TFT_G5                  GPIO_NUM_4
  #define TFT_R0                  GPIO_NUM_45
  #define TFT_R1                  GPIO_NUM_48
  #define TFT_R2                  GPIO_NUM_47
  #define TFT_R3                  GPIO_NUM_21
  #define TFT_R4                  GPIO_NUM_14

  #define TFT_HENABLE             GPIO_NUM_40  // 0 ไม่แสดง
  #define TFT_VSYNC               GPIO_NUM_41
  #define TFT_HSYNC               GPIO_NUM_39
  #define TFT_PCLK                GPIO_NUM_42
  
  #define TFT_HSYNC_POLARITY      0
  #define TFT_HSYNC_FRONT_PORCH   8
  #define TFT_HSYNC_PULSE_WIDTH   4
  #define TFT_HSYNC_BACK_PORCH    16
  #define TFT_VSYNC_POLARITY      0
  #define TFT_VSYNC_FRONT_PORCH   4
  #define TFT_VSYNC_PULSE_WIDTH   4
  #define TFT_VSYNC_BACK_PORCH    4
  #define TFT_PCLK_IDLE_HIGH      1
  #define TFT_PREFER_SPEED        16000000

  #define SD_FSPI_MODE
  #define SD_CS                   10       // SD Chip Select
  #define SD_MOSI                 11
  #define SD_SCLK                 12
  #define SD_MISO                 13

  // #define TOUCH_GT911
  #define TOUCH_GT911_TAMC
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x5D
  #define TOUCH_I2C_SDA           19
  #define TOUCH_I2C_SCL           20
  #define TOUCH_INT               -1
  #define TOUCH_RST               38

#elif defined BEENEXT_4_3IPS
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 4.3\" IPS Touch (capacitive)"
  #define BEENEXT
  #define ROTATE_TYPE             0 //3
  #define OFFSET_ROTATION         0
  #define BLYNKGO_USE_SD          1
  #define BLYNKGO_USE_AGFX        1

  #define RGB_DRIVER
  #define TFT_RGB_BUS
  #define TFT_WIDTH               800
  #define TFT_HEIGHT              480
  #define TFT_BL                  2
  #define TFT_B0                  GPIO_NUM_8
  #define TFT_B1                  GPIO_NUM_3
  #define TFT_B2                  GPIO_NUM_46
  #define TFT_B3                  GPIO_NUM_9
  #define TFT_B4                  GPIO_NUM_1
  #define TFT_G0                  GPIO_NUM_5
  #define TFT_G1                  GPIO_NUM_6
  #define TFT_G2                  GPIO_NUM_7
  #define TFT_G3                  GPIO_NUM_15
  #define TFT_G4                  GPIO_NUM_16
  #define TFT_G5                  GPIO_NUM_4
  #define TFT_R0                  GPIO_NUM_45
  #define TFT_R1                  GPIO_NUM_48
  #define TFT_R2                  GPIO_NUM_47
  #define TFT_R3                  GPIO_NUM_21
  #define TFT_R4                  GPIO_NUM_14

  #define TFT_HENABLE             0 //GPIO_NUM_40
  #define TFT_VSYNC               GPIO_NUM_41
  #define TFT_HSYNC               GPIO_NUM_39
  #define TFT_PCLK                GPIO_NUM_42
  
  #define TFT_HSYNC_POLARITY      0
  #define TFT_HSYNC_FRONT_PORCH   8
  #define TFT_HSYNC_PULSE_WIDTH   4
  #define TFT_HSYNC_BACK_PORCH    8
  #define TFT_VSYNC_POLARITY      0
  #define TFT_VSYNC_FRONT_PORCH   8
  #define TFT_VSYNC_PULSE_WIDTH   4
  #define TFT_VSYNC_BACK_PORCH    8
  #define TFT_PCLK_IDLE_HIGH      1
  #define TFT_PREFER_SPEED        16000000


  #define SD_FSPI_MODE
  #define SD_CS                   10       // SD Chip Select
  #define SD_MOSI                 11
  #define SD_SCLK                 12
  #define SD_MISO                 13

  // #define TOUCH_GT911
  #define TOUCH_GT911_TAMC
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x5D
  #define TOUCH_I2C_SDA           19
  #define TOUCH_I2C_SCL           20
  #define TOUCH_INT               -1
  #define TOUCH_RST               38

#elif defined BEENEXT_5_0IPS
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 5.0\" IPS Touch (capacitive)"
  #define BEENEXT
  #define ROTATE_TYPE             0 //3
  #define OFFSET_ROTATION         0
  #define BLYNKGO_USE_SD          1
  #define BLYNKGO_USE_AGFX        1

  //------------------------------------------
  #define RGB_DRIVER
  #define TFT_RGB_BUS
  #define TFT_WIDTH               800
  #define TFT_HEIGHT              480
  #define TFT_BL                  2
  #define TFT_B0                  GPIO_NUM_8
  #define TFT_B1                  GPIO_NUM_3
  #define TFT_B2                  GPIO_NUM_46
  #define TFT_B3                  GPIO_NUM_9
  #define TFT_B4                  GPIO_NUM_1
  #define TFT_G0                  GPIO_NUM_5
  #define TFT_G1                  GPIO_NUM_6
  #define TFT_G2                  GPIO_NUM_7
  #define TFT_G3                  GPIO_NUM_15
  #define TFT_G4                  GPIO_NUM_16
  #define TFT_G5                  GPIO_NUM_4
  #define TFT_R0                  GPIO_NUM_45
  #define TFT_R1                  GPIO_NUM_48
  #define TFT_R2                  GPIO_NUM_47
  #define TFT_R3                  GPIO_NUM_21
  #define TFT_R4                  GPIO_NUM_14

  #define TFT_HENABLE             GPIO_NUM_40   /* DE */
  #define TFT_VSYNC               GPIO_NUM_41
  #define TFT_HSYNC               GPIO_NUM_39
  #define TFT_PCLK                GPIO_NUM_42

  #define TFT_HSYNC_POLARITY      0
  #define TFT_HSYNC_FRONT_PORCH   80
  #define TFT_HSYNC_PULSE_WIDTH   4
  #define TFT_HSYNC_BACK_PORCH    16
  #define TFT_VSYNC_POLARITY      0
  #define TFT_VSYNC_FRONT_PORCH   22
  #define TFT_VSYNC_PULSE_WIDTH   4
  #define TFT_VSYNC_BACK_PORCH    4
  #define TFT_PCLK_IDLE_HIGH      1         /* PCLK_ACTIVE_NEG */
  #define TFT_PREFER_SPEED        14000000

  //------------------------------------------  
  #define SD_FSPI_MODE
  #define SD_CS                   10       // SD Chip Select
  #define SD_MOSI                 11
  #define SD_SCLK                 12
  #define SD_MISO                 13

  //------------------------------------------
  #define TOUCH_GT911_TAMC
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)  Wire1 สัมผัส
  #define TOUCH_I2C_ADDR          0x5D
  #define TOUCH_I2C_SDA           19
  #define TOUCH_I2C_SCL           20
  #define TOUCH_INT               -1
  #define TOUCH_RST               38

#elif defined BEENEXT_7_0IPS
  #define BLYNKGO_BOARD_NAME      "BeeNeXT 7.0\" IPS Touch (capacitive)"
  #define BEENEXT
  #define ROTATE_TYPE             0 //3
  #define OFFSET_ROTATION         0
  #define BLYNKGO_USE_SD          0
  #define BLYNKGO_USE_AGFX        1

  #define RGB_DRIVER
  #define TFT_RGB_BUS
  #define TFT_WIDTH               800
  #define TFT_HEIGHT              480
  #define TFT_BL                  2
  //--------------------------------------------
  #define TFT_R0                  GPIO_NUM_14
  #define TFT_R1                  GPIO_NUM_21
  #define TFT_R2                  GPIO_NUM_47
  #define TFT_R3                  GPIO_NUM_48
  #define TFT_R4                  GPIO_NUM_45
  //--------------------------------------------
  #define TFT_G0                  GPIO_NUM_9
  #define TFT_G1                  GPIO_NUM_46
  #define TFT_G2                  GPIO_NUM_3
  #define TFT_G3                  GPIO_NUM_8
  #define TFT_G4                  GPIO_NUM_16
  #define TFT_G5                  GPIO_NUM_1
  //--------------------------------------------
  #define TFT_B0                  GPIO_NUM_15
  #define TFT_B1                  GPIO_NUM_7
  #define TFT_B2                  GPIO_NUM_6
  #define TFT_B3                  GPIO_NUM_5
  #define TFT_B4                  GPIO_NUM_4
  //--------------------------------------------
  
  #define TFT_HENABLE             GPIO_NUM_41
  #define TFT_VSYNC               GPIO_NUM_40
  #define TFT_HSYNC               GPIO_NUM_39
  #define TFT_PCLK                GPIO_NUM_42

  //--------------------------------------------
  #define TFT_HSYNC_POLARITY      0
  #define TFT_HSYNC_FRONT_PORCH   210
  #define TFT_HSYNC_PULSE_WIDTH   30
  #define TFT_HSYNC_BACK_PORCH    16
  #define TFT_VSYNC_POLARITY      0
  #define TFT_VSYNC_FRONT_PORCH   22
  #define TFT_VSYNC_PULSE_WIDTH   13
  #define TFT_VSYNC_BACK_PORCH    10
  #define TFT_PCLK_IDLE_HIGH      1
  #define TFT_PREFER_SPEED        16000000
  //--------------------------------------------
  
  #define SD_FSPI_MODE
  #define SD_CS                   10       // SD Chip Select
  #define SD_MOSI                 11
  #define SD_SCLK                 12
  #define SD_MISO                 13

   #define TOUCH_GT911_TAMC
//   #define TOUCH_GT911
  
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x5D
  #define TOUCH_I2C_SDA           19
  #define TOUCH_I2C_SCL           20
  #define TOUCH_INT               -1
  #define TOUCH_RST               38
  #define TOUCH_BUS_SHARED        false


#endif

//------------------------------------------------
#if defined BLYNKGO_EMMA_PRO

  #define BLYNKGO_BOARD_NAME      "BlynkGO EMMA-PRO 485 (ESP32S3) TFT 3.5\" IPS capacitive"
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         1
  #define BLYNKGO_USE_SD          1

  // TFT 3.5
  #define ST7796_DRIVER     // TFT 3.5"  
  #define TFT_PARALLEL8
  #define TFT_IPS
  #define TFT_WIDTH               320
  #define TFT_HEIGHT              480
  #define TFT_CS                  -1  // not defined
  #define TFT_DC                  0
  #define TFT_RST                 4
  #define TFT_BUSY                -1
  #define TFT_BL                  45
  #define TFT_INVERT              true

  #define TFT_PIN_WR              47  // Write clock
  #define TFT_TFT_RD              -1  // not defined
  #define TFT_TFT_RS              0   // [???] Command/Data selectio
  #define TFT_PIN_TE              48  // [???] Frame sync
  
  #define TFT_PIN_D0              9
  #define TFT_PIN_D1              46
  #define TFT_PIN_D2              3
  #define TFT_PIN_D3              8
  #define TFT_PIN_D4              18
  #define TFT_PIN_D5              17
  #define TFT_PIN_D6              16
  #define TFT_PIN_D7              15

  // SD
  // HSPI MISO12, MOSI13, SCK14, SS15;
//  #define SPI_HOST_TYPE           HSPI_HOST  // HSPI_TYPE or VSPI_TYPE;
  #define SD_VSPI_MODE
  #define SD_MISO                 41
  #define SD_MOSI                 2
  #define SD_SCLK                 42
  #define SD_CS                   1       // SD Chip Select

  // capacitive touch i2c  // 
  #define TOUCH_FT6336U
  #define TOUCH_I2C_ADDR          0x38
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_SDA           6
  #define TOUCH_I2C_SCL           5
  #define TOUCH_I2C_RESET         4    // ???
  #define TOUCH_INT               7

#endif

//--------------------------------------------------------------------
#if defined WEMOS_D32_PRO_TFT_2_4_INCH    || defined LOLIN_D32_PRO_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 Pro + TFT Touch 2.4\""
  #define WEMOS_D32_PRO
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined WEMOS_D32_PRO_TFT_2_8_INCH  || defined LOLIN_D32_PRO_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 Pro + TFT Touch 2.8\""
  #define WEMOS_D32_PRO
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined WEMOS_D32_PRO_TFT_3_2_INCH  || defined LOLIN_D32_PRO_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 Pro + TFT Touch 3.2\""
  #define WEMOS_D32_PRO
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined WEMOS_D32_PRO_TFT_3_5_INCH  || defined LOLIN_D32_PRO_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 Pro + TFT Touch 3.5\""
  #define WEMOS_D32_PRO
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined WEMOS_D32_PRO_TFT_4_0_INCH  || defined LOLIN_D32_PRO_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 Pro + TFT Touch 4.0\""
  #define WEMOS_D32_PRO
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

#elif defined WEMOS_D32_PRO_TFT_5_0_INCH  || defined LOLIN_D32_PRO_TFT_5_0_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 Pro + TFT Touch 5.0\""
  #define TFT_5_0_INCH_TOUCH
  #define RA8875_DRIVER      // TFT 5.0"
  #define ROTATE_TYPE             0

  // #define TFT_MOSI                23  // SPI MOSI
  // #define TFT_MISO                19  // SPI MISO
  // #define TFT_SCLK                18  // SPI SCK
  // #define TFT_CS                  5
  // #define TFT_RST                 2

  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  14      // Chip select control pin
  #define TFT_RST                 33
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_BL                  32       // TFT backlight  pin
  #define TFT_INVERT              false

  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

  #define CTP_I2C_SDA             21  // I2C SDA
  #define CTP_I2C_SCL             22  // I2C SCL
  #define CTP_WAKE                14
  #define CTP_INTERRUPT           12

#elif defined LOLIN_D32_PRO_OLED_SSD1306
  #define BLYNKGO_BOARD_NAME      "Lolin D32Pro OLED (SSD1306)"
  #define BLYNKGO_OLED_SSD1306
  #define BLYNKGO_OLED
  #define OLED_SDA                21
  #define OLED_SCL                22

  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select


#elif defined LOLIN_D32_PRO_OLED_SH1106
  #define BLYNKGO_BOARD_NAME     "Lolin D32Pro OLED (SH1106)"
  #define BLYNKGO_OLED_SH1106
  #define BLYNKGO_OLED
  #define OLED_SDA                21
  #define OLED_SCL                22

  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

//----------------------------------------------------------

#elif defined(ESP32_WROOM_ADAFRUIT_TFT_2_8_CAPACITIVE)
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM + Adafruit TFT 2.8 Capacitive\""
  #define ADAFRUIT_TFT_2_8_CAPACITIVE
  #define BLYNKGO_BASIC

#elif defined(ESP32_WROVER_ADAFRUIT_TFT_2_8_CAPACITIVE)
  #define BLYNKGO_BOARD_NAME      "ESP32 WROVER + Adafruit TFT 2.8 Capacitive\""
  #define ADAFRUIT_TFT_2_8_CAPACITIVE

//----------------------------------------------------------
#elif defined ESP32_WROVER_TFT_2_4_INCH 
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM + TFT Touch 2.4\""
  #define ESP32_CUSTOMIZE
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32_WROVER_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM + TFT Touch 2.8\""
  #define ESP32_CUSTOMIZE
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32_WROVER_TFT_3_2_INCH 
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM + TFT Touch 3.2\""
  #define ESP32_CUSTOMIZE
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32_WROVER_TFT_3_5_INCH 
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM + TFT Touch 3.5\""
  #define ESP32_CUSTOMIZE
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER     // TFT 3.5"

#elif defined ESP32_WROVER_TFT_4_0_INCH 
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM + TFT Touch 4.0\""
  #define ESP32_CUSTOMIZE
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------
#elif defined BLYNKGO_ESP32DEVBOARD_OLED_SSD1306
  #define BLYNKGO_BOARD_NAME      "BlynkGO ESP32 DevBoard + OLED (SSD1306)"
  #define BLYNKGO_OLED_SSD1306
  #define BLYNKGO_OLED

#elif defined BLYNKGO_ESP32DEVBOARD_OLED_SH1106
  #define BLYNKGO_BOARD_NAME      "BlynkGO ESP32 DevBoard + OLED (SH1106)"
  #define BLYNKGO_OLED_SH1106
  #define BLYNKGO_OLED

#elif defined BLYNKGO_ESP32DEVBOARD_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "BlynkGO ESP32 DevBoard + TFT Touch 2.4\""
  #define ESP32S_WROOM
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined BLYNKGO_ESP32DEVBOARD_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "BlynkGO ESP32 DevBoard + TFT Touch 2.8\""
  #define ESP32S_WROOM
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined BLYNKGO_ESP32DEVBOARD_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "BlynkGO ESP32 DevBoard + TFT Touch 3.2\""
  #define ESP32S_WROOM
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined BLYNKGO_ESP32DEVBOARD_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "BlynkGO ESP32 DevBoard + TFT Touch 3.5\""
  #define ESP32S_WROOM
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined BLYNKGO_ESP32DEVBOARD_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "BlynkGO ESP32 DevBoard + TFT Touch 4.0\""
  #define ESP32S_WROOM
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"


//----------------------------------------------------------

#elif defined LOLIN_D32_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 + TFT Touch 2.4\""
  #define LOLIN_D32
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined LOLIN_D32_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 + TFT Touch 2.8\""
  #define LOLIN_D32
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined LOLIN_D32_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 + TFT Touch 3.2\""
  #define LOLIN_D32
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined LOLIN_D32_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 + TFT Touch 3.5\""
  #define LOLIN_D32
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined LOLIN_D32_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 + TFT Touch 4.0\""
  #define LOLIN_D32
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

#elif defined LOLIN_D32_OLED_SSD1306
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 + OLED (SSD1306)"
  #define BLYNKGO_OLED_SSD1306
  #define BLYNKGO_OLED

#elif defined LOLIN_D32_OLED_SH1106
  #define BLYNKGO_BOARD_NAME      "LOLIN D32 + OLED (SH1106)"
  #define BLYNKGO_OLED_SH1106
  #define BLYNKGO_OLED

//----------------------------------------------------------
/* ESP32 WROOM DEVKIT (38ขา)*/
#elif defined ESP32_WROOM_DEVKIT_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM DEVKIT + TFT Touch 2.4\""
  #define ESP32_WROOM_DEVKIT
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32_WROOM_DEVKIT_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM DEVKIT + TFT Touch 2.8\""
  #define ESP32_WROOM_DEVKIT
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32_WROOM_DEVKIT_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM DEVKIT + TFT Touch 3.2\""
  #define ESP32_WROOM_DEVKIT
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32_WROOM_DEVKIT_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM DEVKIT + TFT Touch 3.5\""
  #define ESP32_WROOM_DEVKIT
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined ESP32_WROOM_DEVKIT_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM DEVKIT + TFT Touch 4.0\""
  #define ESP32_WROOM_DEVKIT
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

#elif defined ESP32_WROOM_DEVKIT_TFT_5_0_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 WROOM DEVKIT + TFT Touch 5.0\""
  #define BLYNKGO_BASIC
  #define TFT_5_0_INCH_TOUCH
  #define RA8875_DRIVER      // TFT 5.0"
  #define ROTATE_TYPE             0
  #define BLYNKGO_USE_SD          0

  #define TFT_MOSI                23  // SPI MOSI
  #define TFT_MISO                19  // SPI MISO
  #define TFT_SCLK                18  // SPI SCK
  #define TFT_CS                  5
  #define TFT_RST                 4
  #define TFT_BL                  2

  #define CTP_I2C_SDA             21  // I2C SDA
  #define CTP_I2C_SCL             22  // I2C SCL
  #define CTP_WAKE                14
  #define CTP_INTERRUPT           12
#elif defined BLYNKGO_ENTERPRISE_4_3_INCH
  #define BLYNKGO_BOARD_NAME      "BlynkGO Enterprise 4.3\" Touch RS485"
  #define TFT_4_3_INCH_TOUCH_485
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         0

  #define ST7701_DRIVER
  #define TFT_CS                  -1
  #define TFT_SCK                 -1
  #define TFT_SDA                 -1
  #define TFT_DE                  40
  #define TFT_VSYNC               38
  #define TFT_HSYNC               47
  #define TFT_PCLK                14
  #define TFT_R0                  4
  #define TFT_R1                  5
  #define TFT_R2                  6
  #define TFT_R3                  7
  #define TFT_R4                  15
  #define TFT_G0                  16
  #define TFT_G1                  17
  #define TFT_G2                  18
  #define TFT_G3                  8
  #define TFT_G4                  3
  #define TFT_G5                  46
  #define TFT_B0                  9
  #define TFT_B1                  10
  #define TFT_B2                  11
  #define TFT_B3                  12
  #define TFT_B4                  13
  #define TFT_BL                  45


#elif defined BLYNKGO_ENTERPRISE_5_0_INCH
  // สำหรับ ต่อกับสาย jst
  #define BLYNKGO_BOARD_NAME      "BlynkGO Enterprise 5.0\" Capacitive Touch"
  #define BLYNKGO_ENTERPRISE
  #define TFT_5_0_INCH_TOUCH
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         0

  #define RA8875_DRIVER      // TFT 5.0"
  #define TFT_SPI_HOST_TYPE       VSPI_HOST
  #define TFT_WIDTH               800
  #define TFT_HEIGHT              480
  #define TFT_MOSI                23  // SPI MOSI
  #define TFT_MISO                19  // SPI MISO
  #define TFT_SCLK                18  // SPI SCK
  #define TFT_CS                  27
  #define TFT_RST                 33
  #define TFT_BUSY                -1
  #define TFT_BL                  32
  #define TFT_DUMMY_READ_PIXEL    16  // 8 or 16 OK
  #define TFT_DUMMY_READ_BITS     0   // 0 or 1 OK
  #define TFT_SPI_3WIRE           false

  
  #define BLYNKGO_USE_SD          1   // conflict with TFT
  #define SD_VSPI_MODE
  #define SD_MOSI                 23
  #define SD_MISO                 19
  #define SD_SCLK                 18
  #define SD_CS                   4       // SD Chip Select

  #define TOUCH_GSL1680F
  #define TOUCH_INT               -1      // INTが接続されているピン番号
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x40    // I2Cデバイスアドレス番号
  #define TOUCH_I2C_SDA           21      // I2C SDA
  #define TOUCH_I2C_SCL           22      // I2C SCL

#elif defined BLYNKGO_ENTERPRISE_5_0_INCH_OLD
  #define BLYNKGO_BOARD_NAME      "BlynkGO Enterprise 5.0\" Capacitive Touch"
  #define BLYNKGO_ENTERPRISE
  #define TFT_5_0_INCH_TOUCH
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         0

  #define RA8875_DRIVER      // TFT 5.0"
  #define TFT_WIDTH               800
  #define TFT_HEIGHT              480
  #define TFT_MOSI                23  // SPI MOSI
  #define TFT_MISO                19  // SPI MISO
  #define TFT_SCLK                18  // SPI SCK
  #define TFT_CS                  5
  #define TFT_RST                 33
  #define TFT_BUSY                27
  #define TFT_BL                  32
  #define TFT_DUMMY_READ_PIXEL    16  // 8 or 16 OK
  #define TFT_DUMMY_READ_BITS     0   // 0 or 1 OK
  #define TFT_SPI_3WIRE           false
  
  #define BLYNKGO_USE_SD          1   // conflict with TFT
  #define SD_VSPI_MODE
  #define SD_MOSI                 23
  #define SD_MISO                 19
  #define SD_SCLK                 18
  #define SD_CS                   4       // SD Chip Select

  #define TOUCH_GSL1680F
  #define TOUCH_INT               -1      // INTが接続されているピン番号
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x40    // I2Cデバイスアドレス番号
  #define TOUCH_I2C_SDA           21      // I2C SDA
  #define TOUCH_I2C_SCL           22      // I2C SCL

#elif defined BLYNKGO_ENTERPRISE_7_0_INCH
  #define BLYNKGO_BOARD_NAME      "BlynkGO Enterprise 7.0\" Capacitive Touch"
  #define BLYNKGO_ENTERPRISE
  #define TFT_7_0_INCH_TOUCH
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         0

  #define RA8875_DRIVER      // TFT 7.0"
  #define TFT_SPI_HOST_TYPE       VSPI_HOST
  #define TFT_WIDTH               800
  #define TFT_HEIGHT              480
  #define TFT_MOSI                23  // SPI MOSI
  #define TFT_MISO                19  // SPI MISO
  #define TFT_SCLK                18  // SPI SCK
  #define TFT_CS                  27
  #define TFT_RST                 33
  #define TFT_BUSY                5
  #define TFT_BL                  32
  #define TFT_DUMMY_READ_PIXEL    16  // 8 or 16 OK
  #define TFT_DUMMY_READ_BITS     0   // 0 or 1 OK
  #define TFT_SPI_3WIRE           false
  
  #define BLYNKGO_USE_SD          1   // conflict with TFT
  #define SD_VSPI_MODE
  #define SD_MOSI                 23
  #define SD_MISO                 19
  #define SD_SCLK                 18
  #define SD_CS                   4       // SD Chip Select

  #define TOUCH_FT5206
  #define TOUCH_INT               -1      // INTが接続されているピン番号
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x38    // I2Cデバイスアドレス番号
  #define TOUCH_I2C_SDA           21      // I2C SDA
  #define TOUCH_I2C_SCL           22      // I2C SCL

#elif defined BLYNKGO_ENTERPRISE_7_0_INCH_OLD
  #define BLYNKGO_BOARD_NAME      "BlynkGO Enterprise 7.0\" Capacitive Touch"
  #define BLYNKGO_ENTERPRISE
  #define TFT_7_0_INCH_TOUCH
  #define ROTATE_TYPE             0
  #define OFFSET_ROTATION         0


  #define RA8875_DRIVER      // TFT 7.0"
  #define TFT_WIDTH               800
  #define TFT_HEIGHT              480
  #define TFT_MOSI                23  // SPI MOSI
  #define TFT_MISO                19  // SPI MISO
  #define TFT_SCLK                18  // SPI SCK
  #define TFT_CS                  5
  #define TFT_RST                 33
  #define TFT_BUSY                27
  #define TFT_BL                  32
  #define TFT_DUMMY_READ_PIXEL    16  // 8 or 16 OK
  #define TFT_DUMMY_READ_BITS     0   // 0 or 1 OK
  #define TFT_SPI_3WIRE           false
  
  #define BLYNKGO_USE_SD          1   // conflict with TFT
  #define SD_VSPI_MODE
  #define SD_MOSI                 23
  #define SD_MISO                 19
  #define SD_SCLK                 18
  #define SD_CS                   4       // SD Chip Select

  #define TOUCH_FT5206
  #define TOUCH_INT               -1      // INTが接続されているピン番号
  #define TOUCH_I2C_PORT          1       // 使用するI2Cを選択 (0 or 1)
  #define TOUCH_I2C_ADDR          0x38    // I2Cデバイスアドレス番号
  #define TOUCH_I2C_SDA           21      // I2C SDA
  #define TOUCH_I2C_SCL           22      // I2C SCL


//----------------------------------------------------------

#elif defined GEEKCREIT_ESP32_WROVER_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "GeekCreit ESP32 WROVER + TFT Touch 2.4\""
  #define GEEKCREIT_ESP32_WROVER
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined GEEKCREIT_ESP32_WROVER_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "GeekCreit ESP32 WROVER + TFT Touch 2.8\""
  #define GEEKCREIT_ESP32_WROVER
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined GEEKCREIT_ESP32_WROVER_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "GeekCreit ESP32 WROVER + TFT Touch 3.2\""
  #define GEEKCREIT_ESP32_WROVER
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined GEEKCREIT_ESP32_WROVER_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "GeekCreit ESP32 WROVER + TFT Touch 3.5\""
  #define GEEKCREIT_ESP32_WROVER
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined GEEKCREIT_ESP32_WROVER_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "GeekCreit ESP32 WROVER + TFT Touch 4.0\""
  #define GEEKCREIT_ESP32_WROVER
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------
/* IOX ESP32 (ESP32 WROOM) */
#elif defined IOX_ESP32_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32 WROOM + TFT Touch 2.4\""
  #define IOX_ESP32
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined IOX_ESP32_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32 WROOM + TFT Touch 2.8\""
  #define IOX_ESP32
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined IOX_ESP32_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32 WROOM + TFT Touch 3.2\""
  #define IOX_ESP32
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined IOX_ESP32_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32 WROOM + TFT Touch 3.5\""
  #define IOX_ESP32
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined IOX_ESP32_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32 WROOM + TFT Touch 4.0\""
  #define IOX_ESP32
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------
/* IOX ESP32PS (FLASH 4MB PSRAM 4MB) */
#elif defined IOX_ESP32PS_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32PS + TFT Touch 2.4\""
  #define IOX_ESP32PS
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined IOX_ESP32PS_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32PS + TFT Touch 2.8\""
  #define IOX_ESP32PS
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined IOX_ESP32PS_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32PS + TFT Touch 3.2\""
  #define IOX_ESP32PS
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined IOX_ESP32PS_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32PS + TFT Touch 3.5\""
  #define IOX_ESP32PS
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined IOX_ESP32PS_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "IOX ESP32PS + TFT Touch 4.0\""
  #define IOX_ESP32PS
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------
/* ESP32 Wroom Adapter (FLASH 4/16MB PSRAM 4/8MB) */ 
#elif defined ESP32WROVER_ADAPTER_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wroom Adapter + TFT Touch 2.4\""
  #define ESP32WROOM_ADAPTER
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32WROVER_ADAPTER_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wroom Adapter + TFT Touch 2.8\""
  #define ESP32WROOM_ADAPTER
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32WROVER_ADAPTER_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wroom Adapter + TFT Touch 3.2\""
  #define ESP32WROOM_ADAPTER
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32WROVER_ADAPTER_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wroom Adapter + TFT Touch 3.5\""
  #define ESP32WROOM_ADAPTER
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined ESP32WROVER_ADAPTER_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wroom Adapter + TFT Touch 4.0\""
  #define ESP32WROOM_ADAPTER
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------
/* ESP32 Wrover Adapter (FLASH 4/16MB PSRAM 4/8MB) */  //  https://www.elecrow.com/esp32-wrover-burning-testing-mini-system-development-board-for-esp-wroom-32-esp-32s-esp32-wrover.html
#elif defined ESP32WROVER_ADAPTER_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wrover Adapter + TFT Touch 2.4\""
  #define ESP32WROVER_ADAPTER
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32WROVER_ADAPTER_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wrover Adapter + TFT Touch 2.8\""
  #define ESP32WROVER_ADAPTER
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32WROVER_ADAPTER_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wrover Adapter + TFT Touch 3.2\""
  #define ESP32WROVER_ADAPTER
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESP32WROVER_ADAPTER_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wrover Adapter + TFT Touch 3.5\""
  #define ESP32WROVER_ADAPTER
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined ESP32WROVER_ADAPTER_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "ESP32 Wrover Adapter + TFT Touch 4.0\""
  #define ESP32WROVER_ADAPTER
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------
/* ThaiEasyElect ESPino32 (ESP32 WROOM) */
#elif defined ESPINO32_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "ESPino32 + TFT Touch 2.4\""
  #define ESPINO32
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESPINO32_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "ESPino32 + TFT Touch 2.8\""
  #define ESPINO32
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESPINO32_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "ESPino32 + TFT Touch 3.2\""
  #define ESPINO32
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined ESPINO32_TFT_3_5_INCH
  #define BLYNKGO_BOARD_NAME      "ESPino32 + TFT Touch 3.5\""
  #define ESPINO32
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined ESPINO32_TFT_4_0_INCH
  #define BLYNKGO_BOARD_NAME      "ESPino32 + TFT Touch 4.0\""
  #define ESPINO32
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------
#elif defined AI_ESP32A1S_AUDIO_KIT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "AI ESP32A1S AUDIO KIT + TFT Touch 2.4\""
  #define AI_ESP32A1S_AUDIO_KIT
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined AI_ESP32A1S_AUDIO_KIT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "AI ESP32A1S AUDIO KIT + TFT Touch 2.8\""
  #define AI_ESP32A1S_AUDIO_KIT
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined AI_ESP32A1S_AUDIO_KIT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "AI ESP32A1S AUDIO KIT + TFT Touch 3.2\""
  #define AI_ESP32A1S_AUDIO_KIT
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined (AI_ESP32A1S_AUDIO_KIT_3_5_INCH)
  #define BLYNKGO_BOARD_NAME      "AI ESP32A1S AUDIO KIT + TFT Touch 3.5\""
  #define AI_ESP32A1S_AUDIO_KIT
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined (AI_ESP32A1S_AUDIO_KIT_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "AI ESP32A1S AUDIO KIT + TFT Touch 4.0\""
  #define AI_ESP32A1S_AUDIO_KIT
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"
//----------------------------------------------------------

#elif defined WEMOS_D1R32_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "WEMOS D1 R32 + TFT Touch 2.4\""
  #define WEMOS_D1R32
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined WEMOS_D1R32_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "WEMOS D1 R32 + TFT Touch 2.8\""
  #define WEMOS_D1R32
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined WEMOS_D1R32_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "WEMOS D1 R32 + TFT Touch 3.2\""
  #define WEMOS_D1R32
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined (WEMOS_D1R32_TFT_3_5_INCH)
  #define BLYNKGO_BOARD_NAME      "WEMOS D1 R32 + TFT Touch 3.5\""
  #define WEMOS_D1R32
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined (WEMOS_D1R32_TFT_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "WEMOS D1 R32 + TFT Touch 4.0\""
  #define WEMOS_D1R32
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"
//----------------------------------------------------------

#elif defined DOIT_ESP32_DEVKIT_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "DOIT ESP32 Devkit (30pins) + TFT Touch 2.4\""
  #define DOIT_ESP32_DEVKIT
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined DOIT_ESP32_DEVKIT_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "DOIT ESP32 Devkit (30pins) + TFT Touch 2.8\""
  #define DOIT_ESP32_DEVKIT
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined DOIT_ESP32_DEVKIT_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "DOIT ESP32 Devkit (30pins) + TFT Touch 3.2\""
  #define DOIT_ESP32_DEVKIT
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined (DOIT_ESP32_DEVKIT_TFT_3_5_INCH)
  #define BLYNKGO_BOARD_NAME      "DOIT ESP32 Devkit (30pins) + TFT Touch 3.5\""
  #define DOIT_ESP32_DEVKIT
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined (DOIT_ESP32_DEVKIT_TFT_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "DOIT ESP32 Devkit (30pins) + TFT Touch 4.0\""
  #define DOIT_ESP32_DEVKIT
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

#elif defined (DOIT_ESP32_DEVKIT_OLED_SSD1306)
  #define BLYNKGO_BOARD_NAME      "DOIT ESP32 Devkit (30pins) + OLED (SSD1306)"
  #define BLYNKGO_OLED_SSD1306
  #define BLYNKGO_OLED

#elif defined (DOIT_ESP32_DEVKIT_OLED_SH1106)
  #define BLYNKGO_BOARD_NAME      "DOIT ESP32 Devkit (30pins) + OLED (SH1106)"
  #define BLYNKGO_OLED_SH1106
  #define BLYNKGO_OLED


//----------------------------------------------------------

#elif defined KIDBRIGHT_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "KidBright + TFT Touch 2.4\""
  #define KIDBRIGHT_BLYNKGO
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined KIDBRIGHT_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "KidBright + TFT Touch 2.8\""
  #define KIDBRIGHT_BLYNKGO
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined KIDBRIGHT_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "KidBright + TFT Touch 3.2\""
  #define KIDBRIGHT_BLYNKGO
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER      // TFT 2.4", 2.8", 3.2"

#elif defined (KIDBRIGHT_TFT_3_5_INCH)
  #define BLYNKGO_BOARD_NAME      "KidBright + TFT Touch 3.5\""
  #define KIDBRIGHT_BLYNKGO
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER      // TFT 3.5"

#elif defined (KIDBRIGHT_TFT_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "KidBright + TFT Touch 4.0\""
  #define KIDBRIGHT_BLYNKGO
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER      // TFT 4.0"

//----------------------------------------------------------

#elif defined TTGO_T7_MINI32_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T7 MINI32 + TFT Touch 2.4\""
  #define TTGO_T7_MINI32
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8", 3.2"

#elif defined TTGO_T7_MINI32_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T7 MINI32 + TFT Touch 2.8\""
  #define TTGO_T7_MINI32
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8", 3.2"

#elif defined TTGO_T7_MINI32_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T7 MINI32 + TFT Touch 3.2\""
  #define TTGO_T7_MINI32
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8", 3.2"

#elif defined (TTGO_T7_MINI32_TFT_3_5_INCH)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T7 MINI32 + TFT Touch 3.5\""
  #define TTGO_T7_MINI32
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER

#elif defined (TTGO_T7_MINI32_TFT_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T7 MINI32 + TFT Touch 4.0\""
  #define TTGO_T7_MINI32
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER

//----------------------------------------------------------

#elif defined TTGO_T8_TFT_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T8 + TFT Touch 2.4\""
  #define TTGO_T8
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8"

#elif defined TTGO_T8_TFT_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T8 + TFT Touch 2.8\""
  #define TTGO_T8
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8"

#elif defined TTGO_T8_TFT_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T8 + TFT Touch 3.2\""
  #define TTGO_T8
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER

#elif defined (TTGO_T8_TFT_3_5_INCH)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T8 + TFT Touch 3.5\""
  #define TTGO_T8
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER

#elif defined (TTGO_T8_TFT_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T8 + TFT Touch 4.0\""
  #define TTGO_T8
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER

//----------------------------------------------------------


#elif defined TTGO_T_CELL_2_4_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T-Cell + TFT Touch 2.4\""
  #define TTGO_T_CELL
  #define TFT_2_4_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8", 3.2"

#elif defined TTGO_T_CELL_2_8_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T-Cell + TFT Touch 2.8\""
  #define TTGO_T_CELL
  #define TFT_2_8_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8", 3.2"

#elif defined TTGO_T_CELL_3_2_INCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T-Cell + TFT Touch 3.2\""
  #define TTGO_T_CELL
  #define TFT_3_2_INCH_TOUCH
  #define ILI9341_DRIVER       // TFT 2.4", 2.8", 3.2"

#elif defined (TTGO_T_CELL_3_5_INCH)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T-Cell + TFT Touch 3.5\""
  #define TTGO_T_CELL
  #define TFT_3_5_INCH_TOUCH
  #define ILI9488_DRIVER

#elif defined (TTGO_T_CELL_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T-Cell + TFT Touch 4.0\""
  #define TTGO_T_CELL
  #define TFT_4_0_INCH_TOUCH
  #define ST7796_DRIVER

//----------------------------------------------------------

#elif defined (BLYNKGO_M5STACK)
  #define BLYNKGO_BOARD_NAME      "M5Stack (TFT 2.2 no touch)"
  #define BLYNKGO_BASIC
#elif defined (BLYNKGO_M5STACK_FIRE)
  #define BLYNKGO_BOARD_NAME      "M5Stack Fire (TFT 2.2 no touch)"
#endif



//----------------------------------------------------------
//----------------------------------------------------------
#if defined (TFT_2_4_INCH_TOUCH) || defined (TFT_2_8_INCH_TOUCH) || defined (TFT_3_2_INCH_TOUCH) || defined (TFT_3_5_INCH_TOUCH) || defined (TFT_4_0_INCH_TOUCH)
  #define TOUCH_XPT2046
  #define ROTATE_TYPE             3
#endif
//----------------------------------------------------------
#if defined (TFT_5_0_INCH_TOUCH)
  #define TOUCH_GSL1680
  #define ROTATE_TYPE             0
#endif

//----------------------------------------------------------
//----------------------------------------------------------
// HSPI ปกติ MISO12, MOSI13, SCK14, SS15;
// VSPI     MISO19, MOSI23, SCK18, SS5; 


        // _sck = (_spi_num == VSPI) ? SCK : 14;    // ถ้าเป็น HSPI จะกลายเป็นชุด 12,13,14,15ให้อัตโนมัติ
        // _miso = (_spi_num == VSPI) ? MISO : 12;
        // _mosi = (_spi_num == VSPI) ? MOSI : 13;
        // _ss = (_spi_num == VSPI) ? SS : 15;

#if defined WEMOS_D32_PRO
  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  14      // Chip select control pin
  #define TFT_RST                 33
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_BL                  32       // TFT backlight  pin
  #define TFT_INVERT              false

  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                12      // TouchScreen Chip Select

#elif defined GEEKCREIT_ESP32_WROVER
  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  14      // Chip select control pin
  #define TFT_RST                 33
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_BL                  32       // TFT backlight  pin
  #define TFT_INVERT              false

  #define SD_MISO                 2
  #define SD_MOSI                 14
  #define SD_SCLK                 15
  #define SD_CS                   13       // SD Chip Select

  #define TOUCH_MISO              2
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                12      // TouchScreen Chip Select

#elif defined IOX_ESP32
  #define BLYNKGO_BASIC
  #define BLYNKGO_USE_SD          0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  4        // Chip select control pin
  #define TFT_RST                 2
  #define TFT_DC                  15       // Data Command control pin
  #define TFT_BL                  5        // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                8        // TouchScreen Chip Select

#elif defined IOX_ESP32PS
  #define BLYNKGO_USE_SD          0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  4        // Chip select control pin
  #define TFT_RST                 2
  #define TFT_DC                  15       // Data Command control pin
  #define TFT_BL                  5        // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                8        // TouchScreen Chip Select

#elif defined ESP32WROVER_ADAPTER
  #define BLYNKGO_USE_SD          0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  4        // Chip select control pin
  #define TFT_RST                 2
  #define TFT_DC                  15       // Data Command control pin
  #define TFT_BL                  5        // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                14        // TouchScreen Chip Select


#elif defined ESPINO32
  #define BLYNKGO_BASIC
  #define BLYNKGO_USE_SD          0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  1        // Chip select control pin
  #define TFT_RST                 3
  #define TFT_DC                  17       // Data Command control pin
  #define TFT_BL                  16        // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                5        // TouchScreen Chip Select

#elif defined(AI_ESP32A1S_AUDIO_KIT)
  #define BLYNKGO_USE_SD          1
  #define SD_MMC_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TX0                     1
  #define RX0                     3

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  21      // Chip select control pin
  #define TFT_RST                 22
  #define TFT_DC                   5      // Data Command control pin
  #define TFT_BL                   0   //RX0     // TFT backlight  pin
  #define TFT_INVERT              false
  
  #define KEY1                    36
  #define KEY2                    13
  #define KEY3                    19
  #define KEY4                    23
  #define KEY5                    18
  #define KEY6                     5

  #define JT_MTDO                 15
  #define JT_MTCK                 13
  #define JT_MIDI                 12
  #define JT_MTMS                 14

  #define SD_DAT0                 2
  #define SD_DAT1                 4
  #define SD_DAT2                 12
  #define SD_DAT3                 13             // SD_DAT3 = SD_DC = SD_CS
  #define SD_CMD                  15
  #define SD_SIGNAL_CLK           14
  #define SD_DETECT               34

  #define SD_MISO                 SD_DAT0        // 2   SPI_MISO   (ทำงานได้!)  
  #define SD_MOSI                 SD_CMD         // 15  SPI_MOSI
  #define SD_SCLK                 SD_SIGNAL_CLK  // 14  SPI_SCLK
  #define SD_CS                   SD_DAT3        // 13  SD Chip Select



#elif defined(DOIT_ESP32_DEVKIT) || defined(ESP32S_WROOM) || defined(ESP32WROOM_ADAPTER) || defined(LOLIN_D32)
  #define BLYNKGO_BASIC
  #define BLYNKGO_USE_SD           0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  15      // Chip select control pin
  #define TFT_RST                  2
  #define TFT_DC                   4      // Data Command control pin
  #define TFT_BL                   5      // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                27      // TouchScreen Chip Select

#elif defined(ESP32_WROOM_DEVKIT)
  #define BLYNKGO_BASIC
  #define BLYNKGO_USE_SD           0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                   5      // Chip select control pin
  #define TFT_RST                 17
  #define TFT_DC                  16      // Data Command control pin
  #define TFT_BL                   4      // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                 2      // TouchScreen Chip Select

#elif defined(WEMOS_D1R32)
  #define BLYNKGO_BASIC
  #define BLYNKGO_USE_SD           0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  13      // Chip select control pin
  #define TFT_RST                 12
  #define TFT_DC                  14      // Data Command control pin
  #define TFT_BL                   5      // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                27      // TouchScreen Chip Select

#elif defined(KIDBRIGHT_BLYNKGO)
  #define BLYNKGO_BASIC
  #define BLYNKGO_USE_SD           0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  32      // Chip select control pin
  #define TFT_RST                 33
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_BL                  26      // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                21      // TouchScreen Chip Select

#elif defined TTGO_T7_MINI32  // V1.7
  #define BLYNKGO_USE_SD          0

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO  // x
  #define TFT_MOSI                SPI_MOSI  // 23
  #define TFT_SCLK                SPI_SCLK  // 18
  #define TFT_CS                  27    // Chip select control pin
  #define TFT_RST                 32 
  #define TFT_DC                  17    // Data Command control pin
  #define TFT_BL                  16    // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                4     // TouchScreen Chip Select

#elif defined TTGO_T8  // V1.7
  #define BLYNKGO_USE_SD          0   // 1
  // #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO  // x
  #define TFT_MOSI                SPI_MOSI  // 23
  #define TFT_SCLK                SPI_SCLK  // 18
  #define TFT_CS                  2     // Chip select control pin
  #define TFT_RST                 4 
  #define TFT_DC                  13    // Data Command control pin
  #define TFT_BL                  5     // TFT backlight  pin
  #define TFT_INVERT              false

  // #define SD_MISO             2     // SPI_MISO
  // #define SD_MOSI             15    // SPI_MOSI
  // #define SD_SCLK             14    // SPI_SCLK
  // #define SD_CS               13    // SD Chip Select

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                15//12//27      // TouchScreen Chip Select



#elif defined (TTGO_T4)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO T4 (TFT 2.2 no touch)\""

  #define ILI9341_DRIVER
  #define ROTATE_TYPE             1

  #define BLYNKGO_USE_SD          1
  #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                12
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  27      // Chip select control pin
  #define TFT_DC                  32      // Data Command control pin
  #define TFT_RST                 5
  #define TFT_BL                  4       // TFT backlight  pin
  #define TFT_INVERT              false

  #define SD_MISO                 2
  #define SD_MOSI                 15
  #define SD_SCLK                 14
  #define SD_CS                   13       // SD Chip Select

#elif defined (TTGO_T_CELL)
  #define BLYNKGO_USE_SD          1
  #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO  // x
  #define TFT_MOSI                SPI_MOSI  // 23
  #define TFT_SCLK                SPI_SCLK  // 18
  #define TFT_CS                  21    // Chip select control pin
  #define TFT_RST                 22 
  #define TFT_DC                  5     // Data Command control pin
  #define TFT_BL                  4     // TFT backlight  pin
  #define TFT_INVERT              false

  #define SD_DAT0                 2
  #define SD_DAT1                 4
  #define SD_DAT2                 12
  #define SD_DAT3                 13
  #define SD_CMD                  15
  #define SD_SIGNAL_CLK           14

  #define SD_MISO                 SD_DAT0        // 2   SPI_MISO   (ทำงานได้!)  
  #define SD_MOSI                 SD_CMD         // 15  SPI_MOSI
  #define SD_SCLK                 SD_SIGNAL_CLK  // 14  SPI_SCLK
  #define SD_CS                   SD_DAT3        // 13  SD Chip Select

  #define TOUCH_MISO              SPI_MISO     // 19 T_DO
  #define TOUCH_MOSI              SPI_MOSI     // 23 T_DIN
  #define TOUCH_SCLK              SPI_SCLK     // 18 T_CLK
  #define TOUCH_CS                26           // 26 T_CS  TouchScreen Chip Select

#elif defined (ESPRESSIF_ESP_WROVER_KIT)
  #define BLYNKGO_BOARD_NAME      "Espressif ESP WROVER KIT (TFT 2.4 no touch)\""

  #define ILI9341_DRIVER
  #define ROTATE_TYPE             3

  #define BLYNKGO_USE_SD          1
  #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                25
  #define SPI_MOSI                23
  #define SPI_SCLK                19

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  22      // Chip select control pin
  #define TFT_DC                  21      // Data Command control pin
  #define TFT_RST                 18
  #define TFT_BL                  5       // TFT backlight  pin
  #define TFT_INVERT              false

  #define SD_MISO                  2
  #define SD_MOSI                 15
  #define SD_SCLK                 14
  #define SD_CS                   13       // SD Chip Select

#elif defined (BLYNKGO_M5STACK_FIRE) || defined (BLYNKGO_M5STACK)
  #define M5STACK_DEVICE
  #define ILI9342_DRIVER
  #define ROTATE_TYPE              0

  #define BLYNKGO_USE_SD           1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  14      // Chip select control pin
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_RST                 33
  #define TFT_BL                  32       // TFT backlight  pin
  #define TFT_INVERT              true

  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

  // Buttons
  // #define BTN_A                   0
  // #define BTN_B                   1
  // #define BTN_C                   2
  // #define BUTTON_A                0
  // #define BUTTON_B                1
  // #define BUTTON_C                2


  #define BUTTON_A_PIN            39
  #define BUTTON_B_PIN            38
  #define BUTTON_C_PIN            37

  // #define BUTTON1                 BUTTON_A_PIN
  // #define BUTTON2                 BUTTON_B_PIN
  // #define BUTTON3                 BUTTON_C_PIN

  // BEEP PIN
  #define SPEAKER_PIN             25
  #define TONE_PIN_CHANNEL        0

  // #define M5STACK_DEVICE
  // #define ILI9341_DRIVER
  // #define ROTATE_TYPE              1

  // #define BLYNKGO_USE_SD           1
  // #define SD_VSPI_MODE

  // #define SPI_HOST_TYPE           VSPI_HOST
  // #define SPI_MISO                19
  // #define SPI_MOSI                23
  // #define SPI_SCLK                18

  // #define TFT_MISO                SPI_MISO
  // #define TFT_MOSI                SPI_MOSI
  // #define TFT_SCLK                SPI_SCLK
  // #define TFT_CS                  14      // Chip select control pin
  // #define TFT_DC                  27      // Data Command control pin
  // #define TFT_RST                 33
  // #define TFT_BL                  32       // TFT backlight  pin
  // #define TFT_INVERT              false

  // #define SD_MISO                 SPI_MISO
  // #define SD_MOSI                 SPI_MOSI
  // #define SD_SCLK                 SPI_SCLK
  // #define SD_CS                   4       // SD Chip Select

  // // Buttons
  // #define BTN_A                   0
  // #define BTN_B                   1
  // #define BTN_C                   2
  // #define BUTTON_A                0
  // #define BUTTON_B                1
  // #define BUTTON_C                2

  // #define BUTTON_A_PIN            39
  // #define BUTTON_B_PIN            38
  // #define BUTTON_C_PIN            37

  // // BEEP PIN
  // #define SPEAKER_PIN             25
  // #define TONE_PIN_CHANNEL        0

#elif defined (M5STACK_TFT_4_0_INCH)
  #define BLYNKGO_BOARD_NAME      "M5Stack + TFT Touch 4.0\""

  #define M5STACK_DEVICE
  #define BLYNKGO_BASIC

  #define TOUCH_XPT2046
  #define ST7796_DRIVER
  #define ROTATE_TYPE             3

  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  22      // Chip select control pin
  #define TFT_RST                 21
  #define TFT_DC                   2      // Data Command control pin
  #define TFT_BL                   5      // TFT backlight  pin
  #define TFT_INVERT              false

  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   4       // SD Chip Select

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                17      // TouchScreen Chip Select

  // Buttons
  #define BTN_A                   0
  #define BTN_B                   1
  #define BTN_C                   2
  #define BUTTON_A                0
  #define BUTTON_B                1
  #define BUTTON_C                2

  #define BUTTON_A_PIN            39
  #define BUTTON_B_PIN            38
  #define BUTTON_C_PIN            37

  // BEEP PIN
  #define SPEAKER_PIN             25
  #define TONE_PIN_CHANNEL        0

#elif defined (ESP32_CUSTOMIZE)
  #define BLYNKGO_USE_SD          0
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  21       // Chip select control pin
  #define TFT_DC                  5        // Data Command control pin
  #define TFT_RST                 32
  #define TFT_BL                  22       // TFT backlight  pin
  #define TFT_INVERT              false

  #define TOUCH_MISO              SPI_MISO
  #define TOUCH_MOSI              SPI_MOSI
  #define TOUCH_SCLK              SPI_SCLK
  #define TOUCH_CS                4      // TouchScreen Chip Select

  // #define SD_MISO             SPI_MISO
  // #define SD_MOSI             SPI_MOSI
  // #define SD_SCLK             SPI_SCLK
  // #define SD_CS               16       // SD Chip Select


#elif defined (ADAFRUIT_TFT_2_8_CAPACITIVE)
  #define ILI9341_DRIVER
  #define ROTATE_TYPE             3

  #define BLYNKGO_USE_SD          0
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                19
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  21       // Chip select control pin
  #define TFT_DC                  5        // Data Command control pin
  #define TFT_RST                 -1
  #define TFT_BL                  22       // TFT backlight  pin
  #define TFT_INVERT              false

  #define I2C_SDA                 21
  #define I2C_SCL                 22

  #define TOUCH_SDA               16
  #define TOUCH_SCL               17
  #define TOUCH_THRESHOLD         40    // ค่า Theshold ของ TouchScreen

#elif defined (TTGO_CAMERA_PLUS)
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO Camera Plus (TFT 1.3 no touch)"

  #define ST7789_DRIVER
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              240
  #define ROTATE_TYPE             0
  #define BLYNKGO_USE_SD          1
  #define SD_VSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MOSI                19
  #define SPI_SCLK                21
  #define SPI_MISO                22

  // TFT
  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  12      // Chip select control pin
  #define TFT_DC                  15      // Data Command control pin
  #define TFT_RST                 -1      // Not use
  #define TFT_BL                  2       // TFT backlight  pin
  #define TFT_INVERT              true

  // SD
  #define SD_MISO                 SPI_MISO
  #define SD_MOSI                 SPI_MOSI
  #define SD_SCLK                 SPI_SCLK
  #define SD_CS                   0

  // BME280 & IP5306
  #define I2C_SDA                 18
  #define I2C_SCL                 23
  
  // Mic
  #define IIS_SCLK                14
  #define IIS_LCLK                32
  #define IIS_DSIN                -1
  #define IIS_DOUT                33

  // Camera
  #define PWDN_GPIO_NUM           -1
  #define RESET_GPIO_NUM          -1
  #define XCLK_GPIO_NUM           4
  #define SIOD_GPIO_NUM           18
  #define SIOC_GPIO_NUM           23

  #define Y9_GPIO_NUM             36
  #define Y8_GPIO_NUM             37
  #define Y7_GPIO_NUM             38
  #define Y6_GPIO_NUM             39
  #define Y5_GPIO_NUM             35
  #define Y4_GPIO_NUM             26
  #define Y3_GPIO_NUM             13
  #define Y2_GPIO_NUM             34
  #define VSYNC_GPIO_NUM          5
  #define HREF_GPIO_NUM           27
  #define PCLK_GPIO_NUM           25
  #define XCLK_FREQ               20000000

  #define D7_GPIO_NUM             Y9_GPIO_NUM  //     36
  #define D6_GPIO_NUM             Y8_GPIO_NUM  //     37
  #define D5_GPIO_NUM             Y7_GPIO_NUM  //     38
  #define D4_GPIO_NUM             Y6_GPIO_NUM  //     39
  #define D3_GPIO_NUM             Y5_GPIO_NUM  //     35
  #define D2_GPIO_NUM             Y4_GPIO_NUM  //     26
  #define D1_GPIO_NUM             Y3_GPIO_NUM  //     13
  #define D0_GPIO_NUM             Y2_GPIO_NUM  //     34


#elif defined (TTGO_TWATCH_2019)
  #define TTGO_TWATCH
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO TWatch 2019 (TFT 1.54\" Capacitive)"

  #define ST7789_DRIVER
  #define TOUCH_FT5206
  // #define TOUCH_FT6236
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              240
  #define ROTATE_TYPE             0
  #define BLYNKGO_USE_SD          1
  #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                -1
  #define SPI_MOSI                19
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  5       // Chip select control pin
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_RST                 -1
  #define TFT_BL                  12      // TFT backlight  pin
  #define TFT_INVERT              true

  #define SD_MISO                 2
  #define SD_MOSI                 15
  #define SD_SCLK                 14
  #define SD_CS                   13

  #define I2C_SDA                 21
  #define I2C_SCL                 22

  #define TOUCH_SDA               23
  #define TOUCH_SCL               32

  #define TOUCH_I2C_SDA           TOUCH_SDA
  #define TOUCH_I2C_SCL           TOUCH_SCL

  #define SENSOR_SDA              I2C_SDA   // Max30102 heart rate blood oxygen sensor
  #define SENSOR_SCL              I2C_SCL

  #define UART_TX                 33
  #define UART_RX                 34

  #define SPEAKER_PIN             25
  #define TONE_PIN_CHANNEL        0

  #define SPEAKER_PWD             19
  #define SPEAKER_OUT             25
  #define ADC_IN                  35

  // #define VIBRATE_PIN             33  // Vibration motor  LOW ไม่สั่น HIGH สั่น?
  #define MOTOR_PIN               33 
  #define USER_BUTTON             36
  #define RTC_INT                 37
  #define TOUCH_INT               38
  #define AXP202_INT              35
  #define BMA423_INT1             39  // เซนเซอร์นับก้าว
  #define BMA423_INT2             0

  #define DAC_IIS_BCK             26
  #define DAC_IIS_WS              25
  #define DAC_IIS_DOUT            33
  #define DAC_IIS_DIN             -1

  // GPS Shield ( UBlox GPS)
  //! M8M BOARD
  #define GPS_RX                  34
  #define GPS_TX                  33
  #define GPS_BAUD_RATE           9600

#elif defined (TTGO_TWATCH_2020_V1)
  #define TTGO_TWATCH
  #define TTGO_TWATCH_2020
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO TWatch 2020 v1 (TFT 1.54\" Capacitive)"

  #define ST7789_DRIVER
  #define TOUCH_FT6236
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              240
  #define ROTATE_TYPE             0
  #define TFT_ROTATION_OFFSET     2
  #define BLYNKGO_USE_SD          0
  #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                -1
  #define SPI_MOSI                19
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  5       // Chip select control pin
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_RST                 -1
  #define TFT_BL                  12      // TFT backlight  pin
  #define TFT_INVERT              true

  #define TFT_OFFSET_X            0
  #define TFT_OFFSET_Y            -80

  // #define SD_MISO                 2
  // #define SD_MOSI                 15
  // #define SD_SCLK                 14
  // #define SD_CS                   13

  #define I2C_SDA                 21
  #define I2C_SCL                 22

  #define TOUCH_SDA               23
  #define TOUCH_SCL               32

  #define TOUCH_I2C_SDA           TOUCH_SDA
  #define TOUCH_I2C_SCL           TOUCH_SCL

  #define SENSOR_SDA              I2C_SDA   // Max30102 heart rate blood oxygen sensor
  #define SENSOR_SCL              I2C_SCL

  // #define UART_TX                 33
  // #define UART_RX                 34

  #define SPEAKER_PIN             25
  #define TONE_PIN_CHANNEL        0

  #define SPEAKER_PWD             19
  #define SPEAKER_OUT             25
  #define ADC_IN                  35

  // #define VIBRATE_PIN             33  // Vibration motor  LOW ไม่สั่น HIGH สั่น?
  // #define USER_BUTTON             36
  #define RTC_INT                 37
  #define TOUCH_INT               38
  #define AXP202_INT              35
  #define BMA423_INT1             39  // เซนเซอร์นับก้าว
  #define BMA423_INT2             0

  #define MOTOR_PIN               4
  #define IR_SEND_PIN             13

  #define DAC_IIS_BCK             26
  #define DAC_IIS_WS              25
  #define DAC_IIS_DOUT            33
  #define DAC_IIS_DIN             -1

  // #if defined BLYNKGO_USE_AUDIO
  // #undef BLYNKGO_USE_AUDIO
  // #define BLYNKGO_USE_AUDIO       1
  // #endif

#elif defined (TTGO_TWATCH_2020_V2)
  #define TTGO_TWATCH
  #define TTGO_TWATCH_2020
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO TWatch 2020 v2 (TFT 1.54\" Capacitive)"

  #define ST7789_DRIVER
  #define TOUCH_FT6236
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              240
  #define ROTATE_TYPE             0
  #define TFT_ROTATION_OFFSET     2
  #define BLYNKGO_USE_SD          1
  #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                -1
  #define SPI_MOSI                19
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  5       // Chip select control pin
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_RST                 -1
  #define TFT_BL                  25      // TFT backlight  pin
  #define TFT_INVERT              true

  #define TFT_OFFSET_X            0
  #define TFT_OFFSET_Y            -80

  #define SD_MISO                 4
  #define SD_MOSI                 15
  #define SD_SCLK                 14
  #define SD_CS                   13

  #define I2C_SDA                 21
  #define I2C_SCL                 22

  #define TOUCH_SDA               23
  #define TOUCH_SCL               32

  // #define TOUCH_RST                Use AXP202 EXTEN Pin control

  #define TOUCH_I2C_SDA           TOUCH_SDA
  #define TOUCH_I2C_SCL           TOUCH_SCL

  #define SENSOR_SDA              I2C_SDA   // Max30102 heart rate blood oxygen sensor
  #define SENSOR_SCL              I2C_SCL

  // #define UART_TX                 33
  // #define UART_RX                 34

  #define SPEAKER_PIN             25
  #define TONE_PIN_CHANNEL        0

  #define SPEAKER_PWD             19
  #define SPEAKER_OUT             25
  #define ADC_IN                  35

  // #define VIBRATE_PIN             33  // Vibration motor  LOW ไม่สั่น HIGH สั่น?
  // #define USER_BUTTON             36
  #define RTC_INT                 37
  #define TOUCH_INT               38
  #define AXP202_INT              35
  #define BMA423_INT1             39  // เซนเซอร์นับก้าว
  // #define BMA423_INT2             0

  // #define MOTOR_PIN               4

  #define IR_SEND_PIN             2

  //GPS power domain is AXP202 LDO4
  #define GPS_1PPS                34
  #define GPS_RX                  36
  #define GPS_TX                  26
  #define GPS_WAKE                33
  #define GPS_BAUD_RATE           9600

#elif defined (TTGO_TWATCH_2020_V3)
  #define TTGO_TWATCH
  #define TTGO_TWATCH_2020
  #define BLYNKGO_BOARD_NAME      "LilyGO/TTGO TWatch 2020 v3 (TFT 1.54\" Capacitive)"

  #define ST7789_DRIVER
  #define TOUCH_FT6236
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              240
  #define ROTATE_TYPE             0
  #define TFT_ROTATION_OFFSET     2
  #define BLYNKGO_USE_SD          0
  #define SD_HSPI_MODE

  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                -1
  #define SPI_MOSI                19
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  5       // Chip select control pin
  #define TFT_DC                  27      // Data Command control pin
  #define TFT_RST                 -1
  #define TFT_BL                  15      // TFT backlight  pin
  #define TFT_INVERT              true

  #define TFT_OFFSET_X            0
  #define TFT_OFFSET_Y            -80

  // #define SD_MISO                 2
  // #define SD_MOSI                 15
  // #define SD_SCLK                 14
  // #define SD_CS                   13

  #define I2C_SDA                 21
  #define I2C_SCL                 22

  #define TOUCH_SDA               23
  #define TOUCH_SCL               32
  #define TOUCH_RESET             14

  #define TOUCH_I2C_SDA           TOUCH_SDA
  #define TOUCH_I2C_SCL           TOUCH_SCL

  #define SENSOR_SDA              I2C_SDA   // Max30102 heart rate blood oxygen sensor
  #define SENSOR_SCL              I2C_SCL

  #define UART_TX                 33
  #define UART_RX                 34

  #define SPEAKER_PIN             25
  #define TONE_PIN_CHANNEL        0

  #define SPEAKER_PWD             19
  #define SPEAKER_OUT             25
  #define ADC_IN                  35

  // #define VIBRATE_PIN             33  // Vibration motor  LOW ไม่สั่น HIGH สั่น?
  // #define USER_BUTTON             36
  #define RTC_INT                 37
  #define TOUCH_INT               38
  #define AXP202_INT              35
  #define BMA423_INT1             39  // เซนเซอร์นับก้าว
  #define BMA423_INT2             0

  #define MOTOR_PIN               4

  #define IR_RECEIVE_PIN          13
  #define IR_SEND_PIN             13
  #define tone(a,b,c) void()      // no tone() available on ESP32
  #define noTone(a) void()
  #define TONE_PIN                42 // Dummy for examples using it
  #define APPLICATION_PIN         100 //16 // RX2 pin

  #define DAC_IIS_BCK             26
  #define DAC_IIS_WS              25
  #define DAC_IIS_DOUT            33
  #define DAC_IIS_DIN             2

  //TWATCH 2020 V3 PDM microphone pin
  #define MIC_DATA                2
  #define MIC_CLOCK               0

  // #if defined BLYNKGO_USE_AUDIO
  // #undef BLYNKGO_USE_AUDIO
  // #define BLYNKGO_USE_AUDIO       1
  // #endif


#elif defined(BLYNKGO_MINI)
  #define BLYNKGO_BOARD_NAME      "BlynkGO Mini : TTGO T-Display 1.14\" 240x135"

  #define BLYNKGO_BASIC

  #define ST7789_DRIVER
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              135

  #define ROTATE_TYPE             1
  #define BLYNKGO_USE_SD          0

  // TFT
  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                -1
  #define SPI_MOSI                19
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  5        // Chip select control pin
  #define TFT_RST                 23
  #define TFT_DC                  16       // Data Command control pin
  #define TFT_BL                  4        // TFT backlight  pin
  #define TFT_INVERT              true

  #define TFT_OFFSET_X            -53
  #define TFT_OFFSET_Y            40

  // -------------
  #define I2C_SDA                 21
  #define I2C_SCL                 22

  //---------------
  #define BUTTON1                 35
  #define BUTTON2                 0

  #define ADC_POWER               14
  #define ADC_IN                  34


#elif defined(BLYNKGO_SPEED)
  #define BLYNKGO_BOARD_NAME      "BlynkGO SPEED (TFT 1.3\" 240x240)"

  #define BLYNKGO_BASIC

  #define ST7789_DRIVER
  #define TFT_WIDTH               240
  #define TFT_HEIGHT              240

  #define ROTATE_TYPE             3
  #define BLYNKGO_USE_SD          0

  // TFT
  #define SPI_HOST_TYPE           VSPI_HOST
  #define SPI_MISO                -1
  #define SPI_MOSI                23
  #define SPI_SCLK                18

  #define TFT_MISO                SPI_MISO
  #define TFT_MOSI                SPI_MOSI
  #define TFT_SCLK                SPI_SCLK
  #define TFT_CS                  -1        // Chip select control pin
  #define TFT_RST                 2
  #define TFT_DC                  4       // Data Command control pin
  #define TFT_BL                  5        // TFT backlight  pin
  #define TFT_INVERT              true

  #define TFT_OFFSET_X            0
  #define TFT_OFFSET_Y            -80

  // -------------
  #define I2C_SDA                 21
  #define I2C_SCL                 22

  //---------------
  #define BUTTON1                 12 //34
  #define BUTTON2                 13 // 35

#elif defined(BLYNKGO_LORA32)  // SSD1306 OLED
  #define BLYNKGO_BOARD_NAME      "BLYNKGO LORA32 (TTGO-Loro32 OLED 0.96\" 128x64 no Touch)"
  #define BLYNKGO_OLED_SSD1306
  #define BLYNKGO_OLED

#ifdef BLYNKGO_USE_LORA
#undef BLYNKGO_USE_LORA
#endif

  #define BLYNKGO_USE_LORA        1

  #define OLED_ADDR               0x3c
  #define OLED_SDA                4
  #define OLED_SCL                15
  #define OLED_PIN                16
  
  // Lora Module
  #define LORA_SCK                5
  #define LORA_MISO               19
  #define LORA_MOSI               27
  #define LORA_SS                 18
  #define LORA_RST                14
  #define LORA_DIO0               26

#endif //BLYNKGO_SPEED

#if defined(BLYNKGO_OLED)
#ifndef OLED_ADDR
  #define OLED_ADDR             0x3c
#endif

#ifndef OLED_SDA
  #define OLED_SDA              26 //I2C_SDA   // 21
#endif
  
#ifndef OLED_SCL
  #define OLED_SCL              27 //I2C_SCL   // 22
#endif
#endif // BLYNKGO_OLED


#if BLYNKGO_USE_SD
#define CAPTURE_PATH_TEMPLATE     "SD://BlynkGO/captures/capture_%05d.png"
#else
#define CAPTURE_PATH_TEMPLATE     "SPIFFS://capture_%05d.png"
#endif

#if !defined(ENG_TEXT) && !defined(LAOS_TEXT) && !defined(VIETNAM_TEXT)
#define THAI_TEXT
#endif

#define ROTATION_DEFAULT          ROTATE_TYPE
#define ROTATION_VERTICAL         (ROTATION_DEFAULT+3)%4

#endif //__BLYNKGO_SYSTEM_CONFIG_H__

