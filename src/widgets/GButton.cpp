/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GButton.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GButton.h"


#define LV_OBJX_NAME  "GButton" 

static lv_signal_cb_t ancestor_signal_cb;
static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  if(sign != LV_SIGNAL_CORD_CHG) {
    res = ancestor_signal_cb( obj, sign, param);
    if(res != LV_RES_OK ) return res;
  }

  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  if(sign == LV_SIGNAL_CORD_CHG) {
    GButton* pBTN = (GButton*) lv_obj_get_user_data(obj);
    if( pBTN->rounded()) {
      uint16_t side = LV_MATH_MIN(pBTN->width(), pBTN->height());
      pBTN->radius(side/2.0);
    }
    res = ancestor_signal_cb( obj, sign, param);
  }
  /* หลัง del() หรือ deconstrutor ถูกเรียก และภายในได้ลบ child ไปแล้ว */
  if(sign == LV_SIGNAL_CLEANUP) {
#if BLYNGO_DEBUG
    Serial.println("[GButton] signal cleanup");
#endif
    GButton* pBTN = (GButton*) lv_obj_get_user_data(obj);
    gbutton_ext_t* ext = (gbutton_ext_t*) pBTN->ext_attr();
    free_widget(ext->label);

    if(pBTN->hasLocalStyle()) { pBTN->freeButtonStyle(); }  //pBTN->_has_localstyle = false; }
    pBTN->_created = false;
  }
  return LV_RES_OK;
}

static void gbutton_event_cb(lv_obj_t *obj, lv_event_t event);

GButton::GButton(GWidget& parent) : GContainer(&parent) {}
GButton::GButton(GWidget* parent) : GContainer(parent) {}
GButton::GButton(String text, GWidget& parent)      : _btn_text(text.c_str()), GContainer(&parent) {}
GButton::GButton(const char* text, GWidget& parent) : _btn_text(text), GContainer(&parent) {}
GButton::GButton(const char* text, GWidget* parent) : _btn_text(text), GContainer(parent) {}
GButton::GButton(button_type_t type, const char* text, GWidget& parent) : _type(type), _btn_text(text), GContainer(&parent) {}
GButton::GButton(button_type_t type, const char* text, GWidget* parent) : _type(type), _btn_text(text), GContainer(parent)  {}

#if BLYNKGO_USE_BLYNK
GButton::GButton(button_type_t type, String text, uint8_t virtual_pin, GWidget& parent)
: _type(type), _btn_text(text), vpin(virtual_pin), GContainer(&parent) {}

GButton::GButton(button_type_t type, String text, uint8_t virtual_pin, GWidget* parent)
: _type(type), _btn_text(text), vpin(virtual_pin), GContainer(parent) {}

GButton::GButton(button_type_t type, const char* text, uint8_t virtual_pin, GWidget& parent)
: _type(type), _btn_text(text), vpin(virtual_pin), GContainer(&parent) {}

GButton::GButton(button_type_t type, const char* text, uint8_t virtual_pin, GWidget* parent)
: _type(type), _btn_text(text), vpin(virtual_pin), GContainer(parent) {}
#endif

GButton::~GButton() { this->del(); }

void GButton::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_btn_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created  = true;
      this->createButtonStyle();
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gbutton_event_cb);//main_event_cb);

      gbutton_ext_t* ext = (gbutton_ext_t*) this->alloc_ext_attr(sizeof(gbutton_ext_t));
      ext->label = new_GLabel(this);

      ext->label->create();
      ext->label->text_align(LABEL_ALIGN_CENTER);
      ext->label->clickable(false);
      
      this->text(_btn_text);    

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      this->fit(FIT_TIGHT);
      this->align(ALIGN_CENTER,0,0);
      this->auto_realign(true);

      // this->mode(BUTTON_MODE_NORMAL);
      this->mode((this->_type == BUTTON_SWITCH)? BUTTON_MODE_TOGGLE : BUTTON_MODE_NORMAL);
    }
  }
}

void GButton::create(GObject *parent){
  this->_par = parent;
  create();
}

