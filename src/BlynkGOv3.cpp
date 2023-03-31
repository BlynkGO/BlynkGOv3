/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ BlynkGOv3.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ fv
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

#include "BlynkGOv3.h"
#include <Ticker.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <vector>

const char* TAG = "BLYNKGO";

#define BLYNKGO_DEV_LEVEL_BASE     1
#define BLYNKGO_DEV_LEVEL_GFX      2
#define BLYNKGO_DEV_LEVEL_SUPERG   3
#define BLYNKGO_DEV_LEVEL_SD       4

#define BLYNKGO_DEV_LEVEL          BLYNKGO_DEV_LEVEL_SD //BLYNKGO_DEV_LEVEL_SD //BLYNKGO_DEV_LEVEL_GFX //

static BlynkGOv3  *pBlynkGO=NULL;

#if BLYNKGO_USE_SD
  #if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_LEVEL_SD
    #if defined (SD_HSPI_MODE)
      static SPIClass SD_HSPI;  // แบบ HPSI
    #endif
  #endif //#if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_LEVEL_SD
#endif

#if defined(BLYNKGO_OLED)
  #if defined(BLYNKGO_OLED_SSD1306)
    SSD1306Wire  lcd(OLED_ADDR, OLED_SDA, OLED_SCL);
    static uint8_t* lcd_fb;
  #elif defined(BLYNKGO_OLED_SH1106)
    SH1106Wire  lcd(OLED_ADDR, OLED_SDA, OLED_SCL);
    static uint8_t* lcd_fb;
  #endif
  uint8_t _oled_threshold=128;
#else
  #if defined(BLYNKGO_USE_AGFX) && (BLYNKGO_USE_AGFX==1)  // ใช้ AGFX 
    BlynkGO_LCD lcd(TFT_WIDTH, TFT_HEIGHT, 2,
      new Arduino_ESP32RGBPanel(
        TFT_HENABLE /* DE */, TFT_VSYNC /* VSYNC */, TFT_HSYNC /* HSYNC */, TFT_PCLK /* PCLK */,
        TFT_R0 /* R0 */, TFT_R1 /* R1 */, TFT_R2 /* R2 */, TFT_R3 /* R3 */, TFT_R4 /* R4 */,
        TFT_G0 /* G0 */, TFT_G1 /* G1 */, TFT_G2 /* G2 */, TFT_G3 /* G3 */, TFT_G4 /* G4 */, TFT_G5 /* G5 */,
        TFT_B0 /* B0 */, TFT_B1 /* B1 */, TFT_B2 /* B2 */, TFT_B3 /* B3 */, TFT_B4 /* B4 */,
        TFT_HSYNC_POLARITY /* hsync_polarity */, TFT_HSYNC_FRONT_PORCH /* hsync_front_porch */, TFT_HSYNC_PULSE_WIDTH /* hsync_pulse_width */, TFT_HSYNC_BACK_PORCH /* hsync_back_porch */,
        TFT_VSYNC_POLARITY /* vsync_polarity */, TFT_VSYNC_FRONT_PORCH /* vsync_front_porch */, TFT_VSYNC_PULSE_WIDTH /* vsync_pulse_width */, TFT_VSYNC_BACK_PORCH /* vsync_back_porch */,
        TFT_PCLK_IDLE_HIGH /* pclk_active_neg */, 16000000 /* prefer_speed */),
      new TAMC_GT911( TOUCH_I2C_SDA, TOUCH_I2C_SCL, TOUCH_INT, TOUCH_RST, TFT_WIDTH, TFT_HEIGHT)
    );
  #else
    BlynkGO_LCD lcd;
  #endif
#endif

static lv_indev_t * indev  =NULL;
static lv_disp_buf_t disp_buf;
#if defined (BLYNKGO_BASIC)
  static lv_color_t _cbuf[LV_HOR_RES_MAX * 10];
// #elif defined (BLYNKGO_ENTERPRISE)
//   static lv_color_t buf[LV_HOR_RES_MAX * 60];
#else
  static lv_color_t *_cbuf;
  #if CONFIG_IDF_TARGET_ESP32S3
  static lv_color_t *_cbuf2;
  #endif

#endif
static void blynkgo_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#if BLYNKGO_USE_SNAPSHOT
#include "utils/BlynkGO_LCD/LoveCat/lgfx/v1/misc/bitmap.hpp"
bool     bSnapshot;
static uint8_t* capture_buffer =NULL;
static uint8_t* png24_image=NULL;
// static int _rowSize;
static File _file_bmp;
static int32_t _file_pos;
static int32_t _h_pos;
enum :uint8_t { CAPTURE_TYPE_BMP, CAPTURE_TYPE_PNG };
static uint8_t capture_type = CAPTURE_TYPE_BMP;
// static uint8_t *_file_line_buffer=NULL; 
#endif //BLYNKGO_USE_SNAPSHOT


#if defined(BLYNKGO_MINI)
  ButtonISR BTN1  = ButtonISR(BUTTON1);
  ButtonISR BTN2  = ButtonISR(BUTTON2);
#elif defined(BLYNKGO_M5STACK)
  ButtonISR BTN_A = ButtonISR(BUTTON_A_PIN);
  ButtonISR BTN_B = ButtonISR(BUTTON_B_PIN);
  ButtonISR BTN_C = ButtonISR(BUTTON_C_PIN);
#elif defined(BLYNKGO_LORA32) || BLYNKGO_USE_BUTTON_ISR
  ButtonISR BTN   = ButtonISR(0);
#endif

#if defined(BEENEXT_4_3) || defined(BEENEXT_4_3C) || defined(BEENEXT_4_3IPS) ||  defined(BEENEXT_5_0IPS) ||  defined(BEENEXT_7_0IPS)
  #define GUI_TASK_LOOP_NUM   1
  #define LVGL_TICK_PERIOD    60    // ms
#else
  #define GUI_TASK_LOOP_NUM   1
  #define LVGL_TICK_PERIOD    5    // ms
#endif

/************************************
 * Define & typedef
 ************************************/


#if defined(THAI_TEXT)
  #define TEXT_TOUCH_ARROW    "กดสัมผัสที่ลูกศร"
  #define TEXT_TOP_LEFT       "มุมซ้ายบน"
  #define TEXT_TOP_RIGHT      "มุมขวาบน"
  #define TEXT_BOTTOM_LEFT    "มุมซ้ายล่าง"
  #define TEXT_BOTTOM_RIGHT   "มุมขวาล่าง"
#elif defined(ENG_TEXT)
  #define TEXT_TOUCH_ARROW    "Touch at the arrow"
  #define TEXT_TOP_LEFT       "Top Left"
  #define TEXT_TOP_RIGHT      "Top Right"
  #define TEXT_BOTTOM_LEFT    "Bottom Left"
  #define TEXT_BOTTOM_RIGHT   "Bottom Right"
#elif defined(LAOS_TEXT)
  #define TEXT_TOUCH_ARROW    "ແຕະທີ່ລູກສອນ"
  #define TEXT_TOP_LEFT       "ແຈເທິງຊ້າຍ"
  #define TEXT_TOP_RIGHT      "ແຈເທິງຂວາ"
  #define TEXT_BOTTOM_LEFT    "ແຈລຸ່ມຊ້າຍ"
  #define TEXT_BOTTOM_RIGHT   "ແຈລຸ່ມຂວາ"
