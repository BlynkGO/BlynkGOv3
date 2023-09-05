/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GImageButton.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "BlynkGOv3.h"   // BlynkGO object
#include "GImageButton.h"

#define LV_OBJX_NAME  "GImageButton" 

static lv_signal_cb_t ancestor_signal_cb;
static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if(res != LV_RES_OK ) return res;

  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  if(sign == LV_SIGNAL_CLEANUP) {
#if BLYNKGO_DEBUG 
    Serial.println("[GImageButton] signal cleanup");
#endif
    GImageButton* pBTN = (GImageButton*) lv_obj_get_user_data(obj);

    if(pBTN->hasLocalStyle()) { pBTN->freeImageButtonStyle(); }  //pBTN->_has_localstyle = false; }
    pBTN->_created = false;
  }
  return LV_RES_OK;
}

static void gimagebutton_event_cb(lv_obj_t *obj, lv_event_t event);


GImageButton::GImageButton(GWidget& parent) : GContainer(&parent) {}
GImageButton::GImageButton(GWidget* parent) : GContainer(parent) {}
GImageButton::GImageButton( const void *src_btn_rel, const void *src_btn_pr,const void *src_toggle_rel,const void *src_toggle_pr,const void *src_btn_inactive, 
                            GWidget& parent)
: img_btn_rel(src_btn_rel), img_btn_pr(src_btn_pr), img_tgl_rel(src_toggle_rel),img_tgl_pr(src_toggle_pr), img_btn_ina(src_btn_inactive),
GContainer(&parent) { }

GImageButton::GImageButton( const void *src, GWidget& parent)
: img_btn_rel(src), img_btn_pr(src), img_tgl_rel(src),img_tgl_pr(src), img_btn_ina(src),
GContainer(&parent) { }

GImageButton::GImageButton( const lv_img_dsc_t &src, GWidget& parent)
: img_btn_rel(&src), img_btn_pr(&src), img_tgl_rel(&src),img_tgl_pr(&src), img_btn_ina(&src),
GContainer(&parent) { }

GImageButton::GImageButton( const lv_img_dsc_t &src, GWidget* parent)
: img_btn_rel(&src), img_btn_pr(&src), img_tgl_rel(&src),img_tgl_pr(&src), img_btn_ina(&src),
GContainer(parent) { }

GImageButton::GImageButton( const lv_img_dsc_t &src_btn_rel,  const lv_img_dsc_t &src_btn_pr, const lv_img_dsc_t &src_toggle_rel, const lv_img_dsc_t &src_toggle_pr, const lv_img_dsc_t &src_btn_inactive, 
                            GWidget& parent)
: img_btn_rel(&src_btn_rel), img_btn_pr(&src_btn_pr), img_tgl_rel(&src_toggle_rel),img_tgl_pr(&src_toggle_pr), img_btn_ina(&src_btn_inactive),
GContainer(&parent) { }

GImageButton::GImageButton( button_type_t type,  
                            const lv_img_dsc_t &src_btn_rel, const lv_img_dsc_t &src_btn_pr,
                            GWidget& parent)
: img_btn_rel(&src_btn_rel), img_btn_pr(&src_btn_pr), img_tgl_rel(&src_btn_pr),img_tgl_pr(&src_btn_pr), img_btn_ina(NULL),
_type(type),GContainer(&parent) { }

GImageButton::GImageButton( const lv_img_dsc_t &src_btn_rel, 
                  const lv_img_dsc_t &src_btn_pr,
                  GWidget& parent)
: img_btn_rel(&src_btn_rel), img_btn_pr(&src_btn_pr), img_tgl_rel(&src_btn_pr),img_tgl_pr(&src_btn_pr), img_btn_ina(NULL),
_type(BUTTON_PUSH),GContainer(&parent) { }

GImageButton::GImageButton( button_type_t type, 
                            const lv_img_dsc_t &src_btn_rel, 
                            GWidget& parent)
: img_btn_rel(&src_btn_rel), img_btn_pr(NULL), img_tgl_rel(NULL),img_tgl_pr(NULL), img_btn_ina(NULL),
_type(type),GContainer(&parent) { }


GImageButton::~GImageButton() { this->del(); }


