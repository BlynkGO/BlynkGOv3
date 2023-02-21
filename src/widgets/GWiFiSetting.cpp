/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GWiFiSetting.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


FONT_DECLARE(easywifimanager_40);
#define SYMBOL_WIFISCAN     "\xef\x80\x82"
#define SYMBOL_WIFISTATUS   "\xef\x82\x9e"

#define MARGIN_LEFT         20
#define MARGIN_RIGHT        20
#define MARGIN_TOP          10
#define MARGIN_BOTTOM       20
#define WIFISCAN_TIMEOUT    10000L

#if defined(THAI_TEXT)
#define TEXT_WIFI_SETTING   "ตั้งค่า WiFi"
#define TEXT_WIFI_INFO      "ข้อมูลการเชื่อมต่อ"
#elif defined(ENG_TEXT)
#define TEXT_WIFI_SETTING   "WiFi Setting"
#define TEXT_WIFI_INFO      "WiFi Info"
#elif defined(LAOS_TEXT)
// FONT_DECLARE(saysettha_25);
#define TEXT_WIFI_SETTING   "ຕັ້ງຄ່າ WiFi"
#define TEXT_WIFI_INFO      "ຂໍ້ມູນການເຊື່ອມຕໍ່"
#elif defined(VIETNAM_TEXT)
#define TEXT_WIFI_SETTING   "Cài đặt WiFi"
#define TEXT_WIFI_INFO      "Thông tin kết nối"
#endif

static GWiFiSetting* _pWiFiSetting = NULL;
extern const void* wifi_signal_symbol(int8_t rssi );

static int8_t scan_response;
static int    scan_networks_num;
static GTask  WiFiScan_Task;
static GTask  WiFiScan_StatusCheck_Task;
static GTimer wifisetting_timer;
static bool   is_enterprise_size=false;
uint32_t      wifiscan_timeout;

ewm_state_t ewm_state=EWM_STATE_DISCONNECTED;

#define LV_OBJX_NAME  "GWiFiSetting"

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
    Serial.println("[GWiFiSetting] signal cleanup");
#endif
    GWiFiSetting* pWiFiSetting = (GWiFiSetting*) lv_obj_get_user_data(obj);
    gwifisetting_ext_t* ext = (gwifisetting_ext_t*) pWiFiSetting->ext_attr();

    free_widget(ext->lb_wifiinfo);          ext->lb_wifiinfo            = NULL;
    free_widget(ext->page_wifiinfo);        ext->page_wifiinfo          = NULL;

    free_widget(ext->list_wifiscan);        ext->list_wifiscan          = NULL;
    free_widget(ext->lb_wifi_msgbox_status);ext->lb_wifi_msgbox_status  = NULL;
    free_widget(ext->lb_wifi_msgbox_title); ext->lb_wifi_msgbox_title   = NULL;
    free_widget(ext->btn_wifi_msgbox_back); ext->btn_wifi_msgbox_back   = NULL;
    free_widget(ext->app_wifi_msgbox);      ext->app_wifi_msgbox        = NULL;
  
    free_widget(ext->rect_disable_click);   ext->rect_disable_click     = NULL;

    free_widget(ext->ta_dns_ip);            ext->ta_dns_ip              = NULL;
    free_widget(ext->lb_dns_ip);            ext->lb_dns_ip              = NULL;
    free_widget(ext->ta_subnet);            ext->ta_subnet              = NULL;
    free_widget(ext->lb_subnet);            ext->lb_subnet              = NULL;
    free_widget(ext->ta_gateway_ip);        ext->ta_gateway_ip          = NULL;
    free_widget(ext->lb_gateway_ip);        ext->lb_gateway_ip          = NULL;
    free_widget(ext->ta_fix_ip);            ext->ta_fix_ip              = NULL;
    free_widget(ext->lb_fix_ip);            ext->lb_fix_ip              = NULL;
    free_widget(ext->cont_fix_ip);          ext->cont_fix_ip            = NULL;
    free_widget(ext->sw_auto_ip);           ext->sw_auto_ip             = NULL;
    free_widget(ext->lb_auto_ip);           ext->lb_auto_ip             = NULL;
    free_widget(ext->cont_auto_ip);         ext->cont_auto_ip           = NULL;
    
    free_widget(ext->sw_ssid_enable);       ext->sw_ssid_enable         = NULL;
    free_widget(ext->ta_password);          ext->ta_password            = NULL;
    free_widget(ext->lb_pass_info);         ext->lb_pass_info           = NULL;
    free_widget(ext->ta_ssid);              ext->ta_ssid                = NULL;
    free_widget(ext->lb_ssid_info);         ext->lb_ssid_info           = NULL;
    free_widget(ext->cont_ssid);            ext->cont_ssid              = NULL;
    
    free_widget(ext->page_body);            ext->page_body              = NULL;

    free_widget(ext->btn_wifistatus);       ext->btn_wifistatus         = NULL;
    free_widget(ext->btn_wifiscan);         ext->btn_wifiscan           = NULL;
    free_widget(ext->lb_wifisetting_title); ext->lb_wifisetting_title   = NULL;
    free_widget(ext->btn_back);             ext->btn_back               = NULL;

    if(pWiFiSetting->hasLocalStyle()) {pWiFiSetting->freeLocalStyle(); } 
    pWiFiSetting->_created = false;

    _pWiFiSetting = NULL;
#if BLYNGO_DEBUG
    MEM_CHECK;
#endif
  }
  return LV_RES_OK;
}

wifimanager_config_t GWiFiSetting::wifi_config;

GWiFiSetting::GWiFiSetting(GWidget& parent) : GObject(&parent) {}
GWiFiSetting::GWiFiSetting(GWidget* parent) : GObject(parent) {}
GWiFiSetting::~GWiFiSetting() { this->del(); }

