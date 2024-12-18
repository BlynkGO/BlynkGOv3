/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GGaugeLine.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
#include "GGaugeLine.h"

#define LV_OBJX_NAME  "GGaugeLine" 

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
    Serial.println("[GGaugeLine] signal cleanup");
#endif
    GGaugeLine* pGaugeLine = (GGaugeLine*) lv_obj_get_user_data(obj);

    if(pGaugeLine->_color !=NULL) {
      free(pGaugeLine->_color); pGaugeLine->_color=NULL;
    }

    if(pGaugeLine->hasLocalStyle()) {pGaugeLine->freeLocalStyle(); } //pLed->_has_localstyle = false; } // 
    pGaugeLine->_created = false;
  }
  return LV_RES_OK;
}


GGaugeLine::GGaugeLine(GWidget& parent) : GObject(&parent) {}
GGaugeLine::GGaugeLine(GWidget* parent) : GObject(parent) {}
GGaugeLine::~GGaugeLine() { this->del(); }

void GGaugeLine::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_gauge_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      lv_obj_set_size(this->obj, this->_w, this->_h);   // กำหนดก่อน _created = true;
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      this->scale_length(10);
      this->scale_padding(8);
      this->scale_width(3);
      this->text_color( (this->_par->color() == TFT_BLACK)? TFT_WHITE : (this->_par->color() == TFT_WHITE)? TFT_BLACK : TFT_SILVER );
      this->critical_color( TFT_RED);

      this->needle.init(this);
      this->align(ALIGN_CENTER,0,0);


      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GGaugeLine::create(GWidget& parent){
  this->_par = &parent;
  create();
}

void GGaugeLine::create(GWidget* parent) {
  this->_par = parent;
  create();
}

void GGaugeLine::size(uint16_t w, uint16_t h){
  this->_w = w; this->_h = h;
  if(this->isCreated()) this->del();
  create();
}

void GGaugeLine::needle_color(lv_color_t color[], uint8_t num){
  if(!this->isCreated()) this->create();
  lv_gauge_set_needle_count(this->obj, num, color);
}

void GGaugeLine::needle_color(color_t color[], uint8_t num) {
  if(!this->isCreated()) this->create();
  if(this->_color != NULL) { free(this->_color); }
  this->_color = (lv_color_t*) esp32_malloc( num * sizeof(lv_color_t) );

  for(int i=0;i < num; i++)
    this->_color[i].full = color[i];

  lv_gauge_set_needle_count(this->obj, num, this->_color);
}

void GGaugeLine::needle_color(color_t color){
  this->needle_color(&color, 1);
}

uint8_t GGaugeLine::needle_count(){
  if(!this->isCreated()) this->create();
  return lv_gauge_get_needle_count(this->obj);
}

void GGaugeLine::value(uint8_t needle_id, int16_t value){
  if(!this->isCreated()) this->create();
  lv_gauge_set_value(this->obj, needle_id, value);
}

// void  GGaugeLine::value(int16_t value){
//   this->value(0, value);
// }

int16_t GGaugeLine::value(uint8_t needle_id){
  if(!this->isCreated()) this->create();
  return lv_gauge_get_value(this->obj, needle_id);
}

int16_t GGaugeLine::value_min(){
  if(!this->isCreated()) this->create();
  return lv_gauge_get_min_value(this->obj);
}

int16_t GGaugeLine::value_max(){
  if(!this->isCreated()) this->create();
  return lv_gauge_get_max_value(this->obj);
}



void GGaugeLine::range(int16_t value_min, int16_t value_max){
  if(!this->isCreated()) this->create();
  lv_gauge_set_range(this->obj, value_min, value_max);
}

void GGaugeLine::critical_value(int16_t value){
  if(!this->isCreated()) this->create();
  lv_gauge_set_critical_value(this->obj, value);
}

int16_t GGaugeLine::critical_value(){
  if(!this->isCreated()) this->create();
  return lv_gauge_get_critical_value(this->obj);
}

void GGaugeLine::scale_length(uint8_t len){
  if(!this->isCreated()) this->create();
  this->padding_left(len);
}

uint8_t GGaugeLine::scale_length(){
  if(!this->isCreated()) this->create();
  return this->padding_left();
}

void  GGaugeLine::scale_padding(uint8_t label_padding){
  if(!this->isCreated()) this->create();
  this->padding_inner(label_padding);
}

uint8_t  GGaugeLine::scale_padding(){
  if(!this->isCreated()) this->create();
  return this->padding_inner();
}

void  GGaugeLine::scale_width(uint8_t width){
  if(!this->isCreated()) this->create();
  this->line_width(width);
}

uint8_t  GGaugeLine::scale_width(){
  if(!this->isCreated()) this->create();
  return this->line_width();
}

void  GGaugeLine::circle_middle_color(color_t color){
  if(!this->isCreated()) this->create();
  this->border_color(color);
}

color_t  GGaugeLine::circle_middle_color(){
  if(!this->isCreated()) this->create();
  return this->border_color();
}

void  GGaugeLine::circle_middle_radius(uint16_t radius){
  if(!this->isCreated()) this->create();
  this->corner_radius(radius);
}

uint16_t  GGaugeLine::circle_middle_radius(){
  if(!this->isCreated()) this->create();
  return this->corner_radius();
}

void  GGaugeLine::critical_color(color_t color){
  if(!this->isCreated()) this->create();
  this->line_color(color);
}

color_t GGaugeLine::critical_color(){
  if(!this->isCreated()) this->create();
  return this->line_color();
}

// void GGaugeLine::line_width(uint16_t width){
//   if(!this->isCreated()) this->create();
//   this->line_width(width);
// }

void GGaugeLine::angle_offset(uint16_t angle){
  if(!this->isCreated()) this->create();
  // lv_gauge_ext_t* ext = (lv_gauge_ext_t*)this->ext_attr();
  lv_lmeter_set_angle_offset(this->obj, angle);
}

uint16_t  GGaugeLine::angle_offset(){
  if(!this->isCreated()) this->create();
  return lv_lmeter_get_angle_offset(this->obj);
}

void GGaugeLine::scale(uint16_t angle, uint8_t line_count, uint8_t label_count){
  if(!this->isCreated()) this->create();
  lv_gauge_set_scale(this->obj, angle, line_count, label_count);
}

uint16_t  GGaugeLine::scale_angle(){
  if(!this->isCreated()) this->create();
  return lv_gauge_get_scale_angle(this->obj);
}

uint8_t GGaugeLine::line_count() {
  if(!this->isCreated()) this->create();
  return lv_gauge_get_line_count(this->obj);
}

uint8_t GGaugeLine::label_count(){
  if(!this->isCreated()) this->create();
  return lv_gauge_get_label_count(this->obj);
}

void GGaugeLine::operator=(int16_t value){
  if(!this->isCreated()) this->create();
  this->value(0,value);
}