#elif defined (VIETNAM_TEXT)
  #define TEXT_TOUCH_ARROW    "Chạm vào mũi tên"
  #define TEXT_TOP_LEFT       "Trên cùng bên trái"
  #define TEXT_TOP_RIGHT      "Phía trên bên phải"
  #define TEXT_BOTTOM_LEFT    "Dưới cùng bên trái"
  #define TEXT_BOTTOM_RIGHT   "Dưới cùng bên phải"
#endif

static uint32_t _last_touch;
uint32_t last_touch()             { return _last_touch; }
// static void lv_tick_handler(void) { lv_tick_inc(LVGL_TICK_PERIOD); }  /* Interrupt driven periodic handler */
extern void  GWidgets_init();


// BlynkGO TimerAlarmsะ
#define ALARM_ID_INVALID    255
#define ALARM_ID_MAX        250

typedef struct _blynkgo_alarm_t{
  AlarmID_t id=ALARM_ID_INVALID;
  uint8_t isOnce;
  uint8_t isAbsolute;  // สำหรับ alarmOnce แบบ ตั้งเวลา สมบูรณ์
  timeDayOfWeek_t dow;
  time_t secs_of_day;
  OnTick_t onTickHandler;
} blynkgo_alarm_t;

static std::vector<blynkgo_alarm_t> blynkgo_alarms;

#if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)
#else
#if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563
 // now timestamp from RTC
time_t rtc_timestamp(){  return BlynkGO.RTC.timestamp(); }
#endif
#endif //#if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)


#if BLYNKGO_USE_BLYNK
typedef struct {
  String auth;
  String ssid;
  String pass;
  String domain;
  uint16_t port;
  bool   enable;
} blynk_config_t;

static blynk_config_t blynk_config;
static WiFiClient _blynkWifiClient;
bool   _blynkReconnect = false;

static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifi Blynk(_blynkTransport);
#endif //BLYNKGO_USE_BLYNK


#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK


static std::vector<WiFiEvent_t> _wifi_event_list;
static void BlynkGO_WiFiEvent(WiFiEvent_t event);
void WiFiNoOpCbk() {}
WIFI_CONNECTED()    __attribute__((weak, alias("WiFiNoOpCbk")));
WIFI_DISCONNECTED() __attribute__((weak, alias("WiFiNoOpCbk")));
SOFTAP_START()      __attribute__((weak, alias("WiFiNoOpCbk")));
SOFTAP_STOP()       __attribute__((weak, alias("WiFiNoOpCbk")));
SOFTAP_STA_CONNECTED()       __attribute__((weak, alias("WiFiNoOpCbk")));
SOFTAP_STA_DISCONNECTED()    __attribute__((weak, alias("WiFiNoOpCbk")));
#if BLYNKGO_USE_NTP
NTP_SYNCED()        __attribute__((weak, alias("WiFiNoOpCbk")));
#endif // BLYNKGO_USE_NTP
#endif

#if BLYNKGO_USE_WIFI
#if BLYNKGO_USE_NTP
// static GTimer ntp_timer;
static GTask  ntp_task;
static struct tm timeinfo;
static bool   ntp_synced=false;

static float  blynkgo_timezone         = NTP_TIMEZONE;
static float  blynkgo_daylight_saving  = NTP_DAYLIGHT_SAVING;

static void ntp_sync_task(void* param);
static bool get_timeinfo(struct tm * info)
{
  time_t now;
  time(&now);
  localtime_r(&now, info);
  if (info->tm_year > (2016 - 1900)) {
    return true;
  }
  return false;
}
#endif
#endif //BLYNKGO_USE_WIFI



BlynkGOv3   BlynkGO;

String blynkgo_board_name = String(BLYNKGO_BOARD_NAME);

void BlynkGOv3::blynkgo_logo(){
const char* blynkgo_logo = R"BLYNKGO_LOGO(
   ___  __          __   _________ 
  / _ )/ /_ _____  / /__/ ___/ __ \
 / _  / / // / _ \/  '_/ (_ / /_/ /
/____/_/\_, /_//_/_/\_\\___/\____/ 
       /___/ Version %s
)BLYNKGO_LOGO";

  Serial.printf(blynkgo_logo, this->_version.c_str());
#if defined BLYNKGO_BOARD_NAME
  Serial.println(F("[BlynkGO] " BLYNKGO_BOARD_NAME ));
#endif
}

void BlynkGOv3::regist(String blynkgo_key){
  uint64_t blynkgo_key64 =  strtoull (blynkgo_key.c_str(), NULL, 16);
  this->regist(blynkgo_key64);
}

void  BlynkGOv3::regist(uint64_t blynkgo_key){
  if( blynkgo_key != 0) {
    NVS.setInt("blynkgo", blynkgo_key);
  }
}

BlynkGOv3::BlynkGOv3(){
}

void BlynkGOv3::begin(String blynkgo_key){
  uint64_t blynkgo_key64 =  strtoull (blynkgo_key.c_str(), NULL, 16);
  this->begin(blynkgo_key64);
}


void BlynkGOv3::begin(uint64_t blynkgo_key){
  pBlynkGO = this;
  blynkgo_logo();
  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }
  NVS.begin();


#if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_LEVEL_GFX
  hw_lcd_init();
  this->regist(blynkgo_key);
#endif

#if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_LEVEL_SUPERG
  blynkgo_system_init();
#endif

#if BLYNKGO_USE_SD
#if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_>LEVEL_SD
  hw_sd_init();
#endif
#endif // BLYNKGO_USE_SD

#if defined(LDR_RESOLUTION)
  analogReadResolution(LDR_RESOLUTION);
#endif

#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK
  WiFi.onEvent(BlynkGO_WiFiEvent);  // เรียกภายในก่อน ส่งต่อไปให้ด้วย
#endif // BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK

#if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)
  this->ledRGB(0,0,0); // ให้ ledทำงานแบบดับสนิท
#else
#if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563
  #if defined(RTC_I2C_SDA)  && defined(RTC_I2C_SCL) // แบบกำหนดขา I2C แยกจาก Touch I2C
    Wire.begin(RTC_I2C_SDA, RTC_I2C_SCL);
    this->RTC.begin( &Wire);  // เรียกใช้ Wire1.begin(..)
  #elif defined(TOUCH_I2C_SCA) && defined(TOUCH_I2C_SCL) 
    // ภายใน Wire1 ของ Touch I2C ได้มีการ begin แล้ว จึงไม่ต้อง เรียก Wire1.begin(..) อีก
    this->RTC.begin( &Wire1 );  // แบบใช้ Wire I2C ขาร่วมกับ Touch I2C  // Wire ของ Touch มีการ begin(...) ไปก่อนแล้ว
  #endif

  // คำสั่งของ TimeLib.h
  setSyncProvider(rtc_timestamp);
  if (timeStatus() != timeSet){
    Serial.println("[RTC] Unable to sync system time with the RTC");
  }
  else{

#if BLYNKGO_USE_RTC_DS323X  
    Serial.printf("[RTC] DS323x inited to system time! --> %02d/%02d/%04d %02d:%02d:%02d\n", day(), month(),year(), hour(), minute(), second());
#elif BLYNKGO_USE_RTC_DS1307
    Serial.printf("[RTC] DS1307 inited to system time! --> %02d/%02d/%04d %02d:%02d:%02d\n", day(), month(),year(), hour(), minute(), second());
#elif BLYNKGO_USE_RTC_PCF8523
    Serial.printf("[RTC] PCF8523 inited to system time! --> %02d/%02d/%04d %02d:%02d:%02d\n", day(), month(),year(), hour(), minute(), second());
#elif BLYNKGO_USE_RTC_PCF8563
    Serial.printf("[RTC] PCF8563 inited to system time! --> %02d/%02d/%04d %02d:%02d:%02d\n", day(), month(),year(), hour(), minute(), second());
#endif
  }