void GWiFiSetting::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  _pWiFiSetting = NULL;

  if(!this->isCreated()) {
    this->obj = lv_obj_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      is_enterprise_size = (BlynkGO.width()*BlynkGO.height() >=800*480);

      GWiFiSetting::autoconnection();

      this->size(GScreen);                // ให้ขนาด เท่ากับหน้าจอหลัก
      this->color(TFT_COLOR(28,28,28));
      this->clickable(false);       // ไม่ให้รับสัญณาณสัมผัส เพื่อส่งต่อไปยังแม่ที่มี gesture
      this->align(ALIGN_CENTER,0,0);

      gwifisetting_ext_t* ext = (gwifisetting_ext_t*) this->alloc_ext_attr(sizeof(gwifisetting_ext_t)); 

      /* จอง mem จาก PSRAM */
      // ส่วน Title Bar
      ext->btn_back             = new_GButton(SYMBOL_ARROW_LEFT, this);
      ext->lb_wifisetting_title = new_GLabel( TEXT_WIFI_SETTING, this);
      ext->btn_wifiscan         = new_GButton( SYMBOL_WIFISCAN, this);
      ext->btn_wifistatus       = new_GButton( SYMBOL_WIFISTATUS, this); 

      // ส่วน Body
      ext->page_body            = new_GPage(this);

      // กลุ่ม ssid/password ใน Body
      ext->cont_ssid            = new_GContainer(ext->page_body);
      ext->lb_ssid_info         = new_GLabel("SSID :"     , ext->cont_ssid);
      ext->lb_pass_info         = new_GLabel("Password :" , ext->cont_ssid);
      ext->ta_ssid              = new_GTextArea(ext->cont_ssid);
      ext->ta_password          = new_GTextArea(ext->cont_ssid);
      ext->sw_ssid_enable       = new_GSwitch(ext->cont_ssid);

      // กลุ่ม auto ip ใน Body
      ext->cont_auto_ip         = new_GContainer(ext->page_body);
      ext->lb_auto_ip           = new_GLabel("Auto IP", ext->cont_auto_ip);
      ext->sw_auto_ip           = new_GSwitch(ext->cont_auto_ip);

      // กลุ่ม fixed ip
      ext->cont_fix_ip          = new_GContainer(ext->page_body);
      ext->lb_fix_ip            = new_GLabel("Fixed IP :", ext->cont_fix_ip);
      ext->ta_fix_ip            = new_GTextArea(ext->cont_fix_ip);
      ext->lb_gateway_ip        = new_GLabel("Gateway IP :", ext->cont_fix_ip);
      ext->ta_gateway_ip        = new_GTextArea(ext->cont_fix_ip);
      ext->lb_subnet            = new_GLabel("Net Mask :", ext->cont_fix_ip);
      ext->ta_subnet            = new_GTextArea(ext->cont_fix_ip);
      ext->lb_dns_ip            = new_GLabel("DNS IP :", ext->cont_fix_ip);
      ext->ta_dns_ip            = new_GTextArea(ext->cont_fix_ip);
      
      // ตัวกันห้ามกด เวลา messagebox ปรากฏ
      ext->rect_disable_click   = new_GRect(this);

      // ทำเป็นหน้าต่าง message box ใช่ร่วมกันระหว่าง WiFi Scan และ WiFi Info
      ext->app_wifi_msgbox      = new_GRect(this);
      ext->btn_wifi_msgbox_back = new_GButton(SYMBOL_ARROW_LEFT, ext->app_wifi_msgbox);
      ext->lb_wifi_msgbox_title = new_GLabel(ext->app_wifi_msgbox);
      ext->lb_wifi_msgbox_status= new_GLabel(ext->app_wifi_msgbox);
      ext->list_wifiscan        = new_GList(ext->app_wifi_msgbox); 

      ext->page_wifiinfo        = new_GPage(ext->app_wifi_msgbox); 
      ext->lb_wifiinfo          = new_GLabel(ext->page_wifiinfo);

      /* ส่วนออกแบบ  */

#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->btn_back->size(45,45);
      ext->btn_back->align(ALIGN_TOP_LEFT,0,5);
#else
      if(is_enterprise_size) {
        ext->btn_back->font(prasanmit_40);
        ext->btn_back->size(60,60);
      }else{
        ext->btn_back->size(50,50);
      }
      ext->btn_back->align(ALIGN_TOP_LEFT,5,5);        
#endif
      ext->btn_back->opa(0);
      ext->btn_back->rounded(true); 
      ext->btn_back->padding(0); 
      ext->btn_back->border(0);
      ext->btn_back->style[PRESSED].color( TFT_WHITE); //TFT_COLOR_MIX(TFT_WHITE, this->color(), 32 ));
      ext->btn_back->style[PRESSED].opa(32);
      ext->btn_back->user_data(this);

#if defined(LAOS_TEXT)
      ext->lb_wifisetting_title->font(saysettha_25, TFT_WHITE);
#elif defined(VIETNAM_TEXT)
      ext->lb_wifisetting_title->font(arial_vietnam_30, TFT_WHITE);
#else
      if(BlynkGO.width()*BlynkGO.height()>=800*480) {
        ext->lb_wifisetting_title->font(prasanmit_40, TFT_WHITE);
      }
#endif



#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->lb_wifisetting_title->long_mode(LABEL_LONG_SROLL,100);
      ext->lb_wifisetting_title->align(ext->btn_back, ALIGN_RIGHT,2);
      ext->btn_wifiscan->size(45,45);
#else
      ext->lb_wifisetting_title->align(ext->btn_back, ALIGN_RIGHT,10);
      if(is_enterprise_size) {
        ext->btn_wifiscan->font(prasanmit_40, TFT_WHITE);
        ext->btn_wifiscan->size(60,60);
      }else{
        ext->btn_wifiscan->size(50,50);
      }
#endif
      ext->btn_wifiscan->opa(0);
      ext->btn_wifiscan->rounded(true); 
      ext->btn_wifiscan->padding(0); 
      ext->btn_wifiscan->font(easywifimanager_40, TFT_WHITE );
      ext->btn_wifiscan->border(0);
      ext->btn_wifiscan->style[PRESSED].color( TFT_WHITE); 
      ext->btn_wifiscan->style[PRESSED].opa(32); 
#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->btn_wifiscan->align(ALIGN_TOP_RIGHT,1, 5);
#else
      ext->btn_wifiscan->align(ALIGN_TOP_RIGHT,-5, 5);
#endif
      ext->btn_wifiscan->user_data(this);
      ext->btn_wifiscan->clickable(false);


#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->btn_wifistatus->size(45,45);
#else
      if(is_enterprise_size) {
        ext->btn_wifistatus->font(prasanmit_40, TFT_WHITE);
        ext->btn_wifistatus->size(60,60);
      }else{
        ext->btn_wifistatus->size(50,50);
      }
#endif
      ext->btn_wifistatus->opa(0);
      ext->btn_wifistatus->rounded(true); 
      ext->btn_wifistatus->padding(0); 
      ext->btn_wifistatus->font(easywifimanager_40, WiFi.isConnected()? TFT_WHITE : TFT_RED);
      ext->btn_wifistatus->border(0);
      ext->btn_wifistatus->style[PRESSED].color( TFT_WHITE);  
      ext->btn_wifistatus->style[PRESSED].opa(32); 
#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->btn_wifistatus->align(ext->btn_wifiscan, ALIGN_LEFT,2);
      ext->btn_wifiscan->toForeground();
#else
      ext->btn_wifistatus->align(ext->btn_wifiscan, ALIGN_LEFT,-5,0);
#endif
      ext->btn_wifistatus->user_data(this);

      ext->page_body->size(BlynkGO.width(), BlynkGO.height()-(ext->btn_back->height()+10)); // ปรับขนาด
      ext->page_body->align(ALIGN_BOTTOM);          // จัดไว้ชิดล่างภายในเลเยอร์แม่ คือ GWiFiSetting
      ext->page_body->layout(LAYOUT_COL_M);
      ext->page_body->font_color(TFT_WHITE);
      ext->page_body->color(TFT_COLOR(50,50,50));
      ext->page_body->corner_radius(0);
#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->page_body->padding(8);
      ext->page_body->padding_bottom(15);
#else
      ext->page_body->padding(15);
      ext->page_body->padding_bottom(30);
#endif

      //---------------------------------
      
//      ext->cont_ssid->fit(FIT_NONE, FIT_TIGHT);
//      ext->cont_ssid->width(GScreen.width()-MARGIN_LEFT-MARGIN_RIGHT);
      ext->cont_ssid->fit(FIT_FLOOD, FIT_TIGHT);
      ext->cont_ssid->layout(LAYOUT_NONE);
      ext->cont_ssid->corner_radius(10);
      ext->cont_ssid->color(TFT_COLOR(120,120,120));
      ext->cont_ssid->padding_bottom(25);
      if(is_enterprise_size)
        ext->cont_ssid->font(prasanmit_40);

      ext->lb_ssid_info->align(ALIGN_TOP_LEFT, MARGIN_LEFT, MARGIN_TOP);
      ext->ta_ssid->width(ext->cont_ssid->width()-MARGIN_LEFT-MARGIN_RIGHT);
      ext->ta_ssid->border_part(BORDER_BOTTOM);                         // TextArea นี้ ให้มีเส้นขอบเฉพาะด้านล่าง
      ext->ta_ssid->border(2, TFT_RED);                                 // TextArea นี้ ให้มีเส้นขอบ 2 pixel และ สีแดง
      ext->ta_ssid->placeholder("WiFi SSID (32 characters max)");       // ข้อความบน TextArea เพื่ออธิบายว่า ใส่ค่าอะไร
      ext->ta_ssid->placeholder_color(TFT_SILVER,200);                  // สีและ opa ของ placeholder
      ext->ta_ssid->cursor_type(CURSOR_NONE);                           // ไม่ให้แสดง cursor
      ext->ta_ssid->opa(0);                                             // กำหนดโปร่งใส
      if(is_enterprise_size)
        ext->ta_ssid->font(prasanmit_40);
      ext->ta_ssid->align(ext->lb_ssid_info, ALIGN_BOTTOM_LEFT);

      ext->lb_pass_info->align(ext->ta_ssid, ALIGN_BOTTOM_LEFT, 0, 10);
      ext->ta_password->width(ext->cont_ssid->width() - MARGIN_LEFT - MARGIN_RIGHT);
      ext->ta_password->password_mode(true);
      ext->ta_password->password_show_time(0);
      ext->ta_password->placeholder("WiFi Password (64 characters max)");
      ext->ta_password->placeholder_color(TFT_SILVER,200);
      ext->ta_password->cursor_type(CURSOR_NONE);
      ext->ta_password->linkStyle(ext->ta_ssid);
      ext->ta_password->align(ext->lb_pass_info, ALIGN_BOTTOM_LEFT); 

      ext->sw_ssid_enable->align(ALIGN_TOP_RIGHT, -MARGIN_RIGHT, MARGIN_TOP);
      ext->sw_ssid_enable->state(GWiFiSetting::wifi_config.ssid_enable);
      
      ext->sw_ssid_enable->user_data(ext);

      if( String(GWiFiSetting::wifi_config.ssid) != "" )      ext->ta_ssid     ->text(GWiFiSetting::wifi_config.ssid);
      if( String(GWiFiSetting::wifi_config.password) != "")   ext->ta_password ->text(GWiFiSetting::wifi_config.password);

      ext->ta_ssid->onValueChanged    (GWiFiSetting::ta_onValueChanged);
      ext->ta_password->onValueChanged(GWiFiSetting::ta_onValueChanged);

      //---------------------------------

      ext->cont_auto_ip->fit(FIT_FLOOD, FIT_TIGHT);
      ext->cont_auto_ip->layout(LAYOUT_NONE);
      ext->cont_auto_ip->corner_radius(ext->cont_ssid->corner_radius());
      ext->cont_auto_ip->color(ext->cont_ssid->color());
      ext->cont_auto_ip->padding_bottom(MARGIN_TOP);
      if(is_enterprise_size)
        ext->cont_auto_ip->font(prasanmit_40);

      ext->lb_auto_ip->align(ALIGN_TOP_LEFT , MARGIN_LEFT, MARGIN_TOP );

      ext->sw_auto_ip->align(ALIGN_TOP_RIGHT, -MARGIN_RIGHT, MARGIN_TOP);
      ext->sw_auto_ip->user_data(ext);

      
      ext->cont_fix_ip->fit(FIT_FLOOD, FIT_TIGHT);
      ext->cont_fix_ip->layout(LAYOUT_NONE);
      ext->cont_fix_ip->linkStyle(ext->cont_ssid);
      if(is_enterprise_size)
        ext->cont_fix_ip->font(prasanmit_40);

      ext->lb_fix_ip->align(ALIGN_TOP_LEFT , MARGIN_LEFT, MARGIN_TOP);
      ext->ta_fix_ip->width(ext->cont_fix_ip->width() - MARGIN_LEFT -MARGIN_RIGHT);
      ext->ta_fix_ip->align(ext->lb_fix_ip, ALIGN_BOTTOM_LEFT);
      ext->ta_fix_ip->linkStyle(ext->ta_ssid);
      ext->ta_fix_ip->cursor_type(CURSOR_NONE);
      ext->ta_fix_ip->placeholder("192.168.43.100");
      ext->ta_fix_ip->placeholder_color(TFT_SILVER,200);
      
      ext->lb_gateway_ip->align(ext->ta_fix_ip, ALIGN_BOTTOM_LEFT, 0, 10);
      ext->ta_gateway_ip->width(ext->ta_fix_ip);
      ext->ta_gateway_ip->align(ext->lb_gateway_ip, ALIGN_BOTTOM_LEFT);
      ext->ta_gateway_ip->linkStyle(ext->ta_ssid);
      ext->ta_gateway_ip->cursor_type(CURSOR_NONE);
      ext->ta_gateway_ip->placeholder("192.168.43.1");
      ext->ta_gateway_ip->placeholder_color(TFT_SILVER,200);
      
      ext->lb_subnet->align(ext->ta_gateway_ip, ALIGN_BOTTOM_LEFT, 0, 10);
      ext->ta_subnet->width(ext->ta_fix_ip);
      ext->ta_subnet->align(ext->lb_subnet, ALIGN_BOTTOM_LEFT);
      ext->ta_subnet->linkStyle(ext->ta_ssid);
      ext->ta_subnet->cursor_type(CURSOR_NONE);
      ext->ta_subnet->placeholder("255.255.255.000");
      ext->ta_subnet->placeholder_color(TFT_SILVER,200);

      ext->lb_dns_ip->align(ext->ta_subnet, ALIGN_BOTTOM_LEFT, 0, 10);
      ext->ta_dns_ip->width(ext->ta_fix_ip);
      ext->ta_dns_ip->align(ext->lb_dns_ip, ALIGN_BOTTOM_LEFT);
      ext->ta_dns_ip->linkStyle(ext->ta_ssid);
      ext->ta_dns_ip->cursor_type(CURSOR_NONE);
      ext->ta_dns_ip->placeholder("0.0.0.0");
      ext->ta_dns_ip->placeholder_color(TFT_SILVER,200);

      ext->sw_auto_ip->state(GWiFiSetting::wifi_config.auto_ip_enable);
      ext->cont_fix_ip->hidden(GWiFiSetting::wifi_config.auto_ip_enable);
      if( GWiFiSetting::wifi_config.fixed_ip32    != 0)   ext->ta_fix_ip    ->text(IPAddress(GWiFiSetting::wifi_config.fixed_ip32).toString());
      if( GWiFiSetting::wifi_config.gateway_ip32  != 0)   ext->ta_gateway_ip->text(IPAddress(GWiFiSetting::wifi_config.gateway_ip32).toString());
      if( GWiFiSetting::wifi_config.subnet32      != 0)   ext->ta_subnet    ->text(IPAddress(GWiFiSetting::wifi_config.subnet32).toString());
      if( GWiFiSetting::wifi_config.dns32         != 0)   ext->ta_dns_ip    ->text(IPAddress(GWiFiSetting::wifi_config.dns32).toString());

      ext->ta_fix_ip->onValueChanged    (GWiFiSetting::ta_onValueChanged);
      ext->ta_gateway_ip->onValueChanged(GWiFiSetting::ta_onValueChanged);
      ext->ta_subnet->onValueChanged    (GWiFiSetting::ta_onValueChanged);
      ext->ta_dns_ip->onValueChanged    (GWiFiSetting::ta_onValueChanged);

      //---------------------------------
      ext->rect_disable_click->size(GScreen);
      ext->rect_disable_click->opa(0);
      ext->rect_disable_click->hidden(true);
      ext->rect_disable_click->clickable(true);   // กันสัญญาณลงไปที่อื่นๆ
//      ext->rect_disable_click->gesture(true);
  //    ext->rect_disable_click->onGesture(TopMenu_onGesture);
//  
      //---------------------------------
      // ส่วน message box ที่ใช้ร่วมกันระหว่าง WiFi Info กับ WiFi Scan
#if defined(TTGO_TWATCH) //|| defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->app_wifi_msgbox->size( GScreen );
      ext->app_wifi_msgbox->corner_radius(10);
#else
      if(is_enterprise_size) {
        if(GScreen.width() < GScreen.height() ) {
          ext->app_wifi_msgbox->size( 400, 500);
        }else{
          ext->app_wifi_msgbox->size( GScreen.width()*70/100, GScreen.height()*80/100);
        }
        ext->app_wifi_msgbox->font(prasanmit_40);        
      }else{
        if(GScreen.width()<=240 || GScreen.height() <=240) {
          ext->app_wifi_msgbox->size( GScreen.width()*95/100, GScreen.height()*95/100);          
        }else
        if(GScreen.width() < GScreen.height() ) {
          ext->app_wifi_msgbox->size( GScreen.width()*85/100, GScreen.height()*62.5/100);
        }else{
          ext->app_wifi_msgbox->size( GScreen.width()*70/100, GScreen.height()*80/100);
        }
      }
      ext->app_wifi_msgbox->corner_radius(20);
#endif
      ext->app_wifi_msgbox->color( TFT_SILVER);//TFT_COLOR_MIX(TFT_SILVER, TFT_BLACK, 200));
      ext->app_wifi_msgbox->border(2,TFT_DARKGRAY);
      ext->app_wifi_msgbox->corner_mask(true);
      ext->app_wifi_msgbox->font_color(TFT_BLACK);
      ext->app_wifi_msgbox->hidden(true);
      ext->app_wifi_msgbox->clickable(false);

#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->btn_wifi_msgbox_back->size(45,45);
      ext->btn_wifi_msgbox_back->align(ALIGN_TOP_LEFT,0,5);
#else
      if(is_enterprise_size) {
        ext->btn_wifi_msgbox_back->font(prasanmit_40);
        ext->btn_wifi_msgbox_back->size(60,60);
      }else{
        ext->btn_wifi_msgbox_back->size(50,50);
      }
      ext->btn_wifi_msgbox_back->align(ALIGN_TOP_LEFT,5,5);
#endif
      ext->btn_wifi_msgbox_back->opa(0);
      ext->btn_wifi_msgbox_back->rounded(true); 
      ext->btn_wifi_msgbox_back->padding(0); 
      ext->btn_wifi_msgbox_back->font_color(TFT_BLACK);
      ext->btn_wifi_msgbox_back->border(0);
      ext->btn_wifi_msgbox_back->style[PRESSED].color( TFT_BLACK);
      ext->btn_wifi_msgbox_back->style[PRESSED].opa(32);

      ext->btn_wifi_msgbox_back->user_data(this);
      ext->btn_wifi_msgbox_back->onClicked([](GWidget*w){
        GWiFiSetting* pWiFiSetting = (GWiFiSetting*) w->user_data();
        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) pWiFiSetting->ext_attr();
        
        ext->rect_disable_click->clickable(false);
        ext->rect_disable_click->hidden(true);
        ext->app_wifi_msgbox->hidden(true);
        *ext->lb_wifi_msgbox_status = "";
        WiFiScan_StatusCheck_Task.del();
    //    if(WiFi.scanComplete() == WIFI_SCAN_RUNNING)  WiFi.scanDelete();
        
    //     // หากกดถอยแล้วไวไฟยังไม่เชื่อมให้ เชื่อมต่อด้วยค่าเดิม
    //     if(!WiFi.isConnected() && ext->ta_ssid->text() != "" ){    
    // //      _pWiFiSetting->try_connect();
    //       GWiFiSetting::connect_wifi(true);
    //     }
      });

