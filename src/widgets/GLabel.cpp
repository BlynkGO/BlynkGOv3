/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GLabel.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GLabel.h"
#include "GPage.h"
#include "GContainer.h"

#define LV_OBJX_NAME  "GLabel" 

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
    Serial.println("[GLabel] signal cleanup");
#endif
    GLabel* pLabel = (GLabel*) lv_obj_get_user_data(obj);
    if(pLabel->hasLocalStyle())  pLabel->freeLocalStyle();  // หาก มี localStyle จะให้ลบทิ้งเสีย
    // *pLabel = "";
    // pLabel->clear();
    // char* buf = (char*)pLabel->wbuffer();
    // if(buf != NULL) free(buf);

    //pLabel->freeBuffer();   // มีปรรทัดนี้ ทำให้ ตายกลางทางได้
    pLabel->_created = false;
  }
  return LV_RES_OK;
}

//---------------------------------------------

lv_obj_t* lv_glabel_create(lv_obj_t* par, lv_obj_t* copy);
void      lv_label_set_rolling(lv_obj_t* label, bool en);
void      lv_label_set_rolling_width(lv_obj_t* label, uint16_t w);
bool      lv_label_get_rolling(lv_obj_t* label);
uint16_t  lv_label_get_rolling_width(lv_obj_t* label);
static void glabel_event_cb(lv_obj_t* label, lv_event_t event);

//---------------------------------------------

GLabel::GLabel(GWidget& parent)                   : String("")  , GObject(&parent) {}
GLabel::GLabel(GWidget* parent)                   : String("")  , GObject(parent)  {}
GLabel::GLabel(const char *cstr, GWidget& parent) : String(cstr), GObject(&parent) {}
GLabel::GLabel(const String &str, GWidget& parent): String(str) , GObject(&parent) {}
GLabel::GLabel(const char *cstr, GWidget* parent): String(cstr), GObject(parent)  {}
GLabel::GLabel(const String &str, GWidget* parent): String(str) , GObject(parent)  {}
GLabel::~GLabel(){ this->del(); }


void kbd_set_apply_cb(lv_apply_cb_t apply_cb);
GWidget* kbd_get_src();
void* kbd_user_data();
void kbd_user_data(void* user_data);

void kbd_apply_for_glabel(){
#if BLYNKGO_DEBUG 
  Serial.println("[GLabel] kbd_apply");
#endif
  // GWidget* widget = kbd_get_src();
  GLabel* pLabel = (GLabel*) kbd_user_data();
  pLabel->event_send(EVENT_VALUE_CHANGED);

  kbd_delete();
  MEM_CHECK;
}

void lb_editable_clicked(GWidget* object) {
  GLabel *pGLabel = (GLabel*) object;

#if BLYNKGO_USE_KBD
  if(pGLabel->editable()) {
    // Serial.println("GLabel Editable Clicked");
#if defined(LAOS_TEXT)
    kbd_create(pGLabel, KBD_LAOS);
#elif defined(VIETNAM_TEXT)
    // kbd_create(pGLabel, KBD_VIETNAM);
    kbd_create(pGLabel, KBD_THAI); 
#else
    kbd_create(pGLabel, KBD_THAI); 
#endif

    kbd_user_data(pGLabel);
    kbd_set_apply_cb(kbd_apply_for_glabel);
  }
#endif //BLYNKGO_USE_KBD

}


