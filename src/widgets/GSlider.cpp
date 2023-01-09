/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GSlider.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GSlider.h"

#define LV_OBJX_NAME  "GSlider" 

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
    Serial.println("[GSlider] signal cleanup");
#endif
    GSlider* pSlider = (GSlider*) lv_obj_get_user_data(obj);
    if(pSlider->hasLocalStyle()) { pSlider->freeSliderStyle(); } //_has_localstyle = false; }
    pSlider->_created = false;
  }
  return LV_RES_OK;
}

#if BLYNKGO_USE_BLYNK
static void gslider_event_cb(lv_obj_t *obj, lv_event_t event);
#endif

GSlider::GSlider(GWidget& parent) 
: val(0), val_min(0), val_max(100), _slider_type(SLIDER_TYPE_NORMAL), GObject(&parent) {}

GSlider::GSlider(GWidget* parent) 
: val(0), val_min(0), val_max(100), GObject(parent) {}

GSlider::GSlider(int16_t value_min, int16_t value_max, GWidget& parent)
:val_min(value_min), val_max(value_max), _slider_type(SLIDER_TYPE_NORMAL), GObject(&parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}

GSlider::GSlider(int16_t value_min, int16_t value_max, GWidget* parent)
:val_min(value_min), val_max(value_max), GObject(parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}


#if BLYNKGO_USE_BLYNK
GSlider::GSlider(uint8_t virtual_pin, GWidget& parent)
: val(0), val_min(0), val_max(100), vpin(virtual_pin), _slider_type(SLIDER_TYPE_NORMAL), GObject(&parent) {}

GSlider::GSlider(uint8_t virtual_pin, GWidget* parent)
: val(0), val_min(0), val_max(100), vpin(virtual_pin), _slider_type(SLIDER_TYPE_NORMAL), GObject(parent) {}

GSlider::GSlider(int16_t value_min, int16_t value_max, uint8_t virtual_pin, GWidget& parent)
:val_min(value_min), val_max(value_max), vpin(virtual_pin), _slider_type(SLIDER_TYPE_NORMAL), GObject(&parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}

GSlider::GSlider(int16_t value_min, int16_t value_max, uint8_t virtual_pin, GWidget* parent)
:val_min(value_min), val_max(value_max), vpin(virtual_pin), _slider_type(SLIDER_TYPE_NORMAL), GObject(parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}

GSlider::GSlider(int16_t value_min, int16_t value_max, uint8_t virtual_pin, slider_type_t type, GWidget& parent)
:val_min(value_min), val_max(value_max), vpin(virtual_pin), _slider_type(type), GObject(&parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}

GSlider::GSlider(int16_t value_min, int16_t value_max, uint8_t virtual_pin, slider_type_t type, GWidget* parent)
:val_min(value_min), val_max(value_max), vpin(virtual_pin), _slider_type(type), GObject(parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}

#else

GSlider::GSlider(int16_t value_min, int16_t value_max, slider_type_t type, GWidget& parent)
:val_min(value_min), val_max(value_max), _slider_type(type), GObject(&parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}

GSlider::GSlider(int16_t value_min, int16_t value_max, slider_type_t type, GWidget* parent)
:val_min(value_min), val_max(value_max), _slider_type(type), GObject(parent)
{
  // this->val = (this->val_min + this->val_max)/2;
  this->val = this->val_min;
}
#endif

GSlider::~GSlider() { this->del(); }

void GSlider::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_slider_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createSliderStyle();
      lv_obj_set_user_data(this->obj, this);
#if BLYNKGO_USE_BLYNK
      lv_obj_set_event_cb (this->obj, gslider_event_cb);
#else
      lv_obj_set_event_cb (this->obj, main_event_cb);
#endif

      this->style[KNOB].radius(LV_RADIUS_CIRCLE);

      // this->size(200,20);
      this->size(GScreen.width()*42/100,20);
      this->slider_type(this->_slider_type);
      this->range(this->val_min, this->val_max);
      this->align(ALIGN_CENTER);
      // this->auto_realign(true);
      // lv_slider_set_knob_in(this->obj, false);
      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      this->value(this->val);
    }
  }
}

void GSlider::create(GWidget *parent){
  this->_par = parent;
  this->create();
}

void GSlider::create(GWidget& parent) {
  this->_par = &parent;
  this->create();
}