#if defined(LAOS_TEXT)
      ext->lb_wifi_msgbox_title->font(saysettha_25, TFT_BLACK);
#elif defined(VIETNAM_TEXT)
      ext->lb_wifi_msgbox_title->font(arial_vietnam_30, TFT_BLACK);
#endif

#if defined(TTGO_TWATCH) || defined (BEENEXT_2_8) || defined(BEENEXT_2_8C)
      ext->lb_wifi_msgbox_title->align(ext->btn_wifi_msgbox_back, ALIGN_RIGHT, 2);
#else
      ext->lb_wifi_msgbox_title->align(ext->btn_wifi_msgbox_back, ALIGN_RIGHT, 10);
#endif

      ext->lb_wifi_msgbox_status->font(prasanmit_20, TFT_BLACK);
      ext->lb_wifi_msgbox_status->align(ALIGN_BOTTOM_RIGHT,-20);
      *ext->lb_wifi_msgbox_status = "";

      ext->list_wifiscan->size( ext->app_wifi_msgbox->width() - 2* ext->app_wifi_msgbox->border(), ext->app_wifi_msgbox->height()-60 -25 - ext->app_wifi_msgbox->border());
      ext->list_wifiscan->align(ALIGN_TOP, 0, (is_enterprise_size)? 70 : 60 );

      ext->list_wifiscan->border(0);
      ext->list_wifiscan->font(prasanmit_40);     // font ของ item ของ list
      ext->list_wifiscan->color(TFT_CYAN, TFT_NAVY);
      ext->list_wifiscan->padding_top(5);         // padding ด้านบนสำหรับ item ของ list
      ext->list_wifiscan->padding_bottom(5);      // padding ด้านล่างสำหรับ item ของ list
      ext->list_wifiscan->hidden(true);
      ext->list_wifiscan->user_data(this);


      ext->page_wifiinfo->size(  ext->app_wifi_msgbox->width() - 2* ext->app_wifi_msgbox->border(), ext->app_wifi_msgbox->height()-60 -25 - ext->app_wifi_msgbox->border());
      ext->page_wifiinfo->align(ALIGN_TOP, 0, (is_enterprise_size)? 70 : 60 );
      ext->page_wifiinfo->opa(0);
      ext->page_wifiinfo->corner_radius(0);
      ext->page_wifiinfo->border(1,TFT_BLACK);
      ext->page_wifiinfo->border_part(BORDER_TOP);
      ext->page_wifiinfo->layout(LAYOUT_NONE);    // ไม่ให้จัด layout จะจัดตำแหน่ง ลูกๆ เอง
      ext->page_wifiinfo->hidden(true);
      if(is_enterprise_size)
        ext->page_wifiinfo->font(prasanmit_40, TFT_BLACK);

      ext->lb_wifiinfo->text_align(TEXT_ALIGN_LEFT);
      ext->lb_wifiinfo->align(ALIGN_TOP_LEFT, MARGIN_LEFT, MARGIN_TOP);
      ext->lb_wifiinfo->colorful(true);
      ext->lb_wifiinfo->text_line_space(-5);

      //-----------------------------------------------------

      BlynkGO.onWiFiConnected   ( GWiFiSetting::onWiFiConnected ); 
      BlynkGO.onWiFiDisconnected( GWiFiSetting::onWiFiDisconnected);

      //-----------------------------------------------------
      _pWiFiSetting = this;
      wifisetting_timer.setOnce(1000L,[](){
        if( _pWiFiSetting == NULL) return;
        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
        if( ext == NULL) return;
        ext->btn_wifiscan->clickable(true);
      });
      //-----------------------------------------------------
      // จัดการพวก event เมื่อการเปลี่ยนแปลงค่า/ มีการ clicked

      ext->btn_back->onClicked([](GWidget*w){
        WiFiScan_StatusCheck_Task.del();

        GWiFiSetting* pWiFiSetting = (GWiFiSetting*) w->user_data();
        if( pWiFiSetting == NULL) return;

        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) pWiFiSetting->ext_attr();

        // หากมีการกด back ให้ซ่อนเป็นพื้นก่อน ส่วนจะไปทำลายค่อยไปว่ากันที่ ตัวแม่เรียกกำหนด
        ext->rect_disable_click->hidden(true);
        pWiFiSetting->hidden(true);
        if( pWiFiSetting->fn_onback !=NULL) pWiFiSetting->fn_onback(pWiFiSetting);
      });

      // มีการเปลี่ยนแปลง สวิตซ์ ssid enable 
      // ต้องวางหลังจากกำหนด BlynkGO.onWiFiConnected && BlynkGO.onWiFiDisconnected
      ext->sw_ssid_enable->onValueChanged([](GWidget*w){
        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) w->user_data();
        bool sw_ssid_en = ext->sw_ssid_enable->isON();
        
        if(GWiFiSetting::wifi_config.ssid_enable != sw_ssid_en ){
          GWiFiSetting::wifi_config.ssid_enable = sw_ssid_en;
          if( GWiFiSetting::wifi_config.ssid_enable ) {
            GWiFiSetting::connect_WiFi(true);
          }else{
            // Serial.println("sw to disable ssid");
            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);
            GWiFiSetting::save_WiFiConfig();
            GWiFiSetting::onWiFiDisconnected();
          }
        }
      });

      ext->sw_auto_ip->onValueChanged([](GWidget*w){
        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) w->user_data();
        bool sw_auto_ip = ext->sw_auto_ip->isON();
        ext->cont_fix_ip->hidden( sw_auto_ip );

        if(GWiFiSetting::wifi_config.auto_ip_enable != sw_auto_ip) {
          GWiFiSetting::wifi_config.auto_ip_enable = sw_auto_ip;

          if(sw_auto_ip) {
            // เชื่อมต่อแบบ auto ip
            GWiFiSetting::connect_WiFi(true);
          }else{
            // เชื่อมต่อแบบ fix ip
            if( GWiFiSetting::wifi_config.fixed_ip32 != 0 &&
                GWiFiSetting::wifi_config.gateway_ip32 != 0 &&
                GWiFiSetting::wifi_config.subnet32 != 0 )
            {
              GWiFiSetting::connect_WiFi(true);
            }else{
              WiFi.disconnect(true);
              WiFi.mode(WIFI_OFF);
              GWiFiSetting::save_WiFiConfig();
              GWiFiSetting::onWiFiDisconnected();
            }
          }
        }
      });


      // เมื่อมีการ click ที่ ปุ่ม wifistatus บน titlebar
      // ให้แสดงข้อมูลการเชื่อมต่อไวไฟ
      ext->btn_wifistatus->onClicked([](GWidget* w){
        GWiFiSetting* pWiFiSetting = (GWiFiSetting*) w->user_data();
        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) pWiFiSetting->ext_attr();
        
        ext->rect_disable_click->hidden(false);
        ext->app_wifi_msgbox->hidden(false);
        *ext->lb_wifi_msgbox_title = TEXT_WIFI_INFO;
        
        ext->list_wifiscan->hidden(true);
        ext->page_wifiinfo->hidden(false);

        ext->lb_wifi_msgbox_status->hidden(true);
        *ext->lb_wifi_msgbox_status = "";
        pWiFiSetting->update_WiFiInfo();
      });


      // เมื่อมีการ click ที่ ปุ่ม wifistatus บน titlebar
      // ให้ทำการสแกน wifi รอบๆ
      ext->btn_wifiscan->onClicked([](GWidget* w){
        GWiFiSetting* pWiFiSetting = (GWiFiSetting*) w->user_data();
        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) pWiFiSetting->ext_attr();

        ext->rect_disable_click   ->hidden(false);
        ext->app_wifi_msgbox      ->hidden(false);
        
        ext->list_wifiscan        ->hidden( ext->list_wifiscan->item_num() == 0 ); 
        ext->page_wifiinfo        ->hidden(true);
        *ext->lb_wifi_msgbox_title = "WiFi Scan";

        ext->lb_wifi_msgbox_status->hidden(false);
        *ext->lb_wifi_msgbox_status = "";
        
        GWiFiSetting::scan_WiFiNetworks();
      });

      // เมื่อมีการ click เลือก WiFi SSID ที่ ผลที่สแกนได้จากใน list
      ext->list_wifiscan->onClicked([](GWidget* w){
        GWiFiSetting* pWiFiSetting = (GWiFiSetting*) w->user_data();
        gwifisetting_ext_t* ext = (gwifisetting_ext_t*) pWiFiSetting->ext_attr();

        String ssid_selected = ext->list_wifiscan->item_selected();
        if( ext->ta_ssid->text()!= ssid_selected || !WiFi.isConnected()){
          *ext->ta_ssid = ssid_selected;          // จะมีไปเรียก  GWiFiSetting::connect_WiFi() อัตโนมัติ จาก GWiFiSetting::ta_onValueChanged(...)
        }
        
        ext->rect_disable_click->hidden(true);
        ext->app_wifi_msgbox->hidden(true);
        WiFiScan_StatusCheck_Task.del();
      });

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);


    }
  }
}