void GButton::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GButton::createButtonStyle(){
  if(!this->isCreated()) create();

  if(!this->hasLocalStyle()){
    // alloc & copy
    this->style[RELEASED].copy       ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_REL) );
    this->style[PRESSED].copy        ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_PR) );
    this->style[TOGGLE_RELEASED].copy( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_TGL_REL) );
    this->style[TOGGLE_PRESSED].copy ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_TGL_PR) );
    this->style[INACTIVE].copy       ( (style_t*)lv_btn_get_style(this->obj, LV_BTN_STYLE_INA) );

    lv_btn_set_style(this->obj, LV_BTN_STYLE_REL,     this->style[RELEASED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_PR,      this->style[PRESSED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_TGL_REL, this->style[TOGGLE_RELEASED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_TGL_PR,  this->style[TOGGLE_PRESSED].raw());
    lv_btn_set_style(this->obj, LV_BTN_STYLE_INA,     this->style[INACTIVE].raw());

    this->_style = (GStyle*) &this->style[RELEASED];
    this->_has_localstyle = true;
  }
}

void GButton::freeButtonStyle(){
  if(this->hasLocalStyle()){
    for(int i =0; i < 5; i++)
      this->style[i].release();
    this->_has_localstyle = false;
  }
}

void GButton::mode(button_mode_t _mode) {
  if(!this->isCreated()) create();

  switch(_mode) {
    case BUTTON_MODE_NORMAL:
      lv_btn_set_toggle(this->obj, false);
      this->state(BUTTON_RELEASED);
      this->_type = BUTTON_PUSH;
      break;
    case BUTTON_MODE_TOGGLE:
      lv_btn_set_toggle(this->obj, true);
      this->_type = BUTTON_SWITCH;
      break;
    case BUTTON_MODE_INACTIVE:
      this->state(BUTTON_INACTIVE);
      break;
  }
}

button_mode_t GButton::mode(){
  if(!this->isCreated()) create();
  if(this->state() == BUTTON_INACTIVE) return BUTTON_MODE_INACTIVE;
  return (button_mode_t) lv_btn_get_toggle(this->obj);
}

void GButton::state(button_state_t button_state){
  if(!this->isCreated()) create();

  if( this->mode()==BUTTON_MODE_TOGGLE ) {
    button_state = (button_state==1)? 2 : button_state;
  }

  lv_btn_set_state(this->obj, (lv_btn_state_t) button_state);

  this->event_send(EVENT_VALUE_CHANGED);
}

button_state_t GButton::state(){
  if(!this->isCreated()) create();
  return lv_btn_get_state(this->obj);
}


void GButton::toggle(){
  if(!this->isCreated()) create();
  if(this->_type == BUTTON_SWITCH ) {
    this->mode( BUTTON_MODE_TOGGLE);
    lv_btn_toggle(this->obj);
  }
}

void GButton::ON() { 
  if(this->_type == BUTTON_SWITCH ) {
    this->mode( BUTTON_MODE_TOGGLE);
    this->state(TOGGLE_RELEASED); 
  }
}

void GButton::OFF() { 
  if(this->_type == BUTTON_SWITCH ) {
    this->mode( BUTTON_MODE_TOGGLE);
    this->state(RELEASED);
  }
}

void GButton::inactive(bool enable){
  static bool isON = false;

  if(enable){
    if(this->_type == BUTTON_SWITCH) {
      isON = this->isON();    // จำสถานะ เปิด หรือ ปิด หากปุ่มเป็นแบบ BUTTON_SWITCH
    }
    this->mode(BUTTON_MODE_INACTIVE);
    this->clickable(false);
  }else{
    this->clickable(true);
    if(this->_type == BUTTON_PUSH ) {
      this->mode(BUTTON_MODE_NORMAL);
    }else
    if(this->_type == BUTTON_SWITCH) {
      this->mode(BUTTON_MODE_TOGGLE);
      (isON)? this->ON() : this->OFF();
    }
  }
}

GButton& GButton::operator=(String label) {
  if(!this->isCreated()) create();

  this->text(label);
  this->btn_reflesh();

  return *this;
}

GButton& GButton::operator=(const char* cstr) {
  if(!this->isCreated()) create();

  this->text(cstr);
  this->btn_reflesh();

  return *this;
}


GButton&  GButton::operator+=(const String label) {
  if(!this->isCreated()) create();

  this->text(this->text() + label);
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(const char *cstr)
{
  if(!this->isCreated()) create();

  this->text(this->text() + cstr);
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(char c)
{
  if(!this->isCreated()) create();

  this->text(this->text() + c);
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(unsigned char num)
{
  if(!this->isCreated()) create();

  this->text(this->text() + String(num));
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(int num)
{
  if(!this->isCreated()) create();

  this->text(this->text() + String(num));
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(unsigned int num)
{
  if(!this->isCreated()) create();

  this->text(this->text() + String(num));
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(long num)
{
  if(!this->isCreated()) create();

  this->text(this->text() + String(num));
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(unsigned long num)
{
  if(!this->isCreated()) create();

  this->text(this->text() + String(num));
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(float num)
{
  if(!this->isCreated()) create();

  this->text(this->text() + String(num));
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator +=(double num)
{
  if(!this->isCreated()) create();

  this->text(this->text() + String(num));
  this->btn_reflesh();

  return (*this);
}

GButton& GButton::operator += (const __FlashStringHelper *str)
{
  if(!this->isCreated()) create();

  this->text(this->text() + str);
  this->btn_reflesh();

  return (*this);
}



void GButton::text(String text){
  if(!this->isCreated()) create();
  gbutton_ext_t* ext = (gbutton_ext_t*) this->ext_attr();
  lv_label_set_text(ext->label->obj, text.c_str());
}

String  GButton::text(){
  if(!this->isCreated()) create();
  gbutton_ext_t* ext = (gbutton_ext_t*) this->ext_attr();
  return lv_label_get_text(ext->label->obj);
}

void GButton::text_align(label_align_t text_align){
  if(!this->isCreated()) create();
  gbutton_ext_t* ext = (gbutton_ext_t*) this->ext_attr();
  ext->label->text_align(text_align); 
}


void GButton::font(font_t &font){
  if(!this->isCreated()) create();
  gbutton_ext_t* ext = (gbutton_ext_t*) this->ext_attr();

  for(int i= 0; i < 5; i++) {
    this->style[i].font(font);
  }

  ext->label->refreshStyle();
  this->btn_reflesh();
}

void GButton::font(font_t &font, color_t color){
  if(!this->isCreated()) create();
  gbutton_ext_t* ext = (gbutton_ext_t*) this->ext_attr();

  for(int i= 0; i < 5; i++) {
    this->style[i].font(font);
    this->style[i].text_color(color);
  }

  ext->label->refreshStyle();
  this->btn_reflesh();
}

const font_t* GButton::font(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].font();
}

void GButton::text_color(uint16_t color){
  if(!this->isCreated()) create();
  gbutton_ext_t* ext = (gbutton_ext_t*) this->ext_attr();
  
  for(int i= 0; i < 5; i++) {
    this->style[i].text_color(color);
  }
  ext->label->refreshStyle();
  this->btn_reflesh();
}

color_t GButton::text_color(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].text_color();
}

void GButton::rounded(bool enable){
  if(!this->isCreated()) create();
  this->_round = enable;

  lv_area_t ori; lv_obj_get_coords(this->obj, &ori);
  this->obj->signal_cb(this->obj, LV_SIGNAL_CORD_CHG, &ori);
}

bool GButton::rounded(){
  if(!this->isCreated()) create();
  return this->_round;
}


void GButton::radius(uint8_t r){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].radius(r);
  }
  this->refreshStyle();
}

uint8_t GButton::radius(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].radius();
}

void GButton::color(uint16_t color16){
  if(!this->isCreated()) create();
  
  color_t color_dark  = TFT_COLOR_MIX(color16, TFT_BLACK, 128);
  color_t color_dark2 = TFT_COLOR_MIX(color16, TFT_BLACK, 60);

  this->style[RELEASED]       .color(color16     , color_dark);
  this->style[PRESSED]        .color(color_dark  , color_dark);  
  this->style[TOGGLE_RELEASED].color(color_dark2 , color16); 
  this->style[TOGGLE_PRESSED] .color(color_dark  , color_dark); 

  this->refreshStyle();
}

uint16_t GButton::color(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].color();
}

void GButton::border_color(uint16_t color){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].border_color(color); 
  }
  this->refreshStyle();
}

color_t  GButton::border_color(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].border_color();
}

void GButton::border(uint8_t width){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].border(width); 
  }
  this->refreshStyle();
}

