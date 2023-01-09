#include "GGaugeRainbow.h"
#include "GWidgets.h"

#define LV_OBJX_NAME  "GGaugeRainbow"
#define GAUGE_TYPE_ARC_MAX_ARC     30
#define GAUGE_TYPE_FULL_MAX_ARC    40

static lv_signal_cb_t ancestor_signal_cb;
static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if(res != LV_RES_OK ) return res;
  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  GGaugeRainbow* pGaugeRainbow = (GGaugeRainbow*) lv_obj_get_user_data(obj);
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) pGaugeRainbow->ext_attr();
  uint16_t w = pGaugeRainbow->width();
  uint16_t h = pGaugeRainbow->height();

  if(sign == LV_SIGNAL_CORD_CHG || sign == LV_SIGNAL_STYLE_CHG) {
    if( w != h) {
       /* บังคับให้เป็นจตุรัส โดยเอาด้านมากสุดเป็นเกณฑ์ */
      uint16_t side = MATH_MAX(60, MATH_MAX( w, h));
      pGaugeRainbow->size(side,side);
      return LV_RES_OK;
    }

    /* w จะ = h แล้ว */
    if( w != lv_area_get_width ((const lv_area_t*)param) ||
        h != lv_area_get_height((const lv_area_t*)param) || 
        sign == LV_SIGNAL_STYLE_CHG )
    {
      // Serial.printf("[size] size change wxh : %dx%d\n", w, h);
      // มีการเปลี่ยนขนาด
      pGaugeRainbow->refresh_size();
    }   
  }else
  /* หลัง del() หรือ deconstrutor ถูกเรียก และภายในได้ลบ child ไปแล้ว */
  if(sign == LV_SIGNAL_CLEANUP) {
    log_d("[GGaugeRainbow] signal cleanup");

    free_widget(ext->lb_val);
    for(int i=0; i < ext->arc_max_num; i++){
      free_widget( ext->arc[i] );
    }
    free(ext->arc);

    if(pGaugeRainbow->hasLocalStyle()) {pGaugeRainbow->freeLocalStyle(); } //pLed->_has_localstyle = false; } // 
    pGaugeRainbow->_created = false;
  }
  return LV_RES_OK;
}


GGaugeRainbow::GGaugeRainbow(GWidget& parent) : GObject(&parent) {}
GGaugeRainbow::GGaugeRainbow(GWidget* parent) : GObject(parent) {}
GGaugeRainbow::~GGaugeRainbow() { this->del(); }

void GGaugeRainbow::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_obj_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      if(GScreen.width() == 240 || GScreen.height() == 240 ) {
        this->size(200,200);
      }else{
        this->size(240,240);
      }
      this->opa(0);
      this->clickable(false);

      ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->alloc_ext_attr(sizeof(ggaugerainbow_ext_t));
        ext->gauge_type   = GAUGE_TYPE_ARC;
        ext->arc_max_num  = GAUGE_TYPE_ARC_MAX_ARC;
        ext->angle_offset = 135;
        ext->angle_total  = 180 + 2*(180- ext->angle_offset);
        ext->angle_space  = 2;
        ext->angle_unit   = (ext->angle_total - (ext->angle_space*(ext->arc_max_num-1)) ) /ext->arc_max_num;

        ext->color[0] = TFT_BLUE;
        ext->color[1] = TFT_RED;
        ext->color[2] = TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE, 180);
        ext->decimal  = 0;
        ext->val      = 0;
        ext->val_min  = 0;
        ext->val_max  = 100;
        ext->arc_id   = 0;
        

      ext->arc = (GArc**) esp32_malloc(sizeof(GArc*)* ext->arc_max_num);
      for(int i=0; i < ext->arc_max_num; i++){
        ext->arc[i] = new_GArc(this);
        
        ext->arc[i]->direction(ARC_CLOCKWISE);
        ext->arc[i]->thickness(30);
        ext->arc[i]->radius(this->width()/2);
        ext->arc[i]->color(ext->color[2]);
        ext->arc[i]->angle_offset(ext->angle_offset + (ext->angle_unit + ext->angle_space) * i);
        ext->arc[i]->angle( 0, ext->angle_unit );
        ext->arc[i]->clickable(false);
      }
      ext->lb_val = new_GLabel(this);
      ext->lb_val->font(BebasNeueBold_num_45, TFT_WHITE);//prasanmit_40, TFT_WHITE);

      this->value(ext->val);
      this->align(ALIGN_CENTER,0,0);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GGaugeRainbow::create(GWidget& parent){
  this->_par = &parent;
  create();
}