void GWiFiSetting::create(GWidget& parent){
  this->_par = &parent;
  create();
}

void GWiFiSetting::create(GWidget* parent) {
  this->_par = parent;
  create();
}

/*********************************************
 * GWiFiSetting's static member functions
 *********************************************/

void GWiFiSetting::onWiFiConnected() {
  if(_pWiFiSetting == NULL) return;
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;

  // Serial.println("GWiFiSetting OnConnected");

  Serial.printf("[GWiFiSetting] WiFi Connected. IP : %s, Gateway : %s, Subnet : %s\n",
                  WiFi.localIP().toString().c_str(),
                  WiFi.gatewayIP().toString().c_str(),
                  WiFi.subnetMask().toString().c_str());
  GWiFiSetting::save_WiFiConfig();  // บันทึก ssid ที่เชื่อมต่อได้สำเร็จ

  ewm_state = EWM_STATE_CONNECTED;
  // ext->lb_wifistatus->color(TFT_WHITE);
  ext->btn_wifistatus->font_color(TFT_WHITE);
  // ext->btn_wifistatus->style[PRESSED].font_color(TFT_WHITE);
  _pWiFiSetting->update_WiFiInfo();


  if(_pWiFiSetting->user_data() != NULL) {
    GWidget *w = (GWidget*) _pWiFiSetting->user_data();
    if( w->isType("GWiFiManager")){
      GWiFiManager* pWiFiManager = (GWiFiManager*) w;
      gwifimanager_ext_t* ext_wifimanager = (gwifimanager_ext_t*) pWiFiManager->ext_attr();

      ext_wifimanager->lb_wifi_signal[1]->color(TFT_WHITE);
    }
  }
}

// class's static member
void GWiFiSetting::onWiFiDisconnected() {
  if(ewm_state != EWM_STATE_DISCONNECTED) {
    Serial.println("[GWiFiManager] WiFi Disonnected");
    ewm_state = EWM_STATE_DISCONNECTED;
  }

  if(_pWiFiSetting == NULL) return;
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;

   // Serial.println("GWiFiSetting OnDisConnected");
  // ext->lb_wifistatus->color(TFT_RED);
  ext->btn_wifistatus->font_color(TFT_RED);
  // ext->btn_wifistatus->style[PRESSED].font_color(TFT_RED);
  GWiFiSetting::update_WiFiInfo();

  // กรณี GWiFiSetting ถูกสร้างจาก GWiFiManager
  // ให้เปลี่ยนสีของ สัญญาณความแรงไวไฟ ของ GWiFiManager เป็นสีแดงด้วย
  if(_pWiFiSetting->user_data() != NULL) {
    GWidget *w = (GWidget*) _pWiFiSetting->user_data();
    if( w->isType("GWiFiManager")){
      GWiFiManager* pWiFiManager = (GWiFiManager*) w;
      gwifimanager_ext_t* ext_wifimanager = (gwifimanager_ext_t*) pWiFiManager->ext_attr();
      ext_wifimanager->lb_wifi_signal[1]->color(TFT_RED);
    }
  }

#if BLYNKGO_USE_BLYNK
  if( Blynk.fn_user_blynk_disconnected != NULL)
    Blynk.fn_user_blynk_disconnected();
#endif

}


void GWiFiSetting::ta_onValueChanged(GWidget*w){
  if(_pWiFiSetting == NULL) return;
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;

  // Serial.println("ta_onValueChanged call");

  if(ext->ta_ssid->sameWith(w)){
    if( ext->ta_ssid->text() != String(GWiFiSetting::wifi_config.ssid) ){
      snprintf( GWiFiSetting::wifi_config.ssid, 32, "%s", ext->ta_ssid->text().c_str());
      memset(GWiFiSetting::wifi_config.bssid, 0,  sizeof(GWiFiSetting::wifi_config.bssid));
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      GWiFiSetting::onWiFiDisconnected();
      // Serial.printf("[ta_ssid] changed : %s\n", GWiFiSetting::wifi_config.ssid);
    }
  }else
  if(ext->ta_password->sameWith(w)){
    if( ext->ta_password->text() != String(GWiFiSetting::wifi_config.password) ){
      snprintf( GWiFiSetting::wifi_config.password, 64, "%s", ext->ta_password->text().c_str());
      // Serial.println(GWiFiSetting::wifi_config.password);
      if( String(GWiFiSetting::wifi_config.ssid)!="") {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        GWiFiSetting::onWiFiDisconnected();
      }
    }
  }else
  if(ext->ta_fix_ip->sameWith(w)){
    // Serial.printf("ta_fix_ip changed : %s\n", ext->ta_fix_ip->text().c_str());

    ip4_addr_t fix_ip_addr;
    ip4addr_aton( ext->ta_fix_ip->text().c_str(), &fix_ip_addr);
    GWiFiSetting::wifi_config.fixed_ip32    = fix_ip_addr.addr;  // uint32_t
    GWiFiSetting::save_WiFiConfig();
  }else
  if(ext->ta_gateway_ip->sameWith(w)){
    // Serial.printf("ta_gateway_ip changed : %s\n", ext->ta_gateway_ip->text().c_str());

    ip4_addr_t gateway_ip_addr;
    ip4addr_aton( ext->ta_gateway_ip->text().c_str(), &gateway_ip_addr);
    GWiFiSetting::wifi_config.gateway_ip32  = gateway_ip_addr.addr;  // uint32_t
    GWiFiSetting::save_WiFiConfig();
  }else
  if(ext->ta_subnet->sameWith(w)){
    // Serial.printf("ta_subnet changed : %s\n", ext->ta_subnet->text().c_str());

    ip4_addr_t subnet_addr;
    ip4addr_aton( ext->ta_subnet->text().c_str(), &subnet_addr);
    GWiFiSetting::wifi_config.subnet32      = subnet_addr.addr;  // uint32_t
    GWiFiSetting::save_WiFiConfig();
  }else
  if(ext->ta_dns_ip->sameWith(w)){
    // Serial.printf("ta_dns_ip changed : %s\n", ext->ta_dns_ip->text().c_str());

    ip4_addr_t dns_addr;
    ip4addr_aton( ext->ta_dns_ip->text().c_str(), &dns_addr);
    GWiFiSetting::wifi_config.dns32      = dns_addr.addr;  // uint32_t
    GWiFiSetting::save_WiFiConfig();
  }

  if( GWiFiSetting::wifi_config.ssid_enable) {
    if(GWiFiSetting::wifi_config.auto_ip_enable) {
      if( String(GWiFiSetting::wifi_config.ssid) != ""){
        GWiFiSetting::connect_WiFi();
      }
    }else{
      if( String(GWiFiSetting::wifi_config.ssid) != "" && 
          GWiFiSetting::wifi_config.fixed_ip32 != 0 &&
          GWiFiSetting::wifi_config.gateway_ip32 != 0 &&
          GWiFiSetting::wifi_config.subnet32 != 0) {
        GWiFiSetting::connect_WiFi(true);
      }
    }
  }
//  _pWiFiSetting->try_connect();
}

void GWiFiSetting::update_WiFiInfo(){
  if(_pWiFiSetting == NULL) return;
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;
  
  if(WiFi.isConnected()){
    ext->lb_wifiinfo->font(prasanmit_30,TFT_BLACK);
    ext->lb_wifiinfo->align(ALIGN_TOP_LEFT, MARGIN_LEFT, -10);
  
    *ext->lb_wifiinfo =   ((GWiFiSetting::wifi_config.auto_ip_enable)? 
                              String("IP Address :\n   #0000ff ") : String("Fixed IP Address :\n   #0000ff " ))
                        + WiFi.localIP().toString() + " # \n"
                        + "Subnet Mask :\n   #0000ff " + WiFi.subnetMask().toString() + " # \n"
                        + "Gateway :\n   #0000ff " + WiFi.gatewayIP().toString()+ " # \n"
                        + "DNS IP :\n   #0000ff " + WiFi.dnsIP().toString()+ " # \n"
                        + "SSID :\n   #0000ff " + WiFi.SSID()+ " # \n"
                        + "BSSID :\n   #0000ff " + WiFi.BSSIDstr()+ " # \n"
                        + "RSSI :\n   #0000ff " + WiFi.RSSI()+ " dBm # ";
  }else{
    ext->lb_wifiinfo->font(prasanmit_25,TFT_RED);
    *ext->lb_wifiinfo = "WiFi Disconnected";
    ext->lb_wifiinfo->align(ALIGN_CENTER);
  }
  ext->page_wifiinfo->scrollTop();  // เลื่อนไปข้างบนสุด
}