#endif // #if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563

#endif // #if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)


#if defined(BEENEXT) || BLYNKGO_USE_BEENEXT
  #if defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)
    // Serial2.begin(9600, SERIAL_8N1, 35 /*input only*/ ,22);
    // BeeNeXT.begin(&Serial2);
  #else
  // #if defined(BEENEXT_4_3) || defined(BEENEXT_4_3C) | defined(BEENEXT_4_3IPS)
    BeeNeXT.begin();  // ทั่วไปใช้ Serial เป็นจุดเชื่อมต่อกับ MCU อื่น, แต่สำหรับ ESP32S3 แบบนี้จะใช้ Serial2 ในการเชื่อมต่อ
  #endif
#endif

#if defined (TOUCH_XPT2046)
  if( this->flashMem_exists("BLYNKGO_VER")) {
    String _blynkgo_version = this->flashMem_String("BLYNKGO_VER");
    if(_blynkgo_version != BLYNKGO_VERSION_TEXT) {
      this->touch_calibrate();
    }
  }else{
    this->flashMem("BLYNKGO_VER", String(BLYNKGO_VERSION_TEXT));   
    this->touch_calibrate();
  }
#endif

}

void BlynkGOv3::update(bool beenext_loop){
#if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_LEVEL_SUPERG
  for(int i=0; i< GUI_TASK_LOOP_NUM; i++) {
    lv_task_handler(); /* let the GUI do its work */
  }
#endif

#if defined(BEENEXT) || BLYNKGO_USE_BEENEXT
  if(beenext_loop) BeeNeXT.update();
#endif
  
  Alarm.serviceAlarms();  // จัดการ Alarm

#if BLYNKGO_USE_BLYNK
  if( blynk_config.enable == true) { 
    if( blynk_config.auth != "" && WiFi.isConnected()) 
      Blynk.run();
  }

  if(WiFi.isConnected()) {
    if(_blynkReconnect) {
      _blynkReconnect = false;
      Blynk.disconnect();
      Blynk.connect();
    }
  }
#endif

#if BLYNKGO_USE_WIFI
  if(_wifi_event_list.size()){
    WiFiEvent_t event = _wifi_event_list[0];
    _wifi_event_list.erase(_wifi_event_list.begin());

#if BLYNKO_USE_ASYNC_MQTT32
    static GTimer timer_mqtt_connection;
#endif

    //ส่ง event ต่อไปยัง ของ user's wifi_event_sys_cb ด้วย
    if(this->wifi_event_cb){
      this->wifi_event_cb(event);
    }
    //else {

      switch (event) {
        case SYSTEM_EVENT_STA_DISCONNECTED:
#if BLYNKGO_USE_NTP
          // ntp_timer.del();
          ntp_task.del();
#endif
#if BLYNKGO_USE_BLYNK
          if( Blynk.fn_user_blynk_disconnected != NULL)
            Blynk.fn_user_blynk_disconnected();
#endif

          WiFi.reconnect();

          WiFiOnDisconnected();                // BlynkGO System's WiFi disconnected function ---> WIFI_CONNECTED()
          if(pBlynkGO->wifi_disconnected_cb){
            pBlynkGO->wifi_disconnected_cb();  // User's WiFi disconnected function
          }
          break;
        case SYSTEM_EVENT_STA_GOT_IP: {
          // _event = SYSTEM_EVENT_MAX;

          WiFiOnConnected();                // BlynkGO System's WiFi connected function ---> WIFI_CONNECTED()
          if(pBlynkGO->wifi_connected_cb != NULL){
            pBlynkGO->wifi_connected_cb();  // User's WiFi connected function สำหรับใช้ตาม library เป็นต้น
          }

#if BLYNKGO_USE_NTP 
          if(pBlynkGO->autoNTP()){
            pBlynkGO->syncNTP();
          }
          // ntp_synced =false;
          // // Serial.println("configTime call");
          // // configTime(NTP_TIMEZONE * 3600, NTP_DAYLIGHT_SAVING * 3600, NTP_HOSTNAME1, NTP_HOSTNAME2, NTP_HOSTNAME3);
          // configTime(blynkgo_timezone * 3600, blynkgo_daylight_saving * 3600, NTP_HOSTNAME1, NTP_HOSTNAME2, NTP_HOSTNAME3);
          // ntp_task.del();
          // ntp_task.setInterval(1000L, ntp_sync_task, NULL, TASK_PRIO_HIGH);
#endif

#if BLYNKGO_USE_BLYNK
          if(blynk_config.ssid == "" && blynk_config.pass == "" ){  // แสดงว่าเชื่อมต่อ WiFi สำเร็จแบบ GWiFiManager
            Blynk.config(blynk_config.auth.c_str(), blynk_config.domain.c_str(), blynk_config.port);
          }
#endif 

#if BLYNKO_USE_ASYNC_MQTT32
          timer_mqtt_connection.delay(400,[](){ MQTT.connect(); });
#endif
          break;
        }
        case SYSTEM_EVENT_AP_START:
          SoftAPOnStart();                // BlynkGO System's WiFi SoftAP start function ---> SOFTAP_START()
          if(pBlynkGO->softab_start_cb){
            pBlynkGO->softab_start_cb();  // User's WiFi SoftAP start function
          }
          
          break;
        case SYSTEM_EVENT_AP_STOP:
          SoftAPOnStop();                 // BlynkGO System's WiFi SoftAP stop function ---> SOFTAP_STOP()
          if(pBlynkGO->softab_stop_cb){
            pBlynkGO->softab_stop_cb();   // User's WiFi SoftAP stop function
          }
          break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
          SoftAPOnStaConnected();       // BlynkGO System's WiFi SoftAP STA Connected function ---> SOFTAP_STA_CONNECTED()
          if(pBlynkGO->softab_sta_connected_cb){
            pBlynkGO->softab_sta_connected_cb();   // User's WiFi SoftAP STA Connected  function
          }
          break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
          SoftAPOnStaDisconnected();       // BlynkGO System's WiFi SoftAP STA Disconnected function ---> SOFTAP_STA_DISCONNECTED()
          if(pBlynkGO->softab_sta_disconnected_cb){
            pBlynkGO->softab_sta_disconnected_cb();   // User's WiFi SoftAP STA Connected  function
          }
          break;
        default: break;
      } // switch
    //} // else
  } // if(_wifi_event_list.size())

  // #if BLYNKGO_USE_MQTT
  //   if( _mqtt_connection_timer <= millis() ){
  //     _mqtt_connection_timer = millis() + 3000;
  //     if(WiFi.isConnected() && !MQTT.connected() ) MQTT.connect();
  //   }
  // #endif // BLYNKGO_USE_MQTT
#endif //BLYNKGO_USE_WIFI


#if defined(BLYNKGO_MINI)
  BTN1.loop();
  BTN2.loop();
#elif defined(BLYNKGO_M5STACK)
  BTN_A.loop();
  BTN_B.loop();
  BTN_C.loop();
#elif defined(BLYNKGO_LORA32) || BLYNKGO_USE_BUTTON_ISR
  BTN.loop();
#endif


#if (BLYNKGO_USE_WIFI && BLYNKGO_USE_NTP && BLYNKGO_USE_MQTT  && BLYNKGO_USE_QRPAYMENT )
    GBPrimePay.update();
#endif

}

