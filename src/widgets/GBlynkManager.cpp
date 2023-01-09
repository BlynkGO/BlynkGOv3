/**********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GBlynkManager.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

/*********************************************************************
 *
 * Version 1.0.0
 *    - Blynk Connection with green/red indicator for connection
 *
 * Version 1.0.1
 *    - add GSwitch for blynk auth enable
 *    - ta_blynk_auth set to password_mode
 *
 * Version 1.0.2
 *    - design for TTGO TWatch จอเล็ก
 *
 * Version 1.0.3
 *    - เพิ่มคำสั่ง save(..)
 *
 *********************************************************************/

#include "config/blynkgo_config.h"

#if BLYNKGO_USE_WIFI
#if BLYNKGO_USE_BLYNK

#include "GBlynkManager.h"
#include "BlynkGOv3.h"

#define BLYNKSIGNAL_RECHECK  10000L
static GBlynkManager* _pBlynkManager = NULL;

#define LV_OBJX_NAME  "GBlynkManager"

static lv_signal_cb_t ancestor_signal_cb;
static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if (res != LV_RES_OK ) return res;
  if (sign == LV_SIGNAL_GET_TYPE)
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  /* หลัง del() หรือ deconstrutor ถูกเรียก และภายในได้ลบ child ไปแล้ว */
  if (sign == LV_SIGNAL_CLEANUP) {
#if BLYNGO_DEBUG
    Serial.println("[GBlynkManager] signal cleanup");
#endif
    GBlynkManager* pBlynkManager = (GBlynkManager*) lv_obj_get_user_data(obj);
    gblynkmanager_ext_t* ext = (gblynkmanager_ext_t*) pBlynkManager->ext_attr();

//    free(ext->blynk_setting);           ext->blynk_setting    = NULL;
    if( ext->blynk_setting != NULL) {
      ext->blynk_setting->del(); free(ext->blynk_setting); ext->blynk_setting = NULL;
    }
    
    free_widget(ext->lb_blynk_status);  ext->lb_blynk_status  = NULL;

    if (pBlynkManager->hasLocalStyle()) { pBlynkManager->freeLocalStyle(); }
    pBlynkManager->_created = false;

    _pBlynkManager = NULL;
#if BLYNGO_DEBUG
    MEM_CHECK;
#endif
  }
  return LV_RES_OK;
}


void gblynkmanager_event_cb(lv_obj_t *obj, lv_event_t event);
//GTask  GBlynkManager::WiFiSignal_Task;

GBlynkManager::GBlynkManager(GWidget& parent) : GContainer(&parent) {}
GBlynkManager::GBlynkManager(GWidget* parent) : GContainer(parent) {}
GBlynkManager::~GBlynkManager() { this->del(); }


