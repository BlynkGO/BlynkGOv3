/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GStep.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GStep.h"
#include "GWidgets.h"

#define LV_OBJX_NAME  "GStep" 

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
    Serial.println("[GStep] signal cleanup");
#endif
    GStep* pStep = (GStep*) lv_obj_get_user_data(obj);
    gstep_ext_t* ext = (gstep_ext_t*)pStep->ext_attr();

      free_widget(ext->btnPlus);
      free_widget(ext->btnMinus);
      free_widget(ext->lbStep);

    if(pStep->hasLocalStyle())  pStep->freeLocalStyle();  // หาก มี localStyle จะให้ลบทิ้งเสีย
    pStep->_created = false;
  }
  return LV_RES_OK;
}

static void step_event_cb(GWidget *widget, event_t event);

GStep::GStep(GWidget& parent) : GObject(&parent) {}
GStep::GStep(GWidget* parent) : GObject(parent)  {}
GStep::GStep(int16_t val_min, int16_t val_max, GWidget& parent)
: _val_min(val_min), _val_max(val_max), GObject( &parent) {}

GStep::~GStep() { this->del(); }

void GStep::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_obj_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);
      lv_obj_set_style(this->obj, &lv_style_transp);

      

      gstep_ext_t* ext = (gstep_ext_t*)this->alloc_ext_attr(sizeof(gstep_ext_t));

      ext->btnPlus    = new_GButton(this);
      ext->btnMinus   = new_GButton(this);
      ext->lbStep     = new_GLabel(this);

      *ext->btnMinus = SYMBOL_MINUS;
      ext->btnMinus->font(prasanmit_20);
      ext->btnMinus->rounded(true);
      ext->btnMinus->size(40,40);
      ext->btnMinus->color(TFT_BLUE_LVGL);
      ext->btnMinus->event_cb( step_event_cb);
      ext->btnMinus->user_data(this);

      *ext->btnPlus = SYMBOL_PLUS;
      ext->btnPlus->font(prasanmit_20);
      ext->btnPlus->rounded(true);
      ext->btnPlus->size(40,40);
      ext->btnPlus->color(TFT_BLUE_LVGL);
      ext->btnPlus->event_cb( step_event_cb);
      ext->btnPlus->user_data(this);

      ext->lbStep->color(TFT_WHITE);
      ext->step          = 1;
      ext->step_internal = ext->step;
      ext->decimal       = 0;

      this->align(ALIGN_CENTER,0,0);
      this->auto_realign(true);

      this->value(0);
      this->range(this->_val_min,this->_val_max);


      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GStep::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GStep::create(GWidget* parent) {
  this->_par = parent;
  create();
}


void GStep::value(float val) {
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();

  val = constrain(val, this->_val_min, this->_val_max);

  if( this->_val == val) return;

  this->_val = val;
  *ext->lbStep = String((float)this->_val, (unsigned int)ext->decimal);
  this->event_send(EVENT_VALUE_CHANGED);
}

float GStep::value(){
  if(!this->isCreated()) create();
  return this->_val;
}

float GStep::value_min(){
  if(!this->isCreated()) create();
  return this->_val_min;
}

float GStep::value_max(){
  if(!this->isCreated()) create();
  return this->_val_max;
}

void GStep::step(float step) {
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();

  ext->step = LV_MATH_ABS(step);
  ext->step_internal = ext->step;
}

float GStep::step(){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  return ext->step;
}

void GStep::decimal(uint8_t decimal){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  ext->decimal = constrain(decimal,0,3);
  this->refresh_size();
}

uint8_t GStep::decimal(){
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  return ext->decimal;
}


void GStep::plus(float step){
  if(!this->isCreated()) create();
  this->value(this->value() + step);
}

void GStep::minus(float step){
  if(!this->isCreated()) create();
  this->value(this->value() - step);
}

GStep& GStep::operator =(float num){
  this->value(num);
  return *this;
}

void GStep::font(font_t &f){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  ext->lbStep->font(f);

  this->refresh_size();
}

void GStep::font(font_t &font, color_t color){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  ext->lbStep->font(font,color);
  this->refresh_size();
}

const font_t* GStep::font(){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  return ext->lbStep->font();
}

void GStep::text_color(uint16_t color){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  ext->lbStep->color(color);
}

void GStep::color(color_t color){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  ext->btnPlus->color(color);
  ext->btnMinus->color(color);
}

color_t GStep::color(){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();
  return ext->btnPlus->color();
}

void GStep::refresh_size(){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();

  int16_t cur_val = constrain(this->_val, this->_val_min, this->_val_max); 

  uint16_t label_w =0; 
  this->value(this->value_min());
  label_w = ext->lbStep->width();
  this->value(this->value_max());
  label_w = LV_MATH_MAX( label_w, ext->lbStep->width());

  this->value(cur_val);

  uint16_t w = ext->btnPlus->width() + ext->btnMinus->width() + 10 + label_w;
  uint16_t h = LV_MATH_MAX(LV_MATH_MAX(ext->btnPlus->height(), ext->btnMinus->height())
                           , ext->lbStep->height() );

  this->size(w,h);

  ext->lbStep->align(ALIGN_CENTER);  ext->lbStep->auto_realign(true);
  ext->btnMinus->align(ALIGN_IN_LEFT_MID);  ext->btnMinus->auto_realign(true);
  ext->btnPlus->align (ALIGN_IN_RIGHT_MID); ext->btnPlus->auto_realign(true);
}

void GStep::range(float val_min, float val_max){
  if(!this->isCreated()) create();
  gstep_ext_t* ext = (gstep_ext_t*)this->ext_attr();

  this->_val_min = LV_MATH_MIN(val_min, val_max);
  this->_val_max = LV_MATH_MAX(val_min, val_max);

  this->refresh_size();
}

// static void step_event_cb(lv_obj_t *obj, lv_event_t event){
static void step_event_cb(GWidget *widget, event_t event) {
  GButton* pBtn     = (GButton*) widget;
  GStep* pStep      = (GStep*) pBtn->user_data();
  gstep_ext_t* ext  = (gstep_ext_t*)pStep->ext_attr();

  screen_timer_update();

  static enum { STATE_STANDBY,  STATE_SPEED_STEP } state = STATE_STANDBY; 
  // static int8_t step = 1;
  static uint32_t timer;

  switch(state) {
    case STATE_STANDBY:
      if(event == EVENT_CLICKED) {
        if(*ext->btnMinus == *pBtn) {
          pStep->value(pStep->value()-ext->step_internal);
        }else
        if(*ext->btnPlus == *pBtn) {
          pStep->value(pStep->value()+ext->step_internal);
        }
      }else
      if(event == EVENT_LONG_PRESSED) {
        state = STATE_SPEED_STEP;
        timer = millis() + 2000;
      }
      break;
    case STATE_SPEED_STEP:
      if(event == EVENT_PRESSING) {
        if( millis() >= timer) { ext->step_internal += 5*ext->step; timer = millis()+500; }
        if(*ext->btnMinus == *pBtn) {
          pStep->value(pStep->value()-ext->step_internal);
        }else
        if(*ext->btnPlus == *pBtn) {
          pStep->value(pStep->value()+ext->step_internal);
        }
      }else
      if(event == EVENT_RELEASED) {
        state = STATE_STANDBY;
        ext->step_internal = ext->step;
      }
      break;
  }
}