#if defined(BLYNKGO_OLED)
#else
void  BlynkGOv3::rotation(uint8_t r){
  ESP_LOGI(TAG, "[Rotation] old rotation : %d", _rotation);

  r = r%4;
  if( r == _rotation ) return;

  
  if(this->flashMem_exists("TFT_ROTATE")) {
    this->flashMem_erase("TFT_ROTATE");
  }

  this->flashMem("TFT_ROTATE", r);
  _rotation = (uint8_t)this->flashMem_Int("TFT_ROTATE");
  
  ESP_LOGI(TAG, "[Rotation] set %d\n", _rotation);

#if defined(TOUCH_XPT2046)
  if( this->flashMem_exists("TOUCH_DATA")) {
    ESP_LOGI(TAG, "[Rotation] clear old touch data");
    this->flashMem_erase("TOUCH_DATA");
  }
#endif
  ESP_LOGI(TAG, "[Rotation] restart");
  ESP.restart();
}

uint8_t BlynkGOv3::rotation() {
  return _rotation;//lcd.getRotation();
}
#endif


#if defined (TOUCH_XPT2046)
/****************************************************************
 * API touch_calibrate() สำหรับ calibrate ระบบสัมผัสหน้าจอ แบบ resistive
 ****************************************************************/
void BlynkGOv3::touch_calibrate(){
  if (lcd.touch())
  {
    if (lcd.width() < lcd.height()) lcd.setRotation(lcd.getRotation() ^ 1);

    // 画面に案内文章を描画します。
    lcd.fillScreen(0x0000);
    lcd.setTextSize((std::max(lcd.width(), lcd.height()) + 255) >> 8);
    lcd.setTextColor(0xFFFF);
    lcd.setTextDatum(textdatum_t::middle_center);
    lcd.drawString("touch the arrow marker.", lcd.width() >> 1, lcd.height() >> 1);
    lcd.setTextDatum(textdatum_t::top_left);

    // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
    std::uint16_t fg = 0xFFFF;
    std::uint16_t bg = 0x0000;
    if (lcd.isEPD()) std::swap(fg, bg);

    uint16_t touch_data[8];
    lcd.calibrateTouch(touch_data, fg, bg, std::max(lcd.width(), lcd.height()) >> 3);
    // NVS.setObject("TOUCH_DATA", touch_data, sizeof(touch_data)*8); 
    this->flashMem("TOUCH_DATA", &touch_data, sizeof(touch_data));
    lcd.setRotation(this->flashMem_Int("TFT_ROTATE"));  // ตั้ง rotation คืนที่เคยกำหนด
    ESP_LOGI(TAG, "[TouchCalibrate] set to FlashMem Done!");
    GScreen.invalidate();
  }
}
#endif // TOUCH_XPT2046

#if defined(BLYNKGO_OLED)
// สำหรับ OLED
void BlynkGOv3::hw_lcd_init() {

#if defined(OLED_PIN)
  pinMode(OLED_PIN, OUTPUT);
  digitalWrite(OLED_PIN, HIGH);
#endif

  lcd.init();
  lcd.flipScreenVertically();
  lcd.setContrast(255);
  lcd.display();

  lcd_fb = (uint8_t*) esp32_malloc(lcd.width()*lcd.height()/8);

#if defined(BLYNKGO_LORA32)
  //SPI LoRa pins
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  //setup LoRa transceiver module
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
#endif // BLYNKGO_LORA32
}

#else  // #if !defined(BLYNKGO_OLED)
// สำหรับ TFT
void BlynkGOv3::hw_lcd_init(){
  lcd.init();

#if (TFT_BL != -1)
  // lcd.light()->init(255); 
  // #if defined(BLYNKGO_BOARD_V2)
  //   pinMode(TFT_BL, OUTPUT); digitalWrite(TFT_BL, HIGH);
  // #endif
  // พบแล้วหาก เปิดใช้ RTC ด้วย Wire จะไปเริ่มขา 21,22 ทำให้ชนกับ TFT_BL 21
  if(this->flashMem_exists("BRIGHTNESS")) {
    this->brightness( (uint8_t) (this->flashMem_Int("BRIGHTNESS") ), false );
  }else{
    this->brightness( 255, true );
  }
  Serial.printf("[BRIGHTNESS] %d\n", this->brightness());
#endif

#if !defined(BLYNKGO_OLED)
  // this->flashMem_erase("TFT_ROTATE");
  if(this->flashMem_exists("TFT_ROTATE") ){
    _rotation = (uint8_t) this->flashMem_Int("TFT_ROTATE");
    lcd.setRotation(_rotation);
  }else{
    lcd.setRotation(ROTATE_TYPE);
  }
  Serial.printf("[Rotation] %d\n", lcd.getRotation() );
#endif

#if defined(TOUCH_XPT2046)
  if (lcd.touch())
  {
    // this->flashMem_erase("TOUCH_DATA");
    if( this->flashMem_exists("TOUCH_DATA") ){
      uint16_t touch_data[8];
      this->flashMem_Array("TOUCH_DATA", &touch_data, sizeof(touch_data));
      lcd.setTouchCalibrate(touch_data);
      ESP_LOGI(TAG, "[Touch Data] FlashMem Load Done!");
    } else{
      this->touch_calibrate();
    }
  }
#endif //TOUCH_XPT2046

  // --- start logo by image ------------------
  // if( this->flashMem_exists("BLOGO")){
  //   uint8_t* blogo=NULL;
  //   blogo = (uint8_t*) NVS.getObject( "BLOGO", (void*) blogo);
  //   if( blogo ) {
  //     lcd.pushImage( lcd.width()/2 - 40,  lcd.height()/2 -40, 80, 80, (uint16_t*)blogo);
  //     free(blogo);
  //   }
  // }else{
    // --- start logo by text ------------------
    lcd.fillScreen(_TFT_BLACK);
    lcd.setTextSize((std::max(lcd.width(), lcd.height()) + 255) >> 9); //>> 8);
    lcd.setTextColor(0xFF0000U);
    lcd.drawString("C:> BlynkGOv3", 30, 16);
  // }
}
#endif // #if defined(BLYNKGO_OLED)

