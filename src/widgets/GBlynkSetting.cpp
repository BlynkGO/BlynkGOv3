/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GBlynkSetting.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#if defined(THAI_TEXT)
  #define TEXT_BLYNK_SETTING   "ตั้งค่า Blynk"
  #define TEXT_BLYNK_WAITING   "รอสักครู่..."
  #define TEXT_BLYNK_STD_HOST  "Blynk Host มาตรฐาน"
#elif defined(ENG_TEXT)
  #define TEXT_BLYNK_SETTING   "Blynk Setting"
  #define TEXT_BLYNK_WAITING   "wait a minute..."
  #define TEXT_BLYNK_STD_HOST  "Standard Blynk Host"
#elif defined(LAOS_TEXT)
  #define TEXT_BLYNK_SETTING   "ຕັ້ງ​ຄ່າ Blynk"
  #define TEXT_BLYNK_WAITING   "ລໍຖ້າຈັກນ້ອຍ..." // รอถ้าจักหน่อย
  #define TEXT_BLYNK_STD_HOST  "Blynk Host ມາດຕະຖານ"
#elif defined(VIETNAM_TEXT)
  #define TEXT_BLYNK_SETTING   "Cài đặt Blynk"
  #define TEXT_BLYNK_WAITING   "Vui lòng đợi..."
  #define TEXT_BLYNK_STD_HOST  "Blynk Quốc Tế" 
    // "Blynk Quốc Tế" for button on and  "Blynk tuỳ chỉnh" for button off   //  "Blynk mặc định" 
#endif

#define MARGIN_LEFT                 20
#define MARGIN_RIGHT                20
#define MARGIN_TOP                  10
#define MARGIN_BOTTOM               20
#define TITLE_HEIGHT                60

#define WAIT_A_MINUTE               1500L

#define BLYNK_DEFAULT_STD_DOMAIN    "blynk-cloud.com"
#define BLYNK_DEFAULT_STD_PORT      80
#define BLYNK_DEFAULT_STD_PORT_SSL  443

//#define BLYNK_DEFAULT_AUTH          "HOgczPCWrCXTY6X6pzCdOzkIzIbszTIU"
#define BLYNK_DEFAULT_MAN_DOMAIN    "blynk.jpnet.co.th"
#define BLYNK_DEFAULT_MAN_PORT      8080;

static GBlynkSetting* _pBlynkSetting = NULL;
//extern GBlynkManager* _pBlynkManager;

extern bool _blynkReconnect;
static GTimer timer_blynksetting;
static bool _blynksetting_first_create=false;

#define LV_OBJX_NAME  "GBlynkSetting"

static lv_signal_cb_t ancestor_signal_cb;
static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if(res != LV_RES_OK ) return res;
  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  /* หลัง del() หรือ deconstrutor ถูกเรียก และภายในได้ลบ child ไปแล้ว */
  if(sign == LV_SIGNAL_CLEANUP) {
#if BLYNGO_DEBUG
    Serial.println("[GBlynkSetting] signal cleanup");
#endif
    GBlynkSetting* pBlynkSetting = (GBlynkSetting*) lv_obj_get_user_data(obj);
    gblynksetting_ext_t* ext = (gblynksetting_ext_t*) pBlynkSetting->ext_attr();

    free_widget(ext->ta_blynk_port);          ext->ta_blynk_port          = NULL;
    free_widget(ext->lb_blynk_port);          ext->lb_blynk_port          = NULL;
    free_widget(ext->ta_blynk_host);          ext->ta_blynk_host          = NULL;
    free_widget(ext->lb_blynk_host);          ext->lb_blynk_host          = NULL;
    free_widget(ext->cont_man_host);          ext->cont_man_host          = NULL;

    free_widget(ext->sw_std_host);            ext->sw_std_host            = NULL;
    free_widget(ext->lb_std_host);            ext->lb_std_host            = NULL;
    free_widget(ext->cont_std_host);          ext->cont_std_host          = NULL;    

    free_widget(ext->ta_blynk_auth);          ext->ta_blynk_auth          = NULL;
    free_widget(ext->lb_blynk_auth);          ext->lb_blynk_auth          = NULL;
    free_widget(ext->sw_blynk_auth_enable);   ext->sw_blynk_auth_enable   = NULL;
    free_widget(ext->cont_blynk_auth);        ext->cont_blynk_auth        = NULL;   
    
    free_widget(ext->page_body);              ext->page_body              = NULL;

    free_widget(ext->lb_blynk_status);        ext->lb_blynk_status        = NULL;
    free_widget(ext->btn_blynk_status);       ext->btn_blynk_status       = NULL;
    
    free_widget(ext->lb_wait_a_minute);       ext->lb_wait_a_minute  = NULL;
    free_widget(ext->lb_blynksetting_title);  ext->lb_blynksetting_title  = NULL;
    free_widget(ext->btn_back);               ext->btn_back               = NULL;

    if(pBlynkSetting->hasLocalStyle()) {pBlynkSetting->freeLocalStyle(); } 
    pBlynkSetting->_created = false;

    _pBlynkSetting = NULL;
#if BLYNGO_DEBUG
    MEM_CHECK;
#endif
  }
  return LV_RES_OK;
}

