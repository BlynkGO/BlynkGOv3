/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GButtonMatrix.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 *  [History]
 *    [Version 3.0]   @13/08/22
 *       - เพิ่ม API GButtonMatrix(const char* buttons_map[], GWidget& parent=GScreen);
 *       - เพิ่ม API GButtonMatrix(const char* buttons_map[], GWidget* parent);
 * 
 *********************************************************************
 */

#include "GButtonMatrix.h"

#define LV_OBJX_NAME  "GButtonMatrix" 

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
#if BLYNGO_DEBUG
    Serial.println("[GButtonMatrix] signal cleanup");
#endif
    GButtonMatrix* pBtnMX = (GButtonMatrix*) lv_obj_get_user_data(obj);
    if(pBtnMX->hasLocalStyle()) { pBtnMX->freeBTNMStyle(); } //pBtnMX->_has_localstyle = false; }
    pBtnMX->_created = false;
  }
  return LV_RES_OK;
}

GButtonMatrix::GButtonMatrix(GWidget& parent) : GObject(&parent) {}
GButtonMatrix::GButtonMatrix(GWidget* parent) : GObject(parent)  {}
GButtonMatrix::GButtonMatrix(const char* buttons_map[], GWidget& parent) : _map(buttons_map), GObject(&parent) 
{}

GButtonMatrix::GButtonMatrix(const char* buttons_map[], GWidget* parent) : _map(buttons_map), GObject(parent) 
{}

GButtonMatrix::~GButtonMatrix() { this->del(); }

void GButtonMatrix::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_btnm_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createBTNMStyle();
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      this->align(ALIGN_IN_BOTTOM_MID);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      if(this->_map!=NULL) {
        this->map(this->_map);
      }
    }
  }
}

void GButtonMatrix::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GButtonMatrix::create(GWidget* parent) {
  this->_par = parent;
  create();
}


