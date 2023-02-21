/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GWiFiManager.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

/*
 *********************************************************************
 * GWiFiManager 
 * Version 1.0.0
 *  คุณสมบัติ
 *    1.เชื่อมต่อได้แล้วจะมีการจำว่าเชื่อม WiFi Router ตัวไหน
 *      เมื่อ reboot ใหม่ก็จะกลับมาเชื่อมต่อ WiFi Router นั้น
 *      หาก มีการโจมตีด้วย ssid ชื่อเดียวกันซ้ำๆ ก็ยังกลับไปเชื่อมต่อ WiFi Router ตัวเดิมได้
 *    2.ตั้งค่า WiFi ได้ทั้งแบบ Auto IP และ Fixed IP
 *    3.มีระบบ WiFi Scan ค้นหา ไวไฟรอบๆ โดย scan ให้ทุกๆ 10 วินาที เมื่อกดค้นหา
 *
 * Version 1.0.1
 *    1. เปลี่ยน GRect+GLabel เป็น GButton แทน
 *
 * Version 1.0.2
 *    1. เพิ่ม API reset() สำหรับล้าง ข้อมูลเก่าได้
 *
 * Version 1.0.3
 *    1. เพิ่มให้ใส่ DNS IP แบบ Fixed IP เองได้
 *
 * Version 1.0.4
 *    1. ปรับ design ให้เข้ากับจอขนาดเล็กของ T-Watch ได้
 *
 * Version 1.0.5
 *    1. เพิ่มให้สามารถตั้ง save(String ssid, String pass)
 *
 * Version 1.0.6
 *    1. เพิ่มให้สามารถตั้ง color(...) สีของ สัญลักษณ์ไวไฟ เมื่อ connected, disconnected, และสีไวไฟที่เป็นพื้นหลังได้
 *
 * Version 1.0.7 @07/12/21
 *    1. แก้ที่แบบ fixed ip ( static ip ) แล้ว connection แสดงซ้ำ 2 รอบ
 *    2. ปรับให้ ใช้ prasanmit_40 ตั้งแต่ต้น จะได้มีขนาดใหญ่พอนิ้วจิ้มง่าย
 *
 * Version 1.0.8 @09/07/22
 *    1. ปรับขนาด font สำหรับ 800x480 Screen
 *
 * Version 1.0.9 @05/11/22
 *    1. ปรับขนาด สำหรับ 320x240 Screen
 *
 * Version 1.0.10 @10/11/22
 *    1. เพิ่ม API disconnectWiFi() สั่งตัดการเชื่อมต่อได้ ผ่าน API
 *    2. เพิ่ม API connectWiFi()    สั่งเชื่อมต่อไวไฟจากที่บันทึกไว้ ผ่าน API
 *
 * Version 1.0.11 @02/01/23
 *    1. wifi_manager.reset()  ทำการ reset ค่า wifi ทิ้ง พร้อมล้าง widget ข้อความต่างๆของ wifi_manager ออก
 *
 *********************************************************************
 */

#include "config/blynkgo_config.h"
#if BLYNKGO_USE_WIFI

#include "GWiFiManager.h"

#if defined(BLYNKGO_V2)
  #include "BlynkGOv2.h"
#elif defined(BLYNKGO_V3)
  #include "BlynkGOv3.h"
#endif


#define WIFISIGNAL_RECHECK  5000L
static GWiFiManager* _pWiFiManager = NULL;


#define LV_OBJX_NAME  "GWiFiManager"

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
    Serial.println("[GWiFiManager] signal cleanup");
#endif
    GWiFiManager* pWiFiManager = (GWiFiManager*) lv_obj_get_user_data(obj);
    gwifimanager_ext_t* ext = (gwifimanager_ext_t*) pWiFiManager->ext_attr();

    free_widget(ext->lb_wifi_signal[1]);  ext->lb_wifi_signal[1] = NULL;
    free_widget(ext->lb_wifi_signal[0]);  ext->lb_wifi_signal[0] = NULL;
    free(ext->wifisetting); ext->wifisetting = NULL;

    if (pWiFiManager->hasLocalStyle()) { pWiFiManager->freeLocalStyle(); }
    pWiFiManager->_created = false;
#if BLYNGO_DEBUG
    MEM_CHECK;
#endif
  }
  return LV_RES_OK;
}


void gwifimanager_event_cb(lv_obj_t *obj, lv_event_t event);
GTask  GWiFiManager::WiFiSignal_Task;

GWiFiManager::GWiFiManager(GWidget& parent) : GContainer(&parent) {}
GWiFiManager::GWiFiManager(GWidget* parent) : GContainer(parent) {}
GWiFiManager::~GWiFiManager() { this->del(); }


