/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GSwitch3D.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GSwitch3D.h"
#include "objx/lv_sw3d.h"

#define LV_OBJX_NAME  "GSwitch3D" 

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
    Serial.println("[GSwitch3D] signal cleanup");
#endif
    GSwitch3D* pSW3D = (GSwitch3D*) lv_obj_get_user_data(obj);
    if(pSW3D->hasLocalStyle()) { pSW3D->_has_localstyle = false; }
    pSW3D->_created = false;
  }
  return LV_RES_OK;
}

static void gswitch3d_event_cb(lv_obj_t *obj, lv_event_t event);

GSwitch3D::GSwitch3D(GWidget& parent) : GObject(&parent) {}
GSwitch3D::GSwitch3D(GWidget *parent) : GObject(parent) {}

GSwitch3D::GSwitch3D(uint16_t color , sw3d_type_t type, GWidget &parent) 
:_color(color), _sw3d_type(type), GObject(&parent) {}

#if BLYNKGO_USE_BLYNK
GSwitch3D::GSwitch3D(uint16_t color, sw3d_type_t type, uint8_t virtual_pin, GWidget &parent)
:_color(color), vpin(virtual_pin), _sw3d_type(type), GObject(&parent) {}
#endif

GSwitch3D::~GSwitch3D() { this->del(); }

void GSwitch3D::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;
  
  if(!this->isCreated()) {
    this->obj = lv_sw3d_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createLocalStyle();
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gswitch3d_event_cb);//main_event_cb);

      //Serial.printf("[GSwitch3D] type : %d\n", this->_sw3d_type);
      this->sw3d_type(this->_sw3d_type);
      this->color(this->_color);
      this->OFF();
      this->align(ALIGN_CENTER,0,0);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

    }
  }
}

void GSwitch3D::create(GWidget *parent){
  this->_par = parent;
  this->create();
}

void GSwitch3D::create(GWidget &parent){
  this->_par = &parent;
  this->create();
}

void GSwitch3D::sw3d_type(sw3d_type_t type) {
  if(!this->isCreated()) this->create();
  lv_sw3d_set_type(this->obj, type);
}

sw3d_type_t GSwitch3D::sw3d_type(){
  if(!this->isCreated()) this->create();
  return lv_sw3d_get_type(this->obj);
}

void GSwitch3D::ON(){
  if(!this->isCreated()) this->create();
  // lv_sw3d_set_state( this->obj, true);
  this->state(true);
}

void GSwitch3D::OFF(){
  if(!this->isCreated()) this->create();
  // lv_sw3d_set_state( this->obj, false);
  this->state(false);
}

bool GSwitch3D::isON(){
  if(!this->isCreated()) this->create();
  return lv_sw3d_get_state( this->obj);
}

bool GSwitch3D::isOFF(){
  return !this->isON();
}

void GSwitch3D::toggle(){
  if(!this->isCreated()) this->create();
  this->state(!this->state());
}

void GSwitch3D::state(bool sw_state){
  if(!this->isCreated()) this->create();
  lv_sw3d_set_state( this->obj, sw_state);
  if(this->gpio()) digitalWrite(this->gpio(), this->isON()? this->active_high() : !this->active_high());
}

bool GSwitch3D::state(){
  if(!this->isCreated()) this->create();
  return lv_sw3d_get_state( this->obj);
}

#if BLYNKGO_USE_BLYNK
void GSwitch3D::virtualPin(uint8_t vpin){
  if(!this->isCreated()) this->create();
  this->vpin = vpin;
}

uint8_t GSwitch3D::virtualPin() {
  if(!this->isCreated()) create();
  return this->vpin;
}
#endif

void GSwitch3D::gpio(uint8_t pin, bool active_high){
  if(!this->isCreated()) this->create();
  this->gpin = pin;
  this->gpin_active = active_high;
  pinMode(this->gpin, OUTPUT);
  digitalWrite(this->gpin, !this->gpin_active);  // ให้ไม่ active ไปก่อน (ปิดไปก่อน)
}

uint8_t GSwitch3D::gpio() { 
  if(!this->isCreated()) create();
  return this->gpin;
}

void GSwitch3D::active_high(bool active_high){
  if(!this->isCreated()) create();
  this->gpin_active = active_high;
}

bool  GSwitch3D::active_high(){
  if(!this->isCreated()) create();
  return this->gpin_active;
}

void gswitch3d_event_cb(lv_obj_t *obj, lv_event_t event) {
  GSwitch3D* pSW3D = (GSwitch3D*) lv_obj_get_user_data(obj);
  if(event == EVENT_VALUE_CHANGED) {
    if(pSW3D->gpio()) digitalWrite(pSW3D->gpio(), pSW3D->isON()? pSW3D->active_high() : !pSW3D->active_high());
#if BLYNKGO_USE_BLYNK
    /* update ไปยัง Blynk ด้วย*/
    if(pSW3D->virtualPin()) BlynkGO.virtualWrite(pSW3D->virtualPin(), pSW3D->isON());
#endif
  }
  main_event_cb(obj, event);
}
