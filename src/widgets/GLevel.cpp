
/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GLevel.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GLevel.h"
#include "GWidgets.h"

#define LV_OBJX_NAME  "GLevel" 

static lv_signal_cb_t ancestor_signal_cb;
static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if(res != LV_RES_OK ) return res;
  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  GLevel* pLevel = (GLevel*) lv_obj_get_user_data(obj);
  glevel_ext_t* ext = (glevel_ext_t*) pLevel->ext_attr();

  uint16_t w = pLevel->width();
  uint16_t h = pLevel->height();

  if(sign == LV_SIGNAL_CORD_CHG || sign == LV_SIGNAL_STYLE_CHG) {
    if( w != h) {
      if(h > w) {
        pLevel->_vertical_design();
      }else
      if( w > h) {
        pLevel->_horizontal_design();
      }
    }
  }


  /* หลัง del() หรือ deconstrutor ถูกเรียก และภายในได้ลบ child ไปแล้ว */
  if(sign == LV_SIGNAL_CLEANUP) {
#if BLYNKGO_DEBUG 
    Serial.println("[GLevel] signal cleanup");
#endif


    free_widget(ext->btn_up);
    free_widget(ext->btn_down);
    free_widget(ext->bar_indic);
    free_widget(ext->bar_bg);

    if(pLevel->hasLocalStyle()) {pLevel->freeLocalStyle(); } //pLed->_has_localstyle = false; } // 
    pLevel->_created = false;
  }
  return LV_RES_OK;
}

// static void GButton_level_up_event_cb(GWidget*btn, event_t event);
// static void GButton_level_down_event_cb(GWidget*btn, event_t event);
static void GButton_level_up_event_cb(GWidget*btn, event_t event)
{
  static bool long_pressed = false;
  GLevel* pLevel = (GLevel*) btn->user_data();
  glevel_ext_t* ext = (glevel_ext_t*) pLevel->ext_attr();
  
  switch (event) {
    case EVENT_PRESSED:
      pLevel->value(pLevel->value() + 1);
      break;
    case EVENT_PRESSING:
      if (!long_pressed) return;
      pLevel->value(pLevel->value() + 1);
      break;
    case EVENT_RELEASED:
      long_pressed = false;
      break;
    case EVENT_LONG_PRESSED:
      long_pressed = true;
      break;
  }
}

static void GButton_level_down_event_cb(GWidget*btn, event_t event)
{
  static bool long_pressed = false;
  GLevel* pLevel = (GLevel*) btn->user_data();
  glevel_ext_t* ext = (glevel_ext_t*) pLevel->ext_attr();
  
  switch (event) {
    case EVENT_PRESSED:
      pLevel->value(pLevel->value() - 1);
      break;
    case EVENT_PRESSING:
      if (!long_pressed) return;
      pLevel->value(pLevel->value() - 1);
      break;
    case EVENT_RELEASED:
      long_pressed = false;
      break;
    case EVENT_LONG_PRESSED:
      long_pressed = true;
      break;
  }
}



GLevel::GLevel(GObject &parent) : GObject(&parent) {}
GLevel::GLevel(GWidget* parent) : GObject(parent)  {}
GLevel::~GLevel() { this->del(); }
void GLevel::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_obj_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      GObject::size(40,200);
      GObject::padding(5);
      GObject::color(TFT_DARKGRAY);
      GObject::radius(RADIUS_CIRCLE);
      
      glevel_ext_t* ext = (glevel_ext_t*) this->alloc_ext_attr(sizeof(glevel_ext_t));

      /* alloc ext's GWidgets on PSRAM */
      // ext->bar_bg     = (GObject*) esp32_malloc(sizeof(GObject));
      // ext->bar_indic  = (GObject*) esp32_malloc(sizeof(GObject));
      // ext->btn_up     = (GButton*) esp32_malloc(sizeof(GButton));
      // ext->btn_down   = (GButton*) esp32_malloc(sizeof(GButton));
      ext->bar_bg     = new_GObject(this);
      ext->bar_indic  = new_GObject(ext->bar_bg);
      ext->btn_up     = new_GButton(this);
      ext->btn_down   = new_GButton(this);
      ext->val_min    = 0;
      ext->val_max    = 100;
      ext->val        = 0;
      
        /* new instance from PSRAM */
        // new (ext->bar_bg)   GObject(this);
        // new (ext->bar_indic)GObject(ext->bar_bg);
        // new (ext->btn_up)   GButton(this); //GButton(SYMBOL_UP, *this);
        // new (ext->btn_down) GButton(this); //GButton(SYMBOL_DOWN, *this);

      
      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      this->_vertical_design();
      ext->bar_indic->color(TFT_BLUE_LVGL);

      this->value(50);
      this->align(ALIGN_CENTER,0,0);
    }
  }
}

