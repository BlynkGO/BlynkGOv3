/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSwitch.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GSwitch.h"

#define LV_OBJX_NAME  "GSwitch" 

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
    Serial.println("[GSwitch] signal cleanup");
#endif
    GSwitch* pSW = (GSwitch*) lv_obj_get_user_data(obj);

    if(pSW->hasLocalStyle()) { pSW->freeSwitchStyle(); } //pSW->_has_localstyle = false; } // pSW->freeLocalStyle(); } //
    pSW->_created = false;
  }
  return LV_RES_OK;
}

static void gswitch_event_cb(lv_obj_t *obj, lv_event_t event);

GSwitch::GSwitch(GWidget& parent) : GObject(&parent) {}
GSwitch::GSwitch(GWidget* parent) : GObject(parent)  {}

#if BLYNKGO_USE_BLYNK
GSwitch::GSwitch(uint8_t virtual_pin, GWidget &parent)
: vpin(virtual_pin),GObject(&parent) {}
#endif

GSwitch::~GSwitch() { this->del(); }

void GSwitch::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_sw_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createSwitchStyle();
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gswitch_event_cb);//main_event_cb);
      
      this->color(TFT_BLUE_LVGL);
	    this->align(ALIGN_CENTER,0,0);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GSwitch::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GSwitch::create(GWidget* parent) {
  this->_par = parent;
  create();
}

void GSwitch::createSwitchStyle(){
  if(!this->isCreated()) create();

  if(!this->hasLocalStyle()){
    // alloc & copy
    this->style[BG].copy      ( (style_t*)lv_sw_get_style(this->obj, LV_SW_STYLE_BG));
    this->style[INDIC].copy   ( (style_t*)lv_sw_get_style(this->obj, LV_SW_STYLE_INDIC));
    this->style[KNOB_OFF].copy( (style_t*)lv_sw_get_style(this->obj, LV_SW_STYLE_KNOB_OFF));
    this->style[KNOB_ON].copy ( (style_t*)lv_sw_get_style(this->obj, LV_SW_STYLE_KNOB_ON));


    lv_color_t color_indic = lv_color_hex(0x9fc8ef);

    this->style[INDIC].radius(RADIUS_CIRCLE);
    this->style[INDIC].color( color_indic.full, color_indic.full);
    this->style[INDIC].padding(0);

    this->style[KNOB_OFF].radius(RADIUS_CIRCLE);
    this->style[KNOB_OFF].shadow(4);
    this->style[KNOB_OFF].padding(-2);

    this->style[KNOB_ON].radius(RADIUS_CIRCLE);
    this->style[KNOB_ON].shadow(4);
    this->style[KNOB_ON].padding(-2);

    this->style[BG].radius(RADIUS_CIRCLE);
    this->style[BG].border(2);

    lv_sw_set_style(this->obj, LV_SW_STYLE_BG,      this->style[BG].raw());
    lv_sw_set_style(this->obj, LV_SW_STYLE_INDIC,   this->style[INDIC].raw());
    lv_sw_set_style(this->obj, LV_SW_STYLE_KNOB_ON, this->style[KNOB_OFF].raw());
    lv_sw_set_style(this->obj, LV_SW_STYLE_KNOB_OFF,this->style[KNOB_ON].raw());


    this->_style = (GStyle*) &this->style[INDIC];
    this->_has_localstyle = true;
  }
}

void GSwitch::freeSwitchStyle(){
  if(this->hasLocalStyle()){
    for(int i =0; i < 4; i++)
      this->style[i].release();
    this->_has_localstyle = false;
  }
}

void    GSwitch::knob_color(color_t color){
  if(!this->isCreated()) create();
  this->style[KNOB_ON].color(color, color);
  this->style[KNOB_OFF].color(color, color);
}

void    GSwitch::knob_color(color_t color, color_t grad_color){
  if(!this->isCreated()) create();
  this->style[KNOB_ON].color(color, grad_color);
  this->style[KNOB_OFF].color(color, grad_color);
}

color_t GSwitch::knob_color(){
  if(!this->isCreated()) create();
  return this->style[KNOB_ON].color();
}

color_t GSwitch::knob_grad_color(){
  if(!this->isCreated()) create();
  return this->style[KNOB_ON].grad_color();
}

bool GSwitch::state(){
  if(!this->isCreated()) create();
  return lv_sw_get_state(this->obj);
}

void  GSwitch::state(bool sw_on){
  if(!this->isCreated()) create();
  if(sw_on) {
    lv_sw_on(this->obj, false);
  } else {
    lv_sw_off(this->obj, false);
  }
  if(this->gpio()) digitalWrite(this->gpio(),  sw_on? this->active_high() : !this->active_high());
}

void GSwitch::ON(bool anim_enable) {
  if(!this->isCreated()) create();
  lv_sw_on(this->obj, anim_enable);
  if(this->gpio()) digitalWrite(this->gpio(), this->active_high());
}

void GSwitch::OFF(bool anim_enable) {
  if(!this->isCreated()) create();
  lv_sw_off(this->obj, anim_enable);
  if(this->gpio()) digitalWrite(this->gpio(), !this->active_high());
}

bool GSwitch::isON() {
  if(!this->isCreated()) create();
  return (this->state() == true);
}

bool GSwitch::isOFF() {
  if(!this->isCreated()) create();
  return (this->state() == false);
}

void GSwitch::toggle(){
  if(!this->isCreated()) create();
  lv_sw_toggle(this->obj, true);
}

void GSwitch::gpio(uint8_t pin, bool active_high){
  if(!this->isCreated()) create();
  this->gpin = pin;
  this->gpin_active = active_high;
  pinMode(this->gpin, OUTPUT);
  digitalWrite(this->gpin, !this->gpin_active);  // ให้ไม่ active ไปก่อน (ปิดไปก่อน)  
}

uint8_t GSwitch::gpio() { 
  if(!this->isCreated()) create();
  return this->gpin;
}

void GSwitch::active_high(bool active_high){
  if(!this->isCreated()) create();
  this->gpin_active = active_high;
}

bool  GSwitch::active_high(){
  return this->gpin_active;
}

#if BLYNKGO_USE_BLYNK
void GSwitch::virtualPin(uint8_t vpin){
  if(!this->isCreated()) create();
  this->vpin = vpin;
}

uint8_t GSwitch::virtualPin() {
  if(!this->isCreated()) create();
  return this->vpin;
}
#endif

void gswitch_event_cb(lv_obj_t *obj, lv_event_t event) {
  GSwitch* pSW = (GSwitch*) lv_obj_get_user_data(obj);
  if(event == EVENT_VALUE_CHANGED) {
    if(pSW->gpio()) digitalWrite(pSW->gpio(), pSW->isON()? pSW->active_high() : !pSW->active_high());
#if BLYNKGO_USE_BLYNK
    /* update ไปยัง Blynk ด้วย*/
    if(pSW->virtualPin()) BlynkGO.virtualWrite(pSW->virtualPin(), pSW->isON());
#endif
  }
  main_event_cb(obj, event);
}