void GWiFiSetting::scan_WiFiNetworks(){
//  Serial.println("[Easy WiFiManager] scan networks...");
  if(_pWiFiSetting == NULL) return; 
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;
  
  WiFiScan_StatusCheck_Task.del();
  if( !WiFi.isConnected()) WiFi.disconnect();
  if( WiFi.getMode() != WIFI_STA) WiFi.mode(WIFI_STA);
  
  scan_networks_num = -1;
  scan_response = WiFi.scanNetworks(true);
  
  if(scan_response == WIFI_SCAN_FAILED) {
    *ext->lb_wifi_msgbox_status = "error : scan failed";
//    Serial.println("[Easy WiFiManager] scan failed");
    WiFi.disconnect(true);    // หากสแกน fail จะมาจากไวไฟที่เคยเชื่อมได้ถูกปิดไป แล้ว WiFi ยังพยายามจะเชื่อมอยู่ ต้องตัดออกก่อน แล้ว scan ใหม่
    _pWiFiSetting->WiFiReScan(1000);
  }
  else if(scan_response == WIFI_SCAN_RUNNING){
    *ext->lb_wifi_msgbox_status = "scaning wifi ...";
//    Serial.println("[Easy WiFiManager] scan waiting");
    wifiscan_timeout = millis() + WIFISCAN_TIMEOUT;
    WiFiScan_StatusCheck_Task.setInterval(400L,[](void*){
      if(_pWiFiSetting == NULL) return;
      if(!_pWiFiSetting->isCreated()) return;
      gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
      if(ext==NULL) return;
      
      if(WiFi.scanComplete()>=0){
        WiFiScan_StatusCheck_Task.del();
        scan_networks_num = WiFi.scanComplete();
        GWiFiSetting::update_NetworkList();        
        return;
      }
      // หากหมดเลยเวลา 10 วินาที ให้เริ่ม scan ใหม่
      if(WiFi.scanComplete() < 0 && wifiscan_timeout < millis()){
        *ext->lb_wifi_msgbox_status = "error : scan failed exceed limit time";
//        WiFiScan_StatusCheck_Task.del();
//        Serial.println("[Easy WiFiManager] scan failed exceed limit time");
//        WiFi.disconnect(true);
        GWiFiSetting::WiFiReScan(1000);
      }
    });
  }
  else if(scan_response >=0 ) scan_networks_num = scan_response;

  if(scan_networks_num>=0 ){
    GWiFiSetting::update_NetworkList();
  }
}

void GWiFiSetting::update_NetworkList(){
  if(_pWiFiSetting == NULL) return;
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;
  
  if(ext->app_wifi_msgbox->hidden() || *ext->lb_wifi_msgbox_title != "WiFi Scan") return;

  *ext->lb_wifi_msgbox_status = StringX::printf("scan found : %d", scan_networks_num);

//  Serial.printf("[Easy WiFiManager] scan found : %d\n", scan_networks_num);
  ext->list_wifiscan->clear();
  ext->list_wifiscan->hidden( scan_networks_num <= 0 ) ; 
  
  for(int i=0; i < scan_networks_num; i++){
    ext->list_wifiscan->addItem(wifi_signal_symbol(WiFi.RSSI(i)), WiFi.SSID(i));
  }

  // ถ้ายังเปิด หน้าต่าง WiFiScan ขึ้นมาอยู่ ให้ สั่ง task ทำการ rescan WiFi 
  if(!ext->app_wifi_msgbox->hidden() && *ext->lb_wifi_msgbox_title == "WiFi Scan") { GWiFiSetting::WiFiReScan(); }
}

void GWiFiSetting::WiFiReScan(uint32_t period){
  if(_pWiFiSetting == NULL) return;
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;
  
  WiFiScan_StatusCheck_Task.del();
  WiFiScan_Task.setOnce(period, [](void*){
    GWiFiSetting::scan_WiFiNetworks();
  });
}

void GWiFiSetting::autoconnection()  { GWiFiSetting::load_WiFiConfig(); GWiFiSetting::connect_WiFi();}
void GWiFiSetting::load_WiFiConfig(){
  if(BlynkGO.flashMem_exists("WIFICONFIG_SZ")){
    uint32_t config_sz = (uint32_t)BlynkGO.flashMem_Int("WIFICONFIG_SZ");
    // Serial.printf("[config] flash wifi sz : %d <--> wifi_config sz ; %d\n", config_sz, sizeof(wifimanager_config_t) );
    if( config_sz != sizeof(wifimanager_config_t) ) {
      GWiFiSetting::clean_WiFiConfig();
    }
  }

  memset(&GWiFiSetting::wifi_config, 0, sizeof(wifimanager_config_t));

  if(BlynkGO.flashMem_exists("WIFICONFIG")){
    // Serial.println("[GWiFiManager] load config.");

    wifimanager_config_t * blob = NULL;
    blob = (wifimanager_config_t*) BlynkGO.flashMem_Object("WIFICONFIG", (void*)blob);
    memcpy(&GWiFiSetting::wifi_config, blob, sizeof(GWiFiSetting::wifi_config));
    free(blob); blob = NULL;
  }else{
    // Serial.println("[GWiFiManager] load Default config.");
    memset(GWiFiSetting::wifi_config.ssid    , 0, sizeof(GWiFiSetting::wifi_config.ssid));
    memset(GWiFiSetting::wifi_config.password, 0, sizeof(GWiFiSetting::wifi_config.password));
    GWiFiSetting::wifi_config.ssid_enable     = true;
    GWiFiSetting::wifi_config.auto_ip_enable  = true;
    memset(GWiFiSetting::wifi_config.bssid   , 0, sizeof(GWiFiSetting::wifi_config.bssid));
    GWiFiSetting::wifi_config.fixed_ip32      = 0;
    GWiFiSetting::wifi_config.gateway_ip32    = 0;
    GWiFiSetting::wifi_config.subnet32        = uint32_t(IPAddress(255,255,255,0));
    GWiFiSetting::wifi_config.dns32           = 0;
  }

  GWiFiSetting::display_WiFiConfig();
}


void GWiFiSetting::save_WiFiConfig() {
  // Serial.println("[GWiFiManager] save config");

  if(WiFi.isConnected()) {  // จำค่า bssid ที่เชื่อมต่อได้
    memcpy( GWiFiSetting::wifi_config.bssid, WiFi.BSSID(), sizeof( GWiFiSetting::wifi_config.bssid) );
  }

  if( BlynkGO.flashMem_exists("WIFI_CONFIG") )      BlynkGO.flashMem_erase("WIFICONFIG");
  if( BlynkGO.flashMem_exists("WIFICONFIG_SZ") )    BlynkGO.flashMem_erase("WIFICONFIG_SZ");

  ESP_LOGI("GWiFiSetting", "save WiFiConfig");
  BlynkGO.flashMem("WIFICONFIG", &GWiFiSetting::wifi_config, sizeof(GWiFiSetting::wifi_config));
  BlynkGO.flashMem("WIFICONFIG_SZ" , sizeof(GWiFiSetting::wifi_config));
}

void GWiFiSetting::save_WiFiConfig(String ssid, String pass){
  if(WiFi.isConnected()) {  // จำค่า bssid ที่เชื่อมต่อได้
    memcpy( GWiFiSetting::wifi_config.bssid, WiFi.BSSID(), sizeof( GWiFiSetting::wifi_config.bssid) );
  }

  snprintf( GWiFiSetting::wifi_config.ssid, 32, "%s", ssid.c_str());
  snprintf( GWiFiSetting::wifi_config.password, 64, "%s", pass.c_str());

  if( BlynkGO.flashMem_exists("WIFI_CONFIG") )      BlynkGO.flashMem_erase("WIFICONFIG");
  if( BlynkGO.flashMem_exists("WIFICONFIG_SZ") )    BlynkGO.flashMem_erase("WIFICONFIG_SZ");
  ESP_LOGI("GWiFiSetting", "save WiFiConfig");
  BlynkGO.flashMem("WIFICONFIG", &GWiFiSetting::wifi_config, sizeof(GWiFiSetting::wifi_config));
  BlynkGO.flashMem("WIFICONFIG_SZ" , sizeof(GWiFiSetting::wifi_config));
}

void  GWiFiSetting::clean_WiFiConfig(){
  ESP_LOGI("GWiFiSetting", "clean WiFiConfig");
  if( BlynkGO.flashMem_exists("WIFI_CONFIG") )      BlynkGO.flashMem_erase("WIFICONFIG");
  if( BlynkGO.flashMem_exists("WIFICONFIG_SZ") )    BlynkGO.flashMem_erase("WIFICONFIG_SZ");

  // snprintf(GWiFiSetting::wifi_config.ssid, sizeof(GWiFiSetting::wifi_config.ssid), "");
  // snprintf(GWiFiSetting::wifi_config.password, sizeof(GWiFiSetting::wifi_config.password), "");
  // GWiFiSetting::wifi_config.ssid_enable     = true;
  // GWiFiSetting::wifi_config.auto_ip_enable  = true;
  // GWiFiSetting::wifi_config.fixed_ip32      = 0;
  // GWiFiSetting::wifi_config.gateway_ip32    = 0;
  // GWiFiSetting::wifi_config.subnet32        = 0;
  // GWiFiSetting::wifi_config.dns32           = 0;

    memset(GWiFiSetting::wifi_config.ssid    , 0, sizeof(GWiFiSetting::wifi_config.ssid));
    memset(GWiFiSetting::wifi_config.password, 0, sizeof(GWiFiSetting::wifi_config.password));
    GWiFiSetting::wifi_config.ssid_enable     = false;
    GWiFiSetting::wifi_config.auto_ip_enable  = true;
    memset(GWiFiSetting::wifi_config.bssid   , 0, sizeof(GWiFiSetting::wifi_config.bssid));
    GWiFiSetting::wifi_config.fixed_ip32      = 0;
    GWiFiSetting::wifi_config.gateway_ip32    = 0;
    GWiFiSetting::wifi_config.subnet32        = uint32_t(IPAddress(255,255,255,0));
    GWiFiSetting::wifi_config.dns32           = 0;

  if(_pWiFiSetting == NULL) return;
  gwifisetting_ext_t* ext = (gwifisetting_ext_t*) _pWiFiSetting->ext_attr();
  if(ext == NULL) return;

  // Serial.println("reset wifi sub widget");
  ext->ta_ssid->text(GWiFiSetting::wifi_config.ssid);

}

void  GWiFiSetting::display_WiFiConfig(){
  uint8_t *bssid = GWiFiSetting::wifi_config.bssid;
  char bssidStr[18] = { 0 };
  sprintf(bssidStr, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

  Serial.printf("[GWiFiManager] \n"
                "   ssid      : %s\n"
                "   password  : %s\n"
                "   bssid     : %s\n"
                "   ssid en   : %d\n"
                "   autoIP en : %d\n"
                "   fix ip    : %s\n"
                "   gateway ip: %s\n"
                "   subnet    : %s\n"
                "   dns ip    : %s\n",
                GWiFiSetting::wifi_config.ssid, 
                GWiFiSetting::wifi_config.password,
                bssidStr,
                GWiFiSetting::wifi_config.ssid_enable,
                GWiFiSetting::wifi_config.auto_ip_enable,
                IPAddress(GWiFiSetting::wifi_config.fixed_ip32).toString().c_str(),
                IPAddress(GWiFiSetting::wifi_config.gateway_ip32).toString().c_str(),
                IPAddress(GWiFiSetting::wifi_config.subnet32).toString().c_str(),
                IPAddress(GWiFiSetting::wifi_config.dns32).toString().c_str());
}

void GWiFiSetting::connect_WiFi(bool force_reconnect){
  if( String(GWiFiSetting::wifi_config.ssid) == "") return;

  if( GWiFiSetting::wifi_config.ssid_enable == false) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    GWiFiSetting::onWiFiDisconnected();
    return;
  }

  uint8_t *bssid = GWiFiSetting::wifi_config.bssid;
  char bssidStr[18] = { 0 };
  sprintf(bssidStr, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

  if(force_reconnect) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
  }

  if( !WiFi.isConnected()) {
    if( GWiFiSetting::wifi_config.ssid_enable ) {
      if( String(GWiFiSetting::wifi_config.ssid) != "" ) {
        // GWiFiSetting::display_WiFiConfig();
        
        if( GWiFiSetting::wifi_config.auto_ip_enable ) {
          Serial.printf("[GWiFiManager] WiFi Auto Connecting to %s ...\n", GWiFiSetting::wifi_config.ssid);

          if( String(bssidStr) == "00:00:00:00:00:00" ) {
            WiFi.begin(GWiFiSetting::wifi_config.ssid, GWiFiSetting::wifi_config.password);
          }else{
            WiFi.begin(GWiFiSetting::wifi_config.ssid, GWiFiSetting::wifi_config.password, 0, GWiFiSetting::wifi_config.bssid);
          }
        }else{
          if( GWiFiSetting::wifi_config.fixed_ip32 != 0 &&
              GWiFiSetting::wifi_config.gateway_ip32 != 0 &&
              GWiFiSetting::wifi_config.subnet32 != 0 ) {
            Serial.printf("[GWiFiManager] WiFi Auto Connecting to %s ...\n", GWiFiSetting::wifi_config.ssid);
            if( String(bssidStr) == "00:00:00:00:00:00" ) {
              WiFi.begin(GWiFiSetting::wifi_config.ssid, GWiFiSetting::wifi_config.password);
            }else{
              WiFi.begin(GWiFiSetting::wifi_config.ssid, GWiFiSetting::wifi_config.password, 0, GWiFiSetting::wifi_config.bssid);
            }
            if(GWiFiSetting::wifi_config.dns32==0){
              WiFi.config(IPAddress(GWiFiSetting::wifi_config.fixed_ip32),
                          IPAddress(GWiFiSetting::wifi_config.gateway_ip32),
                          IPAddress(GWiFiSetting::wifi_config.subnet32));
            }else{
              WiFi.config(IPAddress(GWiFiSetting::wifi_config.fixed_ip32),
                          IPAddress(GWiFiSetting::wifi_config.gateway_ip32),
                          IPAddress(GWiFiSetting::wifi_config.subnet32),
                          IPAddress(GWiFiSetting::wifi_config.dns32) );
            }
          }
        }
      }
    }
  }
}