#if BLYNKGO_USE_SD
void BlynkGOv3::hw_sd_init() {
#if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_LEVEL_SD
#if defined (SD_VSPI_MODE) || defined(SD_FSPI_MODE)
  Serial.println("[SD] begin...");
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
#if defined BLYNKGO_ENTERPRISE
  if(!SD.begin(SD_CS, SPI, 25000000, "/sd", 20) ) {
#else
  if(!SD.begin(SD_CS, SPI, 40000000, "/sd", 20) ) {
#endif
    Serial.println("[SD] initialization failed!");
    this->_has_sd = false;
  }else{
    Serial.println("[SD] initialization OK");
    this->_has_sd = true;
  }
#elif defined (SD_HSPI_MODE)
  Serial.println("[SD] begin...");
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SD_HSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if(!SD.begin(SD_CS, SD_HSPI, 40000000, "/sd", 20) ) {
    Serial.println("[SD] initialization failed!");
    this->_has_sd = false;
  }else{
    Serial.println("[SD] initialization OK");
    this->_has_sd = true;
  }
#elif defined (SD_MMC_MODE)
  Serial.println("[SD MMC] begin...");
  if(!SD_MMC.begin("/sd", true)){
    Serial.println("[SD MMC] initialization failed!");
    this->_has_sd = false;
  }else{
    Serial.println("[SD MMC] initialization OK");
    this->_has_sd = true;
  }
#elif defined ()
#endif
#endif //#if BLYNKGO_DEV_LEVEL >= BLYNKGO_DEV_LEVEL_SD
}
#endif //BLYNKGO_USE_SD

#if BLYNKGO_USE_SNAPSHOT
//Snap function
bool lv_obj_snapshot(lv_obj_t* obj, String file_path )
{
  if(obj == NULL || file_path == "") return false;

  String f_path = file_path;
  bool spiffs_fs = false;

  
#if BLYNKGO_USE_SD
  // เปลี่ยน file_path เป็น /sd/... แทน
  if( file_path.startsWith("sd://") ||
      file_path.startsWith("SD://") ||
      file_path.startsWith(BLYNKGO_SD_DRIVE) ||
      file_path.startsWith("/") )
  {
    f_path.replace("sd://", "/");
    f_path.replace("SD://", "/");
    f_path.replace(BLYNKGO_SD_DRIVE, "/");
    // f_path = String("/sd") + f_path;
  } else 
#else
  if( file_path.startsWith("sd://") ||
      file_path.startsWith("SD://") ||
      file_path.startsWith(BLYNKGO_SD_DRIVE) ||
      file_path.startsWith("/") )
  {
    return false;
  } else 
#endif
  // เปลี่ยน file_path เป็น /spiffs/... แทน
  if( file_path.startsWith("spiffs://")  ||
      file_path.startsWith("SPIFFS://")  ||
      file_path.startsWith(BLYNKGO_SPIFFS_DRIVE) )
  {
    spiffs_fs = true;
    f_path.replace("spiffs://", "/");
    f_path.replace("SPIFFS://", "/");
    f_path.replace(BLYNKGO_SPIFFS_DRIVE, "/");
    // f_path = String("/spiffs") + f_path;
  }
  

  if( f_path.endsWith(".bmp") ) {
    capture_type = CAPTURE_TYPE_BMP;
  }else if( f_path.endsWith(".png") ) {
#if defined (BLYNKGO_BASIC)
    return false;
#else
    capture_type = CAPTURE_TYPE_PNG;
#endif
  }else{
    return false;
  }

  uint16_t w = lv_obj_get_width(obj);
  uint16_t h = lv_obj_get_height(obj);

  if( capture_type == CAPTURE_TYPE_BMP) {
    ESP_LOGI(TAG, "[Capture] by BMP");
    if(spiffs_fs){
      ESP_LOGI(TAG, "[Capture] file open on SPIFFS : %s", f_path.c_str());
      _file_bmp = SPIFFS.open(f_path, "w");
    }
#if BLYNKGO_USE_SD
    else{
      ESP_LOGI(TAG, "[Capture] file open on SD : %s", f_path.c_str());
      _file_bmp = SD.open(f_path, "w");
    }
#endif
    if( _file_bmp == NULL) return false;


    lgfx::bitmap_header_t bmpheader;
    // BMP 24 bit
    bmpheader.bfType        = 0x4D42;
    bmpheader.bfSize        = 3 * w * h + sizeof(bmpheader); // bmp 24bit -->ฉะนั้น 3byte:1pixel
    bmpheader.bfOffBits     = sizeof(bmpheader);
    bmpheader.biSize        = 40;
    bmpheader.biWidth       = w;
    bmpheader.biHeight      = h;
    bmpheader.biPlanes      = 1;
    bmpheader.biBitCount    = 24;  // 16, 24, 32
    bmpheader.biCompression = 0;

    _file_bmp.write((std::uint8_t*)&bmpheader, sizeof(bmpheader));
    capture_buffer = (uint8_t*)esp32_malloc(3*w);
    memset(capture_buffer, 0, 3*w);
    for(int i=0; i < h; i++){
      _file_bmp.write(capture_buffer, 3*w);
    }
    _file_pos = _file_bmp.position();  // ตำแหน่งสุดท้ายของ file ซึ่งก็จะคือ ขนาด file นี้ในที่สุด
    _h_pos    = 0;
    // ESP_LOGI(TAG, "[BMP] after header .. position : %d", _file_bmp.position());

    bSnapshot = true;
      lv_obj_invalidate(obj);
      lv_refr_now(lv_disp_get_default());  /* Will call our disp_drv.disp_flush function */
      // lv_refr_now(NULL);  /* Will call our disp_drv.disp_flush function */
    bSnapshot = false;

    if(capture_buffer ) {
      free(capture_buffer); 
      capture_buffer = NULL;
    }
    _file_bmp.close();
    ESP_LOGI(TAG, "[Capture] file close : %s", f_path.c_str());
    return true;
  }
  else if (capture_type == CAPTURE_TYPE_PNG) {
    ESP_LOGI(TAG, "[Capture] by PNG");
    png24_image = (uint8_t*)esp32_malloc(w * h *3);
    if(png24_image == NULL) {
      ESP_LOGE(TAG, "[Capture] malloc failed");
      return false;
    }
    capture_buffer = png24_image;

    bSnapshot = true;
      lv_obj_invalidate(obj);
      lv_refr_now(lv_disp_get_default());  /* Will call our disp_drv.disp_flush function */
      // lv_refr_now(NULL);  /* Will call our disp_drv.disp_flush function */
    bSnapshot = false;

    uint8_t error = lodepng_encode24_file( f_path.c_str(), png24_image, w , h);
    if (error) {
      Serial.printf("[BlynkGO] capture encoder error %u: %s\n", error, lodepng_error_text(error));
      return false;
    }

    Serial.println("[BlynkGO] capture save to " + file_path + " done");  
    free(png24_image); 
    png24_image = NULL;
    capture_buffer  = NULL;
    return true;
  }

  return false;  
}

#endif //USE_SNAPSHOT

#if defined(BLYNKGO_OLED)
void    BlynkGOv3::oled_threshold(uint8_t threshold)      { _oled_threshold = threshold; }
uint8_t BlynkGOv3::oled_threshold()                       { return _oled_threshold; }
#endif

// สำหรับ TFT && OLED
static void blynkgo_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  //  lcd.drawBitmap(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);                    // copy 'color_array' to the specifed coordinates
  //  lv_disp_flush_ready(disp);  

#if defined(BLYNKGO_USE_AGFX) && BLYNKGO_USE_AGFX

  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  lcd.draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
  lv_disp_flush_ready(disp);

#else

#if defined(BLYNKGO_OLED)
#else
  int w = (area->x2 - area->x1 + 1);
  int h = (area->y2 - area->y1 + 1);
  lcd.startWrite();                            /* Start new TFT transaction */
  lcd.setAddrWindow(area->x1, area->y1, w, h); /* set the working window */
  uint16_t c;
#endif
  // lcd.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);   //OK

  for (int y = area->y1; y <= area->y2; y++) {
#if BLYNKGO_USE_SNAPSHOT
    int w_i = 0;
#endif
    for (int x = area->x1; x <= area->x2; x++) {
#if defined(BLYNKGO_OLED)
      lcd.setColor(lv_color_to8(*color_p)>=_oled_threshold? WHITE : BLACK  );
      lcd.setPixel( x, y );
#else
      c = color_p->full;
      lcd.writeColor(c, 1);
#endif

#if BLYNKGO_USE_SNAPSHOT
      if(bSnapshot) { 
        switch(capture_type) {
          case CAPTURE_TYPE_BMP:
            capture_buffer[w_i++] = (uint8_t) (color_p->ch.blue  * 8 );
            capture_buffer[w_i++] = (uint8_t) (color_p->ch.green * 4 );
            capture_buffer[w_i++] = (uint8_t) (color_p->ch.red   * 8 );
            break;
          case CAPTURE_TYPE_PNG:
            capture_buffer[0] = color_p->ch.red   * 8;
            capture_buffer[1] = color_p->ch.green * 4;
            capture_buffer[2] = color_p->ch.blue  * 8;
            capture_buffer +=3;
            break;
        }
      }
#endif
      color_p++;
    }
#if BLYNKGO_USE_SNAPSHOT
    if(bSnapshot) { 
      if(capture_type == CAPTURE_TYPE_BMP) {
        int seek_pos = _file_pos - (++_h_pos)*3*GScreen.width();  //ย้ายตำแหน่งไปวางแถวบรรทัดล่างสุดไล่ขึ้นมาแถวด้านบน
        _file_bmp.seek( seek_pos);
        _file_bmp.write(capture_buffer, 3*GScreen.width());
      }
    }
#endif
  }

#if defined(BLYNKGO_OLED)
  lcd.display();
#else
  lcd.endWrite();            /* terminate TFT transaction */
#endif
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */

#endif //#if defined(BLYNKGO_USE_AGFX) && BLYNKGO_USE_AGFX

}


