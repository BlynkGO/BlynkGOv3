/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GObject.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#include "GObject.h"
#include "GWidgets.h"

#define LV_OBJX_NAME  "GObject" 

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
    Serial.println("[GObject] signal cleanup");
#endif
    GObject* pObject = (GObject*) lv_obj_get_user_data(obj);
    if(pObject->hasLocalStyle())  pObject->freeLocalStyle();  // หาก มี localStyle จะให้ลบทิ้งเสีย
    pObject->_created = false;
  }
  return LV_RES_OK;
}

GObject::GObject(GWidget& parent) { this->_par = &parent; }
GObject::GObject(GWidget* parent) { this->_par = parent; }
GObject::~GObject()               { this->del(); }

void GObject::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  // Serial.println("GObject Create...");
  if(!this->isCreated()) {
    this->obj = lv_obj_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created  = true;
      this->_is_linkstyle = false;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      this->size(this->_par->width(), this->_par->height());

      this->align(ALIGN_CENTER,0,0);
      // this->auto_realign(true);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
  // MEM_CHECK;
}

void GObject::create(GWidget& _par) {
  this->_par = &_par;
  GObject::create();
}

void GObject::create(GWidget* _par) {
  this->_par = _par;
  GObject::create();
}

void GObject::del() {
  if(this->isCreated()) {
    lv_obj_del(this->obj);
    this->obj = NULL;
  }
}

void GObject::del_async(){
  if(this->isCreated()) {
    lv_obj_del_async(this->obj);
  }
}

void GObject::clean(){
  if(this->isCreated()) {
    lv_obj_clean(this->obj);
  }
}

String GObject::type(){
  if(!isCreated()) create();

  if(isCreated()) {
    lv_obj_type_t obj_type;
    lv_obj_get_type(this->obj, &obj_type);
    return obj_type.type[0];
  }
  return "unknown";
}

bool GObject::isType(String type){
  if(!isCreated()) create();

  lv_obj_type_t obj_type;
  lv_obj_get_type(this->obj, &obj_type);

  /*Swap the order. The real type comes first*/
  uint8_t i;
  for(i = 0; i < LV_MAX_ANCESTOR_NUM; i++) {
    if( String(obj_type.type[i]) == type) return true;
  }
  return false;
}

void GObject::setObject(lv_obj_t * obj){
  this->obj = obj;
  this->_created = (this->obj!=NULL);
}

bool GObject::sameWith(GWidget &base){
  if(!base.isCreated()) return false;

  if(!isCreated()) create();
  return (this->obj == base.obj);
}

bool GObject::sameWith(GWidget *base){
  if(base == NULL) return false;
  if(!base->isCreated()) return false;

  if(!isCreated()) create();
  return (this->obj == base->obj);
}

bool GObject::operator ==(GWidget &base) {
  if(!base.isCreated()) return false;

  return this->sameWith(base);
}

bool GObject::operator ==(GWidget *base) {
  return this->sameWith(base);
}

bool GObject::operator !=(GWidget &base) {
  if(!base.isCreated()) return false;

  return !this->sameWith(base);
}


void GObject::parent(GWidget* parent) {
  this->_par = parent;
  if(this->isCreated()) {
    lv_obj_set_parent(this->obj, this->_par->obj);
  }
  else{
    create();  // create() เป็น virtual จะวิ่งไปสร้างตาม GWidget นั้นๆให้ ?
  }
}

void GObject::parent(GWidget& parent){
  this->parent(&parent);
}

GWidget& GObject::parent(){
  return *this->_par;
}

GWidget* GObject::child(bool start){
  if(!isCreated()) create();

  this->_child = lv_obj_get_child(this->obj, (start)? NULL : this->_child );
  if(this->_child == NULL)  return NULL;
  GWidget* widget_child = (GWidget*) lv_obj_get_user_data(this->_child);

  while( (widget_child == NULL) || (widget_child->type() == "") ) 
  {
    this->_child = lv_obj_get_child(this->obj, this->_child );
    if(this->_child == NULL)  return NULL;
    GWidget* widget_child = (GWidget*) lv_obj_get_user_data(this->_child);
  }

  return widget_child;
}

GWidget* GObject::child_back(bool start){
  if(!isCreated()) create();

  this->_child = lv_obj_get_child_back(this->obj, (start)? NULL : this->_child );
  if(this->_child == NULL)  return NULL;

  GWidget* _child = (GWidget*) lv_obj_get_user_data(this->_child);
  if(_child == NULL)        return NULL;
  if(_child->type() == "")  return NULL;

  return _child;
}

uint16_t GObject::child_num(){
  if(!isCreated()) create();
  return lv_obj_count_children(this->obj);
}

uint16_t GObject::child_num_recursive(){
  if(!isCreated()) create();
  return lv_obj_count_children_recursive(this->obj);
}

void GObject::child_del_recursive(){
  if(!isCreated()) return;
  Serial.println(this->child_num());
  GWidget* child = this->child(true);
  while(child){
    Serial.println(child->type());
    String    widget_type = child->type();
    uint16_t  child_num   = child->child_num();
    if( ( widget_type=="GButton" && child_num > 1) || (widget_type != "GButton" && child_num >= 1 ) ) {
      child->child_del_recursive(); //recursive del
    }else{
      widget_del(child);
      child = this->child();
    }
  }
}

void GObject::event_cb(event_cb_t event_cb){
  if(!isCreated()) create();
  this->_event_cb = event_cb;
}

event_cb_t GObject::event_cb(){
  if(!isCreated()) create();
  return this->_event_cb;
}

void GObject::signal_cb(signal_cb_t signal_cb) {
  if(!isCreated()) create();
  lv_obj_set_signal_cb(this->obj, signal_cb);
}

signal_cb_t GObject::signal_cb() {
  if(!isCreated()) create();
  return lv_obj_get_signal_cb(this->obj);
}

void GObject::design_cb(design_cb_t design_cb) {
  if(!isCreated()) create();
  lv_obj_set_design_cb(this->obj, design_cb);
}

design_cb_t GObject::design_cb() {
  if(!isCreated()) create();
  return lv_obj_get_design_cb(this->obj);
}

void GObject::user_data(void* user_dat) {
  if(!isCreated()) create();
  this->_user_data = user_dat;
}

void* GObject::user_data(){
  if(!isCreated()) create();
  return this->_user_data;
}

void* GObject::ext_attr(){
  return lv_obj_get_ext_attr(this->obj); // หมายเหตุ ห้ามมี if(!isCreated()) create();
}

void* GObject::alloc_ext_attr(uint16_t ext_size){
  if(!isCreated()) create();
  return lv_obj_allocate_ext_attr(this->obj, ext_size);
}

