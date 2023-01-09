/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GKnob.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GKnob.h"
#include "objx/lv_knob.h"

#define LV_OBJX_NAME  "GKnob" 

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
    Serial.println("[GKnob] signal cleanup");
#endif
    GKnob* pKnob = (GKnob*) lv_obj_get_user_data(obj);

    if(pKnob->hasLocalStyle()) { pKnob->freeKnobStyle(); } //_has_localstyle = false; }
    pKnob->_created = false;
  }
  return LV_RES_OK;
}


#if BLYNKGO_USE_BLYNK
static void gknob_event_cb(lv_obj_t *obj, lv_event_t event);
#endif

GKnob::GKnob(GWidget& parent) : GObject(&parent) {}
GKnob::GKnob(GWidget* parent) : GObject(parent) {}
#if BLYNKGO_USE_BLYNK
GKnob::GKnob(uint8_t virtual_pin, GWidget& parent) :  vpin(virtual_pin), GObject(&parent) {}
#endif
GKnob::~GKnob() { this->del(); }

void GKnob::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;


  if(!this->isCreated()) {
    this->obj = lv_knob_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createKnobStyle();
      lv_obj_set_user_data(this->obj, this);
#if BLYNKGO_USE_BLYNK
      lv_obj_set_event_cb (this->obj, gknob_event_cb);
#else
      lv_obj_set_event_cb (this->obj, main_event_cb);
#endif

      this->align(ALIGN_CENTER,0,0);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

    }
  }
}

void GKnob::create(GWidget& parent) {
  this->_par = &parent;
  GKnob::create();
}

void GKnob::create(GWidget* parent) {
  this->_par = parent;
  GKnob::create();
}

void GKnob::createKnobStyle(){
  if(!this->isCreated()) GKnob::create();

  if(!this->hasLocalStyle()){
    this->style[INDIC].copy ( (style_t*) lv_knob_get_style(this->obj, LV_KNOB_STYLE_INDIC));
    this->style[BG].copy    ( (style_t*) lv_knob_get_style(this->obj, LV_KNOB_STYLE_BG));

    lv_knob_set_style(this->obj, LV_KNOB_STYLE_INDIC, this->style[INDIC].raw());
    lv_knob_set_style(this->obj, LV_KNOB_STYLE_BG   , this->style[BG].raw());

    this->_style = &this->style[BG];
    this->_has_localstyle = true;
  }
}

void GKnob::freeKnobStyle(){
  if(this->hasLocalStyle()){
    for(int i =0; i < 2; i++)
      this->style[i].release();
    this->_has_localstyle = false;
  }
}

GKnob& GKnob::operator =(int num){
  this->value(num);
  return *this;
}

void GKnob::color(uint16_t color){
  if(!this->isCreated()) create();
  this->style[BG].color( color, TFT_COLOR_MIX(color, TFT_BLACK, 128));
  this->style[INDIC].color(TFT_COLOR_MIX(color, TFT_BLACK, 128), color);
  this->style[INDIC].line_color(color);
}

void GKnob::range(int16_t val_min, int16_t val_max){
  if(!this->isCreated()) create();
  lv_knob_set_range(this->obj, val_min, val_max);
}

void GKnob::value(int16_t val){
  if(!this->isCreated()) create();
  lv_knob_set_value(this->obj, val);
}

int16_t GKnob::value(){
  if(!this->isCreated()) create();
  return lv_knob_get_value(this->obj);
}

int16_t GKnob::value_min(){
  if(!this->isCreated()) create();
  return lv_knob_get_value_min(this->obj);
}

int16_t GKnob::value_max(){
  if(!this->isCreated()) create();
  return lv_knob_get_value_max(this->obj);
}

#if BLYNKGO_USE_BLYNK
void GKnob::virtualPin(uint8_t vpin){
  if(!this->isCreated()) this->create();
  this->vpin = vpin;
}

uint8_t GKnob::virtualPin() {
  return this->vpin;
}

static void gknob_event_cb(lv_obj_t *obj, lv_event_t event) {
  GKnob* pKnob = (GKnob*) lv_obj_get_user_data(obj);
    /* update ไปยัง Blynk ด้วย*/
  if(event == EVENT_VALUE_CHANGED) {
    if(pKnob->virtualPin()) BlynkGO.virtualWrite(pKnob->virtualPin(), pKnob->value());
  }
  main_event_cb(obj, event);
}
#endif

