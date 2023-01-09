/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSliderH.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GSliderH.h"

#define LV_OBJX_NAME  "GSliderH" 

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
    Serial.println("[GSliderH] signal cleanup");
#endif
    GSliderH* pSliderH = (GSliderH*) lv_obj_get_user_data(obj);
    if(pSliderH->hasLocalStyle()) { pSliderH->_has_localstyle = false; }
    pSliderH->_created = false;
  }
  return LV_RES_OK;
}

static void gsliderh_event_cb(lv_obj_t *obj, lv_event_t event);

GSliderH::GSliderH(GWidget& parent) 
: val(50), val_min(0), val_max(100), GObject(&parent) {}

GSliderH::GSliderH(GWidget* parent) 
: val(50), val_min(0), val_max(100), GObject(parent) {}

GSliderH::GSliderH(int16_t value_min, int16_t value_max, GWidget& parent)
:val_min(value_min), val_max(value_max), GObject(&parent)
{
  this->val = (this->val_min + this->val_max)/2;
}

GSliderH::GSliderH(int16_t value_min, int16_t value_max, GWidget* parent)
:val_min(value_min), val_max(value_max), GObject(parent)
{
  this->val = (this->val_min + this->val_max)/2;
}

#if BLYNKGO_USE_BLYNK
GSliderH::GSliderH(uint8_t virtual_pin, GWidget& parent)
: val(50), val_min(0), val_max(100), vpin(virtual_pin), GObject(&parent) {}

GSliderH::GSliderH(int16_t val_min, int16_t val_max, uint8_t virtual_pin, GWidget& parent)
:val_min(val_min), val_max(val_max), vpin(virtual_pin), GObject(&parent)
{
  this->val = (this->val_min + this->val_max)/2;
}
#endif

GSliderH::~GSliderH() { this->del(); }

void GSliderH::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_hslider_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gsliderh_event_cb);//main_event_cb);

      this->range(this->val_min, this->val_max);
      this->value(this->val);
      this->align(ALIGN_CENTER);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GSliderH::create(GWidget *parent){
  this->_par = parent;
  this->create();
}

void GSliderH::create(GWidget& parent) {
  this->_par = &parent;
  this->create();
}


GSliderH& GSliderH::operator =(int num){
  this->val = num;

  if(!this->isCreated()) this->create();
  if(this->isCreated()) this->value(this->val);

  return *this;
}

void GSliderH::range(int16_t val_min, int16_t val_max){
  if(!this->isCreated()) this->create();

  lv_hslider_set_range(this->obj, val_min, val_max);
}

void GSliderH::value(int16_t val){
  if(!this->isCreated()) this->create();
  lv_hslider_set_value(this->obj, val);
}

int16_t GSliderH::value(){
  if(!this->isCreated()) this->create();
  return lv_hslider_get_value(this->obj);
}

void  GSliderH::title(const char* title){
  if(!this->isCreated()) this->create();
  lv_hslider_set_text(this->obj, title);
}

const char*  GSliderH::title(){
  if(!this->isCreated()) this->create();
  return lv_hslider_get_text(this->obj);
}

void  GSliderH::unit(const char* unit){
  if(!this->isCreated()) this->create();
  lv_hslider_set_unit(this->obj, unit);
}

const char* GSliderH::unit(){
  if(!this->isCreated()) this->create();
  return lv_hslider_get_unit(this->obj);
}

#if BLYNKGO_USE_BLYNK
void GSliderH::virtualPin(uint8_t vpin){
  if(!this->isCreated()) this->create();
  this->vpin = vpin;
}

uint8_t GSliderH::virtualPin() {
  return this->vpin;
}
#endif


void gsliderh_event_cb(lv_obj_t *obj, lv_event_t event) {
  GSliderH* pSliderH = (GSliderH*) lv_obj_get_user_data(obj);
  if(event == EVENT_VALUE_CHANGED) {
#if BLYNKGO_USE_BLYNK
    /* update ไปยัง Blynk ด้วย*/
    if(pSliderH->virtualPin()) BlynkGO.virtualWrite(pSliderH->virtualPin(), pSliderH->value());
#endif
  }
  main_event_cb(obj, event);
}