#if !defined (BLYNKGO_OLED)
static bool blynkgo_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
   int32_t touchX, touchY;
  if(lcd.getTouch(&touchX, &touchY)){
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
    _last_touch = millis();
    if( pBlynkGO->_fn_last_touch !=NULL ) pBlynkGO->_fn_last_touch();
  }else{
    data->state = LV_INDEV_STATE_REL;
  }
  /*Return `false` because we are not buffering and no more data to read*/
  return false;
}
#endif

void BlynkGOv3::blynkgo_system_init(){
  lv_init();

#if defined (BLYNKGO_BASIC)
  ESP_LOGI(TAG, "[BlynkGO] basic");
  lv_disp_buf_init(&disp_buf, _cbuf, NULL, LV_HOR_RES_MAX * 10);
#elif defined (BLYNKGO_ENTERPRISE)
  ESP_LOGI(TAG, "[BlynkGO] alloc");
  size_t buf_size = sizeof(lv_color_t)* LV_HOR_RES_MAX * 60;
  _cbuf = (lv_color_t*) esp32_malloc(buf_size);
  lv_disp_buf_init(&disp_buf, _cbuf, NULL, LV_HOR_RES_MAX * 60);  // 480/40 = 12 ครั้ง
#elif defined(BEENEXT_4_3) || defined(BEENEXT_4_3C) || defined(BEENEXT_4_3IPS) ||  defined(BEENEXT_5_0IPS) ||  defined(BEENEXT_7_0IPS)
  ESP_LOGI(TAG, "[BeeNeXT] alloc (ESP32S3)");
  _cbuf   = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * lcd.width() * lcd.height() / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  // _cbuf2  = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * lcd.width() * lcd.height() / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  // lv_disp_buf_init(&disp_buf, _cbuf, _cbuf2, lcd.width() * lcd.height()/ 8);
  lv_disp_buf_init(&disp_buf, _cbuf, NULL, lcd.width() * lcd.height()/ 8);
#else
  ESP_LOGI(TAG, "[BlynkGO] full alloc");
  size_t buf_size = sizeof(lv_color_t)* lcd.width() * lcd.height();
  _cbuf = (lv_color_t*) esp32_malloc(buf_size);
  lv_disp_buf_init(&disp_buf, _cbuf, NULL, lcd.width() * lcd.height()); // ห้ามใช้ buf_size ต้องใช้ จำนวน pixel ทั้งหมด
#endif

  /*Initialize the display*/
  lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = lcd.width();
    disp_drv.ver_res  = lcd.height();
    disp_drv.flush_cb = blynkgo_disp_flush;
    disp_drv.buffer   = &disp_buf;
    lv_disp_drv_register(&disp_drv);

#if defined(BLYNKGO_OLED)
#else
  lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = blynkgo_touchpad_read;
    indev = lv_indev_drv_register(&indev_drv);
#if defined(TOUCH_PERIOD)
  #if (TOUCH_PERIOD> 0)
    indev->driver.read_task->period = TOUCH_PERIOD;
  #endif
#endif // #if defined(TOUCH_PERIOD)

#endif //#if defined(BLYNKGO_OLED)
// #endif
  /*Initialize the graphics library's tick*/
  static Ticker tick; /* timer for interrupt handler */
  // tick.attach_ms(LVGL_TICK_PERIOD, lv_tick_handler);
  tick.attach_ms(LVGL_TICK_PERIOD, [](){ lv_tick_inc(LVGL_TICK_PERIOD); });


#if BLYNKGO_USE_FS
  lv_fs_esp32_if_init();    // เพิ่มระบบ filesystem สำหรับ กราฟิก  ; SD --> S:  ; SPIFFS --> F:
#endif

  GWidgets_init();

  // _is_blynkgo_system_inited  = true;

  // lv_task_handler(); /* let the GUI do its work */
  // for(int i=0; i< 5; i++) {
  //   lv_task_handler(); /* let the GUI do its work */
  //   delay(5);
  // }
}

font_t* BlynkGOv3::font_load(String font_path){

  if( (font_path.startsWith("SD://") || font_path.startsWith("sd://"))
      && font_path.endsWith(".bin") )
  {
    font_path.replace("sd://", BLYNKGO_SD_DRIVE);
    font_path.replace("SD://", BLYNKGO_SD_DRIVE);
    return lv_font_load( font_path.c_str());
  }else
  if( (font_path.startsWith("SPIFFS://") || font_path.startsWith("spiffs://"))
      && font_path.endsWith(".bin") )
  {
    font_path.replace("spiffs://", BLYNKGO_SPIFFS_DRIVE);
    font_path.replace("SPIFFS://", BLYNKGO_SPIFFS_DRIVE);
    return lv_font_load( font_path.c_str());
  }
  return NULL;
}

void BlynkGOv3::font_free(font_t * font){
  lv_font_free(font);
}

void BlynkGOv3::brightness(uint8_t level, bool save){
#if defined(BLYNKGO_OLED)
  lcd.setBrightness( level);
  if(save) this->flashMem("BRIGHTNESS", level);
#else
  #if TFT_BL != -1
    lcd.setBrightness( level);
    if(save) this->flashMem("BRIGHTNESS", lcd.getBrightness());
  #endif
#endif
}

uint8_t  BlynkGOv3::brightness(){
#if defined(BLYNKGO_OLED)
  return this->flashMem_Int("BRIGHTNESS");
#else
  #if TFT_BL != -1
    return lcd.getBrightness();
  #else
    return 255;
  #endif
#endif
}