void GBlynkManager::create() {
  if (this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if (this->_par->obj == NULL) return;

  _pBlynkManager = NULL; // ให้ เริ่มเป็น NULL ไปก่อน สร้างเสร็จค่อยลิงค์ไปตัวสร้าง

  GBlynkSetting::load_BlynkConfig();
  
  if (!this->isCreated()) {
    GContainer::create();
    if (this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);//gblynkmanager_event_cb);  //

      this->layout(LAYOUT_NONE);
//      this->size(42,42);
      this->size(36,36);
      this->corner_radius(10);
      this->border(0);
//      this->opa(0);
      this->padding(5);
      this->clickable(true);
      this->color(Blynk.connected()? TFT_GREENBLYNK : TFT_REDBLYNK);
      this->align(ALIGN_CENTER, 0, 0);
      

      gblynkmanager_ext_t* ext = (gblynkmanager_ext_t*) this->alloc_ext_attr(sizeof(gblynkmanager_ext_t));

      ext->lb_blynk_status  = new_GLabel("B", this);
      ext->lb_blynk_status->align(ALIGN_CENTER);
      ext->lb_blynk_status->auto_realign(true);
      ext->lb_blynk_status->clickable(false);
      
//
//      ext->wifisetting = NULL;


      // WiFiConnected/WiFiDisconnect Callback function  ของ GBlynkManager เรียกทำงาน
//      BlynkGO.onWiFiConnected   ( GBlynkManager::onWiFiConnected );
//      BlynkGO.onWiFiDisconnected( GBlynkManager::onWiFiDisconnected);

      // GWiFiSetting::clean_WiFiConfig();
//      GWiFiSetting::autoconnection();


      //-----------------------------------------------------
      _pBlynkManager = this;
      Blynk.onConnected_UserCB([](){
        // Serial.println("[GBlynkManager] Blynk Connected");
        _pBlynkManager->color(TFT_GREENBLYNK);
        BlynkGO.update();
      });

      Blynk.onDisconnected_UserCB([](){
        // Serial.println("[GBlynkManager] Blynk Disconnected");
        _pBlynkManager->color(TFT_REDBLYNK);
        BlynkGO.update();
      });
      //-----------------------------------------------------


      if (ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      this->onClicked(GBlynkManager::onClicked_cb);
    }
  }
}


void GBlynkManager::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GBlynkManager::create(GWidget* parent) {
  this->_par = parent;
  create();
}

void GBlynkManager::save(String blynk_auth, String domain, uint16_t port) {
  GBlynkSetting::save_BlynkConfig(blynk_auth, domain, port);
  // BlynkGO.configBlynk(blynk_auth, domain, port);
  GBlynkSetting::BlynkConnection();
}

String GBlynkManager::auth(){
  return GBlynkSetting::blynk_config.auth;
}

String GBlynkManager::host(){
  return GBlynkSetting::blynk_config.host;
}

uint16_t GBlynkManager::port(){
  return GBlynkSetting::blynk_config.port;
}


void  GBlynkManager::reset()  { GBlynkSetting::clean_BlynkConfig(); }

// void GBlynkManager::font(font_t &font){
//   if(!this->isCreated()) this->create();

//   gblynkmanager_ext_t* ext = (gblynkmanager_ext_t*) this->ext_attr();
//   this->font(font);
//   ext->lb_blynk_status->align(ALIGN_CENTER);
// }

// void GBlynkManager::font(font_t &font, color_t color){
//   if(!this->isCreated()) this->create();
//   gblynkmanager_ext_t* ext = (gblynkmanager_ext_t*) this->ext_attr();
//   this->font(font, color);
//   ext->lb_blynk_status->align(ALIGN_CENTER);
// }

void GBlynkManager::onClicked_cb(GWidget*w){
  if( _pBlynkManager == NULL) return;

  gblynkmanager_ext_t* ext = (gblynkmanager_ext_t*) _pBlynkManager->ext_attr();
  if (ext == NULL) return;

  
  if( ext->blynk_setting != NULL) {
    // free_widget(ext->wifisetting);  ext->wifisetting = NULL;
    ext->blynk_setting->del(); free(ext->blynk_setting); ext->blynk_setting = NULL;
  }

  // ทำการสร้าง GWiFiSetting
  ext->blynk_setting = new_GBlynkSetting();
  ext->blynk_setting->create();

  ext->blynk_setting->onBack([](GWidget * w) {
    if (_pBlynkManager == NULL) return;
    gblynkmanager_ext_t* ext = (gblynkmanager_ext_t*) _pBlynkManager->ext_attr();

    // ทำลาย GBlynkSetting ลง พร้อมคืน ram
    // free_widget(ext->wifisetting);  ext->wifisetting = NULL;
    ext->blynk_setting->del(); free(ext->blynk_setting); ext->blynk_setting = NULL;

    // Serial.println("back to blynk manager");

    // ให้ กลับมา Blynk onConnected_UserCB/ onDisconnected_UserCB  ของ GBlynkManager ทำงานแทน
    // เมื่อตอนที่มีการสร้าง GWiFiSetting ที่จะถูกเปลี่ยนไปใช้ของ GWiFiSetting
    Blynk.onConnected_UserCB([](){
      // Serial.println("[GBlynkManager] Blynk Connected");
      _pBlynkManager->color(TFT_GREENBLYNK);
      BlynkGO.update();
    });

    Blynk.onDisconnected_UserCB([](){
      // Serial.println("[GBlynkManager] Blynk Disconnected");
      _pBlynkManager->color(TFT_REDBLYNK);
      BlynkGO.update();
    });

    // ให้เช็คสีของ สถานะ Blynk Connection
    _pBlynkManager->color(Blynk.connected()? TFT_GREENBLYNK : TFT_REDBLYNK);

  });
}
/*********************************************
   PSRAM Widget Alloc
 *********************************************/

GBlynkManager*   new_GBlynkManager(GWidget& parent){
  GBlynkManager* w = (GBlynkManager*) esp32_malloc(sizeof(GBlynkManager));
  new (w) GBlynkManager(parent);
  return w;
}

GBlynkManager*   new_GBlynkManager(GWidget *parent){
  GBlynkManager* w = (GBlynkManager*) esp32_malloc(sizeof(GBlynkManager));
  new (w) GBlynkManager(parent);
  return w;
}

#endif
#endif