void GObject::invalidate(){
  if(!isCreated()) create();
  lv_obj_invalidate(this->obj);
}

void GObject::refreshStyle(){
  if(!isCreated()) create();
  lv_obj_refresh_style(this->obj);
}

void GObject::auto_realign(bool enable){
  if(!isCreated()) create();
  lv_obj_set_auto_realign(this->obj, !!enable);
}

bool GObject::auto_realign(){
  if(!isCreated()) create();
  return lv_obj_get_auto_realign(this->obj);
}

void GObject::realign(){
  if(!isCreated()) create();
  lv_obj_realign(this->obj);
}

void GObject::reset_align(){
  if(!isCreated()) create();

  this->obj->coords.x1    = this->obj->par->coords.x1;
  this->obj->coords.y1    = this->obj->par->coords.y1;
  this->obj->coords.x2    = this->obj->par->coords.x1 + LV_DPI;
  this->obj->coords.y2    = this->obj->par->coords.y1 + (2 * LV_DPI / 3);
  this->obj->ext_draw_pad = 0;

  this->obj->realign.align        = LV_ALIGN_CENTER;
  this->obj->realign.xofs         = 0;
  this->obj->realign.yofs         = 0;
  this->obj->realign.base         = NULL;
  this->obj->realign.auto_realign = 0;
}

void GObject::align(align_t Align, int16_t offset_x, int16_t offset_y) {
  if(!isCreated()) create();

  align_t _align = Align;
  switch(Align){
    case ALIGN_TOP_LEFT:    _align = ALIGN_IN_TOP_LEFT; break;
    case ALIGN_TOP_MID:     _align = ALIGN_IN_TOP_MID; break;
    case ALIGN_TOP_RIGHT:   _align = ALIGN_IN_TOP_RIGHT; break;
    case ALIGN_BOTTOM_LEFT: _align = ALIGN_IN_BOTTOM_LEFT; break;
    case ALIGN_BOTTOM_MID:  _align = ALIGN_IN_BOTTOM_MID; break;
    case ALIGN_BOTTOM_RIGHT:_align = ALIGN_IN_BOTTOM_RIGHT; break;
    case ALIGN_LEFT_TOP:    _align = ALIGN_IN_TOP_LEFT; break;
    case ALIGN_LEFT_MID:    _align = ALIGN_IN_LEFT_MID; break;
    case ALIGN_LEFT_BOTTOM: _align = ALIGN_IN_BOTTOM_LEFT; break;
    case ALIGN_RIGHT_TOP:   _align = ALIGN_IN_TOP_RIGHT; break;
    case ALIGN_RIGHT_MID:   _align = ALIGN_IN_RIGHT_MID; break;
    case ALIGN_RIGHT_BOTTOM:_align = ALIGN_IN_BOTTOM_RIGHT; break;

    case ALIGN_TOP:         _align = ALIGN_IN_TOP_MID; break;
    case ALIGN_BOTTOM:      _align = ALIGN_IN_BOTTOM_MID; break;
    case ALIGN_LEFT:        _align = ALIGN_IN_LEFT_MID; break;
    case ALIGN_RIGHT:       _align = ALIGN_IN_RIGHT_MID; break;

    case ALIGN_IN_TOP:      _align = ALIGN_IN_TOP_MID; break;
    case ALIGN_OUT_TOP:     _align = ALIGN_OUT_TOP_MID; break;
    case ALIGN_IN_BOTTOM:   _align = ALIGN_IN_BOTTOM_MID; break;
    case ALIGN_OUT_BOTTOM:  _align = ALIGN_OUT_BOTTOM_MID; break;
    case ALIGN_IN_LEFT:     _align = ALIGN_IN_LEFT_MID; break;
    case ALIGN_OUT_LEFT:    _align = ALIGN_OUT_LEFT_MID; break;
    case ALIGN_IN_RIGHT:    _align = ALIGN_IN_RIGHT_MID; break;
    case ALIGN_OUT_RIGHT:   _align = ALIGN_OUT_RIGHT_MID; break;
  }

  lv_obj_align(this->obj, NULL, (lv_align_t) _align, offset_x, offset_y);
  this->obj->realign.align = _align;
}

void GObject::align(align_t Align, Point offset){
  this->align(Align, offset.x, offset.y);
}