void GSlider::createSliderStyle(){
  if(!this->hasLocalStyle()){
    this->style[BG].copy   ( (style_t*)lv_slider_get_style(this->obj, LV_SLIDER_STYLE_BG));
    this->style[INDIC].copy( (style_t*)lv_slider_get_style(this->obj, LV_SLIDER_STYLE_INDIC));
    this->style[KNOB].copy ( (style_t*)lv_slider_get_style(this->obj, LV_SLIDER_STYLE_KNOB));

    this->style[INDIC].grad_dir(GRAD_DIR_HOR);  // เฉดสี ให้ไล่เฉดตามแนวนอน

    lv_slider_set_style(this->obj, LV_SLIDER_STYLE_BG,    this->style[BG].raw());
    lv_slider_set_style(this->obj, LV_SLIDER_STYLE_INDIC, this->style[INDIC].raw());
    lv_slider_set_style(this->obj, LV_SLIDER_STYLE_KNOB,  this->style[KNOB].raw());

    this->_style = (GStyle*) &this->style[INDIC];
    this->_has_localstyle = true;

  }
}

void GSlider::freeSliderStyle(){
  if(this->hasLocalStyle()){
    for(int i =0; i < 3; i++)
      this->style[i].release();
    this->_has_localstyle = false;
  }
}

GSlider& GSlider::operator =(int num){
  if(!this->isCreated()) this->create();

  lv_slider_ext_t *ext = (lv_slider_ext_t*)this->ext_attr();

  this->val = constrain(num, ext->bar.min_value, ext->bar.max_value);

  lv_slider_set_value(this->obj, this->val, false);
  // Serial.printf("[GSlider = ] this->val %d ; get val %d;  val_min %d - val_max %d\n", 
  //     this->val, ext->bar.cur_value,  ext->bar.min_value, ext->bar.max_value);

  this->invalidate();

  return *this;
}

void GSlider::range(int16_t val_min, int16_t val_max){
  if(!this->isCreated()) this->create();
  // this->val_min = val_min;
  // this->val_max = val_max;

  lv_slider_set_range(this->obj, val_min, val_max);

  this->val_min = lv_slider_get_min_value(this->obj);
  this->val_min = lv_slider_get_max_value(this->obj);
}

int16_t GSlider::max_value() {
  if(!this->isCreated()) this->create();
  return lv_slider_get_max_value(this->obj);
}

int16_t GSlider::min_value() {
  if(!this->isCreated()) this->create();
  return lv_slider_get_min_value(this->obj);
}

void GSlider::value(int16_t val, bool anim){
  if(!this->isCreated()) this->create();

  lv_slider_ext_t *ext = (lv_slider_ext_t*)this->ext_attr();
  this->val = constrain(val, ext->bar.min_value, ext->bar.max_value);
  lv_bar_set_value(this->obj, this->val, anim);
  this->invalidate();
}

int16_t GSlider::value(){
  if(!this->isCreated()) this->create();
  return lv_slider_get_value(this->obj); //this->val; //
}

void  GSlider::left_value(int16_t val , bool anim){
  if(!this->isCreated()) this->create();
  lv_slider_set_left_value(this->obj, val, anim);
}

int16_t GSlider::left_value() {
  if(!this->isCreated()) this->create();
  return lv_slider_get_left_value(this->obj);
}

void GSlider::knob_image(const img_t & src_img){
  if(!this->isCreated()) this->create();
  lv_slider_set_knob_img(this->obj, (const void*) &src_img );
}

void GSlider::knob_image(const img_t * src_img){
  if(!this->isCreated()) this->create();
  lv_slider_set_knob_img(this->obj, (const void*) src_img );
}

#if LV_USE_ANIMATION
void GSlider::anim_time(uint16_t time){
  if(!this->isCreated()) this->create();
  lv_slider_set_anim_time(this->obj, time);
}

uint16_t GSlider::anim_time(){
  if(!this->isCreated()) this->create();
  return lv_slider_get_anim_time(this->obj);
}
#endif

void GSlider::slider_type(slider_type_t type){
  if(!this->isCreated()) this->create();
  this->_slider_type = type;
  lv_slider_set_type(this->obj, type);
}

slider_type_t GSlider::slider_type(){
  if(!this->isCreated()) this->create();
  return lv_slider_get_type(this->obj);
}


#if BLYNKGO_USE_BLYNK
void GSlider::virtualPin(uint8_t vpin){
  if(!this->isCreated()) this->create();
  this->vpin = vpin;
}

uint8_t GSlider::virtualPin() {
  return this->vpin;
}

void gslider_event_cb(lv_obj_t *obj, lv_event_t event) {
  GSlider* pSlider = (GSlider*) lv_obj_get_user_data(obj);
    /* update ไปยัง Blynk ด้วย*/
  if(event == EVENT_VALUE_CHANGED) {
    if(pSlider->virtualPin()) BlynkGO.virtualWrite(pSlider->virtualPin(), pSlider->value());
  }
  main_event_cb(obj, event);
}
#endif
