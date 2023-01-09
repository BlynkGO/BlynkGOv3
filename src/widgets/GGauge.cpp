/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GGauge.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GGauge.h"
#include "GWidgets.h"


#define LV_OBJX_NAME  "GGauge" 

static lv_signal_cb_t ancestor_signal_cb;
static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if(res != LV_RES_OK ) return res;
  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  GGauge* pGauge = (GGauge*) lv_obj_get_user_data(obj);
  ggauge_ext_t* ext = (ggauge_ext_t*) pGauge->ext_attr();
  uint16_t w = pGauge->width();
  uint16_t h = pGauge->height();

  if(sign == LV_SIGNAL_CORD_CHG || sign == LV_SIGNAL_STYLE_CHG) {
    if( w != h) {
      /* บังคับให้เป็นจตุรัส โดยเอาด้านมากสุดเป็นเกณฑ์ */
      uint16_t side = LV_MATH_MAX( w, h);
      if(side > 240) {
        if( GScreen.width() >= 320 && GScreen.height() >=320 )
          pGauge->size(320,320);
        else
        if( GScreen.width() >= 240 && GScreen.height() >=240 )
          pGauge->size(240,240);
        else
          pGauge->size(120,120);
      }
      if(side ==240) {
        if( GScreen.width() >= 240 && GScreen.height() >=240 )
          pGauge->size(240,240);
        else
          pGauge->size(120,120);
      }else{
        pGauge->size(120,120);
      }
      return LV_RES_OK;
    }

    /* w จะ = h แล้ว */
    if( w != lv_area_get_width ((const lv_area_t*)param) ||
        h != lv_area_get_height((const lv_area_t*)param) || 
        sign == LV_SIGNAL_STYLE_CHG )
    {
      // Serial.printf("[size] size change wxh : %dx%d\n", w, h);
      // มีการเปลี่ยนขนาด
      pGauge->gauge_type(pGauge->gauge_type());
    } 
  } else
  if(sign == LV_SIGNAL_CLEANUP) {
#if BLYNGO_DEBUG
    Serial.println("[GGauge] signal cleanup");
#endif
    free_widget(ext->arc_bg);       ext->arc_bg = NULL;
    free_widget(ext->arc_fg);       ext->arc_fg = NULL;
    free_widget(ext->title);        ext->title  = NULL;
    free_widget(ext->lb_val);       ext->lb_val = NULL;
    free_widget(ext->lb_val_min);   ext->lb_val_min = NULL;
    free_widget(ext->lb_val_max);   ext->lb_val_max = NULL;

    if(pGauge->hasLocalStyle())  pGauge->freeLocalStyle();  // หาก มี localStyle จะให้ลบทิ้งเสีย
    pGauge->_created = false;
  }
  return LV_RES_OK;
}

GGauge::GGauge(GWidget& parent)  : GObject(&parent) {}
GGauge::GGauge(GWidget* parent)  : GObject(parent)  {}
GGauge::GGauge(uint16_t color, GWidget& parent) : _color(color) , GObject(&parent) {}
GGauge::GGauge(uint16_t color, GWidget* parent) : _color(color) , GObject(parent) {}
GGauge::GGauge(String title, uint16_t color, GWidget& parent) : _color(color), _title(title) , GObject(&parent) {}
GGauge::GGauge(String title, uint16_t color, GWidget* parent) : _color(color), _title(title) , GObject(parent) {}
GGauge::~GGauge(){ this->del(); }

void GGauge::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;


  if(!this->isCreated()) {
    this->obj = lv_obj_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created  = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);
      lv_obj_set_style(this->obj, &lv_style_transp);

      ggauge_ext_t* ext = (ggauge_ext_t*)this->alloc_ext_attr(sizeof(ggauge_ext_t));

      ext->gauge_type = GAUGE_TYPE_ARC;

      ext->arc_bg     = new_GArc(this);   ext->arc_bg->clickable(false);
      ext->arc_fg     = new_GArc(this);   ext->arc_fg->clickable(false);
      ext->title      = new_GLabel(this); ext->title->clickable(false);
      ext->lb_val     = new_GLabel(this); ext->lb_val->clickable(false);
      ext->lb_val_min = new_GLabel(this); ext->lb_val_min->clickable(false);
      ext->lb_val_max = new_GLabel(this); ext->lb_val_max->clickable(false);

      ext->arc_bg->radius(84);
      ext->arc_bg->thickness(33);

      ext->arc_fg->radius(80);
      ext->arc_fg->thickness(25);
      ext->arc_fg->color(this->_color);

      // ext->arc_bg->color(TFT_COLOR_MIX(ext->arc_fg->color(), TFT_BLACK, 100));
      ext->arc_bg->color(TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE, 208));

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      this->size(240, 240);

      ext->lb_val_min->text( String((int)this->_val_min));
      ext->lb_val_max->text( String((int)this->_val_max));
      this->title(this->_title);
      this->title_color(TFT_WHITE);
      this->rounded(true);
      ext->lb_val->text("--");


      this->align(ALIGN_CENTER,0,0);
    }
  }
}