void GObject::align(GWidget &base, align_t Align, int16_t offset_x, int16_t offset_y){
  if(!isCreated()) create();

  align_t _align = Align;
  if( base.obj == this->_par->obj) {
    switch(Align){
      case ALIGN_TOP_LEFT:    _align = ALIGN_IN_TOP_LEFT; break;
      case ALIGN_TOP_MID:     _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_TOP_RIGHT:   _align = ALIGN_IN_TOP_RIGHT; break;
      case ALIGN_BOTTOM_LEFT: _align = ALIGN_IN_BOTTOM_LEFT; break;
      case ALIGN_BOTTOM_MID:  _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_BOTTOM_RIGHT:_align = ALIGN_IN_BOTTOM_RIGHT; break;
      case ALIGN_LEFT_TOP:    _align = ALIGN_IN_TOP_LEFT; break;
      case ALIGN_LEFT_MID:    _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_LEFT_BOTTOM: _align = ALIGN_IN_BOTTOM_LEFT; break;
      case ALIGN_RIGHT_TOP:   _align = ALIGN_IN_TOP_RIGHT; break;
      case ALIGN_RIGHT_MID:   _align = ALIGN_IN_RIGHT_MID; break;
      case ALIGN_RIGHT_BOTTOM:_align = ALIGN_IN_BOTTOM_RIGHT; break;

      case ALIGN_TOP:         _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_BOTTOM:      _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_LEFT:        _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_RIGHT:       _align = ALIGN_IN_RIGHT_MID; break;

      case ALIGN_IN_TOP:      _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_OUT_TOP:     _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_IN_BOTTOM:   _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_OUT_BOTTOM:  _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_IN_LEFT:     _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_OUT_LEFT:    _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_IN_RIGHT:    _align = ALIGN_IN_RIGHT_MID; break;
      case ALIGN_OUT_RIGHT:   _align = ALIGN_OUT_RIGHT_MID; break;
    }
  }
  // else if( base.isType("lv_page") ){
  //   if( lv_page_get_scrl(page) == this->_par->obj) {
  //     switch(Align){
  //       case ALIGN_TOP_LEFT:    _align = ALIGN_IN_TOP_LEFT; break;
  //       case ALIGN_TOP_MID:     _align = ALIGN_IN_TOP_MID; break;
  //       case ALIGN_TOP_RIGHT:   _align = ALIGN_IN_TOP_RIGHT; break;
  //       case ALIGN_BOTTOM_LEFT: _align = ALIGN_IN_BOTTOM_LEFT; break;
  //       case ALIGN_BOTTOM_MID:  _align = ALIGN_IN_BOTTOM_MID; break;
  //       case ALIGN_BOTTOM_RIGHT:_align = ALIGN_IN_BOTTOM_RIGHT; break;
  //       case ALIGN_LEFT_TOP:    _align = ALIGN_IN_TOP_LEFT; break;
  //       case ALIGN_LEFT_MID:    _align = ALIGN_IN_LEFT_MID; break;
  //       case ALIGN_LEFT_BOTTOM: _align = ALIGN_IN_BOTTOM_LEFT; break;
  //       case ALIGN_RIGHT_TOP:   _align = ALIGN_IN_TOP_RIGHT; break;
  //       case ALIGN_RIGHT_MID:   _align = ALIGN_IN_RIGHT_MID; break;
  //       case ALIGN_RIGHT_BOTTOM:_align = ALIGN_IN_BOTTOM_RIGHT; break;

  //       case ALIGN_TOP:         _align = ALIGN_IN_TOP_MID; break;
  //       case ALIGN_BOTTOM:      _align = ALIGN_IN_BOTTOM_MID; break;
  //       case ALIGN_LEFT:        _align = ALIGN_IN_LEFT_MID; break;
  //       case ALIGN_RIGHT:       _align = ALIGN_IN_RIGHT_MID; break;
  //     }
  //   }else{
  //     switch(Align){
  //       case ALIGN_TOP_LEFT:    _align = ALIGN_OUT_TOP_LEFT; break;
  //       case ALIGN_TOP_MID:     _align = ALIGN_OUT_TOP_MID; break;
  //       case ALIGN_TOP_RIGHT:   _align = ALIGN_OUT_TOP_RIGHT; break;
  //       case ALIGN_BOTTOM_LEFT: _align = ALIGN_OUT_BOTTOM_LEFT; break;
  //       case ALIGN_BOTTOM_MID:  _align = ALIGN_OUT_BOTTOM_MID; break;
  //       case ALIGN_BOTTOM_RIGHT:_align = ALIGN_OUT_BOTTOM_RIGHT; break;
  //       case ALIGN_LEFT_TOP:    _align = ALIGN_OUT_LEFT_TOP; break;
  //       case ALIGN_LEFT_MID:    _align = ALIGN_OUT_LEFT_MID; break;
  //       case ALIGN_LEFT_BOTTOM: _align = ALIGN_OUT_LEFT_BOTTOM; break;
  //       case ALIGN_RIGHT_TOP:   _align = ALIGN_OUT_RIGHT_TOP; break;
  //       case ALIGN_RIGHT_MID:   _align = ALIGN_OUT_RIGHT_MID; break;
  //       case ALIGN_RIGHT_BOTTOM:_align = ALIGN_OUT_RIGHT_BOTTOM; break;

  //       case ALIGN_TOP:         _align = ALIGN_OUT_TOP_MID; break;
  //       case ALIGN_BOTTOM:      _align = ALIGN_OUT_BOTTOM_MID; break;
  //       case ALIGN_LEFT:        _align = ALIGN_OUT_LEFT_MID; break;
  //       case ALIGN_RIGHT:       _align = ALIGN_OUT_RIGHT_MID; break;
  //     }
  //   }
  else{
    switch(Align){
      case ALIGN_TOP_LEFT:    _align = ALIGN_OUT_TOP_LEFT; break;
      case ALIGN_TOP_MID:     _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_TOP_RIGHT:   _align = ALIGN_OUT_TOP_RIGHT; break;
      case ALIGN_BOTTOM_LEFT: _align = ALIGN_OUT_BOTTOM_LEFT; break;
      case ALIGN_BOTTOM_MID:  _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_BOTTOM_RIGHT:_align = ALIGN_OUT_BOTTOM_RIGHT; break;
      case ALIGN_LEFT_TOP:    _align = ALIGN_OUT_LEFT_TOP; break;
      case ALIGN_LEFT_MID:    _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_LEFT_BOTTOM: _align = ALIGN_OUT_LEFT_BOTTOM; break;
      case ALIGN_RIGHT_TOP:   _align = ALIGN_OUT_RIGHT_TOP; break;
      case ALIGN_RIGHT_MID:   _align = ALIGN_OUT_RIGHT_MID; break;
      case ALIGN_RIGHT_BOTTOM:_align = ALIGN_OUT_RIGHT_BOTTOM; break;

      case ALIGN_TOP:         _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_BOTTOM:      _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_LEFT:        _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_RIGHT:       _align = ALIGN_OUT_RIGHT_MID; break;

      case ALIGN_IN_TOP:      _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_OUT_TOP:     _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_IN_BOTTOM:   _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_OUT_BOTTOM:  _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_IN_LEFT:     _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_OUT_LEFT:    _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_IN_RIGHT:    _align = ALIGN_IN_RIGHT_MID; break;
      case ALIGN_OUT_RIGHT:   _align = ALIGN_OUT_RIGHT_MID; break;
    }
  }
  lv_obj_align(this->obj, base.obj, (lv_align_t) _align, offset_x, offset_y);
  this->obj->realign.align = (lv_align_t)_align;
}

void GObject::align(GWidget &base, align_t Align, Point offset){
  this->align(base, Align, offset.x, offset.y);
}

void GObject::align(GWidget *base, align_t Align, int16_t offset_x, int16_t offset_y){
  this->align(*base, Align, offset_x, offset_y);
}

void GObject::align(GWidget *base, align_t Align, Point offset){
  this->align(*base, Align, offset);
}

void GObject::align_center(){
  this->align_center(0,0);
}

void  GObject::align_center(int16_t offset_x, int16_t offset_y){
  this->align(ALIGN_CENTER, offset_x, offset_y);
}

void GObject::align_center(Point offset){
  this->align_center(offset.x, offset.y);
}

void GObject::align_center(GWidget &base, int16_t offset_x, int16_t offset_y) {
  this->align(base, ALIGN_CENTER, offset_x, offset_y);
}

void GObject::align_center(GWidget *base, int16_t offset_x, int16_t offset_y){
  this->align(base, ALIGN_CENTER, offset_x, offset_y);
}