void GGaugeRainbow::create(GWidget* parent) {
  this->_par = parent;
  create();
}

void GGaugeRainbow::refresh_size(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();

  for(int i=0; i < ext->arc_max_num; i++){
    ext->arc[i]->thickness( map_f( this->width(),0,240, 0,30) );
    ext->arc[i]->radius(this->width()/2);
  }

  if( this->width() <= 120) {
    ext->lb_val->font(prasanmit_40);
  }else{
    ext->lb_val->font(BebasNeueBold_num_45);
  }
  //ext->lb_val->realign();
  this->value(ext->val);
}


void GGaugeRainbow::color(color_t color_begin, color_t color_end){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  ext->color[0] = color_begin;
  ext->color[1] = color_end;
  this->value(ext->val);
}

color_t GGaugeRainbow::color_begin(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->color[0];
}

color_t GGaugeRainbow::color_end(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->color[1];  
}

color_t GGaugeRainbow::color_bg(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->color[2];  
}

void  GGaugeRainbow::color_bg(color_t color){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  ext->color[2] = color;
  this->value(ext->val);
}



void GGaugeRainbow::decimal(uint8_t decimal_num){  // แสดงทศนิยม 0 หรือ 1 หรือ 2 หรือ 3 ตำแหน่ง
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  ext->decimal = constrain(decimal_num, 0, 3);
}

GGaugeRainbow& GGaugeRainbow::operator =(float num){
  this->value(num);
  return *this;
}

void GGaugeRainbow::font(font_t &font){        // font สำหรับแสดง value ตรงกลาง
  this->label().font(font);
}

const font_t*GGaugeRainbow::font(){
  return this->label().font();
}

void GGaugeRainbow::font_color(color_t color){
  this->label().color(color);
}

color_t GGaugeRainbow::font_color(){
  return this->label().color();
}

void GGaugeRainbow::value(float val , int8_t decimal_num){  // ถ้า decimal_num = -1 จะใช้ค่าที่ระบบกำหนดจาก decimal(..)
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();

  if( ext->val != val) {
    this->event_send(EVENT_VALUE_CHANGED);
  }

  ext->val = val;
  if( decimal_num > 0 ) {
    this->decimal(decimal_num);
  }
  if( ext->decimal == 0) {
    ext->lb_val->raw_text(String((int)ext->val ));
  }else{
    ext->lb_val->raw_text(String(ext->val, (int8_t)ext->decimal));
  }
//  ext->lb_val->raw_text(String(ext->val, (decimal_num==-1)? ext->decimal : constrain(decimal_num,0,3)));
  uint8_t cur_arc_id = constrain(map( ext->val, ext->val_min, ext->val_max, 0,ext->arc_max_num-1), 0, ext->arc_max_num-1);

  if( cur_arc_id > ext->arc_id ) {
    for(int i=ext->arc_id; i<= cur_arc_id;i++){
      //this->_set_arc( i, (ext->val== ext->val_min)? false :  i <= id);  
      this->_set_arc( i, true); 
    }
  }else
  if( cur_arc_id < ext->arc_id ) {
    for(int i=ext->arc_id; i > cur_arc_id;i--){
      this->_set_arc( i, false);  
    }
    if(ext->val== ext->val_min){
      this->_set_arc( 0, false); 
    }
  }
  ext->arc_id = cur_arc_id;
}