void GGauge::create(GWidget& parent) {
  this->_par = &parent;
  GGauge::create();
}

void GGauge::create(GWidget* parent) {
  this->_par = parent;
  GGauge::create();
}

void GGauge::range(float val_min, float val_max) {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  this->_val_min  = min(val_min,val_max); 
  this->_val_max  = max(val_min,val_max); 
  
  ext->lb_val_min->text( String((int)this->_val_min));
  ext->lb_val_max->text( String((int)this->_val_max));

  // this->value(this->_val_min);
  ext->lb_val->text("--");
}

void GGauge::range_font(font_t &font){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->lb_val_min->font(font);

  this->range(this->_val_min, this->_val_max);
}

void GGauge::range_font(font_t &font, color_t color){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->lb_val_min->font(font, color);

  this->range(this->_val_min, this->_val_max);
}

void GGauge::range_color(color_t color){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->lb_val_min->color(color);

  this->range(this->_val_min, this->_val_max);
}

void  GGauge::range_hidden(bool hidden){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();

  ext->lb_val_min->hidden(hidden);
  ext->lb_val_max->hidden(hidden);
}

bool  GGauge::range_hidden(){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  // ext->lb_val_min->color(color);
  return ext->lb_val_min->hidden();
}

GGauge& GGauge::operator =(float num){
  this->value(num);
  return *this;
}

void GGauge::value(float val, int8_t decimal_num) {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();

  this->_val = val;  
  if( decimal_num == -1) {
    ext->lb_val->text( (this->_decimal_point > 0)? String(this->_val, (int8_t)this->_decimal_point) :
                        String((int)(this->_val)));
  }else{
    ext->lb_val->text( (decimal_num > 0)? String(this->_val, (int8_t)decimal_num) :
                        String((int)(this->_val)));
  }

  int16_t angle_end;
  float constrain_val  = constrain(val, this->_val_min, this->_val_max);
  switch(ext->gauge_type) {
    case GAUGE_TYPE_ARC:
      angle_end = round(map_f(constrain_val, this->_val_min, this->_val_max, 0, 240 ));
      break;
    case GAUGE_TYPE_FULL:
      angle_end = round(map_f(constrain_val, this->_val_min, this->_val_max, 0, 360 ));
      break;
  }
  ext->arc_fg->angle(0, angle_end);
  this->event_send(EVENT_VALUE_CHANGED);
}

float GGauge::value() {
  if(!this->isCreated()) create();
  return this->_val;
}

float GGauge::value_max() {
  if(!this->isCreated()) create();
  return this->_val_max;
}

float GGauge::value_min() {
  if(!this->isCreated()) create();
  return this->_val_min;
}

void GGauge::decimal(uint8_t decimal_num){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  this->_decimal_point = constrain(decimal_num, 0,3);
}

void GGauge::font(font_t &font) {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->lb_val->font(font);
}

void  GGauge::font(font_t &font, color_t color){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->lb_val->font(font);
  ext->lb_val->color(color);
}

void GGauge::text_color(uint16_t color) {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->lb_val->color(color);
}

void GGauge::color(uint16_t color){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->arc_fg->color(color);
  // ext->arc_bg->color(TFT_COLOR_MIX(ext->arc_fg->color(), TFT_BLACK, 200));
}

void GGauge::color(color_t arc_fg_color, color_t arc_bg_color){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->arc_fg->color(arc_fg_color);
  ext->arc_bg->color(arc_bg_color);
}

uint16_t GGauge::color(){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  return ext->arc_fg->color();
}