void GObject::c_align(align_t Align, int16_t offset_x, int16_t offset_y){
  if(!isCreated()) create();

  align_t _align = Align;
  switch(Align){
    case ALIGN_TOP_LEFT:    _align = ALIGN_IN_TOP_LEFT; break;
    case ALIGN_TOP_MID:     _align = ALIGN_IN_TOP_MID; break;
    case ALIGN_TOP_RIGHT:   _align = ALIGN_IN_TOP_RIGHT; break;
    case ALIGN_BOTTOM_LEFT: _align = ALIGN_IN_BOTTOM_LEFT; break;
    case ALIGN_BOTTOM_MID:  _align = ALIGN_IN_BOTTOM_MID; break;
    case ALIGN_BOTTOM_RIGHT:_align = ALIGN_IN_BOTTOM_RIGHT; break;
    case ALIGN_LEFT_TOP:    _align = ALIGN_IN_TOP_LEFT; break;
    case ALIGN_LEFT_MID:    _align = ALIGN_IN_LEFT_MID; break;
    case ALIGN_LEFT_BOTTOM: _align = ALIGN_IN_BOTTOM_LEFT; break;
    case ALIGN_RIGHT_TOP:   _align = ALIGN_IN_TOP_RIGHT; break;
    case ALIGN_RIGHT_MID:   _align = ALIGN_IN_RIGHT_MID; break;
    case ALIGN_RIGHT_BOTTOM:_align = ALIGN_IN_BOTTOM_RIGHT; break;

    case ALIGN_TOP:         _align = ALIGN_IN_TOP_MID; break;
    case ALIGN_BOTTOM:      _align = ALIGN_IN_BOTTOM_MID; break;
    case ALIGN_LEFT:        _align = ALIGN_IN_LEFT_MID; break;
    case ALIGN_RIGHT:       _align = ALIGN_IN_RIGHT_MID; break;

    case ALIGN_IN_TOP:      _align = ALIGN_IN_TOP_MID; break;
    case ALIGN_OUT_TOP:     _align = ALIGN_OUT_TOP_MID; break;
    case ALIGN_IN_BOTTOM:   _align = ALIGN_IN_BOTTOM_MID; break;
    case ALIGN_OUT_BOTTOM:  _align = ALIGN_OUT_BOTTOM_MID; break;
    case ALIGN_IN_LEFT:     _align = ALIGN_IN_LEFT_MID; break;
    case ALIGN_OUT_LEFT:    _align = ALIGN_OUT_LEFT_MID; break;
    case ALIGN_IN_RIGHT:    _align = ALIGN_IN_RIGHT_MID; break;
    case ALIGN_OUT_RIGHT:   _align = ALIGN_OUT_RIGHT_MID; break;
  }

  lv_obj_align_origo(this->obj, NULL, (lv_align_t) _align, offset_x, offset_y);
}

void GObject::c_align(align_t Align, Point offset){
  this->c_align(Align, offset.x, offset.y);
}

void GObject::c_align(GWidget &base, align_t Align, int16_t offset_x, int16_t offset_y){
  if(!isCreated()) create();

  align_t _align = Align;
  if( base.obj == this->_par->obj) {
    switch(Align){
      case ALIGN_TOP_LEFT:    _align = ALIGN_IN_TOP_LEFT; break;
      case ALIGN_TOP_MID:     _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_TOP_RIGHT:   _align = ALIGN_IN_TOP_RIGHT; break;
      case ALIGN_BOTTOM_LEFT: _align = ALIGN_IN_BOTTOM_LEFT; break;
      case ALIGN_BOTTOM_MID:  _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_BOTTOM_RIGHT:_align = ALIGN_IN_BOTTOM_RIGHT; break;
      case ALIGN_LEFT_TOP:    _align = ALIGN_IN_TOP_LEFT; break;
      case ALIGN_LEFT_MID:    _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_LEFT_BOTTOM: _align = ALIGN_IN_BOTTOM_LEFT; break;
      case ALIGN_RIGHT_TOP:   _align = ALIGN_IN_TOP_RIGHT; break;
      case ALIGN_RIGHT_MID:   _align = ALIGN_IN_RIGHT_MID; break;
      case ALIGN_RIGHT_BOTTOM:_align = ALIGN_IN_BOTTOM_RIGHT; break;

      case ALIGN_TOP:         _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_BOTTOM:      _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_LEFT:        _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_RIGHT:       _align = ALIGN_IN_RIGHT_MID; break;

      case ALIGN_IN_TOP:      _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_OUT_TOP:     _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_IN_BOTTOM:   _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_OUT_BOTTOM:  _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_IN_LEFT:     _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_OUT_LEFT:    _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_IN_RIGHT:    _align = ALIGN_IN_RIGHT_MID; break;
      case ALIGN_OUT_RIGHT:   _align = ALIGN_OUT_RIGHT_MID; break;
    }
  }else{
    switch(Align){
      case ALIGN_TOP_LEFT:    _align = ALIGN_OUT_TOP_LEFT; break;
      case ALIGN_TOP_MID:     _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_TOP_RIGHT:   _align = ALIGN_OUT_TOP_RIGHT; break;
      case ALIGN_BOTTOM_LEFT: _align = ALIGN_OUT_BOTTOM_LEFT; break;
      case ALIGN_BOTTOM_MID:  _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_BOTTOM_RIGHT:_align = ALIGN_OUT_BOTTOM_RIGHT; break;
      case ALIGN_LEFT_TOP:    _align = ALIGN_OUT_LEFT_TOP; break;
      case ALIGN_LEFT_MID:    _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_LEFT_BOTTOM: _align = ALIGN_OUT_LEFT_BOTTOM; break;
      case ALIGN_RIGHT_TOP:   _align = ALIGN_OUT_RIGHT_TOP; break;
      case ALIGN_RIGHT_MID:   _align = ALIGN_OUT_RIGHT_MID; break;
      case ALIGN_RIGHT_BOTTOM:_align = ALIGN_OUT_RIGHT_BOTTOM; break;

      case ALIGN_TOP:         _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_BOTTOM:      _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_LEFT:        _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_RIGHT:       _align = ALIGN_OUT_RIGHT_MID; break;

      case ALIGN_IN_TOP:      _align = ALIGN_IN_TOP_MID; break;
      case ALIGN_OUT_TOP:     _align = ALIGN_OUT_TOP_MID; break;
      case ALIGN_IN_BOTTOM:   _align = ALIGN_IN_BOTTOM_MID; break;
      case ALIGN_OUT_BOTTOM:  _align = ALIGN_OUT_BOTTOM_MID; break;
      case ALIGN_IN_LEFT:     _align = ALIGN_IN_LEFT_MID; break;
      case ALIGN_OUT_LEFT:    _align = ALIGN_OUT_LEFT_MID; break;
      case ALIGN_IN_RIGHT:    _align = ALIGN_IN_RIGHT_MID; break;
      case ALIGN_OUT_RIGHT:   _align = ALIGN_OUT_RIGHT_MID; break;
    }
  }
  
  lv_obj_align_origo(this->obj, base.obj, (lv_align_t) _align, offset_x, offset_y);
  this->obj->realign.align = (lv_align_t)Align;
}