#if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)
void BlynkGOv3::ledRGB(uint8_t r, uint8_t g, uint8_t b){
  static bool _led_pwm_inited = false;

  if(r==0 && g==0 && b==0) {
    if(_led_pwm_inited) {
      ledcDetachPin(LED_R);
      ledcDetachPin(LED_G);
      ledcDetachPin(LED_B);
      _led_pwm_inited = false;
    }
    pinMode(LED_R,OUTPUT); digitalWrite(LED_R, HIGH);
    pinMode(LED_G,OUTPUT); digitalWrite(LED_G, HIGH);
    pinMode(LED_B,OUTPUT); digitalWrite(LED_B, HIGH);
  }else{
    if(!_led_pwm_inited) {
      ledcSetup(LED_R_CHANNEL, 12000, 8);    ledcAttachPin(LED_R, LED_R_CHANNEL);
      ledcSetup(LED_G_CHANNEL, 12000, 8);    ledcAttachPin(LED_G, LED_G_CHANNEL);
      ledcSetup(LED_B_CHANNEL, 12000, 8);    ledcAttachPin(LED_B, LED_B_CHANNEL);
      _led_pwm_inited = true;
    }
    ledcWrite(LED_R_CHANNEL, 255-r);
    ledcWrite(LED_G_CHANNEL, 255-g);
    ledcWrite(LED_B_CHANNEL, 255-b);
  }
}
float BlynkGOv3::readLDR(){
  return constrain(map_f(analogRead(LDR_PIN),0,4095, 100, 0),0.0, 100.0);
}
#endif // #if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)

#if BLYNKGO_USE_SNAPSHOT
bool lv_obj_snapshot(lv_obj_t* obj, String file_path );
bool BlynkGOv3::capture(const char* filename) {
  bool ret;

  screen_timer_update();
  // GScreen.capture(filename);
  if(!NVS.exists("CAPTURE")){
    NVS.setInt("CAPTURE", (uint16_t) 0 );
  }

  Serial.println("[BlynkGO] capture screen...");
  if( filename == NULL) {
    uint16_t capture_no = (uint16_t) NVS.getInt("CAPTURE");
    char file_name[255];
    sprintf(file_name, CAPTURE_PATH_TEMPLATE, ++capture_no);
    // if( lv_snapshot(&this->Lcd, file_name, lv_scr_act()) ){
    ret = lv_obj_snapshot(lv_scr_act(), file_name ) ;
    if(ret ) {
      NVS.setInt("CAPTURE", (uint16_t) capture_no);
    }
  }else{
    ret = lv_obj_snapshot(lv_scr_act(), filename );
  }
  return ret;
}
#endif // BLYNKGO_USE_SNAPSHOT

bool  BlynkGOv3::flashMem_eraseAll(){
  uint64_t _bkey = 0;
  if( this->flashMem_exists("blynkgo")){
    _bkey = (uint64_t)this->flashMem_Int("blynkgo");
  }
  bool ret = NVS.eraseAll();
  if(_bkey != 0) this->regist(_bkey);
  return ret;
}

#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK
void BlynkGOv3::onWiFiEvent(WiFiEventCb cbEvent){
  this->wifi_event_cb = cbEvent;
}
void BlynkGOv3::onWiFiConnected(WiFiConnectedCb cbConnected){
  this->wifi_connected_cb = cbConnected;
}
void BlynkGOv3::onWiFiDisconnected(WiFiDisconnectedCb cbDisconnected){
  this->wifi_disconnected_cb = cbDisconnected;
}

void  BlynkGOv3::onSoftAPStart(SoftAPStartCb cbSoftAPStart){
  this->softab_start_cb = cbSoftAPStart;
}

void BlynkGOv3::onSoftAPStop(SoftAPStopCb  cbSoftAPStop){
  this->softab_stop_cb = cbSoftAPStop;
}

void BlynkGOv3::onSoftAPStaConnected(SoftAPStaConnectedCb cbSoftAPStaConnected){
  this->softab_sta_connected_cb= cbSoftAPStaConnected;
}

void BlynkGOv3::onSoftAPStaDisconnected(SoftAPStaDisconnectedCb cbSoftAPStaDisconnected){
  this->softab_sta_disconnected_cb= cbSoftAPStaDisconnected;
}

#if BLYNKGO_USE_NTP
void BlynkGOv3::onNTPSynced(NTPSyncedCb cbNtpSynced){
  this->ntp_synced_cb = cbNtpSynced;
}

bool BlynkGOv3::getLocalTime(struct tm &timeinfo){
  return get_timeinfo(&timeinfo);
}

bool BlynkGOv3::isNTPSynced(){
  return ntp_synced;
}

void BlynkGOv3::syncNTP(){
  if(!WiFi.isConnected()) return;

  ntp_synced =false;
  configTime(blynkgo_timezone * 3600, blynkgo_daylight_saving * 3600, NTP_HOSTNAME1, NTP_HOSTNAME2, NTP_HOSTNAME3);
  ntp_task.del();
  ntp_task.setInterval(1000L, ntp_sync_task, NULL, TASK_PRIO_HIGH);
}


void BlynkGOv3::alarmRepeat(time_t value, OnTick_t onTickHandler) {
  if(blynkgo_alarms.size() >=ALARM_ID_MAX ) return;
  blynkgo_alarm_t new_alarm;
    new_alarm.id            = ALARM_ID_INVALID;
    new_alarm.isOnce        = false;
    new_alarm.isAbsolute    = false;
    new_alarm.dow           = dowInvalid;
    new_alarm.secs_of_day   = value;
    new_alarm.onTickHandler = onTickHandler;
  blynkgo_alarms.push_back(new_alarm);

  BlynkGOv3::blynkgo_alarm_handler();
}

void BlynkGOv3::alarmRepeat(const int H, const int M, const int S, OnTick_t onTickHandler){
  this->alarmRepeat(AlarmHMS(H,M,S), onTickHandler);
}

void BlynkGOv3::alarmRepeat(const alarm_dow_t DOW, const int H, const int M, const int S, OnTick_t onTickHandler){
  if(blynkgo_alarms.size() >=ALARM_ID_MAX ) return;
  blynkgo_alarm_t new_alarm;
    new_alarm.id            = ALARM_ID_INVALID;
    new_alarm.isOnce        = false;
    new_alarm.isAbsolute    = false;
    new_alarm.dow           = (timeDayOfWeek_t) DOW;
    new_alarm.secs_of_day   = AlarmHMS(H,M,S);
    new_alarm.onTickHandler = onTickHandler;
  blynkgo_alarms.push_back(new_alarm);

  BlynkGOv3::blynkgo_alarm_handler();
}

void BlynkGOv3::alarmOnce(time_t value, OnTick_t onTickHandler, bool absolute_timestamp) {
  if(blynkgo_alarms.size() >=ALARM_ID_MAX ) return;
  blynkgo_alarm_t new_alarm;
    new_alarm.id            = ALARM_ID_INVALID;
    new_alarm.isOnce        = true;
    new_alarm.isAbsolute    = absolute_timestamp;
    new_alarm.dow           = dowInvalid;
    new_alarm.secs_of_day   = value;
    new_alarm.onTickHandler = onTickHandler;
  blynkgo_alarms.push_back(new_alarm);

  BlynkGOv3::blynkgo_alarm_handler();
}