void GImageButton::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_imgbtn_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createImageButtonStyle();
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gimagebutton_event_cb);////main_event_cb); //gbutton_event_cb);//


      if( this->img_btn_rel != NULL) {
        lv_imgbtn_set_src(this->obj, LV_BTN_STATE_REL,    this->img_btn_rel);
        lv_imgbtn_set_src(this->obj, LV_BTN_STATE_PR,     (this->img_btn_pr!=NULL)?  this->img_btn_pr  : this->img_btn_rel);
        lv_imgbtn_set_src(this->obj, LV_BTN_STATE_TGL_REL,(this->img_tgl_rel!=NULL)? this->img_tgl_rel : this->img_btn_rel);
        lv_imgbtn_set_src(this->obj, LV_BTN_STATE_TGL_PR, (this->img_tgl_pr!=NULL)?  this->img_tgl_pr  : this->img_btn_rel);
        lv_imgbtn_set_src(this->obj, LV_BTN_STATE_INA,    (this->img_btn_ina!=NULL)? this->img_btn_ina : this->img_btn_rel);
      }

      this->mode((this->_type == BUTTON_SWITCH)? BUTTON_MODE_TOGGLE : BUTTON_MODE_NORMAL);      
      this->align(ALIGN_CENTER,0,0);
      // this->auto_realign(true);

      // this->mode(BUTTON_MODE_NORMAL);
      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      // ขณะ ปุ่ม non-toggle แล้วกด
      if(img_btn_pr == img_btn_rel) {
        this->style[PRESSED].image_color(TFT_BLACK);
        this->style[PRESSED].image_intense(128);
      }
      // ขณะ ปุ่ม toggle อยู่ แล้วกด
      if(img_tgl_pr == img_btn_rel) {
        this->style[TOGGLE_PRESSED].image_color(TFT_BLACK);
        this->style[TOGGLE_PRESSED].image_intense(128);
      }
      // ขณะ ปุ่ม toggle
      if(img_tgl_rel == img_btn_rel) {
        this->style[TOGGLE_RELEASED].image_color(TFT_BLACK);
        this->style[TOGGLE_RELEASED].image_intense(200);
      }
    }
  }
}

void GImageButton::create(GWidget *parent){
  this->_par = parent;
  GImageButton::create();
}

void GImageButton::create(GWidget &parent){
  this->_par = &parent;
  GImageButton::create();
}