void GButton::border(uint8_t width, color_t color){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].border(width); 
    this->style[i].border_color(color); 
  }
  this->refreshStyle();
}

uint8_t GButton::border(){
  if(!this->isCreated()) create();
  return this->style[RELEASED].border();
}

void GButton::shadow(uint16_t width){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].shadow(width); 
  }
  this->refreshStyle();
}

void GButton::shadow(uint16_t width, color_t color){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].shadow(width, color); 
  }
  this->refreshStyle();
}

uint16_t GButton::shadow(){
  if(!this->isCreated()) create();
  return this->style[0].shadow(); 
}

void GButton::shadow_color(color_t color){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].shadow_color(color); 
  }
  this->refreshStyle();
}

color_t GButton::shadow_color(){
  if(!this->isCreated()) create();
  return this->style[0].shadow_color(); 
}

void GButton::shadow_opa(uint8_t opa){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].shadow_opa(opa); 
  }
  this->refreshStyle();
}


uint8_t GButton::shadow_opa(){
  if(!this->isCreated()) create();
  return this->style[0].shadow_opa(); 
}

void GButton::shadow_offset(point_t point){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].shadow_offset(point); 
  }
  this->refreshStyle();
}


void GButton::shadow_offset(int16_t offset_x, int16_t offset_y){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].shadow_offset(offset_x, offset_y); 
  }
  this->refreshStyle();
}