blynkmanager_config_t GBlynkSetting::blynk_config;

GBlynkSetting::GBlynkSetting(GWidget& parent) : GObject(&parent) {}
GBlynkSetting::GBlynkSetting(GWidget* parent) : GObject(parent) {}
GBlynkSetting::~GBlynkSetting() { this->del(); }

void GBlynkSetting::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  _pBlynkSetting = NULL;

  if(!this->isCreated()) {
    this->obj = lv_obj_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      _blynksetting_first_create = true;
//      GBlynkSetting::clean_BlynkConfig();
      GBlynkSetting::load_BlynkConfig();
//      GBlynkSetting::display_BlynkConfig();
      
      this->size(GScreen);                // ให้ขนาด เท่ากับหน้าจอหลัก
      this->color(TFT_COLOR(28,28,28));
      this->clickable(false);             // ไม่ให้รับสัญณาณสัมผัส เพื่อส่งต่อไปยังแม่ที่มี gesture
      this->align(ALIGN_CENTER,0,0);

      gblynksetting_ext_t* ext = (gblynksetting_ext_t*) this->alloc_ext_attr(sizeof(gblynksetting_ext_t)); 

      //-----------------------------------------------------
      /* จอง mem จาก PSRAM */
      // ส่วน Title Bar
      ext->btn_back               = new_GButton(SYMBOL_ARROW_LEFT, this);
      ext->lb_blynksetting_title  = new_GLabel(TEXT_BLYNK_SETTING, this);
      ext->lb_wait_a_minute       = new_GLabel(TEXT_BLYNK_WAITING, this);
      
//      ext->btn_blynk_status       = new_GButton( "B", this); 
      ext->btn_blynk_status       = new_GRect( this);
      ext->lb_blynk_status        = new_GLabel( "B", ext->btn_blynk_status);

      // ส่วน Body
      ext->page_body              = new_GPage(this);

      // กลุ่ม Blynk Auth. ใน Body
      ext->cont_blynk_auth        = new_GContainer(ext->page_body);
      ext->lb_blynk_auth          = new_GLabel("Blynk Auth. :"     , ext->cont_blynk_auth);
      ext->ta_blynk_auth          = new_GTextArea(ext->cont_blynk_auth);
      ext->sw_blynk_auth_enable   = new_GSwitch(ext->cont_blynk_auth);

      // กลุ่ม STD. Blynk Host ใน Body
      ext->cont_std_host          = new_GContainer(ext->page_body);
      ext->lb_std_host            = new_GLabel(TEXT_BLYNK_STD_HOST, ext->cont_std_host);
      ext->sw_std_host            = new_GSwitch(ext->cont_std_host);

      // กลุ่ม Manual Blynk Host
      ext->cont_man_host          = new_GContainer(ext->page_body);
      ext->lb_blynk_host          = new_GLabel("Blynk Host :", ext->cont_man_host);
      ext->ta_blynk_host          = new_GTextArea(ext->cont_man_host);
      ext->lb_blynk_port          = new_GLabel("Blynk Port :", ext->cont_man_host);
      ext->ta_blynk_port          = new_GTextArea(ext->cont_man_host);
      
      //-----------------------------------------------------
      /* ส่วนออกแบบ  */
      /* ส่วน title bar */
#if defined(TTGO_TWATCH)
      ext->btn_back->size(45,45); 
      ext->btn_back->align(ALIGN_TOP_LEFT,0,5);
#else
      ext->btn_back->size(50,50); 
      ext->btn_back->align(ALIGN_TOP_LEFT,5,5);
#endif
      ext->btn_back->opa(0); 
      ext->btn_back->border(0);
      ext->btn_back->rounded(true); 
      ext->btn_back->padding(0); 
      ext->btn_back->style[PRESSED].color( TFT_WHITE); 
      ext->btn_back->style[PRESSED].opa( 32); 
      ext->btn_back->user_data(this);

#if defined(LAOS_TEXT)
      ext->lb_blynksetting_title->font(saysettha_25, TFT_WHITE);
#elif defined(VIETNAM_TEXT)
      ext->lb_blynksetting_title->font(arial_vietnam_30, TFT_WHITE);
#endif

#if defined(TTGO_TWATCH)
      ext->lb_blynksetting_title->align(ext->btn_back, ALIGN_RIGHT, 2);
#else
      ext->lb_blynksetting_title->align(ext->btn_back, ALIGN_RIGHT, 10);
#endif

//      ext->btn_blynk_status->size(42,42);
      ext->btn_blynk_status->size(36,36);
      ext->btn_blynk_status->corner_radius(10);
      ext->btn_blynk_status->color(Blynk.connected()? TFT_GREENBLYNK : TFT_REDBLYNK);
      ext->btn_blynk_status->border(0);
      ext->btn_blynk_status->padding(0);
      int8_t padding = (TITLE_HEIGHT - ext->btn_blynk_status->width())/2;
      ext->btn_blynk_status->align(ALIGN_TOP_RIGHT,-padding,padding);
      ext->btn_blynk_status->user_data(this);

      ext->lb_blynk_status->create();

      ext->lb_wait_a_minute->align( ext->btn_blynk_status, ALIGN_LEFT, -10);
      
#if defined(LAOS_TEXT)
      ext->lb_wait_a_minute->font(saysettha_15, TFT_WHITE);
#elif defined(VIETNAM_TEXT)
      ext->lb_wait_a_minute->font(arial_vietnam_20, TFT_WHITE);
#else
      ext->lb_wait_a_minute->font(prasanmit_20, TFT_WHITE);
#endif
      ext->lb_wait_a_minute->opa(180);
      ext->lb_wait_a_minute->hidden(true);
      
//      ext->lb_wait_a_minute->hidden(Blynk.connected());
//      if(!Blynk.connected()){
//        timer_blynksetting.setOnce(WAIT_A_MINUTE, [](){
//          if(_pBlynkSetting == NULL) return;
//          gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
//          if(ext == NULL) return;
//          ext->lb_wait_a_minute->hidden(true);
//        });
//      }

      /* ส่วน body */
      ext->page_body->size(BlynkGO.width(), BlynkGO.height()-TITLE_HEIGHT);
      ext->page_body->align(ALIGN_BOTTOM);
      ext->page_body->layout(LAYOUT_COL_M);
      ext->page_body->font_color(TFT_WHITE);
      ext->page_body->color(TFT_COLOR(50,50,50));
      ext->page_body->corner_radius(0);
#if defined(TTGO_TWATCH)
      ext->page_body->padding(8);
      ext->page_body->padding_bottom(15);
#else
      ext->page_body->padding(15);
      ext->page_body->padding_bottom(30);
#endif

      /* ส่วน กลุ่ม blynk auth */
      ext->cont_blynk_auth->fit(FIT_FLOOD, FIT_TIGHT);
      ext->cont_blynk_auth->layout(LAYOUT_NONE);
      ext->cont_blynk_auth->corner_radius(10);
      ext->cont_blynk_auth->color(TFT_COLOR(120,120,120));
      ext->cont_blynk_auth->padding_bottom(25);

      ext->lb_blynk_auth->align(ALIGN_TOP_LEFT, MARGIN_LEFT, MARGIN_TOP);
      ext->ta_blynk_auth->width(ext->cont_blynk_auth->width()-MARGIN_LEFT-MARGIN_RIGHT);
      ext->ta_blynk_auth->border_part(BORDER_BOTTOM);                         // TextArea นี้ ให้มีเส้นขอบเฉพาะด้านล่าง
      ext->ta_blynk_auth->border(2, TFT_RED);                                 // TextArea นี้ ให้มีเส้นขอบ 2 pixel และ สีแดง
      ext->ta_blynk_auth->placeholder("Blynk Authorization");                        // ข้อความบน TextArea เพื่ออธิบายว่า ใส่ค่าอะไร
      ext->ta_blynk_auth->placeholder_color(TFT_SILVER,200);                  // สีและ opa ของ placeholder
      ext->ta_blynk_auth->password_mode(true);
      ext->ta_blynk_auth->password_show_time(0);
      ext->ta_blynk_auth->cursor_type(CURSOR_NONE);                           // ไม่ให้แสดง cursor
      ext->ta_blynk_auth->opa(0);                                             // กำหนดโปร่งใส
      ext->ta_blynk_auth->align(ext->lb_blynk_auth, ALIGN_BOTTOM_LEFT);

      ext->sw_blynk_auth_enable->align(ALIGN_TOP_RIGHT, -MARGIN_RIGHT, MARGIN_TOP);
      ext->sw_blynk_auth_enable->state(GBlynkSetting::blynk_config.blynk_auth_enable);
      ext->sw_blynk_auth_enable->user_data(ext);

      /* ส่วน กลุ่ม switch เลือก blynk standard/manual host */
      ext->cont_std_host->fit(FIT_FLOOD, FIT_TIGHT);
      ext->cont_std_host->layout(LAYOUT_NONE);
      ext->cont_std_host->corner_radius(ext->cont_blynk_auth->corner_radius());
      ext->cont_std_host->color(ext->cont_blynk_auth->color());
      ext->cont_std_host->padding_bottom(MARGIN_TOP);

      ext->sw_std_host->align(ALIGN_TOP_RIGHT, -MARGIN_RIGHT, MARGIN_TOP);
      ext->sw_std_host->user_data(ext);

#if defined (LAOS_TEXT)
      ext->cont_std_host->font(saysettha_25, TFT_WHITE);
      // ext->lb_std_host->align(ALIGN_TOP_LEFT , MARGIN_LEFT, MARGIN_TOP );
#elif defined (VIETNAM_TEXT)
      ext->cont_std_host->font(arial_vietnam_30, TFT_WHITE);
#else
#endif
      // ext->lb_std_host->align(ALIGN_TOP_LEFT , MARGIN_LEFT, MARGIN_TOP );
      // Serial.println(TEXT_BLYNK_SETTING);
      ext->lb_std_host->align(ALIGN_LEFT , MARGIN_LEFT);
      ext->lb_std_host->text(TEXT_BLYNK_STD_HOST);

      uint16_t max_lb_std_host_len = ext->sw_std_host->posX() - MARGIN_LEFT - MARGIN_RIGHT;
      if(ext->lb_std_host->width() > max_lb_std_host_len)
        ext->lb_std_host->long_mode(LABEL_LONG_SROLL, max_lb_std_host_len);

      /* ส่วน กลุ่ม manual blynk host/port */
      ext->cont_man_host->fit(FIT_FLOOD, FIT_TIGHT);
      ext->cont_man_host->layout(LAYOUT_NONE);
      ext->cont_man_host->linkStyle(ext->cont_blynk_auth);
      

      ext->lb_blynk_host->align(ALIGN_TOP_LEFT, MARGIN_LEFT, MARGIN_TOP);
      ext->ta_blynk_host->width(ext->cont_blynk_auth->width()-MARGIN_LEFT-MARGIN_RIGHT);
      ext->ta_blynk_host->border_part(BORDER_BOTTOM);                         // TextArea นี้ ให้มีเส้นขอบเฉพาะด้านล่าง
      ext->ta_blynk_host->border(2, TFT_RED);                                 // TextArea นี้ ให้มีเส้นขอบ 2 pixel และ สีแดง
      ext->ta_blynk_host->placeholder("Blynk Server");                        // ข้อความบน TextArea เพื่ออธิบายว่า ใส่ค่าอะไร
      ext->ta_blynk_host->placeholder_color(TFT_SILVER,200);                  // สีและ opa ของ placeholder
      ext->ta_blynk_host->cursor_type(CURSOR_NONE);                           // ไม่ให้แสดง cursor
      ext->ta_blynk_host->opa(0);                                             // กำหนดโปร่งใส
      ext->ta_blynk_host->align(ext->lb_blynk_host, ALIGN_BOTTOM_LEFT);
      
      ext->lb_blynk_port->align(ext->ta_blynk_host, ALIGN_BOTTOM_LEFT, 0, 10);
      ext->ta_blynk_port->width(ext->cont_blynk_auth->width()-MARGIN_LEFT-MARGIN_RIGHT);
      ext->ta_blynk_port->border_part(BORDER_BOTTOM);                         // TextArea นี้ ให้มีเส้นขอบเฉพาะด้านล่าง
      ext->ta_blynk_port->border(2, TFT_RED);                                 // TextArea นี้ ให้มีเส้นขอบ 2 pixel และ สีแดง
      ext->ta_blynk_port->placeholder("Blynk Port");                        // ข้อความบน TextArea เพื่ออธิบายว่า ใส่ค่าอะไร
      ext->ta_blynk_port->placeholder_color(TFT_SILVER,200);                  // สีและ opa ของ placeholder
      ext->ta_blynk_port->cursor_type(CURSOR_NONE);                           // ไม่ให้แสดง cursor
      ext->ta_blynk_port->opa(0);                                             // กำหนดโปร่งใส
      ext->ta_blynk_port->align(ext->lb_blynk_port, ALIGN_BOTTOM_LEFT);


      ext->ta_blynk_auth->text(GBlynkSetting::blynk_config.auth);
      ext->ta_blynk_host->text(GBlynkSetting::blynk_config.host);
      ext->ta_blynk_port->text(String(GBlynkSetting::blynk_config.port));
      
      //-----------------------------------------------------
      ext->ta_blynk_auth->onValueChanged(GBlynkSetting::ta_onValueChanged);
      ext->ta_blynk_host->onValueChanged(GBlynkSetting::ta_onValueChanged);
      ext->ta_blynk_port->onValueChanged(GBlynkSetting::ta_onValueChanged);
      //-----------------------------------------------------
      // จัดการพวก event เมื่อการเปลี่ยนแปลงค่า/ มีการ clicked
      ext->btn_back->onClicked([](GWidget*w){
        GBlynkSetting* pBlynkSetting = (GBlynkSetting*) w->user_data();
        if( pBlynkSetting == NULL) return;

        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) pBlynkSetting->ext_attr();
        pBlynkSetting->hidden(true);
        if( pBlynkSetting->fn_onback !=NULL) pBlynkSetting->fn_onback(pBlynkSetting);
      });

      ext->sw_std_host->state(GBlynkSetting::blynk_config.std_host_enable);
      ext->cont_man_host->hidden(GBlynkSetting::blynk_config.std_host_enable);

      ext->sw_std_host->onValueChanged([](GWidget*w){
        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) w->user_data();
        bool sw_std_host = ext->sw_std_host->isON();
        ext->cont_man_host->hidden( sw_std_host );

        if( GBlynkSetting::blynk_config.std_host_enable != sw_std_host) {
          GBlynkSetting::blynk_config.std_host_enable = sw_std_host;
          GBlynkSetting::save_BlynkConfig();

          ext->lb_wait_a_minute->hidden(false);
          timer_blynksetting.setOnce(WAIT_A_MINUTE, [](){
            if(_pBlynkSetting == NULL) return;
            gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
            if(ext == NULL) return;
            
            ext->lb_wait_a_minute->hidden(true);
          });
//          Serial.println("blynk connect from sw changed");
          GBlynkSetting::BlynkConnection();
        }
      });



      //-----------------------------------------------------
      _pBlynkSetting = this;
      Blynk.onConnected_UserCB([](){
//        Serial.println("[GBlynkSetting] Blynk Connected");
        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
        ext->btn_blynk_status->color(TFT_GREENBLYNK);
        ext->lb_wait_a_minute->hidden(true);
        BlynkGO.update();
      });

      Blynk.onDisconnected_UserCB([](){
        Serial.println("[GBlynkSetting] Blynk Disconnected");
        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
        ext->btn_blynk_status->color(TFT_REDBLYNK);
        BlynkGO.update();
      });

      //-----------------------------------------------------
      // มีการเปลี่ยนแปลง วิตเจ็ตสวิตซ์เลื่อน ให้ Blynk Auth ทำงานไหม
      ext->sw_blynk_auth_enable->onValueChanged([](GWidget*w){
        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) w->user_data();

        bool sw_blynk_auth_en = ext->sw_blynk_auth_enable->isON();

        // Serial.printf("ext->sw_blynk_auth_enable : %d\n", sw_blynk_auth_en);
        if(GBlynkSetting::blynk_config.blynk_auth_enable != sw_blynk_auth_en) {
          GBlynkSetting::blynk_config.blynk_auth_enable = sw_blynk_auth_en;
          if(GBlynkSetting::blynk_config.blynk_auth_enable) {
            // Serial.println("[ext->sw_blynk_auth_enable : ON] trying.. blynk connection");
            GBlynkSetting::BlynkConnection();
          }else{
            // Serial.println("[ext->sw_blynk_auth_enable : OFF] turn off blynk connection");
            BlynkGO.enableBlynk(false);
          }
        }
        GBlynkSetting::save_BlynkConfig(); 
      });
      //-----------------------------------------------------
      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
      
    }
  }
}


