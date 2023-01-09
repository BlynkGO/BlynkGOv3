
/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GIcon.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * History
 *   V.2.0.0 @ 05/09/22
 *     - fix ให้ event ภายในรองรับคำสั่ง font_color(..), font(..) ได้ด้วย
 *
 *********************************************************************
 */

#include "GIcon.h"
#include "GWidgets.h"

#define LV_OBJX_NAME  "GIcon" 

static lv_signal_cb_t ancestor_signal_cb;
lv_res_t gicon_signal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if(res != LV_RES_OK ) return res;
  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  GIcon* pIcon = (GIcon*) lv_obj_get_user_data(obj);
  gicon_ext_t* ext = (gicon_ext_t*)  pIcon->ext_attr();

  if( sign == LV_SIGNAL_CORD_CHG ) {
    if( pIcon->fit_left()  == FIT_NONE && pIcon->fit_right()  == FIT_NONE ) {
      uint16_t w = pIcon->width();
      if( w != lv_area_get_width ((const lv_area_t*)param) )
      {
        if( ext->label->width()  > w  ){
          ext->label->long_mode(LABEL_LONG_SROLL, w);
        }else{
          ext->label->long_mode(LABEL_LONG_EXPAND);
        }
      }
    }
  }
  else
  /* หลัง del() หรือ deconstrutor ถูกเรียก และภายในได้ลบ child ไปแล้ว */
  if(sign == LV_SIGNAL_CLEANUP) {
#if BLYNKGO_DEBUG 
    Serial.println("[GIcon] signal cleanup");
#endif
    GIcon* pIcon = (GIcon*) lv_obj_get_user_data(obj);
    gicon_ext_t* ext = (gicon_ext_t*)  pIcon->ext_attr();

    free_widget(ext->img);
    free_widget(ext->label);

    if(pIcon->hasLocalStyle()) {pIcon->freeLocalStyle(); } //pIcon->_has_localstyle = false; } // 
    pIcon->_created = false;
  }
  return LV_RES_OK;
}
void gicon_event_cb(lv_obj_t *obj, lv_event_t event);

GIcon::GIcon(GWidget& parent) : GContainer(&parent) {}
GIcon::GIcon(GWidget* parent) : GContainer(parent) {}

GIcon::GIcon(const img_t & src_img, String name, GWidget& parent)
: GContainer(&parent), _src_img(&src_img), _name(name) {}

GIcon::GIcon(const img_t & src_img, String name, GWidget* parent) 
: GContainer(parent), _src_img(&src_img), _name(name) {}

GIcon::GIcon(String file_path, String name, GWidget& parent)
: GContainer(&parent), _src_img(NULL), _name(name), _file_path(file_path) {}

GIcon::GIcon(String file_path, String name, GWidget* parent)
: GContainer(parent), _src_img(NULL), _name(name), _file_path(file_path) {}


GIcon::~GIcon() { this->del(); }

void GIcon::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;


  if(!this->isCreated()) {
    GContainer::create();

    if(this->obj != NULL) {
      this->_created  = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gicon_event_cb);//main_event_cb);

      gicon_ext_t* ext = (gicon_ext_t*) this->alloc_ext_attr(sizeof(gicon_ext_t)); //this->allocGExtension(sizeof(gicon_ext_t));

      this->layout(LAYOUT_COL_M);
      this->padding(5);
      this->padding_top(10);
      this->padding_inner(3);
      this->padding_left(5);
      this->padding_right(5);
      this->corner_radius(5);
      GObject::font(prasanmit_25, TFT_WHITE);
      this->body_draw(false);

      // /* alloc ext's GWidgets on PSRAM (if exist) */
      ext->img    = new_GImage(this);
      ext->label  = new_GLabel(this);


      ext->img->create(); 
      ext->label->create(); 
      // ext->label->font(prasanmit_25, TFT_WHITE);

      if(this->_src_img != NULL)        ext->img->setImage(this->_src_img);
      else if(this->_file_path != "")   ext->img->setImage(this->_file_path);
      if(this->_name != "")             this->text(this->_name); // จะมีการปรับ long_mode ให้อัตโนมัติ

      ext->img->event_parent(true);
      ext->label->event_parent(true);

      this->align(ALIGN_CENTER);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(gicon_signal_cb);
    }
  }
}