void GLevel::_vertical_design(){
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();

  uint16_t w_fit = this->width_fit();
  uint16_t h_fit = this->height_fit();
  int16_t  padding_inner = this->padding_inner();

  ext->bar_bg->size(w_fit, h_fit - 2*padding_inner -2 * w_fit);
  ext->bar_bg->padding(5);
  ext->bar_bg->radius(5);
  ext->bar_bg->align(ALIGN_CENTER);
  ext->bar_bg->border(0);
//      ext->bar_bg->border_color(TFT_GRAY);
  ext->bar_bg->color(TFT_BLACK);
  
  ext->bar_indic->size_fit(ext->bar_bg);
  ext->bar_indic->copyStyle(STYLE_PRETTY);
  ext->bar_indic->radius(5);
  ext->bar_indic->align(ALIGN_BOTTOM, 0, -ext->bar_bg->padding());
  // ext->bar_indic->color(TFT_BLUE_LVGL);

  ext->btn_up->text(SYMBOL_UP);
  ext->btn_up->size(w_fit, w_fit);
  ext->btn_up->radius(RADIUS_CIRCLE);
  ext->btn_up->align(ext->bar_bg, ALIGN_TOP, 0, -padding_inner);
  ext->btn_up->auto_realign(true);
  ext->btn_up->color(TFT_SILVER);
  ext->btn_up->font(prasanmit_20);
  ext->btn_up->text_color(TFT_BLACK);
  ext->btn_up->user_data(this);
  ext->btn_up->event_cb(GButton_level_up_event_cb);

  ext->btn_down->text(SYMBOL_DOWN);
  ext->btn_down->size(w_fit, w_fit);
  ext->btn_down->radius(RADIUS_CIRCLE);
  ext->btn_down->align( ext->bar_bg, ALIGN_BOTTOM, 0, padding_inner);
  ext->btn_down->auto_realign(true);
  ext->btn_down->color(TFT_SILVER);
  ext->btn_down->font(prasanmit_20);
  ext->btn_down->text_color(TFT_BLACK);
  ext->btn_down->user_data(this);
  ext->btn_down->event_cb(GButton_level_down_event_cb);

  this->value(this->value());

}