void GBlynkSetting::create(GWidget& parent){
  this->_par = &parent;
  create();
}

void GBlynkSetting::create(GWidget* parent) {
  this->_par = parent;
  create();
}

void GBlynkSetting::hidden(bool enable){
  if(!this->isCreated()) this->create();
  
  GObject::hidden(enable);
}

bool GBlynkSetting::hidden(){
  if(!this->isCreated()) this->create();
  
  return GObject::hidden();
}


void GBlynkSetting::ta_onValueChanged(GWidget*w){
  if(_pBlynkSetting == NULL) return;
  gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
  if(ext == NULL) return;

//   Serial.println("ta_onValueChanged call");
  if(ext->ta_blynk_auth->sameWith(w)){
    if( ext->ta_blynk_auth->text() != String(GBlynkSetting::blynk_config.auth) ){
      snprintf( GBlynkSetting::blynk_config.auth, sizeof(GBlynkSetting::blynk_config.auth), "%s", ext->ta_blynk_auth->text().c_str());
      GBlynkSetting::save_BlynkConfig();
//      Blynk.disconnect();
      ext->lb_wait_a_minute->hidden(false);
      timer_blynksetting.setOnce(WAIT_A_MINUTE, [](){
        if(_pBlynkSetting == NULL) return;
        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
        if(ext == NULL) return;

        ext->lb_wait_a_minute->hidden(true);
      });
//      Serial.println("[ta auth] เรียก");
      GBlynkSetting::BlynkConnection();
    }
  }else
  if(ext->ta_blynk_host->sameWith(w)){
    if( ext->ta_blynk_host->text() != String(GBlynkSetting::blynk_config.host) ){
      snprintf( GBlynkSetting::blynk_config.host, sizeof(GBlynkSetting::blynk_config.host), "%s", ext->ta_blynk_host->text().c_str());
      GBlynkSetting::save_BlynkConfig();
      ext->lb_wait_a_minute->hidden(false);
      timer_blynksetting.setOnce(WAIT_A_MINUTE, [](){
        if(_pBlynkSetting == NULL) return;
        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
        if(ext == NULL) return;
        ext->lb_wait_a_minute->hidden(true);
      });
//      Serial.println("[ta host] เรียก");
      GBlynkSetting::BlynkConnection();
    }
  }else
  if(ext->ta_blynk_port->sameWith(w)){
    if( ext->ta_blynk_port->text().toInt() != GBlynkSetting::blynk_config.port ){
      GBlynkSetting::blynk_config.port = ext->ta_blynk_port->text().toInt();
      GBlynkSetting::save_BlynkConfig();
      ext->lb_wait_a_minute->hidden(false);
      timer_blynksetting.setOnce(WAIT_A_MINUTE, [](){
        if(_pBlynkSetting == NULL) return;
        gblynksetting_ext_t* ext = (gblynksetting_ext_t*) _pBlynkSetting->ext_attr();
        if(ext == NULL) return;
        ext->lb_wait_a_minute->hidden(true);
      });
//      Serial.println("[ta port] เรียก");
      GBlynkSetting::BlynkConnection();
    }
  }
}