void GLabel::create(){
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  // Serial.println("GLabel Create...");
  if(!this->isCreated()) {
    this->obj = lv_glabel_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created  = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, glabel_event_cb);//main_event_cb);

      this->clickable(false);
      this->onClicked(lb_editable_clicked);
      // lv_label_set_static_text(this->obj, this->c_str());
      lv_label_set_text(this->obj, this->c_str());

      String par_type = this->_par->type();
      if( this->_par->isType("lv_page") || this->_par->isType("lv_cont") ||
          par_type == "GTab"  || par_type == "GPage" || par_type == "GContainer" ) {
        // Serial.println("label on lv_page or lv_cont");
        // if(par_type == "GPage") {
        //   GPage *par_page = (GPage*) this->_par;
        //   if( par_page->layout() == LAYOUT_NONE) {
        //     this->align(ALIGN_CENTER);
        //     this->auto_realign(true);
        //   }
        // }else
        // if(par_type == "GContainer") {
        //   GContainer *par_cont = (GContainer*) this->_par;
        //   if( par_cont->layout() == LAYOUT_NONE) {
        //     this->align(ALIGN_CENTER);
        //     this->auto_realign(true);
        //   }
        // }
        //else
        // if( this->_par->isType("lv_page")  ) {
        //   lv_layout_t layout = lv_page_get_scrl_layout(this->_par->obj);
        //   if( layout == LAYOUT_NONE) {
        //     this->align(ALIGN_CENTER);
        //     this->auto_realign(true);
        //   }
        // }else
        // if(this->_par->isType("lv_cont")  ) {
        //   lv_layout_t layout = lv_cont_get_layout(this->_par->obj);
        //   if( layout == LAYOUT_NONE) {
        //     this->align(ALIGN_CENTER);
        //     this->auto_realign(true);
        //   }
        // }
      }else{
        this->align(ALIGN_CENTER);
        this->auto_realign(true);        
      }
      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);
    }
  }
}

void GLabel::create(GObject *parent){
  this->_par = parent;
  create();
}

void GLabel::create(GWidget& parent) {
  this->_par = &parent;
  create();
}


void GLabel::text(String txt){
  if(!this->isCreated()) create();
  
  if(txt.c_str()) {
    copy(txt.c_str(), txt.length());
  } else {
    String::invalidate();
  }

  if(this->isCreated()) {
    /* เปลี่ยนมาใช้ static_text จะได้ไม่ต้องเปลือง mem 2 รอบ ใช้การ link ออกมาที่ของ String แทน */
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str()); // เปลี่ยนมาเป็น แบบไม่ static text เนื่องจากไปมีปัญหาเวลา จองแบบ pointer
  }
  this->realign();
}

void  GLabel::raw_text(String txt){
  if(!this->isCreated()) create();
  lv_label_set_text(this->obj, txt.c_str()); 
}


String GLabel::text(){
  if(!this->isCreated()) create();
  return lv_label_get_text(this->obj);
}

#include <stdio.h>// vsnprintf 
String GLabel::printf(const char* format, ...) {
    char loc_buf[256];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    size_t len = vsnprintf(NULL, 0, format, arg);
    va_end(copy);
    if(len >= sizeof(loc_buf)){
        temp = new char[len+1];
        if(temp == NULL) {
            return "";
        }
    }
    len = vsnprintf(temp, len+1, format, arg);

    String buf = temp;

    va_end(arg);
    if(len >= sizeof(loc_buf)){
        delete[] temp;
    }
    return buf;
}

int GLabel::split(String source, String delimiter, String** str_array) {
  if(*str_array != NULL) {
    delete[] *str_array;
    *str_array = NULL;
  }

  if(source.length() == 0) {
    *str_array = new String[1];
    (*str_array)[0] = "";
    return 0;
  }

  String input = source; 
  int token_size = 0;

  char *pChar = strtok( (char*)input.c_str(), (char*)delimiter.c_str());
  while ( pChar != NULL ) {
    pChar = strtok( NULL, (char*)delimiter.c_str());
    ++token_size;
  }
  
  if(token_size == 0) {
    *str_array = new String[1]; (*str_array)[0] = input;
    return 1;
  }

  *str_array = new String[token_size];
  input = source; 
  token_size = 0;

  pChar = strtok( (char*) input.c_str(), (char*)delimiter.c_str());
  while ( pChar != NULL ) {
    (*str_array)[token_size] = String(pChar);
    pChar = strtok( NULL, (char*)delimiter.c_str());
    ++token_size;
  }
  return token_size;
}

GLabel& GLabel::operator=(const String &label) {
  if(!this->isCreated()) create();

  if(label.c_str()) {
    copy(label.c_str(), label.length());
  } else {
    String::invalidate();
  }

  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return *this;
}