void GObject::c_align(GWidget &base, align_t Align, Point offset) {
  this->c_align(base, Align, offset.x, offset.y);
}

void GObject::offset(int16_t offset_x, int16_t offset_y){
  if(!isCreated()) create();
  lv_obj_align(this->obj, obj->realign.base,  this->obj->realign.align, offset_x , offset_y);
}

void GObject::offset(Point offset) {
  this->offset((int16_t)offset.x, (int16_t)offset.y);
}

    // obj->realign.xofs        = x_mod;
    // obj->realign.yofs        = y_mod;
void GObject::offsetX(int16_t offset_x){
  if(!isCreated()) create();
  lv_obj_align(this->obj, obj->realign.base,  this->obj->realign.align, offset_x , this->obj->realign.yofs);
}

int16_t GObject::offsetX(){
  if(!isCreated()) create();
  return this->obj->realign.xofs;
}

void GObject::offsetY(int16_t offset_y){
  if(!isCreated()) create();
  lv_obj_align(this->obj, obj->realign.base,  this->obj->realign.align, this->obj->realign.xofs , offset_y);
}

int16_t GObject::offsetY(){
  if(!isCreated()) create();
  return this->obj->realign.yofs;
}

Point GObject::centerpoint(){
  if(!isCreated()) create();
  return Point(this->width()/2, this->height()/2);
}

void GObject::center(int16_t x, int16_t y){
  if(!isCreated()) create();
  this->align(ALIGN_CENTER, x - this->_par->width()/2, y - this->_par->height()/2);
}

void GObject::center(Point p) {
  this->center( p.x , p.y); 
}

Point GObject::center() {
  if(!isCreated()) create();
  return this->position() + Point( this->width()/2, this->height()/2 );
}

void GObject::position(int16_t x, int16_t y){
  if(!isCreated()) create();
  // Serial.printf("(position) x,y : %d,%d\n", x, y);
  this->align(ALIGN_IN_TOP_LEFT, x, y);
}

void GObject::position(Point p){
  this->position(p.x, p.y);
}

Point GObject::position() {
  if(!isCreated()) create();
  Point pos;
  pos.x = lv_obj_get_x(this->obj);
  pos.y = lv_obj_get_y(this->obj);
  return pos;
}

void  GObject::pos(int16_t x, int16_t y){
  if(!isCreated()) create();
  lv_obj_set_pos(this->obj, x,y);
}

int16_t GObject::posX(){
  if(!isCreated()) create();
  return lv_obj_get_x(this->obj);
}

int16_t GObject::posY(){
  if(!isCreated()) create();
  return lv_obj_get_y(this->obj);
}

void GObject::posX(int16_t x){
  if(!isCreated()) create();
  lv_obj_set_x(this->obj,x);
}

void GObject::posY(int16_t y){
  if(!isCreated()) create();
  lv_obj_set_y(this->obj,y);
}

area_t GObject::coordinate(){
  if(!isCreated()) create();
  return this->obj->coords;
}

int16_t GObject::leftX() {    // left x
  if(!isCreated()) create();
  return lv_obj_get_x(this->obj);
}

int16_t GObject::rightX() {   // right x
  if(!isCreated()) create();
  return lv_obj_get_x(this->obj) + lv_obj_get_width(this->obj);
}

int16_t GObject::topY() {     // top y
  if(!isCreated()) create();
  return lv_obj_get_y(this->obj);
}

int16_t GObject::bottomY() {  // bottom y
  if(!isCreated()) create();
  return lv_obj_get_y(this->obj) + lv_obj_get_height(this->obj);
}

void GObject::top(bool enable){
  if(!isCreated()) create();
  lv_obj_set_top(this->obj, enable);
}

bool GObject::top(){
  if(!isCreated()) create();
  return lv_obj_get_top(this->obj);
}

void GObject::toForeground() {
  if(!isCreated()) create();
  lv_obj_move_foreground(this->obj);
}

void GObject::toBackground() {
  if(!isCreated()) create();
  lv_obj_move_background(this->obj);
}

void GObject::size(uint16_t w, uint16_t h){
  if(!isCreated()) create();
  lv_obj_set_size(this->obj, w, h);
  this->realign();
}

void GObject::size(GWidget& widget){
  this->size(&widget);
}

void GObject::size(GWidget* widget) {
  if(!isCreated()) create();
  lv_obj_set_size(this->obj, widget->width(), widget->height());
  this->realign();
}

void  GObject::size_fit(GWidget& widget){
  this->size_fit(&widget);
}

void  GObject::size_fit(GWidget* widget){
  if(!isCreated()) create();
  lv_obj_set_size(this->obj, widget->width_fit(), widget->height_fit());
  this->realign();
}

uint16_t GObject::width() {
  if(!isCreated()) create();
  return lv_obj_get_width(this->obj);
}

uint16_t GObject::height() {
  if(!isCreated()) create();
  return lv_obj_get_height(this->obj);
}

void GObject::width(uint16_t w){
  if(!isCreated()) create();
  lv_obj_set_width(this->obj, w);
  this->realign();
}

void GObject::width(GWidget& widget){
  if(!widget.isCreated()) return;

  this->width(widget.width());
}

void GObject::width(GWidget* widget){
  if(!widget->isCreated()) return;

  this->width(widget->width());
}

uint16_t GObject::width_fit(){
  if(!isCreated()) create();
  return lv_obj_get_width_fit(this->obj);
}

void GObject::height(uint16_t h){
  if(!isCreated()) create();
  lv_obj_set_height(this->obj, h);
  this->realign();
}

void GObject::height(GWidget& widget){
  if(!widget.isCreated()) return;

  this->height(widget.height());
}

void GObject::height(GWidget* widget){
  if(!widget->isCreated()) return;

  this->height(widget->height());
}

uint16_t GObject::height_fit(){
  if(!isCreated()) create();
  return lv_obj_get_height_fit(this->obj);
}

void GObject::createLocalStyle(style_t *base_style){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()){
    this->_style = (GStyle*) esp32_malloc(sizeof(GStyle));
    LV_ASSERT_MEM(this->_style);
    if(this->_style != NULL) {
      // if( base_style == NULL) {
      //   this->_style->alloc((style_t*)lv_obj_get_style(this->obj));
      // }else{
        this->_style->alloc(base_style);
      // }
      lv_obj_set_style(obj, this->_style->raw());

      this->_has_localstyle = true;
      this->_is_linkstyle = false;
    }
  }
}