void  GBlynkSetting::load_BlynkConfig(){
  if(BlynkGO.flashMem_exists("BLYNKCONFIG_SZ")){
    uint32_t config_sz = (uint32_t)BlynkGO.flashMem_Int("BLYNKCONFIG_SZ");
    // Serial.printf("[blynk config] flash blynk sz : %d <--> blynk_config sz ; %d\n", config_sz, sizeof(blynkmanager_config_t) );
    if( config_sz != sizeof(blynkmanager_config_t) ) {
      GBlynkSetting::clean_BlynkConfig();
    }
  }

  memset(&GBlynkSetting::blynk_config, 0, sizeof(blynkmanager_config_t));

  if(BlynkGO.flashMem_exists("BLYNKCONFIG")){
   // Serial.println("[GBlynkSetting] load config.");
    blynkmanager_config_t * blob = NULL;
    blob = (blynkmanager_config_t*) BlynkGO.flashMem_Object("BLYNKCONFIG", (void*)blob);
    memcpy(&GBlynkSetting::blynk_config, blob, sizeof(GBlynkSetting::blynk_config));
    free(blob); blob = NULL;
  }else{
   Serial.println("[GBlynkSetting] load Default config.");
    memset(  GBlynkSetting::blynk_config.auth, 0, sizeof(GBlynkSetting::blynk_config.auth));
//    snprintf( GBlynkSetting::blynk_config.auth, sizeof(GBlynkSetting::blynk_config.auth), "%s", BLYNK_DEFAULT_AUTH );
//    memset(  GBlynkSetting::blynk_config.host, 0, sizeof(GBlynkSetting::blynk_config.host));
    snprintf( GBlynkSetting::blynk_config.host, sizeof(GBlynkSetting::blynk_config.host), "%s", BLYNK_DEFAULT_MAN_DOMAIN);
    GBlynkSetting::blynk_config.port = BLYNK_DEFAULT_MAN_PORT;
    GBlynkSetting::blynk_config.std_host_enable  = true;
    GBlynkSetting::blynk_config.blynk_auth_enable = false;
  }

  GBlynkSetting::BlynkConnection();
}