void GIcon::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GIcon::create(GWidget* parent) {
  this->_par = parent;
  create();
}


void GIcon::setImage(String file_path){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  ext->img->setImage(file_path);
}

void GIcon::setImage(const img_t & src_img){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  ext->img->setImage(src_img);
}

void GIcon::setImage(const img_t * src_img){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  ext->img->setImage(src_img);
}

GImage* GIcon::getImage(){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  return ext->img;
}

GLabel* GIcon::getLabel(){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  return ext->label;
}

void GIcon::body_draw(bool enable){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  ext->body_draw = !!enable;
  if(ext->body_draw){
    this->opa(255);
    this->border_opa(255);
    this->width(120);
  }else{
    this->opa(0);
    this->border_opa(0);
  }
}

bool GIcon::body_draw(){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  return ext->body_draw;
}

void GIcon::text(String txt){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();

  lv_area_t ori;
  lv_obj_get_coords(ext->label->obj, &ori);


  ext->label->text(txt);
  this->_name = *ext->label;
  this->obj->signal_cb( this->obj, LV_SIGNAL_CORD_CHG, &ori);


  // if(ext->label->width() > ext->img->width() + 35 ) {
  //   ext->label->long_mode(LABEL_LONG_SROLL);
  //   ext->label->width(ext->img->width() + 35 + this->padding_left() + this->padding_right());
  // }else{
  //   ext->label->long_mode(LABEL_LONG_EXPAND);
  // }

}


String GIcon::text(){
  if(!this->isCreated()) create();
  // gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  // return ext->label->text();
  return this->_name;
}

void GIcon::text_hidden(bool enable) {
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  return ext->label->hidden(enable);
}

void GIcon::long_mode(long_mode_t long_mode){
  if(!this->isCreated()) create();
  gicon_ext_t* ext = (gicon_ext_t*)this->ext_attr();
  ext->label->long_mode(long_mode);
}

void gicon_event_cb(lv_obj_t *obj, lv_event_t event) {
  GIcon* pIcon = (GIcon*) lv_obj_get_user_data(obj);
  gicon_ext_t* ext = (gicon_ext_t*)pIcon->ext_attr();
  static color_t text_color_origin;
  static color_t body_main_color_origin;
  static color_t body_grad_color_origin;
  static color_t border_color_origin;

  if(event == EVENT_PRESSED) {
    text_color_origin       = (ext->label->hasLocalStyle())? ext->label->color() : pIcon->text_color(); // pIcon->text_color(); //
    pIcon->image_color(TFT_BLACK);
    pIcon->image_intense(255-150);
    pIcon->text_color(TFT_COLOR_MIX(text_color_origin, TFT_BLACK, 110 ));

    if(pIcon->body_draw()) {
      body_main_color_origin  = pIcon->color();
      body_grad_color_origin  = pIcon->grad_color();
      border_color_origin     = pIcon->border_color();
      pIcon->color(TFT_COLOR_MIX(body_main_color_origin, TFT_BLACK, 150 ), TFT_COLOR_MIX(body_grad_color_origin, TFT_BLACK, 150 ));
      pIcon->border_color(TFT_COLOR_MIX(border_color_origin, TFT_BLACK, 150 ));
    }
  }else
  if(event == EVENT_RELEASED || event == EVENT_PRESS_LOST){
    pIcon->image_intense(0);
    if( pIcon->text_color() == TFT_COLOR_MIX(text_color_origin, TFT_BLACK, 110 ))
      pIcon->text_color(text_color_origin);
    if(pIcon->body_draw()) {
      pIcon->color(body_main_color_origin,body_grad_color_origin);
      pIcon->border_color(border_color_origin);
    }
  }
  main_event_cb(obj, event);
}