void GObject::freeLocalStyle(){
  if(!isCreated()) create();
  if(this->hasLocalStyle()){
    if(this->_style != NULL) {
      this->_style->release();
      free(this->_style);
      this->_style = NULL;
    }
    this->_has_localstyle = false;
  }else
  if(this->_is_linkstyle){
    this->_is_linkstyle = false;
  }
}

void GObject::copyStyle(style_t * p_style) {
  if( p_style == NULL) return;

  if(!isCreated()) create();
  if(!hasLocalStyle()) createLocalStyle(p_style);
  else this->_style->copy(p_style); 
  lv_obj_set_style( this->obj, this->_style->raw());

  this->_is_linkstyle = false;
}

void GObject::copyStyle(style_t &style) {
  this->copyStyle((style_t *)&style);
}

void GObject::copyStyle(GStyle *style){
  if(style == NULL) return;
  this->copyStyle(style->raw());
}

void GObject::copyStyle(GStyle &style) {
  this->copyStyle(style.raw());
}

void GObject::copyStyle(GWidget &object){
  this->copyStyle(object.getStyle());
}

void GObject::copyStyle(GWidget *object){
  if(object != NULL )  this->copyStyle(object->getStyle());
}

void GObject::linkStyle(GStyle *style){
  if(!isCreated()) create();
  if(hasLocalStyle()) freeLocalStyle();
  this->_style = style;
  lv_obj_set_style( this->obj, this->_style->raw());
  this->_is_linkstyle = true;
}

void GObject::linkStyle(GStyle &style){
  this->linkStyle(&style);
}

void GObject::linkStyle(GWidget &object) {
  this->linkStyle((GWidget*) &object);
}

void GObject::linkStyle(GWidget *object) {
  if(object == NULL || !object->isCreated() ) return;

  GStyle* style = object->getStyle();
  if(style == NULL) return;

  this->linkStyle((GStyle*)style );
}

// style_t* GObject::getStyle(){
//   if(!isCreated()) create();
//   return (style_t*) lv_obj_get_style( this->obj);
// }

GStyle* GObject::getStyle(){
  if(!isCreated()) create();
  return this->_style;
}

void GObject::color(uint16_t color){
  if(this->_is_linkstyle) return;  //ไม่ให้มีการแก้ไขได้ หากเป็น linkStyle

  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  // Serial.println("GObject::color(..)");
  this->_style->color(color, color, 255);
  this->refreshStyle();
}

void GObject::color(uint16_t color,uint16_t gradian_color){
  if(this->_is_linkstyle) return;

  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->color(color, gradian_color, 255);
  this->refreshStyle();
}

uint16_t GObject::color(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->color();
}

void  GObject::grad_color(uint16_t gradian_color){
  if(this->_is_linkstyle) return;  //ไม่ให้มีการแก้ไขได้ หากเป็น linkStyle

  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->grad_color(gradian_color);
  this->refreshStyle();
}

uint16_t GObject::grad_color(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->grad_color();
}

void GObject::grad_dir(grad_dir_t type){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->grad_dir(type);
  this->refreshStyle();
}

grad_dir_t GObject::grad_dir(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->grad_dir();
}

void  GObject::main_color_stop(uint8_t stop){ // 0..255
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->main_color_stop(stop);
  this->refreshStyle();
}

uint8_t GObject::main_color_stop() {
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->main_color_stop();
}

void GObject::grad_color_stop(uint8_t stop){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->grad_color_stop(stop);
  this->refreshStyle();
}

uint8_t GObject::grad_color_stop(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->grad_color_stop();
}

void GObject::opa(uint8_t opa){
  if(this->_is_linkstyle) return;

  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->opa(opa);
  this->refreshStyle();
}

uint8_t GObject::opa(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->opa();
}

// ทำให้ child ทุก child ก้ปรับ opa ตาม แม่
void GObject::opa_scalable(bool enable) {
  if(!isCreated()) create();
  lv_obj_set_opa_scale_enable( this->obj, enable);
}

bool GObject::opa_scalable() {
  if(!isCreated()) create();
  return lv_obj_get_opa_scale_enable( this->obj);
}

void GObject::opa_scale(uint8_t opa_scale) {
  if(!isCreated()) create();
  lv_obj_set_opa_scale(this->obj, opa_scale);
}

uint8_t GObject::opa_scale() {
  if(!isCreated()) create();
  return lv_obj_get_opa_scale(this->obj);
}

void GObject::padding(int16_t pd) {
  if(this->_is_linkstyle) return;

  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->padding(pd);
  this->refreshStyle();
}

int16_t GObject::padding(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->padding();
}

void GObject::padding_inner(int16_t pd){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->padding_inner(pd);
  this->refreshStyle();
}

void GObject::padding_top(int16_t pd) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->padding_top(pd);
  this->refreshStyle();
}

void GObject::padding_bottom(int16_t pd) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->padding_bottom(pd);
  this->refreshStyle();
}

void GObject::padding_left(int16_t pd) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->padding_left(pd);
  this->refreshStyle();
}

void GObject::padding_right(int16_t pd) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->padding_right(pd);
  this->refreshStyle();
}

int16_t GObject::padding_inner(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->padding_inner();
}

int16_t GObject::padding_top(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->padding_top();
}

int16_t GObject::padding_bottom(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->padding_bottom();
}

int16_t GObject::padding_left(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->padding_left();
}

int16_t GObject::padding_right(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  return this->_style->padding_right();
}

void GObject::setCircle(bool enable){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->radius(LV_RADIUS_CIRCLE);
  this->refreshStyle();
}


void GObject::corner_radius(uint16_t r){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->radius(r);
  this->refreshStyle();
}

uint16_t GObject::corner_radius(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return  this->_style->radius();
}

void GObject::corner_mask(bool enable){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->corner_mask(enable);
  this->refreshStyle();
}

bool GObject::corner_mask(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->corner_mask();
}

void GObject::border(uint16_t width){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->border(width);
  this->refreshStyle();
}

void GObject::border(uint16_t width, color_t color){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->border(width, color);
  this->refreshStyle();
}

uint16_t GObject::border() {
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->border();
}

void GObject::border_color(uint16_t color){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->border_color(color);
  this->refreshStyle();
}

color_t GObject::border_color(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->border_color();
}

void GObject::border_part(border_part_t part_type){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->border_part(part_type);
  this->refreshStyle();
}

border_part_t GObject::border_part(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->border_part();
}

void GObject::border_opa(uint8_t opa){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->border_opa(opa);
  this->refreshStyle();
}

uint8_t GObject::border_opa(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->border_opa();
}

void GObject::shadow_color(uint16_t color) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->shadow_color(color);
  this->refreshStyle();
}