void GBlynkSetting::save_BlynkConfig() {
//  Serial.println("[GBlynkSetting] save config");

  BlynkGO.flashMem("BLYNKCONFIG",     &GBlynkSetting::blynk_config, sizeof(GBlynkSetting::blynk_config));
  BlynkGO.flashMem("BLYNKCONFIG_SZ" , sizeof(GBlynkSetting::blynk_config));
}

void GBlynkSetting::save_BlynkConfig(String blynk_auth, String domain, uint16_t port){
  snprintf( GBlynkSetting::blynk_config.auth, sizeof(GBlynkSetting::blynk_config.auth), "%s", blynk_auth.c_str());
  snprintf( GBlynkSetting::blynk_config.host, sizeof(GBlynkSetting::blynk_config.host), "%s", domain.c_str());
  GBlynkSetting::blynk_config.port = port;

  BlynkGO.flashMem("BLYNKCONFIG",     &GBlynkSetting::blynk_config, sizeof(GBlynkSetting::blynk_config));
  BlynkGO.flashMem("BLYNKCONFIG_SZ" , sizeof(GBlynkSetting::blynk_config));
}

void  GBlynkSetting::clean_BlynkConfig(){
//  Serial.println("[GBlynkSetting] clean config");
  BlynkGO.flashMem_erase("BLYNKCONFIG");
  BlynkGO.flashMem_erase("BLYNKCONFIG_SZ");
}