GLabel& GLabel::operator=(const GLabel gLabel) {
  if(!this->isCreated()) create();

  if(gLabel.c_str()) {
    copy(gLabel.c_str(), gLabel.length());
  } else {
    String::invalidate();
  }

  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return *this;
}

#if LABEL_USE_TEXTAREA
GLabel& GLabel::operator=(GTextArea& gTextArea) {
  this->text(gTextArea.text());
  return *this;
}
#endif

GLabel& GLabel::operator=(const char* cstr) {
  if(!this->isCreated()) create();

  if(cstr) {
    copy(cstr, strlen(cstr));
  } else {
    String::invalidate(); 
  }

  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return *this;
}

GLabel& GLabel::operator =(int num){
  if(!this->isCreated()) create();

  String num_str = String(num);
  if(num_str.length()) {
    copy(num_str.c_str(), num_str.length());
  } else {
    String::invalidate(); 
  }

  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return *this;
}

GLabel&  GLabel::operator+=(const String label) {
  if(!this->isCreated()) create();

  concat(label);

  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel& GLabel::operator +=(const char *cstr)
{
  if(!this->isCreated()) create();

  concat(cstr);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(char c)
{
  if(!this->isCreated()) create();

  concat(c);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(unsigned char num)
{
  if(!this->isCreated()) create();

  concat(num);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(int num)
{
  if(!this->isCreated()) create();

  concat(num);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(unsigned int num)
{
  if(!this->isCreated()) create();

  concat(num);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(long num)
{
  if(!this->isCreated()) create();

  concat(num);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(unsigned long num)
{
  if(!this->isCreated()) create();

  concat(num);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(float num)
{
  if(!this->isCreated()) create();

  concat(num);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator +=(double num)
{
  if(!this->isCreated()) create();

  concat(num);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

GLabel & GLabel::operator += (const __FlashStringHelper *str)
{
  if(!this->isCreated()) create();

  concat(str);
  if(this->isCreated()) {
    // lv_label_set_static_text(this->obj, this->c_str());
    lv_label_set_text(this->obj, this->c_str());
  }
  return (*this);
}

void GLabel::font(font_t &font){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::font(font);
}

void GLabel::font(font_t &font, color_t color){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::font(font, color);
}

const font_t* GLabel::font(){
  if(!this->isCreated()) create();
  return GObject::font();  
}

void GLabel::color(color_t color16){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::text_color(color16);
}

color_t GLabel::color() {
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return GObject::text_color();
}

void GLabel::sel_color(color_t color16){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::text_sel_color(color16);
}

color_t GLabel::sel_color(){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return GObject::text_sel_color();
}

void GLabel::opa(uint8_t opa){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::text_opa(opa);
}

uint8_t GLabel::opa(){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return GObject::text_opa();
}

void GLabel::body_draw(bool enable) {
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  lv_label_set_body_draw(this->obj, enable);
}

bool GLabel::body_draw() {
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  return lv_label_get_body_draw(this->obj);
}

void GLabel::body_color(color_t color) {
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::color(color);
}

void GLabel::body_color(color_t color, color_t grad_color) {
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::color(color, grad_color);
}

void GLabel::body_opa(uint8_t opa){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) createLocalStyle();
  GObject::opa(opa);
}

void GLabel::editable(bool enable){
  if(!this->isCreated()) create();
  glabel_ext_t * ext = (glabel_ext_t *) this->ext_attr();

  ext->editable = enable;
  this->clickable(enable);
}

bool GLabel::editable(){
  if(!this->isCreated()) create();
  glabel_ext_t * ext = (glabel_ext_t *) this->ext_attr();
  return ext->editable;
}

void GLabel::wrapable(bool enable){
  if(!this->isCreated()) create();
  if(enable) {
    if(this->_par->isType("lv_page")) {
      lv_label_set_long_mode(this->obj, LV_LABEL_LONG_BREAK);
      lv_obj_set_width(this->obj, lv_page_get_fit_width(this->_par->obj));
    }
    else if(this->_par->isType("lv_win")) {
      lv_label_set_long_mode(this->obj, LV_LABEL_LONG_BREAK);
      lv_win_ext_t* win_ext = (lv_win_ext_t*)this->_par->ext_attr();
      lv_obj_set_width(this->obj, lv_page_get_fit_width(win_ext->page));
    }
    else{
      lv_label_set_long_mode(this->obj, LV_LABEL_LONG_SROLL);
      this->auto_realign(true);
    }
  }else{
    lv_label_set_long_mode(this->obj, LV_LABEL_LONG_SROLL);
    this->auto_realign(true);
  }
}

void GLabel::long_mode(long_mode_t long_mode){
  if(!this->isCreated()) create();
  lv_label_set_long_mode(this->obj, long_mode);
}

void GLabel::long_mode(long_mode_t long_mode, uint16_t width, uint16_t speed_pps){
  if(!this->isCreated()) create();
  lv_label_set_long_mode(this->obj, long_mode);
  this->width(width);
  lv_label_set_anim_speed(this->obj, speed_pps);
}

long_mode_t GLabel::long_mode(){
  if(!this->isCreated()) create();
  return (long_mode_t) lv_label_get_long_mode(this->obj);
}

void GLabel::text_align(label_align_t align){
  if(!this->isCreated()) create();
  lv_label_set_align(this->obj, align);
}

label_align_t GLabel::text_align(){
  if(!this->isCreated()) create();
  return lv_label_get_align(this->obj);
}

void GLabel::colorful(bool enable){
  if(!this->isCreated()) create();
  lv_label_set_recolor(this->obj, enable);
}

bool GLabel::colorful(){
  if(!this->isCreated()) create();
  return lv_label_get_recolor(this->obj);
}

void GLabel::rolling(bool enable, uint16_t rolling_width , uint16_t rolling_speed) {
  if(!this->isCreated()) create();
  if(enable){
    lv_label_set_rolling(this->obj, true);
    this->rolling_width(rolling_width);
    this->rolling_speed(rolling_speed);
  }else{
    lv_label_set_rolling(this->obj, false);
  }
}

bool GLabel::rolling(){
  if(!this->isCreated()) create();
  return lv_label_get_rolling(this->obj);
}

void GLabel::rolling_width(uint16_t rolling_width) {
  if(!this->isCreated()) create();
  lv_label_set_rolling_width(this->obj, rolling_width);

}

uint16_t GLabel::rolling_width() {
  if(!this->isCreated()) create();
  return lv_label_get_rolling_width(this->obj);
}

void GLabel::rolling_speed(uint16_t speed_pps){
  if(!this->isCreated()) create();
  lv_label_set_anim_speed(this->obj, speed_pps);
}

uint16_t GLabel::rolling_speed(){
  if(!this->isCreated()) create();
  return lv_label_get_anim_speed(this->obj);
}

point_t  GLabel::letter_position(uint16_t index){
  if(!this->isCreated()) create();
  point_t pos= (point_t){0,0};
  lv_label_get_letter_pos(this->obj, index, &pos);
  return pos;
}

uint16_t GLabel::letter_on(point_t pos){
  if(!this->isCreated()) create();
  return lv_label_get_letter_on(this->obj, &pos);
}

String GLabel::text_select(){
  if(!this->isCreated()) create();
  if(this->text_select_start() == NO_TXT_SEL || 
     this->text_select_end() == NO_TXT_SEL )
    return "";

  uint32_t start_pos = this->text_select_start();
  uint32_t end_pos = this->text_select_end();

  start_pos = lv_txt_encoded_get_byte_id(this->c_str(), start_pos); /*Convert to byte index instead of letter index*/
  end_pos   = lv_txt_encoded_get_byte_id(this->c_str(), end_pos);
  // Serial.printf("text_select : start %d ; end %d\n", start_pos, end_pos);
  char buf[end_pos-start_pos+1];
  char* inner_buf = (char*) this->c_str();
  memset(buf, 0, sizeof(buf));
  memcpy(buf, &inner_buf[start_pos], end_pos-start_pos);
  return buf;
}

uint16_t GLabel::encoded_length(){
  if(!this->isCreated()) create();
  // return this->letter_on((point_t) {this->width(),0});
  return lv_txt_get_encoded_length(this->c_str());
}

int GLabel::find(const String &txt) {
  return this->find(txt, 0);
}

int GLabel::find(const String &txt, unsigned int fromIndex) {
  if(!this->isCreated()) create();

  int from_byte_id = (fromIndex==0)? 0 : lv_txt_encoded_get_byte_id(this->c_str(), fromIndex);
  // return from_byte_id;
  int byte_id = this->indexOf(txt, from_byte_id);
  if(byte_id == -1) return -1;

  int char_id = lv_txt_encoded_get_char_id(this->c_str(), byte_id);
  return char_id;
}

void GLabel::freeBuffer(){
  String::changeBuffer(0);
}

String GLabel::text_select(uint16_t index_start, uint16_t index_end){
  if(!this->isCreated()) create();
  this->text_select_start(index_start);
  if(index_end < 0x7FFF) {
    this->text_select_end(index_end);
  }else{
    this->text_select_end(this->encoded_length());
  }
  return this->text_select();
}

void GLabel::text_select_start(uint16_t index){
  if(!this->isCreated()) create();
  lv_label_set_text_sel_start(this->obj, index);
}

uint16_t GLabel::text_select_start(){
  if(!this->isCreated()) create();
  return lv_label_get_text_sel_start(this->obj);
}

void GLabel::text_select_end(uint16_t index){
  if(!this->isCreated()) create();
  lv_label_set_text_sel_end(this->obj, index);
}

uint16_t GLabel::text_select_end(){
  if(!this->isCreated()) create();
  return lv_label_get_text_sel_end(this->obj);
}



void GLabel::insert(uint32_t index, const char* text) {
  if(!this->isCreated()) create();
  // lv_label_ins_text(this->obj, index, text.c_str()); // ใช้ไม่ได้สำหรับ label ที่ผ่านทาง static text

  // สำหรับ static text ต้อง insert เอง
  char label_txt[this->length()+1+ strlen(text)+1];
  memset(label_txt,0, strlen(label_txt));
  this->toCharArray(label_txt, this->length()+1);
  lv_txt_ins(label_txt, index, text);
  this->text( label_txt);
}

void GLabel::cut(uint32_t index, uint32_t count){
  if(!this->isCreated()) create();
  // lv_label_cut_text(this->obj, index, count); // ใช้ไม่ได้สำหรับ label ที่ผ่านทาง static text

  // สำหรับ static text ต้องตัดเอง
  char label_txt[this->length()+1];
  this->toCharArray(label_txt, this->length()+1);
  lv_txt_cut(label_txt, index, count);
  this->text( label_txt);
}

// void lb_event_cb(obj_t* obj, event_t event){
//   screen_timer_update();
//   GLabel* pGLabel = (GLabel*) lv_obj_get_user_data(obj);
//   if(pGLabel == NULL) return;
//   Serial.printf("[lb_event_cb] event : %d\n", event);
//   switch(event){
//     case LV_EVENT_CLICKED:
//       Serial.println("Clicked");
//       if(pGLabel->editable()) {
//         Serial.println("edit mode");
//         kbd = make_unique<GKeyboard>();
//         kbd->onApply([](){ kbd->del(); });
//         kbd->setSource(pGLabel);
//       }
//       break;
//   }
// }

//---------------------------------------------
// lv_glabel

static lv_design_cb_t ancestor_label_design;
static lv_signal_cb_t ancestor_label_signal;
static lv_design_res_t  lv_glabel_design(lv_obj_t * label, const lv_area_t * clip_area, lv_design_mode_t mode);
static lv_res_t         lv_glabel_signal(lv_obj_t* label, lv_signal_t sign, void* param);

#if LV_USE_ANIMATION
static void lv_label_set_offset_x(lv_obj_t * label, lv_coord_t x);
static void lv_label_set_offset_y(lv_obj_t * label, lv_coord_t y);
#endif


lv_obj_t* lv_glabel_create(lv_obj_t* par, lv_obj_t* copy){
  lv_obj_t* new_label = lv_label_create(par, copy);
  glabel_ext_t *ext = (glabel_ext_t *) lv_obj_allocate_ext_attr(new_label,sizeof(glabel_ext_t));

  if(ancestor_label_design == NULL) ancestor_label_design = lv_obj_get_design_cb(new_label);
  if(ancestor_label_signal == NULL) ancestor_label_signal = lv_obj_get_signal_cb(new_label);
    
  ext->rolling = false;
  ext->rolling_by_long_mode = false;
  ext->rolling_w = 0;

  lv_obj_set_design_cb(new_label, lv_glabel_design);
  lv_obj_set_signal_cb(new_label, lv_glabel_signal);
  
  if(copy != NULL){
    glabel_ext_t *copy_ext = (glabel_ext_t *) lv_obj_get_ext_attr(copy);
    ext->rolling      = copy_ext->rolling;
    ext->rolling_by_long_mode = copy_ext->rolling_by_long_mode;
    ext->rolling_w    = copy_ext->rolling_w;
  }
  lv_obj_invalidate(new_label);
  return  new_label;
}

void lv_label_set_rolling(lv_obj_t* label, bool en){
  glabel_ext_t *ext = (glabel_ext_t *) lv_obj_get_ext_attr(label);
  ext->rolling = en;
  if( ext->rolling == false) {
    ext->rolling_by_long_mode = false;
  }
  lv_obj_invalidate(label);
}

void lv_label_set_rolling_width(lv_obj_t* label, uint16_t w){
  glabel_ext_t *ext = (glabel_ext_t *) lv_obj_get_ext_attr(label);
  ext->rolling_w = w;
  lv_obj_invalidate(label);
}

bool lv_label_get_rolling(lv_obj_t* label){
  glabel_ext_t *ext = (glabel_ext_t *) lv_obj_get_ext_attr(label);
  return ext->rolling;
}

uint16_t lv_label_get_rolling_width(lv_obj_t* label){
  glabel_ext_t *ext = (glabel_ext_t *) lv_obj_get_ext_attr(label);
  return ext->rolling_w;
}

static lv_design_res_t lv_glabel_design(lv_obj_t * label, const lv_area_t * clip_area, lv_design_mode_t mode)
{
  glabel_ext_t *ext = (glabel_ext_t *) lv_obj_get_ext_attr(label);

  /* หากไม่ใช่ rolling หรือ ใช่ แต่ใช้ rolling_by_long_mode ให้วาดด้วย design เดิมของ label ปกติ*/
  if(!ext->rolling || (ext->rolling && ext->rolling_by_long_mode) ) {
    if(!ext->rolling) lv_label_set_long_mode(label, LV_LABEL_LONG_EXPAND);
    return ancestor_label_design(label, clip_area, mode);
  }

  /* วาดเฉพาะของ rolling mode */
  /* A label never covers an area */
  if(mode == LV_DESIGN_COVER_CHK)
      return LV_DESIGN_RES_NOT_COVER;
  else 
  if(mode == LV_DESIGN_DRAW_MAIN){
    if(ext->label.text == NULL) return LV_DESIGN_RES_OK;

    if(ext->rolling ) {
      const lv_style_t * style = lv_obj_get_style(label);
      const lv_font_t * font   = style->text.font;
      
      /*Calc. the height and longest line*/
      lv_point_t size; // ขนาดความยาว กว้าง ของข้อความที่แท้จริง
      lv_txt_flag_t flag = LV_TXT_FLAG_NONE;
      if(ext->label.recolor != 0) flag |= LV_TXT_FLAG_RECOLOR;
      if(ext->label.expand != 0)  flag |= LV_TXT_FLAG_EXPAND;
      lv_txt_get_size(&size, ext->label.text, font, style->text.letter_space, style->text.line_space, LV_COORD_MAX, flag);

      lv_coord_t rolling_w         = (ext->rolling_w>0)? ext->rolling_w : size.x; //lv_obj_get_width(label);  //ขนาดระยะกว้าง object ที่จะแสดงได้
    
      if( rolling_w < size.x ) {
        // Serial.println("glabel design cb [ rolling by long-mode design ]");
        lv_label_set_long_mode(label, LV_LABEL_LONG_SROLL_CIRC);
        lv_obj_set_width(label, rolling_w);
        ext->rolling_by_long_mode = true;
        return ancestor_label_design(label, clip_area, mode);
      }else{
        // Serial.println("glabel design cb [ rolling design ]");
        lv_area_t coords;
        const lv_style_t * style = lv_obj_get_style(label);
        lv_opa_t opa_scale       = lv_obj_get_opa_scale(label);
        lv_obj_set_width(label, rolling_w );

        /* วาดส่วนพื้นหลังด้วย หาก ext->label.body_draw มีการกำหนดเป็น 1 */
        lv_obj_get_coords(label, &coords);
        if(ext->label.body_draw) {
            lv_area_t bg;
            lv_obj_get_coords(label, &bg);
            bg.x1 -= style->body.padding.left;
            bg.x2 += style->body.padding.right;
            bg.y1 -= style->body.padding.top;
            bg.y2 += style->body.padding.bottom;

            lv_draw_rect(&bg, clip_area, style, lv_obj_get_opa_scale(label));
        }
        

        lv_label_align_t align = lv_label_get_align(label);

        lv_txt_flag_t flag = LV_TXT_FLAG_NONE;
        if(ext->label.recolor != 0) flag |= LV_TXT_FLAG_RECOLOR;
        if(ext->label.expand != 0)  flag |= LV_TXT_FLAG_EXPAND;
        if(align == LV_LABEL_ALIGN_CENTER) flag |= LV_TXT_FLAG_CENTER;
        if(align == LV_LABEL_ALIGN_RIGHT)  flag |= LV_TXT_FLAG_RIGHT;

        if(ext->label.align == LV_LABEL_ALIGN_CENTER || ext->label.align == LV_LABEL_ALIGN_RIGHT) {
          lv_point_t size;
          lv_txt_get_size(&size, ext->label.text, style->text.font, style->text.letter_space, style->text.line_space,
                          LV_COORD_MAX, flag);
          if(size.x > lv_obj_get_width(label)) {
              flag &= ~LV_TXT_FLAG_RIGHT;
              flag &= ~LV_TXT_FLAG_CENTER;
          }
        }
#if LV_LABEL_LONG_TXT_HINT
        lv_draw_label_hint_t * hint = &ext->label.hint;
        if(ext->label.long_mode == LV_LABEL_LONG_SROLL_CIRC || lv_obj_get_height(label) < LV_LABEL_HINT_HEIGHT_LIMIT)
            hint = NULL;

#else
        /*Just for compatibility*/
        lv_draw_label_hint_t * hint = NULL;
#endif
        lv_draw_label_txt_sel_t sel;

        sel.start = lv_label_get_text_sel_start(label);
        sel.end = lv_label_get_text_sel_end(label);
        lv_draw_label(&coords, clip_area, style, opa_scale, ext->label.text, flag, &ext->label.offset, &sel, hint, lv_obj_get_base_dir(label));
        
      }
    }
  }

  return LV_DESIGN_RES_OK;
}


#if LV_USE_ANIMATION
static void lv_label_set_offset_x(lv_obj_t * label, lv_coord_t x)
{
    glabel_ext_t * ext = (glabel_ext_t *)lv_obj_get_ext_attr(label);
    ext->label.offset.x        = x;
    lv_obj_invalidate(label);
}

static void lv_label_set_offset_y(lv_obj_t * label, lv_coord_t y)
{
    glabel_ext_t * ext = (glabel_ext_t *)lv_obj_get_ext_attr(label);
    ext->label.offset.y        = y;
    lv_obj_invalidate(label);
}
#endif

static lv_res_t lv_glabel_signal(lv_obj_t* label, lv_signal_t sign, void* param)
{
  lv_res_t res;
  glabel_ext_t *ext = (glabel_ext_t *) lv_obj_get_ext_attr(label);

  if(!ext->rolling )  return ancestor_label_signal(label, sign, param);
  if(sign != LV_SIGNAL_STYLE_CHG && sign != LV_SIGNAL_CORD_CHG) return ancestor_label_signal(label, sign, param);

  if(sign == LV_SIGNAL_STYLE_CHG || sign == LV_SIGNAL_CORD_CHG) {
    // Serial.println("rolling signal : cord chg / style chg");
    if(ext->label.text == NULL) return LV_RES_OK;
#if LV_LABEL_LONG_TXT_HINT
    ext->label.hint.line_start = -1; /*The hint is invalid if the text changes*/
#endif
    lv_coord_t rolling_w     = ext->rolling_w;//lv_obj_get_width(label);
    const lv_style_t * style = lv_obj_get_style(label);
    const lv_font_t * font   = style->text.font;
    /*Calc. the height and ongest line*/
    lv_point_t size;
    lv_txt_flag_t flag = LV_TXT_FLAG_NONE;
    if(ext->label.recolor != 0) flag |= LV_TXT_FLAG_RECOLOR;
    if(ext->label.expand != 0)  flag |= LV_TXT_FLAG_EXPAND;
    lv_txt_get_size(&size, ext->label.text, font, style->text.letter_space, style->text.line_space, LV_COORD_MAX, flag);


#if LV_USE_ANIMATION
    lv_label_align_t align = lv_label_get_align(label);

    lv_anim_t anim;
    anim.var      = label;
    anim.repeat   = 1;
    anim.playback = 0;
    anim.act_time = 0; //-(((lv_font_get_glyph_width(style->text.font, ' ', ' ') + style->text.letter_space) * 1000) / ext->label.anim_speed) * LV_LABEL_WAIT_CHAR_COUNT;
    anim.ready_cb       = NULL;
    anim.path_cb        = lv_anim_path_linear;
    anim.playback_pause = 0;
    anim.repeat_pause   = 0;

    bool hor_anim = false;
    if(size.x > rolling_w ) {
      // Serial.println(" size.x > rolling_w ");
      if(align == LV_LABEL_ALIGN_RIGHT) {
          anim.end    = 0;
          anim.start     = -size.x - lv_font_get_glyph_width(font, ' ', ' ') * LV_LABEL_WAIT_CHAR_COUNT;
      } else {
          anim.start    = 0;
          anim.end     = -size.x - lv_font_get_glyph_width(font, ' ', ' ') * LV_LABEL_WAIT_CHAR_COUNT;
      }

      anim.exec_cb = (lv_anim_exec_xcb_t)lv_label_set_offset_x;
      anim.time    = lv_anim_speed_to_time(ext->label.anim_speed, anim.start, anim.end);
      lv_anim_create(&anim);
      hor_anim = true;
    } else {
      // Serial.println(" size.x <= rolling_w ");

      /* กำหนด amin สำหรับ x เลื่อนจากขอบขวา ไป ขอบซ้าย */
      anim.start   = rolling_w + lv_font_get_glyph_width(font, ' ', ' ');
      anim.end     = -size.x;//  - lv_font_get_glyph_width(font, ' ', ' ') * LV_LABEL_WAIT_CHAR_COUNT;
      
      anim.exec_cb = (lv_anim_exec_xcb_t)lv_label_set_offset_x;
      anim.time    = lv_anim_speed_to_time(ext->label.anim_speed, anim.start, anim.end);
      lv_anim_create(&anim);
      hor_anim = true;
    }

#endif
  }
  
  return LV_RES_OK;
}


static void glabel_event_cb(lv_obj_t* label, lv_event_t event)
{

  GLabel* pLabel = (GLabel*) lv_obj_get_user_data(label);
  if( event == LV_EVENT_VALUE_CHANGED){
    pLabel->text(lv_label_get_text(label));
  }
  main_event_cb( pLabel->obj, event);
}