void GImageButton::createImageButtonStyle(){
  if(!this->isCreated()) create();

  if(!this->hasLocalStyle()){
    // alloc & copy
    this->style[RELEASED].copy       ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_REL) );
    this->style[PRESSED].copy        ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_PR) );
    this->style[TOGGLE_RELEASED].copy( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_TGL_REL) );
    this->style[TOGGLE_PRESSED].copy ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_TGL_PR) );
    this->style[INACTIVE].copy       ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_INA) );

    lv_btn_set_style(this->obj, LV_BTN_STYLE_REL,     this->style[RELEASED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_PR,      this->style[PRESSED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_TGL_REL, this->style[TOGGLE_RELEASED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_TGL_PR,  this->style[TOGGLE_PRESSED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_INA,     this->style[INACTIVE].raw());

    this->_style = (GStyle*) &this->style[RELEASED];
    this->_has_localstyle = true;
  }
}

void GImageButton::freeImageButtonStyle(){
  if(this->hasLocalStyle()){
    for(int i =0; i < 5; i++)
      this->style[i].release();
    this->_has_localstyle = false;
  }
}
/* button's mode : BUTTON_MODE_NORMAL or BUTTON_MODE_TOGGLE or BUTTON_MODE_INACTIVE */
void  GImageButton::mode(button_mode_t _mode){
  if(!this->isCreated()) this->create();

  switch(_mode) {
    case BUTTON_MODE_NORMAL:  //---> BUTTON_PUSH
      lv_btn_set_toggle(this->obj, false);
      this->state(BUTTON_RELEASED);
      break;
    case BUTTON_MODE_TOGGLE:  //--> BUTTON_SWITCH
      lv_btn_set_toggle(this->obj, true);
      break;
    case BUTTON_MODE_INACTIVE:
      this->state(BUTTON_INACTIVE);
      break;
  }
}

button_mode_t GImageButton::mode(){
  if(!this->isCreated()) create();
  if(this->state() == BUTTON_INACTIVE) return BUTTON_MODE_INACTIVE;
  return (button_mode_t) lv_btn_get_toggle(this->obj);
}

void GImageButton::toggle(){
  if(!this->isCreated()) create();
  lv_btn_toggle(this->obj);
}

void GImageButton::ON()           { if(mode() == BUTTON_MODE_TOGGLE) this->state(TOGGLE_RELEASED); }
void GImageButton::OFF()          { if(mode() == BUTTON_MODE_TOGGLE) this->state(RELEASED); }
bool GImageButton::isON()         { return this->state() == TOGGLE_RELEASED;}
bool GImageButton::isOFF()        { return this->state() != TOGGLE_RELEASED;}

void GImageButton::inactive(bool enable){
  static bool isON = false;

  if(enable){
    if(this->_type == BUTTON_SWITCH) {
      isON = this->isON();    // จำสถานะ เปิด หรือ ปิด หากปุ่มเป็นแบบ BUTTON_SWITCH
    }
    this->mode(BUTTON_MODE_INACTIVE);
  }else{
    if(this->_type == BUTTON_PUSH ) {
      this->mode(BUTTON_MODE_NORMAL);
    }else
    if(this->_type == BUTTON_SWITCH) {
      this->mode(BUTTON_MODE_TOGGLE);
      (isON)? this->ON() : this->OFF();
    }
  }
}

/* button's state : RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE, */
void GImageButton::state(button_state_t button_state){
  if(!this->isCreated()) create();

  if( this->mode()==BUTTON_MODE_TOGGLE ) {
    button_state = (button_state==1)? 2 : button_state;
  }

  lv_btn_set_state(this->obj, (lv_btn_state_t) button_state);
  // this->clickable(button_state != INACTIVE);

  this->event_send(EVENT_VALUE_CHANGED);
}

button_state_t GImageButton::state(){
  if(!this->isCreated()) create();
  return lv_btn_get_state(this->obj);
}


void GImageButton::setImage(const void *src_btn_rel, 
                            const void *src_btn_pr,
                            const void *src_toggle_rel,
                            const void *src_toggle_pr,
                            const void *src_btn_inactive)
{
  this->img_btn_rel     = src_btn_rel;
  this->img_btn_pr      = src_btn_pr;
  this->img_tgl_rel     = src_toggle_rel;
  this->img_tgl_pr      = src_toggle_pr;
  this->img_btn_ina     = src_btn_inactive;

  if(!this->isCreated()) GImageButton::create();
  else{
    lv_imgbtn_set_src(this->obj, LV_BTN_STATE_REL,    this->img_btn_rel);
    lv_imgbtn_set_src(this->obj, LV_BTN_STATE_PR,     (this->img_btn_pr!=NULL)?  this->img_btn_pr  : this->img_btn_rel);
    lv_imgbtn_set_src(this->obj, LV_BTN_STATE_TGL_REL,(this->img_tgl_rel!=NULL)? this->img_tgl_rel : this->img_btn_rel);
    lv_imgbtn_set_src(this->obj, LV_BTN_STATE_TGL_PR, (this->img_tgl_pr!=NULL)?  this->img_tgl_pr  : this->img_btn_rel);
    lv_imgbtn_set_src(this->obj, LV_BTN_STATE_INA,    (this->img_btn_ina!=NULL)? this->img_btn_ina : this->img_btn_rel);
  }

}

void GImageButton::setImage(const lv_img_dsc_t & src_btn){
  this->setImage((const void *)& src_btn);
}

void GImageButton::setImage(
              const lv_img_dsc_t &src_btn_rel, 
              const lv_img_dsc_t &src_btn_pr,
              const lv_img_dsc_t &src_toggle_rel,
              const lv_img_dsc_t &src_toggle_pr,
              const lv_img_dsc_t &src_btn_inactive)
{
  this->setImage( (const void *) &src_btn_rel,
                  (const void *) &src_btn_pr,
                  (const void *) &src_toggle_rel,
                  (const void *) &src_toggle_pr,
                  (const void *) &src_btn_inactive);
}

  // button_state_t : RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE
void GImageButton::setImage(button_state_t btn_type, const void *src){
  if(!this->isCreated()) GImageButton::create();

  switch(btn_type){
    case RELEASED: {
      this->img_btn_rel = src;
      lv_imgbtn_set_src(this->obj, LV_BTN_STATE_REL,    this->img_btn_rel);
    }break;
    case PRESSED: {
      this->img_btn_pr  = src;
      lv_imgbtn_set_src(this->obj, LV_BTN_STATE_PR,     this->img_btn_pr);
    }
    break;
    case TOGGLE_RELEASED: {
      this->img_tgl_rel = src;
      lv_imgbtn_set_src(this->obj, LV_BTN_STATE_TGL_REL,this->img_tgl_rel);
    }
    break;
    case TOGGLE_PRESSED: {
      this->img_tgl_pr  = src; 
      lv_imgbtn_set_src(this->obj, LV_BTN_STATE_TGL_PR, this->img_tgl_pr);
    }
    break;
    case INACTIVE: {
      this->img_btn_ina = src;
      lv_imgbtn_set_src(this->obj, LV_BTN_STATE_INA,    this->img_btn_ina);
    }
    break;
  }

  // this->realign();
}


GImageButton& GImageButton::operator =(const img_t * src_img){
  if(!this->isCreated()) create();
  this->setImage(src_img);
  return *this;
}

GImageButton& GImageButton::operator =(const img_t & src_img){
  if(!this->isCreated()) create();
  this->setImage(src_img);
  return *this;
}

void GImageButton::gpio(uint8_t pin, bool active_high){
  if(!this->isCreated()) this->create();
  this->gpin = pin;
  this->gpin_active = active_high;
  pinMode(this->gpin, OUTPUT);
  digitalWrite(this->gpin, !this->gpin_active);  // ให้ไม่ active ไปก่อน (ปิดไปก่อน)
}

uint8_t GImageButton::gpio() { 
  if(!this->isCreated()) create();
  return this->gpin;
}

void GImageButton::active_high(bool active_high){
  if(!this->isCreated()) create();
  this->gpin_active = active_high;
}

bool  GImageButton::active_high(){
  if(!this->isCreated()) create();
  return this->gpin_active;
}

#if BLYNKGO_USE_BLYNK
void GImageButton::virtualPin(uint8_t vpin){
  if(!this->isCreated()) create();
  this->vpin = vpin;
}

uint8_t GImageButton::virtualPin() {
  if(!this->isCreated()) create();
  return this->vpin;
}
#endif


static void gimagebutton_event_cb(lv_obj_t *obj, lv_event_t event)
{
  GImageButton* pGImgBtn = (GImageButton*) lv_obj_get_user_data(obj);
  bool pressed = false;

  if(event == EVENT_PRESSED || event == EVENT_RELEASED || event == EVENT_PRESS_LOST) {
    if(pGImgBtn->mode() == BUTTON_MODE_NORMAL){ // --> BUTTON_PUSH
      if(event == EVENT_PRESSED) pressed = true;
      if(event == EVENT_RELEASED || event == EVENT_PRESS_LOST) pressed = false;

      // Serial.printf("Push Set : %d : %d\n", pGBtn->state(), pressed );
      if(pGImgBtn->gpio()) digitalWrite(pGImgBtn->gpio(), pressed? pGImgBtn->active_high() : !pGImgBtn->active_high());
#if BLYNKGO_USE_BLYNK
      if(pGImgBtn->virtualPin()) BlynkGO.virtualWrite(pGImgBtn->virtualPin(), pressed );
#endif
    }

    // else
    // if(pGImgBtn->mode() ==BUTTON_MODE_TOGGLE) { // --> BUTTON_SWITCH

    // }
  } else
  if(event == EVENT_VALUE_CHANGED) {
    switch(pGImgBtn->mode()){
      case BUTTON_MODE_NORMAL: pressed = (pGImgBtn->state()==1)? true : false; break;
      case BUTTON_MODE_TOGGLE: pressed = (pGImgBtn->state()==2)? true : false; break;
    }
    // Serial.printf("Set : %d : %d\n", pGBtn->state(), pressed );

    if(pGImgBtn->gpio()) digitalWrite(pGImgBtn->gpio(), pressed? pGImgBtn->active_high() : !pGImgBtn->active_high());
#if BLYNKGO_USE_BLYNK
    if(pGImgBtn->virtualPin()) BlynkGO.virtualWrite(pGImgBtn->virtualPin(), pressed );
#endif
  }
  main_event_cb(obj, event);
}