void GGauge::arc_bg_color(color_t color){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->arc_bg->color(color);
}

color_t GGauge::arc_bg_color(){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  return ext->arc_fg->color();
}

void  GGauge::title(String title){
  if(!this->isCreated()) create();
  this->_title = title;
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  /* ใช้แบบ static_text จะได้ไม่ต้องเปลืองเม้ม 2 รอบ */
  lv_label_set_static_text(ext->title->obj, this->_title.c_str());
}

String GGauge::title(){
  if(!this->isCreated()) create();
  return this->_title;
}

void GGauge::title_color(color_t color) {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->title->color(color);
}

color_t GGauge::title_color() {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  return ext->title->color();
}

void GGauge::title_font(font_t &font){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->title->font(font);
}

void  GGauge::title_font(font_t &font, color_t color){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->title->font(font);
  ext->title->color(color);
}


void  GGauge::gauge_type(GGauge_type_t type){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->gauge_type = type;

  uint16_t w = this->width();
  uint16_t h = this->height();
  switch(ext->gauge_type) {
    case GAUGE_TYPE_ARC : {
      ext->arc_bg->angle_offset(150);
      ext->arc_bg->angle(0, 240);

      ext->arc_fg->angle_offset(150);
      ext->arc_fg->angle(0, 180);

      if(w<=320) {
        ext->arc_bg->radius(map_f(w, 0, 240, 0, 84)); ext->arc_bg->thickness( map_f(w, 0, 240, 0, 33));
        ext->arc_fg->radius(map_f(w, 0, 240, 0, 80)); ext->arc_fg->thickness( map_f(w, 0, 240, 0, 25));
      }else{
        int16_t radius_arg_fg = map_f(w, 0, 240, 0, 80);  // รัศมี บังคับเหลือ  25% ของด้านกว้าง
        int16_t thick_arg_fg  = map_f(w, 0, 400, 0, 32);  // ความหนา เบื้องต้น 8% ของด้านกว้าง

        ext->arc_fg->radius( radius_arg_fg); 
        this->thickness(thick_arg_fg);
      }

      ext->lb_val->color(TFT_WHITE);
      ext->lb_val_min->color(TFT_GRAY);
      ext->lb_val_max->linkStyle(ext->lb_val_min);


      ext->arc_bg->align(ALIGN_CENTER,0, map_f(w, 0, 240, 0, 25)); ext->arc_bg->auto_realign(true);
      ext->arc_fg->align(ALIGN_CENTER,0, map_f(w, 0, 240, 0, 25)); ext->arc_fg->auto_realign(true);
      ext->lb_val->align(ALIGN_CENTER,0, map_f(w, 0, 240, 0, 25)); ext->lb_val->auto_realign(true);

      if( w > 240) {
        ext->lb_val_min->hidden(false);
        ext->lb_val_max->hidden(false);
        ext->title->font(prasanmit_35);
        ext->lb_val->font(prasanmit_40);
        ext->lb_val_min->font(prasanmit_20);
        // ext->lb_val_max->font(prasanmit_20); // linkStyle จาก lb_val_min แล้ว

        ext->lb_val_min->align(ext->arc_bg, ALIGN_IN_BOTTOM_LEFT,  40,-20);  ext->lb_val_min->auto_realign(true);
        ext->lb_val_max->align(ext->arc_bg, ALIGN_IN_BOTTOM_RIGHT,-40,-20);  ext->lb_val_max->auto_realign(true);

      }else
      if( w ==240) {
        ext->lb_val_min->hidden(false);
        ext->lb_val_max->hidden(false);
        ext->title->font(prasanmit_30);
        ext->lb_val->font(prasanmit_35);
        ext->lb_val_min->font(prasanmit_20);
        // ext->lb_val_max->font(prasanmit_20);

        ext->lb_val_min->align(ext->arc_bg, ALIGN_IN_BOTTOM_LEFT,  30,-10);  ext->lb_val_min->auto_realign(true);
        ext->lb_val_max->align(ext->arc_bg, ALIGN_IN_BOTTOM_RIGHT,-30,-10);  ext->lb_val_max->auto_realign(true);

      }else{
        ext->lb_val_min->hidden(true);
        ext->lb_val_max->hidden(true);
        ext->title->font(prasanmit_20);
        ext->lb_val->font(prasanmit_25);
        ext->lb_val_min->font(prasanmit_20);
        // ext->lb_val_max->font(prasanmit_20);
      }

      ext->title->align(ALIGN_IN_TOP_MID,0,(h/2.0  - ext->arc_bg->posY() - ext->title->font_height())/2.0 );
      break;
    }//case GAUGE_TYPE_ARC_ROUND
    case GAUGE_TYPE_FULL: {
      ext->arc_bg->angle_offset(270);
      ext->arc_bg->angle(0, 360);
      ext->arc_bg->radius(map_f(w, 0, 240, 0, 84)); 
      ext->arc_bg->thickness( map_f(w, 0, 240, 0, 33));

      ext->arc_fg->angle_offset(270);
      ext->arc_fg->angle(0, 360);
      ext->arc_fg->radius(map_f(w, 0, 240, 0, 80)); 
      ext->arc_fg->thickness( map_f(w, 0, 240, 0, 25));

      ext->lb_val->color(TFT_WHITE);
      ext->lb_val_min->hidden(true);
      ext->lb_val_max->hidden(true);

      ext->arc_bg->align(ALIGN_CENTER,0, map_f(w, 0, 240, 0, 25)); ext->arc_bg->auto_realign(true);
      ext->arc_fg->align(ALIGN_CENTER,0, map_f(w, 0, 240, 0, 25)); ext->arc_fg->auto_realign(true);
      ext->lb_val->align(ALIGN_CENTER,0, map_f(w, 0, 240, 0, 25)); ext->lb_val->auto_realign(true);

      if( w > 240) {
        ext->title->font(prasanmit_35);
        ext->lb_val->font(prasanmit_40);
      }else
      if( w ==240) {
        ext->title->font(prasanmit_30);
        ext->lb_val->font(prasanmit_35);
      }else{
        ext->title->font(prasanmit_20);
        ext->lb_val->font(prasanmit_25);
      }
      break;
    }//case GAUGE_TYPE_FULL_ROUND
  } // switch

  this->value(this->value());
}