void  GBlynkSetting::display_BlynkConfig(){
  Serial.printf("std blynk host : %s\n"
                "blynk auth     : %s\n"
                "blynk host     : %s\n"
                "blynk port     : %d\n"
                "blynk enable   : %s\n",
                  GBlynkSetting::blynk_config.std_host_enable? "true" : "false",
                  GBlynkSetting::blynk_config.auth,
                  GBlynkSetting::blynk_config.host,
                  GBlynkSetting::blynk_config.port,
                  GBlynkSetting::blynk_config.blynk_auth_enable);
}

void GBlynkSetting::BlynkConnection(){
  // Serial.println("BlynkConnection ...");
  if( !GBlynkSetting::blynk_config.blynk_auth_enable ) return; // กันไม่ให้มีการเชื่อมต่อ

  if( String(GBlynkSetting::blynk_config.auth) != "") {
    if( GBlynkSetting::blynk_config.std_host_enable == true) {
//      Serial.println("[BlynkConnection] configBlynk by standard host");
      BlynkGO.configBlynk( GBlynkSetting::blynk_config.auth, BLYNK_DEFAULT_STD_DOMAIN, BLYNK_DEFAULT_STD_PORT);
    }else{
//      Serial.println("[BlynkConnection] configBlynk by manual host");
      BlynkGO.configBlynk( GBlynkSetting::blynk_config.auth, GBlynkSetting::blynk_config.host, GBlynkSetting::blynk_config.port);
    }
  }
  
  if( WiFi.isConnected()){
//    Blynk.disconnect();
    if( GBlynkSetting::blynk_config.std_host_enable == true) {
//      Serial.println("[BlynkConnection] connect by std host");
      Blynk.config(GBlynkSetting::blynk_config.auth, BLYNK_DEFAULT_STD_DOMAIN, BLYNK_DEFAULT_STD_PORT);
//      Blynk.connect();
    }else{
//      Serial.println("[BlynkConnection] connect by man host");
      Blynk.config(GBlynkSetting::blynk_config.auth, GBlynkSetting::blynk_config.host, GBlynkSetting::blynk_config.port);
//      Blynk.connect();
    }
    
    BlynkGO.update();

    
    if(_blynksetting_first_create ) {
//      Serial.println("[เรียกครั้งแรก ไม่ให้ทำงาน]");
      _blynksetting_first_create = false;
    }else{
//      Serial.println("ทดลองเชื่อม blynk");
      _blynkReconnect = true; // ส่งคำสั่งไปให้ทำการ reconnect
    }
  }
}

/*********************************************
   PSRAM Widget Alloc
 *********************************************/

GBlynkSetting*   new_GBlynkSetting(GWidget& parent){
  GBlynkSetting* w = (GBlynkSetting*) esp32_malloc(sizeof(GBlynkSetting));
  new (w) GBlynkSetting(parent);
  return w;
}

GBlynkSetting*   new_GBlynkSetting(GWidget *parent){
  GBlynkSetting* w = (GBlynkSetting*) esp32_malloc(sizeof(GBlynkSetting));
  new (w) GBlynkSetting(parent);
  return w;
}


#endif
#endif
