/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GRoller.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#include "GRoller.h"

#define LV_OBJX_NAME  "GRoller" 

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
    Serial.println("[GRoller] signal cleanup");
#endif
    GRoller* pRoller = (GRoller*) lv_obj_get_user_data(obj);

    if(pRoller->hasLocalStyle()) { pRoller->freeRollerStyle(); } //_has_localstyle = false; } //pRoller->freeLocalStyle(); } // 
    pRoller->_created = false;
  }
  return LV_RES_OK;
}


GRoller::GRoller(GWidget& parent) : GObject(&parent) {}
GRoller::GRoller(GWidget* parent) : GObject(parent)  {}
GRoller::~GRoller() { this->del(); }

void GRoller::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_roller_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      this->createRollerStyle();

      lv_roller_set_visible_row_count(this->obj, 4);
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      this->align(ALIGN_CENTER,0,0);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GRoller::create(GWidget& parent) {
  this->_par = &parent;
  create();

}

void GRoller::create(GWidget* parent) {
  this->_par = parent;
  create();

}

void GRoller::createRollerStyle(){
  if(!this->isCreated()) create();

  if(!this->hasLocalStyle()){

    this->style[SELECT].copy ( (style_t*) lv_roller_get_style(this->obj, LV_ROLLER_STYLE_SEL));
    this->style[BG].copy     ( (style_t*) lv_roller_get_style(this->obj, LV_ROLLER_STYLE_BG));

      lv_roller_set_style(this->obj, LV_ROLLER_STYLE_SEL, this->style[SELECT].raw());
      lv_roller_set_style(this->obj, LV_ROLLER_STYLE_BG,  this->style[BG].raw());

    this->_style = &this->style[BG];
    this->_has_localstyle = true;
  }
}

void GRoller::freeRollerStyle(){
  if(this->hasLocalStyle()){
    for(int i =0; i < 2; i++)
      this->style[i].release();
    this->_has_localstyle = false;
  }
}

String GRoller::selected(){
  if(!this->isCreated()) create();

	char buf[256];
  lv_roller_get_selected_str(obj, buf, sizeof(buf));

  return String(buf);
}

uint16_t GRoller::selected_id(){
  if(!this->isCreated()) create();

  return lv_roller_get_selected(this->obj);
}

void GRoller::selected(uint16_t option_id, lv_anim_enable_t anim) {
  if(!this->isCreated()) create();

  lv_roller_set_selected(this->obj, option_id, anim );
}



// void  GRoller::selected(String option, lv_anim_enable_t anim ){
//   if(!this->isCreated()) create();

//   String _option = String(lv_roller_get_options(this->obj));
//   Serial.println("------------------------");
//   Serial.println(_option);
//   Serial.println("------------------------");
// }

void GRoller::width(uint16_t w){
  if(!this->isCreated()) create();

  lv_roller_set_fix_width(this->obj, w);
  // lv_roller_ext_t * ext = (lv_roller_ext_t*) this->ext_attr();
  // lv_obj_set_width( ext->ddlist.label,w);
}

uint16_t GRoller::width(){
  if(!this->isCreated()) create();

  return lv_obj_get_width(this->obj);
}

void GRoller::visible_rows(uint16_t count){
  if(!this->isCreated()) create();

  lv_roller_set_visible_row_count(this->obj, count);
}

void GRoller::option_align(lv_label_align_t align){
  if(!this->isCreated()) create();

  lv_roller_set_align(this->obj, align);
}

void GRoller::options(String* option, uint16_t option_num){
  if(!this->isCreated()) create();

  String _option="";
  for(int i =0; i < option_num; i++ ){
    _option += option[i];
    if(i < option_num-1) _option += "\n";
  }
  // Serial.println("=========================");
  // Serial.println(_option);
  // Serial.println("=========================");
  lv_roller_set_options(this->obj, _option.c_str(),  LV_ROLLER_MODE_INIFINITE);
}

void GRoller::options(int from, int to, bool zero_front){
  if(!this->isCreated()) create();

  String _option="";
  if( zero_front ) {
    char buf[20];
    char format[10];

    uint16_t totalDigits=0;
    int max_num = max(abs(from), abs(to));
    while(max_num!=0){
      max_num = max_num/10;
      totalDigits ++;
    }
    sprintf(format, "%%0%dd", totalDigits);
    // Serial.println(format);
    for(int i =from; i <= to; i++ ){
      sprintf(buf, format , i);
      _option += String(buf);
      if(i < to) _option += "\n";
    }
  }else {
    for(int i =from; i <= to; i++ ){
      _option += String(i);
      if(i < to) _option += "\n";
    }
  }
  lv_roller_set_options(this->obj, _option.c_str(),  LV_ROLLER_MODE_INIFINITE);
}

uint16_t GRoller::options_count(){
  if(!this->isCreated()) create();
  return lv_roller_get_option_cnt(this->obj);
}

void GRoller::font(font_t &font){
  if(!this->isCreated()) create();
  this->style[BG].font(font);
}

void GRoller::font(font_t &font, color_t color) {
  if(!this->isCreated()) create();
  this->style[BG].font(font);
  this->style[BG].font_color(color);
}

const font_t* GRoller::font(){
  if(!this->isCreated()) create();
  return this->style[BG].font();
}

void  GRoller::font_color(color_t color){
  if(!this->isCreated()) create();
  this->style[SELECT].font_color(color);
}

color_t GRoller::font_color(){
  if(!this->isCreated()) create();
  return this->style[SELECT].font_color();
}


void GRoller::color(color_t color) {
  if(!this->isCreated()) create();
  this->style[SELECT].color(color);
}

color_t GRoller::color(){
  if(!this->isCreated()) create();
  return this->style[SELECT].color();
}

void GRoller::opa(uint8_t opa) {
  if(!this->isCreated()) create();
  this->style[SELECT].opa(opa);
  this->style[BG].opa(opa);
}

uint8_t GRoller::opa() {
  if(!this->isCreated()) create();
  return this->style[SELECT].opa();
}

// void GRoller::roller_event_cb(lv_obj_t *obj, lv_event_t event){
//   GRoller* pRoller = (GRoller*) lv_obj_get_user_data(obj);
//   screen_timer_update();
//   char buf[64];

//   switch(event){
//     case LV_EVENT_VALUE_CHANGED:
//       // Serial.printf("Selected month: %s\n", buf);
//       if( pRoller->fn_valchanged != NULL) {
//         lv_roller_get_selected_str(obj, buf, sizeof(buf));
//         pRoller->fn_valchanged(String(buf));
//       }
//       break;
//   }
// }