GGauge_type_t GGauge::gauge_type() {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  return ext->gauge_type;
}


void GGauge::rounded(bool enable){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  ext->arc_bg->rounded(enable);
  ext->arc_fg->rounded(enable);
}

bool GGauge::rounded(){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  return ext->arc_fg->rounded();
}

void GGauge::thickness(uint8_t thick) {
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();

  int16_t thick_arg_fg  = thick;
  int16_t thick_arg_bg  = map_f(thick, 0, 160, 0, 192);

  int16_t radius_arg_fg = ext->arc_fg->radius();
  int16_t radius_arg_bg = radius_arg_fg + (thick_arg_bg - thick_arg_fg)/2;

  int16_t font_h_val_min = ext->lb_val_min->font_height();

  float   r_val_min      = radius_arg_bg - thick_arg_bg/4;    // รัศมีตำแหน่งของศูนย์กลางของ val_min, val_max
  float   angle_font_h   = map_f( font_h_val_min, 0, 2*PI* r_val_min, 0, 360); // แปลงความสูง font ของ val_min เป็น องศา
  Serial.printf("r_fg %.2f ; r_bg %.2f ; r_val_min : %.2f ; angle font-h of val-min : %.2f\n", 
        radius_arg_fg, radius_arg_bg, r_val_min, angle_font_h); 

  ext->arc_bg->thickness( thick_arg_bg);
  ext->arc_bg->radius(radius_arg_bg); 

  ext->arc_fg->thickness(thick_arg_fg );
  ext->arc_fg->radius( radius_arg_fg); 

  Point topleft_arc_bg = Point(ext->arc_bg->leftX(), ext->arc_bg->topY());  // relative to the widget top left point
  Point c              = ext->arc_bg->centerpoint();                        // inner center point in arc_bg
  Point c_val_min      = Point( r_val_min, 150 - angle_font_h, c, CLOCKWISE) + topleft_arc_bg;
  Point c_val_max      = Point( r_val_min,  30 + angle_font_h, c, CLOCKWISE) + topleft_arc_bg;
  ext->lb_val_min->center(c_val_min);
  ext->lb_val_max->center(c_val_max);
}

uint8_t GGauge::thickness(){
  if(!this->isCreated()) create();
  ggauge_ext_t* ext = (ggauge_ext_t*) this->ext_attr();
  return ext->arc_fg->thickness();
}