void GWiFiManager::create() {
  if (this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if (this->_par->obj == NULL) return;

  _pWiFiManager = NULL; // ให้ เริ่มเป็น NULL ไปก่อน สร้างเสร็จค่อยลิงค์ไปตัวสร้าง
  
  if (!this->isCreated()) {
    GContainer::create();
    if (this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gwifimanager_event_cb);  //main_event_cb);//

      this->layout(LAYOUT_NONE);
      this->border(0);
      this->opa(0);
      this->padding(5);
      this->align(ALIGN_CENTER, 0, 0);
      this->clickable(true);
      this->onClicked(GWiFiManager::onClicked_cb);

      gwifimanager_ext_t* ext = (gwifimanager_ext_t*) this->alloc_ext_attr(sizeof(gwifimanager_ext_t));

      ext->lb_wifi_signal[0]  = new_GLabel(this);
      ext->lb_wifi_signal[1]  = new_GLabel(ext->lb_wifi_signal[0]);

      ext->color[0] = TFT_DARKGRAY;
      ext->color[1] = TFT_RED;
      ext->color[2] = TFT_WHITE;


      ext->lb_wifi_signal[0]->color(ext->color[0]);
      ext->lb_wifi_signal[0]->clickable(false);
     *ext->lb_wifi_signal[0] = SYMBOL_WIFI_4;

      ext->lb_wifi_signal[1]->color(ext->color[1]);
      ext->lb_wifi_signal[1]->clickable(false);
     *ext->lb_wifi_signal[1] = SYMBOL_WIFI_NO_CON_1;

      ext->wifisetting = NULL;

      this->font(prasanmit_40);

      // WiFiConnected/WiFiDisconnect Callback function  ของ GWiFiManager เรียกทำงาน
      BlynkGO.onWiFiConnected   ( GWiFiManager::onWiFiConnected );
      BlynkGO.onWiFiDisconnected( GWiFiManager::onWiFiDisconnected);

      _pWiFiManager = this;
      // GWiFiSetting::clean_WiFiConfig();
      GWiFiSetting::autoconnection();

      if (ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}


void GWiFiManager::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GWiFiManager::create(GWidget* parent) {
  this->_par = parent;
  create();
}

// void GWiFiManager::body_draw(bool enable){
//   if(!this->isCreated()) create();
//   gwifimanager_ext_t* ext = (gwifimanager_ext_t*)this->ext_attr();
//   ext->body_draw = !!enable;
//   if(ext->body_draw){
//     this->opa(255);
//     this->border_opa(255);
//     this->corner_radius(15);
//     this->padding(5);
//   }else{
//     this->opa(0);
//     this->border_opa(0);
//     this->padding(5);
//   }
// }

// bool GWiFiManager::body_draw(){
//   if(!this->isCreated()) create();
//   gwifimanager_ext_t* ext = (gwifimanager_ext_t*)this->ext_attr();
//   return ext->body_draw;
// }

void GWiFiManager::font(font_t& font) {
  if (!this->isCreated()) create();
  gwifimanager_ext_t* ext = (gwifimanager_ext_t*) this->ext_attr();
  ext->lb_wifi_signal[0]->font(font);
  ext->lb_wifi_signal[1]->font(font);
  // if(this->body_draw()) { this->fit(FIT_TIGHT); this->body_draw(); }
}

void  GWiFiManager::reset()  { 
  GWiFiSetting::clean_WiFiConfig();
  WiFi.disconnect(true,true); // bool WiFiSTAClass::disconnect(bool wifioff, bool eraseap)
}

String GWiFiManager::ssid(){
    return GWiFiSetting::wifi_config.ssid;
}

String GWiFiManager::password(){
    return GWiFiSetting::wifi_config.password;
}

void GWiFiManager::color(color_t color_wifi_connected, color_t color_wifi_disconnected, color_t color_wifi_background){
  if (!this->isCreated()) create();
  gwifimanager_ext_t* ext = (gwifimanager_ext_t*) this->ext_attr();
  ext->color[0] = color_wifi_background;
  ext->color[1] = color_wifi_disconnected;
  ext->color[2] = color_wifi_connected;

  ext->lb_wifi_signal[0]->color(ext->color[0]);
  ext->lb_wifi_signal[1]->color(WiFi.isConnected()? ext->color[2] :ext->color[1]);
}

void GWiFiManager::bg_color(color_t color){
  if (!this->isCreated()) create();
  GContainer::color(color);
}

void GWiFiManager::bg_color(color_t main_color,color_t grad_color){
  if (!this->isCreated()) create();
  GContainer::color(main_color, grad_color);
}

color_t GWiFiManager::bg_color(){
  if (!this->isCreated()) create();
  return GContainer::color();
}

void GWiFiManager::save(String ssid, String pass)   {  
  GWiFiSetting::save_WiFiConfig(ssid, pass);
}

void  GWiFiManager::connectWiFi(){
  GWiFiSetting::connect_WiFi(true);
}

void  GWiFiManager::disconnectWiFi(){
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  GWiFiSetting::onWiFiDisconnected();
}

// class's static member
void GWiFiManager::onWiFiConnected() {
  //  if( ancestor_wifi_connected!=NULL) ancestor_wifi_connected();
  //  Serial.println("GWiFiManager OnConnected");

  ewm_state = EWM_STATE_CONNECTED;  
  static uint32_t _timer;
  if( millis() <  _timer) return;
  _timer = millis() + 1000;

  Serial.printf("[GWiFiManager] WiFi Connected. IP : %s, Gateway : %s, Subnet : %s\n",
                  WiFi.localIP().toString().c_str(),
                  WiFi.gatewayIP().toString().c_str(),
                  WiFi.subnetMask().toString().c_str());
  GWiFiSetting::save_WiFiConfig();  // บันทึก ssid ที่เชื่อมต่อได้สำเร็จ

  if (_pWiFiManager == NULL) return;
  gwifimanager_ext_t* ext = (gwifimanager_ext_t*) _pWiFiManager->ext_attr();

  if (ext == NULL) return;

  // ext->lb_wifi_signal[1]->color(TFT_WHITE);
  // *ext->lb_wifi_signal[1] = (const char*) wifi_signal_symbol(WiFi.RSSI());   // เปลี่ยนระดับสัญญาณความแรง WiFi เป็น อักขระแล้วใส่ของใน glabel แสดงสัญญาณไวไฟ ระดับความแรง
  
  GWiFiManager::WiFiSignalUpdate_Task_cb();
  GWiFiManager::WiFiSignal_Task.setInterval( WIFISIGNAL_RECHECK, GWiFiManager::WiFiSignalUpdate_Task_cb );
}

// class's static member
void GWiFiManager::onWiFiDisconnected() {
  //  if( ancestor_wifi_disconnected!=NULL) ancestor_wifi_disconnected();
  //  Serial.println("GWiFiManager OnDisConnected");
  static uint32_t _timer;
  if( millis() <  _timer) return;
  _timer = millis() + 1000;

  if(ewm_state != EWM_STATE_DISCONNECTED) {
    Serial.println("[GWiFiManager] WiFi Disonnected");
    ewm_state = EWM_STATE_DISCONNECTED;
  }

  if (_pWiFiManager == NULL) return;
  gwifimanager_ext_t* ext = (gwifimanager_ext_t*) _pWiFiManager->ext_attr();
  if (ext == NULL) return;

  // ext->lb_wifi_signal[1]->color(TFT_RED);
  // *ext->lb_wifi_signal[1] = SYMBOL_WIFI_NO_CON_1;
  GWiFiManager::WiFiSignalUpdate_Task_cb();  
  GWiFiManager::WiFiSignal_Task.del();
}

const void* wifi_signal_symbol(int8_t rssi ) {
  int8_t wifi_signal =  constrain(100 + rssi, 0, 90);

  if ( wifi_signal >= 0 && wifi_signal < 30 ) {
    return SYMBOL_WIFI_1;
  } else if ( wifi_signal >= 30 && wifi_signal < 40 ) {
    return SYMBOL_WIFI_2;
  } else if ( wifi_signal >= 40 && wifi_signal < 50 ) {
    return SYMBOL_WIFI_3;
  } else if ( wifi_signal >= 50 ) {
    return SYMBOL_WIFI_4;
  }
}

void GWiFiManager::WiFiSignalUpdate_Task_cb(void*) {
  if (_pWiFiManager == NULL) return;
  gwifimanager_ext_t* ext = (gwifimanager_ext_t*) _pWiFiManager->ext_attr();
  if (ext == NULL) return;

  if(WiFi.isConnected()) {
    ext->lb_wifi_signal[1]->color(ext->color[2]);
    *ext->lb_wifi_signal[1] = (const char*) wifi_signal_symbol(WiFi.RSSI());   // เปลี่ยนระดับสัญญาณความแรง WiFi เป็น อักขระแล้วใส่ของใน glabel แสดงสัญญาณไวไฟ ระดับความแรง
  }else{
    ext->lb_wifi_signal[1]->color(ext->color[1]);
    *ext->lb_wifi_signal[1] = SYMBOL_WIFI_NO_CON_1;
  }
}



void  GWiFiManager::onClicked_cb(GWidget* widget){
  if( _pWiFiManager == NULL) return;

  GWiFiManager::WiFiSignal_Task.del();  // สัญญาณความแรงไวไฟ เดิมที่เคยทำงานอยู่ให้หยุดไปก่อน
  gwifimanager_ext_t* ext = (gwifimanager_ext_t*) _pWiFiManager->ext_attr();
  if (ext == NULL) return;

  if( ext->wifisetting != NULL) {
    // free_widget(ext->wifisetting);  ext->wifisetting = NULL;
    ext->wifisetting->del(); free(ext->wifisetting); ext->wifisetting = NULL;
  }

  // ทำการสร้าง GWiFiSetting
  ext->wifisetting   = new_GWiFiSetting();
  ext->wifisetting->create();

  ext->wifisetting->onBack([](GWidget * w) {
    if (_pWiFiManager == NULL) return;
    gwifimanager_ext_t* ext = (gwifimanager_ext_t*) _pWiFiManager->ext_attr();

    // ทำลาย GWiFiSetting ลง พร้อมคืน ram
    // free_widget(ext->wifisetting);  ext->wifisetting = NULL;
    ext->wifisetting->del(); free(ext->wifisetting); ext->wifisetting = NULL;

    // ให้ กลับมา WiFiConnected/WiFiDisconnect Callback function  ของ GWiFiManager ทำงานแทน
    // เมื่อตอนที่มีการสร้าง GWiFiSetting ที่จะถูกเปลี่ยนไปใช้ของ GWiFiSetting
    BlynkGO.onWiFiConnected   ( GWiFiManager::onWiFiConnected );
    BlynkGO.onWiFiDisconnected( GWiFiManager::onWiFiDisconnected);

    // ให้เช็คสีของ wifi signal อีกทีนึง จากสถานะ WiFi Connection
    ext->lb_wifi_signal[1]->color(WiFi.isConnected()? ext->color[2] : ext->color[1]);

    // ให้ การทำงาน GTask สำหรับเช็ความแรง wifi signal กลับมาทำงานเป็นช่วงๆ อีกครั้ง
    if (WiFi.isConnected()) {
      GWiFiManager::WiFiSignalUpdate_Task_cb();
      GWiFiManager::WiFiSignal_Task.setInterval( WIFISIGNAL_RECHECK, GWiFiManager::WiFiSignalUpdate_Task_cb );
    }
  });
}



void gwifimanager_event_cb(lv_obj_t *obj, lv_event_t event) {
  GWiFiManager* _pWiFiManager = (GWiFiManager*) lv_obj_get_user_data(obj);
  gwifimanager_ext_t* ext = (gwifimanager_ext_t*) _pWiFiManager->ext_attr();
  static color_t body_main_color_origin;
  static color_t body_grad_color_origin;
  // static color_t text_color_origin;
  static color_t border_color_origin;

  if(event == EVENT_PRESSED) {
    if(_pWiFiManager->opa()) {
      body_main_color_origin  = _pWiFiManager->bg_color(); //color();
      body_grad_color_origin  = _pWiFiManager->grad_color();
      border_color_origin     = _pWiFiManager->border_color();

      _pWiFiManager->color( TFT_COLOR_MIX(body_main_color_origin, TFT_BLACK, 150 ),
                            TFT_COLOR_MIX(body_grad_color_origin, TFT_BLACK, 150 ));
      _pWiFiManager->border_color(TFT_COLOR_MIX(border_color_origin, TFT_BLACK, 150 ));
    }
  }else
  if(event == EVENT_RELEASED || event == EVENT_PRESS_LOST){
    if(_pWiFiManager->opa()) {
      _pWiFiManager->bg_color(body_main_color_origin,body_grad_color_origin); //color(body_main_color_origin,body_grad_color_origin);
      _pWiFiManager->border_color(border_color_origin);
    }
  }
  main_event_cb(obj, event);
}





/*********************************************
   PSRAM Widget Alloc
 *********************************************/

GWiFiManager*  new_GWiFiManager(GWidget& parent) {
  GWiFiManager* w = (GWiFiManager*) esp32_malloc(sizeof(GWiFiManager));
  new (w) GWiFiManager(parent);
  return w;
}

GWiFiManager*  new_GWiFiManager(GWidget *parent) {
  GWiFiManager* w = (GWiFiManager*) esp32_malloc(sizeof(GWiFiManager));
  new (w) GWiFiManager(parent);
  return w;
}


#endif //BLYNKGO_USE_WIFI