point_t GButton::shadow_offset(){
  if(!this->isCreated()) create();
  return this->style[0].shadow_offset(); 
}

void GButton::shadow_spread(uint16_t spread){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].shadow_spread(spread); 
  }
  this->refreshStyle();
}

uint16_t GButton::shadow_spread(){
  if(!this->isCreated()) create();
  return this->style[0].shadow_spread(); 
}

void GButton::padding(uint8_t padding){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].padding(padding);
  }
  this->refreshStyle();
}

void GButton::padding_top(uint8_t padding){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].padding_top(padding);
  }
  this->refreshStyle();
}

void GButton::padding_bottom(uint8_t padding){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].padding_bottom(padding);
  }
  this->refreshStyle();
}

void GButton::padding_left(uint8_t padding){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].padding_left(padding);
  }
  this->refreshStyle();
}

void GButton::padding_right(uint8_t padding){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].padding_right(padding);
  }
  this->refreshStyle();
}

void GButton::padding_inner(uint8_t padding){
  if(!this->isCreated()) create();

  for(int i= 0; i < 5; i++) {
    this->style[i].padding_inner(padding);
  }
  this->refreshStyle();
}

void GButton::round_design(){
  if(!this->isCreated()) create();
  this->size(50,50);
  this->opa(0);                            // ปรับค่าโปร่งใส 0 เวลาปกติ    
  this->rounded(true); 
  this->padding(0); 
  this->border(0);
  this->style[PRESSED].color( TFT_WHITE);
  this->style[PRESSED].opa(96);            // ปรับค่าให้โปร่งใส 32 เวลากด
  this->refreshStyle();
}

void GButton::btn_reflesh(){
  this->obj->signal_cb(this->obj, LV_SIGNAL_CHILD_CHG, NULL);
}

void GButton::gpio(uint8_t pin, bool active_high){
  if(!this->isCreated()) this->create();
  this->gpin = pin;
  this->gpin_active = active_high;
  pinMode(this->gpin, OUTPUT);
  digitalWrite(this->gpin, !this->gpin_active);  // ให้ไม่ active ไปก่อน (ปิดไปก่อน)
}

uint8_t GButton::gpio() { 
  if(!this->isCreated()) create();
  return this->gpin;
}

void GButton::active_high(bool active_high){
  if(!this->isCreated()) create();
  this->gpin_active = active_high;
}

bool  GButton::active_high(){
  if(!this->isCreated()) create();
  return this->gpin_active;
}

GLabel& GButton::label(){
  if(!this->isCreated()) create();
  gbutton_ext_t* ext = (gbutton_ext_t*) this->ext_attr();
  return *ext->label;
}

#if BLYNKGO_USE_BLYNK
void GButton::virtualPin(uint8_t vpin){
  if(!this->isCreated()) this->create();
  this->vpin = vpin;
}

uint8_t GButton::virtualPin() {
  return this->vpin;
}
#endif

void gbutton_event_cb(lv_obj_t *obj, lv_event_t event) {
  GButton* pGBtn = (GButton*) lv_obj_get_user_data(obj);
  bool pressed = false;

  if(event == EVENT_PRESSED || event == EVENT_RELEASED || event == EVENT_PRESS_LOST) {
    if(pGBtn->mode() == BUTTON_MODE_NORMAL){
      if(event == EVENT_PRESSED) pressed = true;
      if(event == EVENT_RELEASED || event == EVENT_PRESS_LOST) pressed = false;

      // Serial.printf("Push Set : %d : %d\n", pGBtn->state(), pressed );
      if(pGBtn->gpio()) digitalWrite(pGBtn->gpio(), pressed? pGBtn->active_high() : !pGBtn->active_high());
#if BLYNKGO_USE_BLYNK
      if(pGBtn->virtualPin()) BlynkGO.virtualWrite(pGBtn->virtualPin(), pressed );
#endif
    }
  } else
  if(event == EVENT_VALUE_CHANGED) {
    switch(pGBtn->mode()){
      case BUTTON_MODE_NORMAL: pressed = (pGBtn->state()==1)? true : false; break;
      case BUTTON_MODE_TOGGLE: pressed = (pGBtn->state()==2)? true : false; break;
    }
    // Serial.printf("Set : %d : %d\n", pGBtn->state(), pressed );

    if(pGBtn->gpio()) digitalWrite(pGBtn->gpio(), pressed? pGBtn->active_high() : !pGBtn->active_high());
#if BLYNKGO_USE_BLYNK
    if(pGBtn->virtualPin()) BlynkGO.virtualWrite(pGBtn->virtualPin(), pressed );
#endif
  }
  main_event_cb(obj, event);
}