/*********************************************
   PSRAM Widget Alloc
 *********************************************/

GWiFiSetting*  new_GWiFiSetting(GWidget& parent){
  GWiFiSetting* w = (GWiFiSetting*) esp32_malloc(sizeof(GWiFiSetting));
  new (w) GWiFiSetting(parent);
  return w;
}

GWiFiSetting*  new_GWiFiSetting(GWidget *parent){
  GWiFiSetting* w = (GWiFiSetting*) esp32_malloc(sizeof(GWiFiSetting));
  new (w) GWiFiSetting(parent);
  return w;
}

//----------------------------------------------------------------------------------
// Font
//----------------------------------------------------------------------------------
#include "blynkgo_lib.h"

/*******************************************************************************
 * Size: 40 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifndef EASYWIFIMANAGER_40
#define EASYWIFIMANAGER_40 1
#endif

#if EASYWIFIMANAGER_40

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+F002 "" */
    0x0, 0xff, 0xe7, 0x9, 0xab, 0x44, 0xd5, 0xdb,
    0x31, 0xbb, 0x66, 0x2e, 0xa9, 0x2e, 0xc8, 0x42,
    0x1, 0xff, 0xee, 0x13, 0x68, 0xbe, 0xfe, 0xca,
    0x97, 0x65, 0x44, 0x19, 0x88, 0xa3, 0x32, 0x2a,
    0x9a, 0x26, 0xf7, 0xb9, 0xb4, 0xe8, 0x40, 0x1f,
    0xfe, 0x41, 0x48, 0xcf, 0xc9, 0x74, 0x10, 0xf,
    0xfe, 0x70, 0x92, 0xc5, 0xef, 0x5b, 0x98, 0x7,
    0xff, 0x74, 0x9a, 0xfe, 0xdc, 0xc4, 0x3, 0xff,
    0xb2, 0x29, 0x19, 0xb2, 0x82, 0x1, 0xff, 0xd6,
    0x14, 0x9c, 0x94, 0x10, 0xf, 0xfe, 0x10, 0x9a,
    0x33, 0xba, 0x23, 0x9d, 0x95, 0x8, 0x40, 0x3f,
    0xf8, 0x44, 0xd7, 0xae, 0x40, 0x1f, 0xfd, 0x21,
    0x5a, 0xb6, 0x10, 0xf, 0xf8, 0x4d, 0xa7, 0x3b,
    0x99, 0x73, 0x10, 0x77, 0xe8, 0x9a, 0xbd, 0xee,
    0x5c, 0x29, 0x0, 0x7f, 0xf0, 0xe, 0x3e, 0x4c,
    0x3, 0xff, 0x9e, 0x91, 0x68, 0x1, 0xff, 0x12,
    0xd6, 0x62, 0x58, 0xc4, 0x3, 0xff, 0x8a, 0x28,
    0xf5, 0x98, 0x83, 0x10, 0xf, 0xf1, 0x37, 0xb1,
    0x0, 0x7f, 0xf2, 0xc5, 0xa1, 0x84, 0x3, 0xfc,
    0x71, 0xf4, 0xa4, 0x1, 0xff, 0xce, 0x13, 0x7d,
    0xa6, 0x20, 0xf, 0xf1, 0xc5, 0xa0, 0x7, 0xff,
    0x24, 0x5f, 0x4c, 0x3, 0xfc, 0x51, 0xce, 0x40,
    0x1f, 0xfd, 0x41, 0x49, 0x92, 0x88, 0x7, 0xf0,
    0xbe, 0xa0, 0x7, 0xff, 0x21, 0xf5, 0x0, 0x3f,
    0xc9, 0x6e, 0x40, 0x1f, 0xfd, 0x94, 0x96, 0x20,
    0xf, 0xe1, 0x7c, 0x30, 0xf, 0xfe, 0x31, 0x73,
    0x0, 0x7f, 0x8f, 0x1c, 0x3, 0xff, 0xb8, 0x78,
    0x82, 0x1, 0xfc, 0x26, 0xa0, 0x1f, 0xfc, 0x66,
    0x12, 0x0, 0xff, 0x31, 0x8, 0x7, 0xff, 0x75,
    0xb8, 0xc0, 0x3f, 0xce, 0x24, 0x1, 0xff, 0xc6,
    0x30, 0xf, 0xff, 0x68, 0x80, 0x7f, 0x84, 0x3,
    0xff, 0x8e, 0xc4, 0x40, 0xf, 0xf3, 0x18, 0x80,
    0x7f, 0xf7, 0x5b, 0x88, 0x3, 0xfc, 0xc2, 0x60,
    0x1f, 0xfc, 0x52, 0xe6, 0x0, 0xff, 0x1e, 0xb8,
    0x7, 0xff, 0x70, 0xf1, 0x4, 0x3, 0xf8, 0x48,
    0x82, 0x1, 0xff, 0xc6, 0x7d, 0x30, 0xf, 0xf2,
    0x53, 0x90, 0x7, 0xff, 0x65, 0x25, 0x88, 0x3,
    0xfc, 0x98, 0x80, 0x1f, 0xfc, 0x71, 0x7a, 0x30,
    0xf, 0xf1, 0xc4, 0xb1, 0x0, 0x7f, 0xf5, 0x5,
    0x26, 0x4a, 0x20, 0x1f, 0xc2, 0xbe, 0xa0, 0x1f,
    0xfc, 0x91, 0x68, 0x61, 0x0, 0xff, 0x24, 0xfc,
    0xa8, 0x80, 0x7f, 0xf3, 0xcd, 0xea, 0x8c, 0x40,
    0x1f, 0xe2, 0x69, 0x50, 0xf, 0xfe, 0x62, 0x44,
    0x10, 0x3, 0xfe, 0x26, 0xaf, 0xc8, 0x52, 0x0,
    0xff, 0xe3, 0x9, 0xb5, 0x6e, 0x42, 0x8, 0x7,
    0xf8, 0x56, 0x20, 0x60, 0x1f, 0xfc, 0xe1, 0x6a,
    0xa2, 0x88, 0x7, 0xfc, 0x26, 0xf5, 0xbf, 0xb9,
    0x53, 0xe, 0xe6, 0x69, 0xdd, 0x13, 0x79, 0xdc,
    0xc9, 0x52, 0x0, 0xff, 0xe1, 0xa, 0xcc, 0x18,
    0x80, 0x7f, 0xf3, 0xc5, 0x6b, 0x60, 0xc4, 0x3,
    0xff, 0x84, 0x46, 0xac, 0xf1, 0x9, 0x9a, 0x20,
    0xec, 0x86, 0x20, 0x1f, 0xfc, 0x93, 0x7c, 0xd8,
    0x30, 0xf, 0xfe, 0x81, 0xbe, 0x62, 0x98, 0x80,
    0x3f, 0xfd, 0x6, 0xf9, 0x88, 0x30, 0xf, 0xfe,
    0x81, 0x2c, 0xef, 0x64, 0xb1, 0x88, 0x7, 0xff,
    0x3c, 0x91, 0xeb, 0x7f, 0xad, 0x88, 0x3, 0xff,
    0x80, 0x6f, 0x98, 0x83, 0x0, 0xff, 0xe8, 0x89,
    0xb4, 0xe7, 0x73, 0x6e, 0x61, 0x95, 0x10, 0x66,
    0x22, 0xf1, 0xa2, 0x19, 0xe2, 0xaf, 0x7f, 0x6e,
    0x14, 0x80, 0x52, 0x72, 0xd8, 0x80, 0x3f, 0xf8,
    0x6, 0xf9, 0x88, 0x30, 0xf, 0xfe, 0xa0, 0x92,
    0x33, 0xcd, 0x5d, 0xb3, 0x1b, 0xbe, 0xcb, 0xb4,
    0xc3, 0xaa, 0x10, 0x7, 0xf8, 0x52, 0x72, 0xdc,
    0xc0, 0x3f, 0xf8, 0x6, 0xf9, 0x87, 0x30, 0xf,
    0xff, 0xc0, 0xa4, 0x6d, 0xb9, 0x80, 0x7f, 0xf0,
    0xe, 0x20, 0xa0, 0x1f, 0xff, 0xa1, 0x48, 0xdc,
    0x73, 0x0, 0xff, 0xe0, 0x18, 0x7, 0xff, 0xf4,
    0x4e, 0x37, 0x20, 0xc4, 0x3, 0x9, 0xc4, 0x18,
    0x3, 0xff, 0xfe, 0x2, 0x6f, 0x9d, 0xde, 0xc7,
    0x30, 0xc,

    /* U+F013 "" */
    0x0, 0xff, 0xea, 0x8a, 0x3c, 0xcf, 0xfc, 0xe8,
    0x20, 0x1f, 0xff, 0x31, 0x7f, 0x86, 0x6f, 0xfa,
    0x3d, 0x0, 0x3f, 0xfc, 0xa4, 0x66, 0x20, 0xf,
    0xf1, 0x7a, 0x80, 0x7f, 0xf0, 0x5b, 0x4c, 0x3,
    0xfc, 0x46, 0x62, 0x0, 0xff, 0xe9, 0xa, 0x46,
    0x6e, 0x63, 0x75, 0x2a, 0x40, 0x1e, 0x62, 0x10,
    0xf, 0xfe, 0x9, 0x11, 0x80, 0x3c, 0x4b, 0x3b,
    0xac, 0xcc, 0xe6, 0x1, 0xff, 0xcc, 0x14, 0x9c,
    0xb7, 0x30, 0xc, 0x4d, 0x5b, 0x93, 0x13, 0x7b,
    0x48, 0x1, 0xff, 0xc3, 0x5a, 0xdb, 0x98, 0xad,
    0xd5, 0x31, 0x0, 0x42, 0x71, 0x98, 0x83, 0x0,
    0xff, 0xe3, 0x93, 0xed, 0xb1, 0x0, 0x7f, 0xc6,
    0xce, 0xc8, 0x40, 0x1f, 0xfc, 0x72, 0x46, 0x75,
    0x20, 0xf, 0xf8, 0xdf, 0x6d, 0x44, 0x3, 0xff,
    0xfe, 0x0, 0x10, 0x30, 0xf, 0xfe, 0x29, 0x3e,
    0xca, 0x8, 0x7, 0xff, 0xa4, 0x56, 0xa8, 0xc2,
    0x1, 0xff, 0xc6, 0x16, 0xab, 0x51, 0x0, 0xff,
    0xf2, 0x8a, 0xd5, 0x14, 0x40, 0x3f, 0xf9, 0x66,
    0xc2, 0x1, 0xff, 0xc4, 0x25, 0x8b, 0xde, 0xff,
    0xee, 0xda, 0x84, 0x20, 0xf, 0xfe, 0x20, 0xb1,
    0x8, 0x7, 0xff, 0x8, 0x4d, 0x59, 0xdd, 0x13,
    0x57, 0x90, 0x60, 0x1f, 0xfc, 0x22, 0x6b, 0xca,
    0x74, 0x21, 0x0, 0xe1, 0x25, 0x7b, 0xcb, 0x62,
    0x0, 0xff, 0xe1, 0x1c, 0x6d, 0x44, 0x1d, 0x95,
    0xc, 0x84, 0x2, 0x4b, 0xca, 0x98, 0x83, 0xb2,
    0xa1, 0x8, 0x7, 0xff, 0x4, 0x5a, 0x64, 0x82,
    0x1, 0xff, 0xc3, 0x14, 0x99, 0x30, 0x80, 0x7f,
    0xf0, 0x44, 0xd5, 0xdd, 0x13, 0x57, 0x9b, 0x68,
    0x0, 0x10, 0xf, 0xfe, 0x6a, 0x62, 0x0, 0x7f,
    0xf2, 0x53, 0x10, 0x3, 0xff, 0x9a, 0x20, 0x1f,
    0xfc, 0xf1, 0x31, 0x0, 0xff, 0xe5, 0x88, 0x7,
    0xff, 0xa8, 0x54, 0xc4, 0x3, 0xff, 0x92, 0x46,
    0xc0, 0x1f, 0xfd, 0x7, 0xf6, 0x54, 0x32, 0x10,
    0xf, 0xfe, 0x31, 0xcc, 0x20, 0x7, 0xff, 0x1d,
    0x2e, 0x4c, 0x3, 0xff, 0x8e, 0x24, 0x68, 0xde,
    0xe0, 0x2, 0x59, 0xab, 0xcd, 0xef, 0xfa, 0xd8,
    0x40, 0x3f, 0xf8, 0x22, 0xd5, 0x90, 0x82, 0x1,
    0xff, 0xa, 0x46, 0x53, 0x8, 0x7, 0xff, 0x4,
    0x56, 0xbb, 0xfe, 0xed, 0xcb, 0x95, 0x20, 0xf,
    0xfe, 0xa, 0x53, 0x88, 0x7, 0xff, 0x8, 0x4d,
    0xef, 0xb7, 0x2a, 0x65, 0x13, 0x2a, 0xcd, 0xeb,
    0x73, 0x10, 0xf, 0xfe, 0x10, 0xbf, 0x28, 0x80,
    0x7f, 0xf2, 0x85, 0x61, 0xc4, 0x3, 0xff, 0x8e,
    0x46, 0xac, 0xc7, 0x66, 0x29, 0x90, 0x7, 0xff,
    0x1c, 0x5f, 0xd8, 0x80, 0x3f, 0xf9, 0x4, 0xfb,
    0x48, 0x20, 0x1f, 0xfe, 0x71, 0x49, 0xf8, 0x30,
    0xf, 0xfe, 0x29, 0xd4, 0x98, 0x7, 0xff, 0xb4,
    0x9e, 0x90, 0x3, 0xff, 0x88, 0x73, 0x4c, 0x40,
    0x1f, 0xfd, 0xc1, 0x0, 0xff, 0xe1, 0x12, 0xd4,
    0x20, 0x7, 0xff, 0x14, 0x52, 0x72, 0xd8, 0x80,
    0x3c, 0x26, 0xf7, 0xdf, 0xed, 0xb6, 0x20, 0xf,
    0xfe, 0x19, 0x35, 0xef, 0xf7, 0x2d, 0xcc, 0x40,
    0x3c, 0x2b, 0x5f, 0x4a, 0x20, 0x1f, 0xfc, 0x91,
    0x49, 0xcc, 0x42, 0xa9, 0xeb, 0x31, 0x8, 0x20,
    0x2, 0x4a, 0x41, 0x0, 0xff, 0xe0, 0x8a, 0x6a,
    0x10, 0x0, 0x52, 0x33, 0xed, 0xd5, 0x4f, 0x7f,
    0x4a, 0x40, 0x1f, 0xfc, 0xf1, 0x37, 0xaa, 0x42,
    0x90, 0x7, 0xe2, 0xe2, 0x0, 0xff, 0xe0, 0x97,
    0x28, 0x7, 0xe1, 0x48, 0xaa, 0x42, 0x8, 0x7,
    0xff, 0x88, 0x51, 0x2, 0x1, 0xff, 0xa, 0x20,
    0x40, 0x3f, 0xfe, 0x67, 0x1d, 0xff, 0xff, 0x74,
    0x18, 0x7, 0xff, 0x4c,

    /* U+F023 "" */
    0x0, 0xff, 0xe5, 0x12, 0x34, 0x4d, 0xdb, 0x37,
    0x6c, 0xc5, 0xd4, 0xc3, 0x21, 0x0, 0x7f, 0xf6,
    0x85, 0x63, 0x3b, 0x6e, 0x5d, 0x91, 0x6, 0x45,
    0x19, 0x91, 0x59, 0xe6, 0xf7, 0xae, 0x10, 0x40,
    0x3f, 0xfa, 0x44, 0xd7, 0xf4, 0xe6, 0x20, 0x1f,
    0xfc, 0x81, 0x47, 0xbf, 0xa5, 0x10, 0xf, 0xfe,
    0x69, 0xc7, 0xca, 0x8, 0x7, 0xc2, 0x6a, 0xf1,
    0x35, 0x49, 0x87, 0x53, 0x10, 0xf, 0x85, 0x6b,
    0x5c, 0x80, 0x3f, 0xf9, 0x24, 0xfe, 0xe4, 0x1,
    0xe1, 0x48, 0xce, 0xca, 0x87, 0x65, 0x53, 0x3c,
    0x56, 0x7e, 0x39, 0x80, 0x7c, 0x71, 0x2c, 0x20,
    0x1f, 0xfc, 0x62, 0xaa, 0x18, 0x7, 0x85, 0x6a,
    0xdc, 0xc0, 0x3f, 0xf8, 0x22, 0x71, 0xb4, 0x80,
    0x1f, 0x25, 0x30, 0x80, 0x7f, 0xf1, 0x5, 0xc,
    0x80, 0x3c, 0x4d, 0x28, 0x1, 0xff, 0xc7, 0x15,
    0xf6, 0x10, 0xf, 0x1e, 0xa0, 0x7, 0xff, 0x10,
    0xf9, 0x80, 0x3e, 0x6e, 0x30, 0xf, 0xfe, 0x52,
    0xf9, 0x0, 0x7c, 0xc4, 0x20, 0x1f, 0xfc, 0x31,
    0x0, 0xff, 0x18, 0x7, 0xff, 0x54, 0x40, 0xc0,
    0x3f, 0xff, 0xe0, 0x1f, 0xff, 0x13, 0x7b, 0xde,
    0xc7, 0x20, 0xf, 0x92, 0xbf, 0xff, 0xf9, 0x7d,
    0x26, 0x1, 0xf1, 0x46, 0xf6, 0xd3, 0x10, 0x6,
    0x49, 0x84, 0x21, 0x0, 0xff, 0xf2, 0x9, 0x2c,
    0x59, 0x80, 0x42, 0x1, 0xff, 0xec, 0x10, 0x70,
    0xf, 0xff, 0xf8, 0x7, 0xff, 0xfc, 0x3, 0xff,
    0xfe, 0x1, 0xff, 0xff, 0x0, 0xff, 0xff, 0x80,
    0x7f, 0xff, 0xc0, 0x3f, 0xfd, 0xa2, 0x60, 0x1f,
    0xfe, 0xb1, 0x13, 0x80, 0x49, 0x72, 0xa6, 0x42,
    0x3f, 0xfe, 0x42, 0x36, 0xba, 0x30,

    /* U+F05A "" */
    0x0, 0xff, 0xe9, 0x11, 0xa2, 0xb3, 0xc4, 0xca,
    0xab, 0xa6, 0x51, 0x7, 0x65, 0x32, 0x0, 0xff,
    0xf5, 0x12, 0xbd, 0x67, 0x7e, 0xe5, 0xd4, 0xc3,
    0xb3, 0x15, 0x79, 0x98, 0xee, 0x89, 0xac, 0xdf,
    0xec, 0xa8, 0x53, 0x10, 0xf, 0xfe, 0xe9, 0x2c,
    0xe7, 0x6d, 0x42, 0x98, 0x80, 0x7f, 0xf3, 0x44,
    0xd5, 0xeb, 0x3f, 0x25, 0x88, 0x3, 0xff, 0xaa,
    0x29, 0x39, 0xb4, 0xc6, 0x20, 0x1f, 0xf1, 0xc6,
    0xff, 0xff, 0x74, 0xa0, 0x7, 0xfc, 0x26, 0xd3,
    0xba, 0x95, 0x20, 0xf, 0xfe, 0x70, 0xa4, 0xe5,
    0xb1, 0x80, 0x7f, 0xf1, 0x1c, 0x48, 0x3, 0xf8,
    0x4c, 0x40, 0x3f, 0xf8, 0x84, 0xd5, 0x94, 0xa2,
    0x1, 0xff, 0xc9, 0x27, 0xdb, 0x62, 0x0, 0xff,
    0xf4, 0x8a, 0xd6, 0x39, 0x80, 0x7f, 0xf1, 0x49,
    0xfa, 0xc, 0x3, 0xff, 0x94, 0xde, 0x86, 0x7f,
    0xc9, 0xac, 0x1, 0xff, 0xca, 0x28, 0xe8, 0x30,
    0xf, 0xfe, 0x8, 0xac, 0x40, 0xc0, 0x3f, 0xf9,
    0x84, 0xd7, 0x99, 0xfd, 0x6e, 0x60, 0x1f, 0xfc,
    0xc2, 0x78, 0x61, 0x0, 0xff, 0x13, 0x4a, 0x80,
    0x7f, 0xfd, 0xd2, 0xe4, 0x80, 0x3f, 0x12, 0xd9,
    0x80, 0x7f, 0xf2, 0x56, 0xff, 0xff, 0xf8, 0xb2,
    0x80, 0x1f, 0xfc, 0xf2, 0x5a, 0x20, 0xf, 0x93,
    0xc, 0x3, 0xff, 0x92, 0x26, 0x1, 0xff, 0xc6,
    0x31, 0x0, 0xff, 0xe8, 0x11, 0xa0, 0x80, 0x71,
    0xf2, 0x80, 0x7f, 0xff, 0x57, 0xd4, 0x3, 0x9c,
    0x84, 0x3, 0xff, 0x94, 0x2b, 0xac, 0xab, 0x21,
    0x0, 0x7f, 0xf7, 0x84, 0xc4, 0x3, 0xff, 0xa6,
    0x6d, 0x35, 0x57, 0xa0, 0x7, 0xff, 0x7c, 0x40,
    0x3c, 0xe6, 0x20, 0x1f, 0xff, 0xe1, 0x10, 0x7,
    0x17, 0x10, 0x7, 0xff, 0xf4, 0xb5, 0x80, 0x38,
    0x51, 0x44, 0x3, 0xff, 0xf8, 0x86, 0x40, 0x1e,
    0x3d, 0x71, 0x0, 0xff, 0xfc, 0x8a, 0xea, 0x0,
    0x7e, 0x4b, 0x62, 0x0, 0xff, 0xe4, 0x2d, 0x7f,
    0xeb, 0x60, 0xf, 0xf9, 0x6b, 0xff, 0x5b, 0x8,
    0x7, 0xff, 0x1c, 0x9e, 0x50, 0x3, 0xfc, 0x53,
    0xa, 0x20, 0x1f, 0xfc, 0x51, 0x31, 0x0, 0xff,
    0xe5, 0xb8, 0x98, 0x7, 0xff, 0x19, 0x2e, 0xc,
    0x3, 0xff, 0x80, 0x2b, 0x70, 0x82, 0x1, 0xff,
    0xf0, 0x49, 0x93, 0x8, 0x7, 0xff, 0xc, 0x5a,
    0xa8, 0xc4, 0x1, 0xff, 0xc1, 0x15, 0xc4, 0x4f,
    0xfe, 0x61, 0x90, 0x7, 0xff, 0x4, 0x56, 0xa8,
    0xc4, 0x1, 0xff, 0xc7, 0x14, 0x9f, 0xa5, 0x20,
    0xf, 0xf8, 0xde, 0xef, 0xff, 0x97, 0x8, 0x1,
    0xff, 0xa, 0xce, 0x52, 0x88, 0x7, 0xff, 0x30,
    0x96, 0xb3, 0x10, 0x82, 0x1, 0xff, 0xdd, 0x14,
    0x7b, 0xfa, 0x62, 0x0, 0xff, 0xe9, 0x9, 0xbd,
    0xf6, 0xd3, 0xa1, 0x0, 0x7f, 0xf4, 0xc9, 0x1e,
    0xb3, 0xee, 0x10, 0x40, 0x3f, 0xfb, 0x44, 0xb1,
    0x7b, 0xfb, 0x73, 0xc, 0xa8, 0x64, 0x20, 0x1f,
    0xc2, 0x46, 0x8a, 0xcf, 0x37, 0xbd, 0xb7, 0xa,
    0x62, 0x1, 0xff, 0xe4, 0x24, 0x67, 0x9a, 0xbc,
    0xde, 0xff, 0xfe, 0xed, 0xcb, 0xa9, 0x86, 0x42,
    0x10, 0xf, 0xfe, 0x78,

    /* U+F09E "" */
    0x0, 0x8d, 0xa6, 0xa9, 0x32, 0x88, 0x3b, 0x2a,
    0x19, 0x8, 0x7, 0xff, 0xb9, 0x66, 0x4c, 0xaa,
    0x66, 0x3b, 0xa2, 0x6a, 0xf3, 0x7b, 0xfb, 0x72,
    0xa5, 0xd4, 0xc4, 0x3, 0xff, 0xec, 0x24, 0x6a,
    0xd1, 0x59, 0xdc, 0xda, 0x85, 0x31, 0x0, 0xff,
    0xfc, 0x9, 0x2b, 0xd6, 0x76, 0xdc, 0x21, 0x0,
    0x7f, 0xf6, 0x13, 0xd8, 0xc8, 0x44, 0x1, 0xff,
    0xcf, 0x24, 0x7b, 0xdd, 0x53, 0x98, 0x80, 0x7f,
    0xf4, 0xc5, 0x67, 0x37, 0xb9, 0xff, 0x76, 0xe5,
    0xcc, 0x32, 0x99, 0x0, 0x7f, 0xf1, 0xc9, 0x63,
    0x31, 0x4c, 0x60, 0x1f, 0xfd, 0xa1, 0x23, 0x46,
    0x79, 0xac, 0xdf, 0xdb, 0x97, 0x41, 0x0, 0xff,
    0xe2, 0x12, 0xce, 0x62, 0x50, 0x40, 0x3f, 0xfc,
    0x24, 0x8d, 0x17, 0xdb, 0x70, 0xa4, 0x1, 0xff,
    0xc3, 0x36, 0xbc, 0x94, 0x10, 0xf, 0xfe, 0x68,
    0x80, 0x7f, 0xf2, 0x89, 0x1e, 0xb7, 0xb2, 0x10,
    0x40, 0x3f, 0xf8, 0x24, 0xd7, 0xb0, 0x60, 0x1f,
    0xfc, 0x72, 0x7d, 0xfe, 0xff, 0x77, 0x37, 0x2e,
    0x61, 0x90, 0x80, 0x3f, 0xf8, 0x62, 0x6f, 0x7f,
    0x4c, 0x40, 0x1f, 0xfc, 0x13, 0x7d, 0xb5, 0x10,
    0xf, 0xfe, 0x23, 0xe9, 0x0, 0x61, 0x11, 0x1a,
    0x33, 0xcd, 0xef, 0xf6, 0x4b, 0x18, 0x80, 0x7f,
    0xf0, 0x5, 0x67, 0x72, 0x10, 0x40, 0x3f, 0xe1,
    0x4a, 0xd7, 0x20, 0xf, 0xfe, 0xa0, 0x9b, 0x4e,
    0x76, 0xd3, 0x98, 0x80, 0x7f, 0xc6, 0xf5, 0x46,
    0x20, 0xf, 0xfe, 0x1, 0xc7, 0x31, 0x0, 0x7f,
    0xf0, 0x18, 0xc4, 0x3, 0xff, 0x92, 0x4b, 0x19,
    0x8a, 0x62, 0x0, 0xff, 0xa, 0x4f, 0xca, 0x8,
    0x7, 0xfc, 0x73, 0x25, 0x10, 0xf, 0xf1, 0xcf,
    0xe5, 0xd4, 0xc3, 0xaa, 0x10, 0x7, 0xff, 0x10,
    0x96, 0x77, 0x1c, 0x80, 0x3f, 0xc4, 0xd5, 0x68,
    0x1, 0xff, 0xc0, 0x48, 0x93, 0x0, 0xff, 0x84,
    0xd1, 0x59, 0xe2, 0xaf, 0x7f, 0xae, 0x58, 0xc4,
    0x3, 0xff, 0x82, 0x71, 0x94, 0xa2, 0x1, 0xfc,
    0x2b, 0x30, 0x60, 0x1f, 0xf1, 0x3c, 0x28, 0x80,
    0x7f, 0xf2, 0x5, 0x1a, 0x73, 0xed, 0xcc, 0x40,
    0x3f, 0xe1, 0x5a, 0xa3, 0x10, 0x7, 0xf1, 0x3f,
    0xb1, 0x0, 0x7f, 0xc9, 0x2e, 0x20, 0x1f, 0xfc,
    0xc1, 0x48, 0xcc, 0x4a, 0x8, 0x7, 0xfc, 0x91,
    0x5, 0x10, 0xf, 0xe3, 0x8b, 0x40, 0xf, 0xf8,
    0xe9, 0x88, 0x3, 0xfc, 0x24, 0x8c, 0xef, 0x32,
    0x98, 0x80, 0x78, 0x9a, 0xf5, 0xc8, 0x3, 0xfc,
    0x2b, 0x12, 0x60, 0x1f, 0xc2, 0xdc, 0x80, 0x1f,
    0xf1, 0xe2, 0x8, 0x7, 0x89, 0xaf, 0xf6, 0xe6,
    0x22, 0x9a, 0xcf, 0xc7, 0x30, 0xf, 0x1c, 0x73,
    0x90, 0x7, 0xf8, 0x9a, 0x50, 0x3, 0xfc, 0xba,
    0x80, 0x1f, 0xf2, 0x62, 0x0, 0x61, 0x69, 0x92,
    0x8, 0x7, 0xf0, 0x9c, 0x73, 0x90, 0x7, 0x8e,
    0x20, 0xa0, 0x1f, 0xe1, 0x5e, 0x50, 0xf, 0xf3,
    0xe1, 0x80, 0x7f, 0xce, 0x44, 0x0, 0x97, 0xd0,
    0x3, 0xff, 0x86, 0x58, 0xe0, 0x1f, 0x97, 0x98,
    0x40, 0x3f, 0xcb, 0x88, 0x1, 0xfc, 0x26, 0xa0,
    0x1f, 0xf1, 0x6b, 0x80, 0x4c, 0x24, 0x1, 0xff,
    0xc3, 0x12, 0x10, 0xf, 0xe4, 0xc4, 0x0, 0xff,
    0x94, 0xc4, 0x3, 0xf9, 0xb8, 0x80, 0x3f, 0xe1,
    0x30, 0x8, 0xa6, 0x10, 0x40, 0x3f, 0xf8, 0x7,
    0x36, 0x80, 0x1f, 0xe7, 0xd3, 0x0, 0xff, 0x1f,
    0x28, 0x7, 0xf1, 0x88, 0x80, 0x3f, 0xe7, 0x1,
    0x0, 0x85, 0x6f, 0x29, 0xd4, 0xc8, 0xa3, 0x46,
    0x9d, 0xc7, 0x20, 0xf, 0xf8, 0x96, 0x90, 0x84,
    0x7c, 0x46, 0xd8, 0xa0, 0x1f, 0xc2, 0x94, 0x84,
    0x23, 0xf1, 0x25, 0x20, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 446, .box_w = 90, .box_h = 29, .ofs_x = -1, .ofs_y = -6},
    {.bitmap_index = 554, .adv_w = 411, .box_w = 81, .box_h = 27, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 1046, .adv_w = 309, .box_w = 63, .box_h = 24, .ofs_x = -1, .ofs_y = -1},
    {.bitmap_index = 1276, .adv_w = 411, .box_w = 84, .box_h = 27, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 1696, .adv_w = 377, .box_w = 75, .box_h = 24, .ofs_x = -1, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x11, 0x21, 0x58, 0x9c
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61442, .range_length = 157, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 5, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = gylph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 1
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
lv_font_t easywifimanager_40 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 29,          /*The maximum line height required by the font*/
    .base_line = 6,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_HOR,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};


#endif /*#if EASYWIFIMANAGER_40*/

#endif //BLYNKGO_USE_WIFI
