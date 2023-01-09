/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GLed.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GLed.h"

#define LV_OBJX_NAME  "GLed" 

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
#if BLYNKGO_DEBUG 
    Serial.println("[GLed] signal cleanup");
#endif
    GLed* pLed = (GLed*) lv_obj_get_user_data(obj);

    if(pLed->hasLocalStyle()) {pLed->freeLocalStyle(); } //pLed->_has_localstyle = false; } // 
    pLed->_created = false;
  }
  return LV_RES_OK;
}

static void gled_event_cb(lv_obj_t *obj, lv_event_t event);

GLed::GLed(GWidget& parent) :_color(TFT_RED), GObject(&parent) {}
GLed::GLed(GWidget* parent) :_color(TFT_RED), GObject(parent) {}
GLed::GLed(uint16_t color, GWidget& parent) :_color(color), GObject(&parent) {}
#if BLYNKGO_USE_BLYNK
GLed::GLed(uint16_t color, uint8_t virtual_pin, GWidget &parent)
:_color(color), vpin(virtual_pin), GObject(&parent) {}
#endif
GLed::~GLed() { this->del(); }

void GLed::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  
  if(!this->isCreated()) {
    this->obj = lv_led_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createLocalStyle(&lv_style_pretty_color);

      this->color(this->_color, TFT_COLOR_MIX(this->_color, TFT_BLACK, 128));
      this->corner_radius(LV_RADIUS_CIRCLE);
      this->border_opa(LV_OPA_30);
      this->border(3);
      this->shadow(15);
      this->shadow_opa(200);
      this->shadow_offset(0,0);
      this->shadow_spread(5);

      // this->_style->color(this->_color, TFT_COLOR_MIX(this->_color, TFT_BLACK, 128));
      // this->_style->radius(LV_RADIUS_CIRCLE);
      // this->_style->border_opa(LV_OPA_30);
      // this->_style->border_color(TFT_COLOR_MIX(this->_color, TFT_BLACK, 200));
      // this->_style->border(3);
      // this->_style->shadow_color(this->_color);
      // this->_style->shadow(15);
      // this->_style->shadow_opa(200);
      // this->_style->shadow_offset(0,0);
      // this->_style->shadow_spread(5);


      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gled_event_cb); //main_event_cb);

      this->align(ALIGN_CENTER,0,0);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GLed::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GLed::create(GWidget* parent) {
  this->_par = parent;
  create();
}


void GLed::color(uint16_t color16) {
  if(!this->isCreated()) this->create();

  this->_color = color16;

  this->_style->color(this->_color, TFT_COLOR_MIX(this->_color, TFT_BLACK, 128));
  this->_style->border_color(TFT_COLOR_MIX(this->_color, TFT_BLACK, 200));
  this->_style->shadow_color(this->_color);

  this->refreshStyle();
}

void GLed::color(uint16_t color16, uint16_t gradian_color16){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) this->createLocalStyle(&lv_style_pretty_color);

  this->_color = color16;

  this->_style->color(this->_color, gradian_color16);
  this->_style->border_color(TFT_COLOR_MIX(this->_color, TFT_BLACK, 200));
  this->_style->shadow_color(this->_color);

  this->refreshStyle();
}

void GLed::OFF() {
  if(!this->isCreated()) this->create();
  lv_led_off(this->obj);
  if(this->_state != false) {
    this->_state = false;
    this->event_send(EVENT_VALUE_CHANGED);
  }
}

void GLed::ON() {
  if(!this->isCreated()) this->create();
  lv_led_on(this->obj);
  if(this->_state != true) {
    this->_state = true;
    this->event_send(EVENT_VALUE_CHANGED);
  } 
}

bool GLed::isON(){
  if(!this->isCreated()) this->create();
  return (this->_state == true);
}

bool GLed::isOFF(){
  return (this->_state == false);
}

bool GLed::toggle() {
  if(!this->isCreated()) this->create();

  bool state = !this->_state;
  if(state) this->ON();
    else    this->OFF();

  return this->state();
}

void GLed::brightness(uint8_t brightness) {
  if(!this->isCreated()) this->create();
  lv_led_set_bright(this->obj, brightness);
}

uint8_t GLed::brightness(){
  if(!this->isCreated()) this->create();
  return lv_led_get_bright(this->obj);
}

/* ไว้กำหนดจาก ภายนอก แต่ไม่มีการส่ง EVENT_VALUE_CHANGED */
void GLed::state(bool led_state){
  if(!this->isCreated()) this->create();
  // this->_state = led_state;
  if(led_state) this->ON();
  else          this->OFF();
}

bool GLed::state()  { 
  if(!this->isCreated()) this->create();
  return _state;
}

#if BLYNKGO_USE_BLYNK
void GLed::virtualPin(uint8_t vpin){
  if(!this->isCreated()) this->create();
  this->vpin = vpin;
}

uint8_t GLed::virtualPin() {
  return this->vpin;
}
#endif

void GLed::gpio(uint8_t pin, bool active_high){
  if(!this->isCreated()) this->create();
  this->gpin = pin;
  this->gpin_active = active_high;
  pinMode(this->gpin, OUTPUT);
  digitalWrite(this->gpin, !this->gpin_active);  // ให้ไม่ active ไปก่อน (ปิดไปก่อน)
}

uint8_t GLed::gpio() { 
  if(!this->isCreated()) create();
  return this->gpin;
}

void GLed::active_high(bool active_high){
  if(!this->isCreated()) create();
  this->gpin_active = active_high;
}

bool  GLed::active_high(){
  if(!this->isCreated()) create();
  return this->gpin_active;
}


void gled_event_cb(lv_obj_t *obj, lv_event_t event) {
  GLed* pLED = (GLed*) lv_obj_get_user_data(obj);
  if(event == EVENT_VALUE_CHANGED) {
    if(pLED->gpio()) digitalWrite(pLED->gpio(), pLED->isON()? pLED->active_high() : !pLED->active_high());
#if BLYNKGO_USE_BLYNK
    /* update ไปยัง Blynk ด้วย*/
    if(pLED->virtualPin()) BlynkGO.virtualWrite(pLED->virtualPin(), pLED->isON());
#endif
  }
  main_event_cb(obj, event);
}