void BlynkGOv3::alarmOnce(const int H, const int M, const int S, OnTick_t onTickHandler){
  this->alarmOnce(AlarmHMS(H,M,S), onTickHandler);
}

void BlynkGOv3::alarmOnce(const alarm_dow_t DOW, const int H, const int M, const int S, OnTick_t onTickHandler){
  if(blynkgo_alarms.size() >=ALARM_ID_MAX ) return;
  blynkgo_alarm_t new_alarm;
    new_alarm.id            = ALARM_ID_INVALID;
    new_alarm.isOnce        = true;
    new_alarm.isAbsolute    = false;
    new_alarm.dow           = (timeDayOfWeek_t) DOW;
    new_alarm.secs_of_day   = AlarmHMS(H,M,S);
    new_alarm.onTickHandler = onTickHandler;
  blynkgo_alarms.push_back(new_alarm);

  BlynkGOv3::blynkgo_alarm_handler();
}

void BlynkGOv3::blynkgo_alarm_handler(){
  if(!BlynkGO.isNTPSynced()) return;

  if( blynkgo_alarms.size()) {
    for(int i=0; i < blynkgo_alarms.size(); i++) {
      if(blynkgo_alarms[i].id == ALARM_ID_INVALID) {
        if(blynkgo_alarms[i].dow == dowInvalid) {
          if(blynkgo_alarms[i].isOnce) {
            if(blynkgo_alarms[i].isAbsolute){
                                    // ตั้งทำงาน ครั้งเดียว ในอนาคต
              blynkgo_alarms[i].id = Alarm.triggerOnce( blynkgo_alarms[i].secs_of_day,  blynkgo_alarms[i].onTickHandler);
            }else{
                                    // ตั้งทำงาน ครั้งเดียว ด้วยเวลา timestamp ภายในวัน
              blynkgo_alarms[i].id = Alarm.alarmOnce( blynkgo_alarms[i].secs_of_day,  blynkgo_alarms[i].onTickHandler);
            }
          }
          else{
                                    // ตั้งทำงาน ซ้ำๆ 
            blynkgo_alarms[i].id = Alarm.alarmRepeat( blynkgo_alarms[i].secs_of_day,  blynkgo_alarms[i].onTickHandler);
          }
        }else{
          if(blynkgo_alarms[i].isOnce)
            blynkgo_alarms[i].id = Alarm.alarmOnce( blynkgo_alarms[i].dow, blynkgo_alarms[i].secs_of_day,  blynkgo_alarms[i].onTickHandler);
          else
            blynkgo_alarms[i].id = Alarm.alarmRepeat( blynkgo_alarms[i].dow, blynkgo_alarms[i].secs_of_day,  blynkgo_alarms[i].onTickHandler);
        }
      }
    }
  }
}

void BlynkGOv3::timezone(float tz){
  if( blynkgo_timezone != tz) {
    blynkgo_timezone = tz;
    this->syncNTP();
  }
}

float BlynkGOv3::timezone() {
  return blynkgo_timezone;
}

void  BlynkGOv3::daylight_saving(float daylight){
  if( blynkgo_daylight_saving != daylight) {
    blynkgo_daylight_saving = daylight;
    this->syncNTP();
  }
}

float BlynkGOv3::daylight_saving(){
  return blynkgo_daylight_saving;
}
#endif //BLYNKGO_USE_NTP
#endif // BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK



//---------------------------------------
// static fucntion
#if BLYNKGO_USE_WIFI
#if BLYNKGO_USE_NTP
static void ntp_sync_task(void* param){
  // Serial.println("[ntp_sync_task]");
  if (!ntp_synced){
    if (!get_timeinfo(&timeinfo))  return;
  }

  ntp_synced =true;
  Serial.printf("[NTP] synced : %02d:%02d:%02d\n",
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  ntp_task.del();

  // กำหนดเวลาให้ TimeLib - TimeAlarms
  setTime( timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon+1, timeinfo.tm_year+1900-2000);
  BlynkGOv3::blynkgo_alarm_handler();

#if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)
#else
#if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563
  time_t now;
  time(&now);
  pBlynkGO->RTC.set(now + NTP_TIMEZONE*3600);
  setSyncProvider(rtc_timestamp);
  // setSyncProvider(pBlynkGO->RTC.get);
  Serial.printf("[RTC] auto-synced by NTP : %02d/%02d/%04d %02d:%02d:%02d\n", day(), month(),year(), hour(), minute(), second());
#endif //#if BLYNKGO_USE_RTC_DS323X || BLYNKGO_USE_RTC_DS1307 || BLYNKGO_USE_RTC_PCF8523 || BLYNKGO_USE_RTC_PCF8563
#endif //#if defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C)


#if defined (TTGO_TWATCH)
  TWatch.RTC.setDateTime(timeinfo.tm_year, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  Serial.printf("[RTC] auto-synced by NTP : %02d/%02d/%04d %02d:%02d:%02d\n", day(), month(),year(), hour(), minute(), second());
#endif

#if BLYNKGO_USE_NTP_RESYNC
  ntp_task.setInterval(NTP_INTERVAL_SYNC*60000,[](void*){
    Serial.println("[NTP] resyncing...");
    pBlynkGO->syncNTP();
    // ntp_synced = false;
    // // configTime(NTP_TIMEZONE * 3600, NTP_DAYLIGHT_SAVING * 3600, NTP_HOSTNAME1, NTP_HOSTNAME2, NTP_HOSTNAME3);
    // configTime(blynkgo_timezone * 3600, blynkgo_daylight_saving * 3600, NTP_HOSTNAME1, NTP_HOSTNAME2, NTP_HOSTNAME3);
    // ntp_task.del();
    // ntp_task.setInterval(1000L, ntp_sync_task);
  },NULL, TASK_PRIO_HIGH);
#endif // BLYNKGO_USE_NTP_RESYNC

  NTPOnSynced();                  // BlynkGO System's ntp_synced function
  if(pBlynkGO->ntp_synced_cb) {
    pBlynkGO->ntp_synced_cb();    // User's ntp_synced function
  }
}
#endif // BLYNKGO_USE_NTP
#endif // BLYNKGO_USE_WIFI


#if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK
static void BlynkGO_WiFiEvent(WiFiEvent_t event)
{

  //Send the event to the vector list
  _wifi_event_list.push_back(event);

  //ส่ง event ต่อไปยัง ของ user's wifi_event_sys_cb ด้วย
  // if(pBlynkGO->wifi_event_cb){
  //   pBlynkGO->wifi_event_cb(event);
  // }
}

// #if BLYNKGO_USE_MQTT
// static void Mqtt_onConnected(bool session)
// {
//   MqttOnConnected();
// }

// static void Mqtt_onDisconnected(AsyncMqttClientDisconnectReason reason)
// {
//   MqttOnDisconnected();
// }

// static void Mqtt_onSubscribed(uint16_t packetId, uint8_t qos)
// {
//   MqttOnSubscribed();
// }

// static void Mqtt_onUnsubscribed(uint16_t packetId)
// {
//   MqttOnUnsubscribed();
// }

// static void Mqtt_onPublished(uint16_t packetId)
// {
//   MqttOnPublished();
// }
// #endif 
#endif  // #if BLYNKGO_USE_WIFI || BLYNKGO_USE_BLYNK