void GButtonMatrix::createBTNMStyle(){
  if(!this->isCreated()) create();

  if(!this->hasLocalStyle()){
    // this->createLocalStyle( (style_t*)lv_btnm_get_style(this->obj, LV_BTNM_STYLE_BG) );

    // alloc & copy
    this->style[RELEASED].copy       ( (style_t*)lv_btnm_get_style(this->obj, LV_BTNM_STYLE_BTN_REL) );
    this->style[PRESSED].copy        ( (style_t*)lv_btnm_get_style(this->obj, LV_BTNM_STYLE_BTN_PR) );
    this->style[TOGGLE_RELEASED].copy( (style_t*)lv_btnm_get_style(this->obj, LV_BTNM_STYLE_BTN_TGL_REL) );
    this->style[TOGGLE_PRESSED].copy ( (style_t*)lv_btnm_get_style(this->obj, LV_BTNM_STYLE_BTN_TGL_PR) );
    this->style[INACTIVE].copy       ( (style_t*)lv_btnm_get_style(this->obj, LV_BTNM_STYLE_BTN_INA) );
    this->style[BTNM_BG].copy        ( (style_t*)lv_btnm_get_style(this->obj, LV_BTNM_STYLE_BG) );

    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_REL,     this->style[RELEASED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_PR,      this->style[PRESSED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_TGL_REL, this->style[TOGGLE_RELEASED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_TGL_PR,  this->style[TOGGLE_PRESSED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_INA,     this->style[INACTIVE].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BG,          this->style[BTNM_BG].raw()); //this->_style->raw());

    this->_style = (GStyle*) &this->style[BTNM_BG];
    this->_has_localstyle = true;
  }
}

void GButtonMatrix::freeBTNMStyle(){
  if(this->hasLocalStyle()){
    for(int i =0; i < 6; i++)
      this->style[i].release();
    this->_has_localstyle = false;
  }
}

void GButtonMatrix::map(const char * buttons_map[]){
  if(!this->isCreated()) create();
  lv_btnm_set_map(this->obj, buttons_map);
}

const char ** GButtonMatrix::map(){
  if(!this->isCreated()) create();
  return lv_btnm_get_map_array(this->obj);
}

void GButtonMatrix::control_map(const button_ctrl_t ctrl_map[]){
  if(!this->isCreated()) create();
  lv_btnm_set_ctrl_map(this->obj, ctrl_map);
}

void GButtonMatrix::control_all(const button_ctrl_t control){
  if(!this->isCreated()) create();
  lv_btnm_set_btn_ctrl_all(this->obj, control);
}


void GButtonMatrix::button_width(uint16_t button_id, uint8_t button_w){
  if(!this->isCreated()) create();
  lv_btnm_set_btn_width(this->obj, button_id, button_w);
}

void GButtonMatrix::button_control(uint16_t button_id, button_ctrl_t control){
  if(!this->isCreated()) create();
  lv_btnm_set_btn_ctrl(this->obj, button_id, control);
}

String GButtonMatrix::active_button(){
  if(!this->isCreated()) create();
  const char* txt = lv_btnm_get_active_btn_text(this->obj);
  return (txt != NULL)? String(txt) : "";
}

uint16_t GButtonMatrix::pressed_button_id(){
  if(!this->isCreated()) create();
  return lv_btnm_get_pressed_btn(this->obj);
}

void GButtonMatrix::bg_radius(uint8_t r) {
  if(!this->isCreated()) create();
  this->_style->radius(r);
}


void GButtonMatrix::bg_color(uint16_t bg_color){
  if(!this->isCreated()) create();
  this->_style->color(bg_color, bg_color);
}

void  GButtonMatrix::bg_opa(uint8_t opa){
  if(!this->isCreated()) create();
  this->_style->opa(opa);
}

void GButtonMatrix::bg_border(uint8_t width){
  if(!this->isCreated()) create();
  this->_style->border(width);
}

void GButtonMatrix::bg_border(uint8_t width, color_t color){      // ความหน้าขอบพื้นหลัง
  if(!this->isCreated()) create();
  this->_style->border(width, color);
}

void GButtonMatrix::bg_border_color(color_t color){      // ความหน้าขอบพื้นหลัง
  if(!this->isCreated()) create();
  this->_style->border_color(color);
}

void GButtonMatrix::bg_border_opa(uint8_t opa){      // ความหน้าขอบพื้นหลัง
  if(!this->isCreated()) create();
  this->_style->border_opa(opa);
}


void GButtonMatrix::color(uint16_t color16) {
  if(!this->isCreated()) create();

  lv_color_t color; color.full = color16;
  lv_color_t color_dark = lv_color_mix(color, LV_COLOR_BLACK, 128);
  lv_color_t color_dark2 = lv_color_mix(color, LV_COLOR_BLACK, 60);

  this->style[RELEASED].color(color16, color_dark.full); 
  this->style[PRESSED].color(color_dark.full, color_dark.full);  
  this->style[TOGGLE_RELEASED].color(color_dark2.full, color16); 
  this->style[TOGGLE_PRESSED].color(color_dark.full, color_dark.full); 
}

void GButtonMatrix::font(font_t &font) {
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].font(font);
  }

  // this->obj->signal_cb(this->obj, LV_SIGNAL_CHILD_CHG, NULL);
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_REL,     this->style[RELEASED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_PR,      this->style[PRESSED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_TGL_REL, this->style[TOGGLE_RELEASED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_TGL_PR,  this->style[TOGGLE_PRESSED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_INA,     this->style[INACTIVE].raw());
    // lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BG,          this->_style->raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BG,          this->style[BTNM_BG].raw()); 
}

void GButtonMatrix::font(font_t &font, color_t color){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].font(font);
    this->style[i].text_color(color);
  }

  // this->obj->signal_cb(this->obj, LV_SIGNAL_CHILD_CHG, NULL);
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_REL,     this->style[RELEASED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_PR,      this->style[PRESSED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_TGL_REL, this->style[TOGGLE_RELEASED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_TGL_PR,  this->style[TOGGLE_PRESSED].raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BTN_INA,     this->style[INACTIVE].raw());
    // lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BG,          this->_style->raw());
    lv_btnm_set_style(this->obj, LV_BTNM_STYLE_BG,          this->style[BTNM_BG].raw()); 
}

const font_t* GButtonMatrix::font(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].font();
}



void GButtonMatrix::text_color(uint16_t color){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].text_color(color);
  }
  this->obj->signal_cb(this->obj, LV_SIGNAL_CHILD_CHG, NULL);
}

color_t GButtonMatrix::text_color(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].text_color();
}

void GButtonMatrix::border(uint8_t width) {
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].border(width); 
  }
}

uint8_t GButtonMatrix::border(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].border();
}

void GButtonMatrix::border_color(uint16_t color){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].border_color(color); 
  }
}

color_t GButtonMatrix::border_color(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].border_color();
}

void GButtonMatrix::radius(uint8_t r){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].radius(r);
  }
}

uint8_t GButtonMatrix::radius(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].radius();
}
