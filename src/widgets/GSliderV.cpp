/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSliderV.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GSliderV.h"

#define LV_OBJX_NAME  "GSliderV" 

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
    Serial.println("[GSliderV] signal cleanup");
#endif
    GSliderV* pSliderV = (GSliderV*) lv_obj_get_user_data(obj);
    if(pSliderV->hasLocalStyle()) { pSliderV->_has_localstyle = false; }
    pSliderV->_created = false;
  }
  return LV_RES_OK;
}

static void gsliderv_event_cb(lv_obj_t *obj, lv_event_t event);

GSliderV::GSliderV(GWidget& parent) 
: val(50), val_min(0), val_max(100), GObject(&parent) {}

GSliderV::GSliderV(GWidget* parent) 
: val(50), val_min(0), val_max(100), GObject(parent) {}

GSliderV::GSliderV(int16_t value_min, int16_t value_max, GWidget& parent)
:val_min(value_min), val_max(value_max), GObject(&parent)
{
  this->val = (this->val_min + this->val_max)/2;
}

GSliderV::GSliderV(int16_t value_min, int16_t value_max, GWidget* parent)
:val_min(value_min), val_max(value_max), GObject(parent)
{
  this->val = (this->val_min + this->val_max)/2;
}

GSliderV::~GSliderV() { this->del(); }

void GSliderV::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_vslider_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      // this->createSliderVStyle();
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gsliderv_event_cb);//main_event_cb);

      // this->style[KNOB].radius(LV_RADIUS_CIRCLE);

      // this->size(200,20);
      this->range(this->val_min, this->val_max);
      this->value(this->val);
      this->align(ALIGN_CENTER);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GSliderV::create(GWidget *parent){
  this->_par = parent;
  this->create();
}

void GSliderV::create(GWidget& parent) {
  this->_par = &parent;
  this->create();
}

// void GSliderV::createSliderVStyle(){
//   if(!this->hasLocalStyle()){
//     this->style[BG].copy   ( (style_t*)lv_slider_get_style(this->obj, LV_SLIDER_STYLE_BG));
//     this->style[INDIC].copy( (style_t*)lv_slider_get_style(this->obj, LV_SLIDER_STYLE_INDIC));
//     this->style[KNOB].copy ( (style_t*)lv_slider_get_style(this->obj, LV_SLIDER_STYLE_KNOB));

//     lv_slider_set_style(this->obj, LV_SLIDER_STYLE_BG,    this->style[BG].raw());
//     lv_slider_set_style(this->obj, LV_SLIDER_STYLE_INDIC, this->style[INDIC].raw());
//     lv_slider_set_style(this->obj, LV_SLIDER_STYLE_KNOB,  this->style[KNOB].raw());

//     this->_style = (GStyle*) &this->style[INDIC];
//     this->_has_localstyle = true;
//   }
// }

GSliderV& GSliderV::operator =(int num){
  this->val = num;

  if(!this->isCreated()) this->create();
  if(this->isCreated()) this->value(this->val);

  return *this;
}

void GSliderV::range(int16_t val_min, int16_t val_max){
  if(!this->isCreated()) this->create();

  lv_vslider_set_range(this->obj, val_min, val_max);
}

void GSliderV::value(int16_t val){
  if(!this->isCreated()) this->create();
  lv_vslider_set_value(this->obj, val);
}

int16_t GSliderV::value(){
  if(!this->isCreated()) this->create();
  return lv_vslider_get_value(this->obj);
}

void  GSliderV::title(const char* title){
  if(!this->isCreated()) this->create();
  lv_vslider_set_text(this->obj, title);
}

const char*  GSliderV::title(){
  if(!this->isCreated()) this->create();
  return lv_vslider_get_text(this->obj);
}

void  GSliderV::unit(const char* unit){
  if(!this->isCreated()) this->create();
  lv_vslider_set_unit(this->obj, unit);
}

const char* GSliderV::unit(){
  if(!this->isCreated()) this->create();
  return lv_vslider_get_unit(this->obj);
}

#if BLYNKGO_USE_BLYNK
void GSliderV::virtualPin(uint8_t vpin){
  if(!this->isCreated()) this->create();
  this->vpin = vpin;
}

uint8_t GSliderV::virtualPin() {
  return this->vpin;
}
#endif


void gsliderv_event_cb(lv_obj_t *obj, lv_event_t event) {
  GSliderV* pSliderV = (GSliderV*) lv_obj_get_user_data(obj);
  if(event == EVENT_VALUE_CHANGED) {
#if BLYNKGO_USE_BLYNK
    /* update ไปยัง Blynk ด้วย*/
    if(pSliderV->virtualPin()) BlynkGO.virtualWrite(pSliderV->virtualPin(), pSliderV->value());
#endif
  }
  main_event_cb(obj, event);
}