void GLevel::_horizontal_design(){
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();

  uint16_t w_fit = this->width_fit();
  uint16_t h_fit = this->height_fit();
  int16_t  padding_inner = this->padding_inner();

  ext->bar_bg->size(w_fit - 2*padding_inner -2 * h_fit, h_fit); //(w_fit, h_fit - 2*padding_inner -2 * w_fit);
  ext->bar_bg->padding(5);
  ext->bar_bg->radius(5);
  ext->bar_bg->align(ALIGN_CENTER);
  ext->bar_bg->border(0);
//      ext->bar_bg->border_color(TFT_GRAY);
  ext->bar_bg->color(TFT_BLACK);
  
  ext->bar_indic->size_fit(ext->bar_bg);
  ext->bar_indic->copyStyle(STYLE_PRETTY);
  ext->bar_indic->radius(5);
  // ext->bar_indic->align(ALIGN_BOTTOM, 0, -ext->bar_bg->padding());
  ext->bar_indic->align(ALIGN_LEFT, ext->bar_bg->padding());


  ext->btn_up->text(SYMBOL_RIGHT);
  ext->btn_up->size(h_fit, h_fit);
  ext->btn_up->radius(RADIUS_CIRCLE);
  // ext->btn_up->align(ext->bar_bg, ALIGN_TOP, 0, -padding_inner);
  ext->btn_up->align(ext->bar_bg, ALIGN_RIGHT, padding_inner);
  ext->btn_up->auto_realign(true);
  ext->btn_up->color(TFT_SILVER);
  ext->btn_up->font(prasanmit_20);
  ext->btn_up->text_color(TFT_BLACK);
  ext->btn_up->user_data(this);
  ext->btn_up->event_cb(GButton_level_up_event_cb);

  ext->btn_down->text(SYMBOL_LEFT);
  ext->btn_down->size(h_fit, h_fit);
  ext->btn_down->radius(RADIUS_CIRCLE);
  // ext->btn_down->align( ext->bar_bg, ALIGN_BOTTOM, 0, padding_inner);
  ext->btn_down->align( ext->bar_bg, ALIGN_LEFT, -padding_inner);
  ext->btn_down->auto_realign(true);
  ext->btn_down->color(TFT_SILVER);
  ext->btn_down->font(prasanmit_20);
  ext->btn_down->text_color(TFT_BLACK);
  ext->btn_down->user_data(this);
  ext->btn_down->event_cb(GButton_level_down_event_cb);  

  this->value(this->value());

}


void GLevel::create(GWidget& parent){
  this->_par = &parent;
  create();
}

void GLevel::create(GWidget* parent) {
  this->_par = parent;
  create();
}

GLevel& GLevel::operator =(int16_t val){
  this->value(val);
  return *this;
}

void GLevel::range(int16_t val_min, int16_t val_max){
  if(!this->isCreated()) create();
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();
  ext->val_min = LV_MATH_MIN(val_min, val_max);
  ext->val_max = LV_MATH_MAX(val_min, val_max);
  ext->val     = constrain(ext->val, ext->val_min, ext->val_max);
  
  int16_t bar_indic_h = map_f( ext->val, ext->val_min, ext->val_max, 0, ext->bar_bg->height_fit());
  ext->bar_indic->height(bar_indic_h);
}

void GLevel::value(int16_t val) {
  if(!this->isCreated()) create();
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();
  
  ext->val = constrain(val, ext->val_min, ext->val_max);

  if( this->height() > this->width() ) {
    int16_t bar_indic_h = map_f( ext->val, ext->val_min, ext->val_max, 0, ext->bar_bg->height_fit());
    ext->bar_indic->height(bar_indic_h);
  }else{
    int16_t bar_indic_w = map_f( ext->val, ext->val_min, ext->val_max, 0, ext->bar_bg->width_fit());
    ext->bar_indic->width(bar_indic_w);
  }
  this->event_send(EVENT_VALUE_CHANGED);
}

int16_t GLevel::value(){
  if(!this->isCreated()) create();
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();
  return ext->val;
}

int16_t GLevel::value_min(){
  if(!this->isCreated()) create();
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();
  return ext->val_min;
}

int16_t GLevel::value_max(){
  if(!this->isCreated()) create();
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();
  return ext->val_max;
}

void GLevel::color(color_t c){
  this->color(c,c);
}

void GLevel::color(color_t c, color_t gradian_color){
  if(!this->isCreated()) create();
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();
  
  ext->bar_indic->color(c, gradian_color);
}

color_t GLevel::color(){
  if(!this->isCreated()) create();
  glevel_ext_t* ext = (glevel_ext_t*) this->ext_attr();
  return ext->bar_indic->color();
}

void GLevel::bg_color(color_t color){
  if(!this->isCreated()) create();
  GObject::color(color);
}

color_t GLevel::bg_color(){
  if(!this->isCreated()) create();
  return GObject::color();
}