float GGaugeRainbow::value(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->val;
}

float GGaugeRainbow::value_max(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->val_max;
}

float GGaugeRainbow::value_min(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->val_min;
}

void GGaugeRainbow::range(float val_min, float val_max){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();

  ext->val_min = MATH_MIN(val_min, val_max);
  ext->val_max = MATH_MAX(val_max, val_min);
  ext->val = constrain( ext->val, ext->val_min, ext->val_max);
}

void  GGaugeRainbow::thickness(uint8_t thick){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();

  for(int i=0; i < ext->arc_max_num; i++){
    ext->arc[i]->thickness(thick);
  }
}

uint8_t GGaugeRainbow::thickness(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->arc[0]->thickness();
}

GLabel& GGaugeRainbow::label(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return *ext->lb_val;
}

void GGaugeRainbow::_set_arc(uint8_t id, bool active){
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  color_hsv_t hsv[2];
  hsv[0] = TFT_COLOR_TO_HSV( ext->color[0] );
  hsv[1] = TFT_COLOR_TO_HSV( ext->color[1] );
  ext->arc[id]->color( active?  TFT_COLOR_HSV( map_f( id,0,ext->arc_max_num-1, hsv[0].h, hsv[1].h) , map_f( id,0,ext->arc_max_num-1, hsv[0].s, hsv[1].s),map_f( id,0,ext->arc_max_num-1, hsv[0].v, hsv[1].v) ) :
                                ext->color[2] );
}


void GGaugeRainbow::gauge_type(GGauge_type_t type){  // type : GAUGE_TYPE_ARC (defaut) , GAUGE_TYPE_FULL
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();

  if( ext->gauge_type != type){
    // คืนเม็มเดิมๆออกไปก่อน
    for(int i=0; i < ext->arc_max_num; i++){
      free_widget( ext->arc[i] );
    }
    free(ext->arc);

    // เปลี่ยนประเภทและจำนวน
    ext->gauge_type = type;  
    switch(ext->gauge_type){
      case GAUGE_TYPE_ARC:
        ext->arc_max_num  = GAUGE_TYPE_ARC_MAX_ARC;
        ext->angle_offset = 135;
        ext->angle_total  = 180 + 2*(180- ext->angle_offset);
        ext->angle_space  = 2;
        ext->angle_unit   = (ext->angle_total - (ext->angle_space* (ext->arc_max_num-1)) ) /ext->arc_max_num;
        break;
      case GAUGE_TYPE_FULL:
        ext->arc_max_num  = GAUGE_TYPE_FULL_MAX_ARC;
        ext->angle_offset = 270;
        ext->angle_total  = 360;
        ext->angle_space  = 2;
        ext->angle_unit   = (ext->angle_total - (ext->angle_space*(ext->arc_max_num)) ) /ext->arc_max_num;
        break;
    }

    // จองเม็มใหม่
    ext->arc = (GArc**) esp32_malloc(sizeof(GArc*)* ext->arc_max_num);
    for(int i=0; i < ext->arc_max_num; i++){
      ext->arc[i] = new_GArc(this);
      
      ext->arc[i]->direction(ARC_CLOCKWISE);
      ext->arc[i]->thickness(map_f( this->width(),0,240, 0,30));
      ext->arc[i]->radius(this->width()/2);
      ext->arc[i]->color(ext->color[2]);
      ext->arc[i]->angle_offset(ext->angle_offset + (ext->angle_unit + ext->angle_space) * i);
      ext->arc[i]->angle( 0, ext->angle_unit );
      ext->arc[i]->clickable(false);
    }
    
  }  
}

GGauge_type_t GGaugeRainbow::gauge_type(){
  if(!this->isCreated()) create();
  ggaugerainbow_ext_t* ext = (ggaugerainbow_ext_t*) this->ext_attr();
  return ext->gauge_type;
}