uint16_t GObject::shadow_color(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->shadow_color();
}

void GObject::shadow(uint16_t width) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->shadow(width);
  this->refreshStyle();
}

void GObject::shadow(uint16_t width, color_t color) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->shadow(width, color);
  this->refreshStyle();
}

uint16_t GObject::shadow(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->shadow();
}

void GObject::shadow_opa(uint8_t opa){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->shadow_opa(opa);
  this->refreshStyle();
}

uint8_t GObject::shadow_opa(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->shadow_opa();
}

void GObject::shadow_offset(point_t point){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->shadow_offset(point);
  this->refreshStyle();
}

void GObject::shadow_offset(int16_t offset_x, int16_t offset_y)
{
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  point_t offset = (point_t) {offset_x,offset_y};
  this->shadow_offset(offset);
  this->refreshStyle();
}

point_t GObject::shadow_offset(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->shadow_offset();
}

void GObject::shadow_spread(uint16_t spread){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->shadow_spread(spread);
  this->refreshStyle();
}

uint16_t GObject::shadow_spread(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->shadow_spread();
}

void GObject::line_width(uint16_t width, color_t color){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->line_width(width);
  this->_style->line_color(color);
  this->refreshStyle();
}

void GObject::line_width(uint16_t width){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->line_width(width);
  // lv_obj_set_style(obj, this->_style->raw());
  this->refreshStyle();
}

uint16_t GObject::line_width(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->line_width();
}

void GObject::line_color(uint16_t color16){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->line_color(color16);
  // lv_obj_set_style(obj, this->_style->raw());
  this->refreshStyle();
}

uint16_t GObject::line_color(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->line_color();
}

void GObject::line_opa(uint8_t opa){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->line_opa(opa);
  // lv_obj_set_style(obj, style->raw());
  this->refreshStyle();
}

uint8_t GObject::line_opa(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->line_opa();
}

void GObject::line_rounded(bool enable){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->line_rounded(enable);
  // lv_obj_set_style(obj, style->raw());
  this->refreshStyle();
}

bool GObject::line_rounded(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->line_rounded();
}


void GObject::image_color(uint16_t color) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->image_color(color);
  // lv_obj_set_style(obj, style->raw());
  this->refreshStyle();
}

uint16_t GObject::image_color(){
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->image_color();
}

void  GObject::image_intense(uint8_t opa) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->image_intense(opa);
  // lv_obj_set_style(obj, style->raw());
  this->refreshStyle();
}

uint8_t GObject::image_intense() {
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->image_intense();
}


void  GObject::image_opa(uint8_t opa) {
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->image_opa(opa);
  // lv_obj_set_style(obj, style->raw());
  this->refreshStyle();
}

uint8_t GObject::image_opa() {
  if(!this->isCreated()) this->create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->image_opa();
}

void GObject::font(font_t &font){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->font(font);
  this->refreshStyle();    // ต้องมี ไม่งั้นแสดงผิดพลาดแสดงไม่ครบ ..เพราะไม่ได้ update ?
}

void GObject::font(font_t &font, color_t color){
  if(this->_is_linkstyle) return;
  
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->font(font);
  this->_style->text_color(color);
  this->refreshStyle();    // ต้องมี ไม่งั้นแสดงผิดพลาดแสดงไม่ครบ ..เพราะไม่ได้ update ?
}


const font_t* GObject::font(){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->font();
}

int16_t GObject::font_height(){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->font_height();
}

void GObject::text_color(uint16_t color){
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->text_color(color);
  this->refreshStyle();
}

uint16_t GObject::text_color(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->text_color();
}

void  GObject::text_opa(uint8_t opa){
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->text_opa(opa);
  this->refreshStyle();
}

uint8_t  GObject::text_opa(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->text_opa();
}

void GObject::text_sel_color(uint16_t color16) {
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();

  this->_style->text_sel_color(color16);
  this->refreshStyle();
}

uint16_t GObject::text_sel_color(){
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->text_sel_color();
}


void GObject::text_line_space(int16_t line_space) {
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->text_line_space(line_space);
  this->refreshStyle();
}

int16_t GObject::text_line_space() {
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->text_line_space();
}

void GObject::text_letter_space(int16_t letter_space) {
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->text_letter_space(letter_space);
  this->refreshStyle();
}

int16_t GObject::text_letter_space() {
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->text_letter_space();
}

void GObject::text_blend_mode(uint8_t blend_mode) {
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->text_blend_mode(blend_mode);
  this->refreshStyle();
}

uint8_t GObject::text_blend_mode() {
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->text_blend_mode();
}

void GObject::underline(bool enable) {
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->underline(enable);
  this->refreshStyle();
}

bool GObject::underline() {
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->underline();
}

void GObject::strikethrough(bool enable) {
  if(this->_is_linkstyle) return;
  
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  this->_style->strikethrough(enable);
  this->refreshStyle();
}

bool GObject::strikethrough() {
  if(!isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return this->_style->strikethrough();
}


void GObject::clickable(bool enable){
  if(!isCreated()) create();
  lv_obj_set_click(this->obj, !!enable);
}

bool GObject::clickable(){
  if(!isCreated()) create();
  return lv_obj_get_click(this->obj);
}

void GObject::dragable(bool enable){
  if(!isCreated()) create();
  lv_obj_set_drag(this->obj, !!enable);
}

bool GObject::dragable(){
  if(!isCreated()) create();
  return lv_obj_get_drag(this->obj);
}

void GObject::drag_throw(bool enable){
  if(!isCreated()) create();
  lv_obj_set_drag_throw(this->obj, !!enable);
}

bool GObject::drag_throw() {
  if(!isCreated()) create();
  return lv_obj_get_drag_throw(this->obj);
}

void GObject::drag_direction(lv_drag_dir_t drag_direction){
  if(!isCreated()) create();
  lv_obj_set_drag_dir(this->obj, drag_direction );
}

drag_dir_t GObject::drag_direction(){
  if(!isCreated()) create();
  return lv_obj_get_drag_dir(this->obj );
}

void GObject::drag_parent(bool enable){
  if(!isCreated()) create();
  this->obj->parent_event = 1;
  lv_obj_set_drag_parent(this->obj, true);
}

bool GObject::drag_parent(){
  if(!isCreated()) create();
  return lv_obj_get_drag_parent(this->obj);
}


void GObject::gesture(bool enable) {
  if( this->obj == lv_scr_act()) return;

  if(!isCreated()) create();
  this->gesture_parent(!enable);
}

bool GObject::gesture(){
  if(!isCreated()) create();
  return !this->gesture_parent();
}

void GObject::gesture_parent(bool enable) {
  if(!isCreated()) create();
  lv_obj_set_gesture_parent(this->obj, enable);
}

bool GObject::gesture_parent() {
  if(!isCreated()) create();
  return lv_obj_get_gesture_parent(this->obj);
}

gesture_dir_t GObject::touch_gesture() {
  if(!isCreated()) create();
  if(!this->gesture()) return GESTURE_DIR_NONE;
  return touch_gest();
}

void GObject::finish_drag() {
  if(!isCreated()) create();
  fin_drag();
}

point_t GObject::touch_point(bool absolute){
  if( absolute) return touch_pt();  // absolute touch point
  point_t tp = touch_pt();;
  tp.x -= this->obj->coords.x1;
  tp.y -= this->obj->coords.y1;
  return tp; // relative touch point
}

point_t GObject::touch_vector(){
  return touch_vect();
}

void GObject::page_glue(bool enable){
  if(!isCreated()) create();
  lv_page_glue_obj(this->obj, enable);
}

void GObject::hidden(bool enable) {
  if(!isCreated()) create();
  lv_obj_set_hidden(this->obj, !!enable);
}

bool GObject::hidden(){
  return lv_obj_get_hidden(this->obj);
}

void GObject::protect(protect_t type, bool enable){
  if(!isCreated()) create();
  if( enable) lv_obj_set_protect  (this->obj, type);
  else        lv_obj_clear_protect(this->obj, type);
}

bool GObject::isProtected(protect_t type){
  if(!isCreated()) create();
  return lv_obj_is_protected(this->obj, type);
}


// bool GObject::capture(const char* filename){
//   if(!isCreated()) create();

//   if(!NVS.exists("CAPTURE")){
//     NVS.setInt("CAPTURE", (uint16_t) 0 );
//   }

//   if( filename == NULL) {
//     uint16_t capture_no = (uint16_t) NVS.getInt("CAPTURE");
//     char file_name[255];
//     sprintf(file_name, "SD://TridentTD/captures/capture_%05d.png", ++capture_no);
//     // if( lv_snapshot(&this->Lcd, file_name, lv_scr_act()) ){
//     // if( lv_obj_snapshot(this->obj, file_name ) ) {
//     if( lv_obj_snapshot(lv_scr_act(), file_name ) ) {
//       NVS.setInt("CAPTURE", (uint16_t) capture_no);
//     }
//   }else{
//     // lv_obj_snapshot(this->obj, filename );
//     lv_obj_snapshot(lv_scr_act(), filename );
//   }
// }


bool GObject::event_send(event_t event, const void * event_data) {
  if(!isCreated()) create();
  return (LV_RES_OK == lv_event_send(this->obj,  event,  event_data) );
}

const void *GObject::event_data(){
  if(!isCreated()) create();
  return lv_event_get_data();
}

void GObject::parent_event(bool enable){
  if(!isCreated()) create();
  lv_obj_set_parent_event(this->obj, enable);
}

//---------------------------------
void main_event_cb(lv_obj_t *obj, lv_event_t event){
  screen_timer_update();

  GObject* pObj = (GObject*) lv_obj_get_user_data(obj);
  if(pObj == NULL) return;

  event_cb_t event_cb = pObj->event_cb();
  if(event_cb !=NULL) { 
    event_cb(pObj, event);
  }
  // else {
    switch(event) {
      case LV_EVENT_CLICKED :
        if( pObj->fn_onclicked) pObj->fn_onclicked(pObj); 
        break;
      case LV_EVENT_PRESSED:
        if( pObj->fn_onpressed) pObj->fn_onpressed(pObj); 
        break;
      case LV_EVENT_PRESSING:
        if( pObj->fn_onpressing) pObj->fn_onpressing(pObj); 
        break;
      case LV_EVENT_RELEASED:
        if( pObj->fn_onreleased) pObj->fn_onreleased(pObj); 
        break;
      case LV_EVENT_LONG_PRESSED:
        if( pObj->fn_onlongpressed) pObj->fn_onlongpressed(pObj); 
        break;
      case LV_EVENT_LONG_PRESSED_REPEAT:
        if( pObj->fn_onlongpressed_repeat) pObj->fn_onlongpressed_repeat(pObj); 
        break;
      case LV_EVENT_VALUE_CHANGED:
        if( pObj->fn_onvaluechanged) pObj->fn_onvaluechanged(pObj); 
        break;
      case LV_EVENT_DELETE:
        if( pObj->fn_ondelete) pObj->fn_ondelete(pObj); 
        break;
      case LV_EVENT_GESTURE:
        if( pObj->isType("lv_list") ) { Serial.println("GList LV_EVENT_GESTURE"); break;}
        if( pObj->fn_ongesture) pObj->fn_ongesture(pObj); 
        break;
    }
  // }
}



//------------------------

extern "C" {
#include "kbd/lv_keyboard.h"
}

static lv_obj_t* kb;  // สำหรับสร้าง kb
// void kbd_create(GWidget* widget, kbd_lang_t lang2nd = KBD_THAI);
// void kbd_delete();
void kbd_set_apply_cb(lv_apply_cb_t apply_cb);
GWidget* kbd_get_src();
void* kbd_user_data();
void kbd_user_data(void* user_data);

void kbd_create(GWidget* widget, kbd_lang_t lang2nd){
  String widget_type = widget->type();
  if(widget_type != "GLabel" &&  widget_type != "GTextArea") return;

  // MEM_CHECK;
  kbd_delete();  // เรียกก่อน เผื่อลบของเดิมที่เคยประกาศไป
  kb = lv_keyboard_create(lv_scr_act(), NULL);
  lv_keyboard_set_src(kb, widget->obj, NULL);
  lv_keyboard_set_apply_cb(kb, kbd_delete);
  lv_keyboard_set_language2nd(kb, lang2nd);
  // Serial.println("edit mode start");
  // MEM_CHECK;
}

void kbd_delete(){
  if(kb != NULL){
#if BLYNKGO_DEBUG 
    Serial.println("kbd_delete");
#endif    
    lv_obj_del(kb);
    kb = NULL;
    MEM_CHECK;
  }
}

void kbd_set_apply_cb(lv_apply_cb_t apply_cb){
  lv_keyboard_set_apply_cb(kb, apply_cb);
}

GWidget* kbd_get_src(){
  lv_obj_t* src = lv_keyboard_get_src(kb);
  GWidget* pWidget = (GWidget*) lv_obj_get_user_data(src);
  return pWidget;
}

void* kbd_user_data(){
  return lv_keyboard_get_user_data(kb);
}

void kbd_user_data(void* user_data){
  lv_keyboard_set_user_data(kb, user_data);
}
