/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GTextArea.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GTextArea.h"
// #include "GKeyboard.h"
#include "../config/blynkgo_config.h"



#if defined (LAOS_TEXT)
FONT_DECLARE(saysettha_15);
FONT_DECLARE(saysettha_25);
#elif defined (VIETNAM_TEXT)
FONT_DECLARE(arial_vietnam_20);
FONT_DECLARE(arial_vietnam_30);
#endif

#define LV_OBJX_NAME  "GTextArea" 

static lv_signal_cb_t ancestor_signal_cb;
static lv_signal_cb_t anc_ta_scrl_signal;

static lv_res_t GSignal_cb(lv_obj_t *obj, lv_signal_t sign, void* param)
{
  lv_res_t res;
  res = ancestor_signal_cb( obj, sign, param);
  if(res != LV_RES_OK ) return res;
  if(sign == LV_SIGNAL_GET_TYPE) 
    return lv_obj_handle_get_type_signal((lv_obj_type_t*)param, LV_OBJX_NAME);

  if(sign == LV_SIGNAL_CLEANUP) {
#if BLYNKGO_DEBUG 
    Serial.println("[GTextArea] signal cleanup");
#endif
    GTextArea* pTA = (GTextArea*) lv_obj_get_user_data(obj);

    if(pTA->hasLocalStyle()) { pTA->freeLocalStyle(); }
    pTA->_created = false;
  }
  return LV_RES_OK;
}

/* signal_cb สำหรับ scrl ของ textarea ให้สามารถ clickable== false แล้วไม่ให้กดคลิกได้ */
static lv_res_t ta_ext_scrl_signal(lv_obj_t * scrl, lv_signal_t sign, void * param)
{
  lv_res_t res = LV_RES_OK;
  if(sign == LV_SIGNAL_PRESSING || sign == LV_SIGNAL_PRESSED || sign == LV_SIGNAL_PRESS_LOST ||
     sign == LV_SIGNAL_RELEASED)
  {
    if(lv_obj_get_click(lv_obj_get_parent(scrl)))
      res = anc_ta_scrl_signal(scrl, sign, param);
  }else{
    res = anc_ta_scrl_signal(scrl, sign, param);
  }
  return res;
}


static void gtextarea_event_cb(lv_obj_t *obj, lv_event_t event);
// extern void kbd_delete();
// extern lv_obj_t* kb;

GTextArea::GTextArea(GWidget& parent) : GObject(&parent) {}
GTextArea::GTextArea(GWidget* parent) : GObject(parent)  {}
// GTextArea::GTextArea(const char *cstr, GWidget& parent) : _text(cstr), GObject(parent)  {}
// GTextArea::GTextArea(const String &str, GWidget& parent): _text(str) , GObject(parent)  {}
// GTextArea::GTextArea(const char *cstr, GWidget* parent) : _text(cstr), GObject(parent)  {}
// GTextArea::GTextArea(const String &str, GWidget* parent): _text(str) , GObject(parent)  {}
GTextArea::GTextArea(const String str) : _text(str), GObject(GScreen) {}
GTextArea::GTextArea(const String str, GWidget& parent): _text(str) , GObject(parent)  {}
GTextArea::GTextArea(const String str, GWidget* parent): _text(str) , GObject(parent)  {}

GTextArea::~GTextArea() { this->del(); }

void GTextArea::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_ta_create( this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created = true;

      GTextArea_ext_t* ext = (GTextArea_ext_t*) this->alloc_ext_attr(sizeof(GTextArea_ext_t));
        ext->default_kbd = true;

      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, gtextarea_event_cb);//this->TextArea_event_cb);

#if defined (LAOS_TEXT)
      this->font(saysettha_25, TFT_BLACK);
      // this->placeholder_font(saysettha_25, TFT_DARKGRAY);
#elif defined (VIETNAM_TEXT)
      this->font(arial_vietnam_30, TFT_BLACK);
#endif
      this->text(_text);
      this->placeholder("");
      this->oneline(true);
      this->cursor_type(CURSOR_LINE);
      this->width(this->_par->width());

      this->align(ALIGN_CENTER);

      if(ancestor_signal_cb == NULL) ancestor_signal_cb = this->signal_cb();
      this->signal_cb(GSignal_cb);

      /* signal_cb สำหรับ scrl ของ textarea */
      lv_obj_t* ta_scrl = lv_page_get_scrl(this->obj);
      if(anc_ta_scrl_signal== NULL) anc_ta_scrl_signal = lv_obj_get_signal_cb(ta_scrl);
      lv_obj_set_signal_cb(ta_scrl, ta_ext_scrl_signal);
    }
  }
}

void GTextArea::create(GWidget& parent) {
  this->_par = &parent;
  create();
}

void GTextArea::create(GWidget* parent) {
  this->_par = parent;
  create();
}


void GTextArea::default_keyboard(bool enable){
  if(!this->isCreated()) create();
  GTextArea_ext_t* ext = (GTextArea_ext_t*) this->ext_attr();
  ext->default_kbd = !!enable;
}

void  GTextArea::cursor_pos(int16_t pos){
  if(!this->isCreated()) create();
  lv_ta_set_cursor_pos(this->obj, pos);
}

int16_t GTextArea::cursor_pos(){
  if(!this->isCreated()) create();
  return lv_ta_get_cursor_pos(this->obj);
}

void GTextArea::cursor_type(cursor_type_t cur_type) {
  if(!this->isCreated()) create();
  lv_ta_set_cursor_type(this->obj, cur_type);
}

cursor_type_t GTextArea::cursor_type(){
  if(!this->isCreated()) create();
  return lv_ta_get_cursor_type(this->obj);
}

void GTextArea::cursor_click_pos(bool enable) {
  if(!this->isCreated()) create();
  lv_ta_set_cursor_click_pos(this->obj, enable);
}

bool  GTextArea::cursor_click_pos(){
  if(!this->isCreated()) create();
  return lv_ta_get_cursor_click_pos(this->obj);
}

void GTextArea::cursor_right(){
  if(!this->isCreated()) create();
  return lv_ta_cursor_right(this->obj);
}

void GTextArea::cursor_left(){
  if(!this->isCreated()) create();
  return lv_ta_cursor_left(this->obj);
}

void GTextArea::cursor_up(){
  if(!this->isCreated()) create();
  return lv_ta_cursor_up(this->obj);
}

void GTextArea::cursor_down(){
  if(!this->isCreated()) create();
  return lv_ta_cursor_down(this->obj);
}

void GTextArea::text(String txt) {
  if(!this->isCreated()) create();
  lv_ta_set_text(this->obj, txt.c_str());
}

String GTextArea::text() {
  if(!this->isCreated()) create();
  const char* txt = lv_ta_get_text(this->obj);
  return txt; //(txt==NULL)? String(txt): "";
}

const char* GTextArea::c_str(){
  if(!this->isCreated()) create();
  return (const char*) lv_ta_get_text(this->obj);
}

void  GTextArea::add(String txt){
  if(txt == "") return;
  if(!this->isCreated()) create();
  lv_ta_add_text(this->obj, txt.c_str());
}

void GTextArea::add(uint32_t c){
  if( c == 0) return;
  if(!this->isCreated()) create();
  lv_ta_add_char(this->obj, c);
}

void  GTextArea::backspace(){
  if(!this->isCreated()) create();
  lv_ta_del_char(this->obj);
}

void  GTextArea::del_forward() {
  if(!this->isCreated()) create();
  lv_ta_del_char_forward(this->obj);
}

void GTextArea::text_align(label_align_t align){
  if(!this->isCreated()) create();
  lv_ta_set_text_align(this->obj, align);
}

void  GTextArea::accepted_chars(String accepted_chars_list){
  if(!this->isCreated()) create();
  lv_ta_set_accepted_chars(this->obj, accepted_chars_list.c_str());
}

String GTextArea::accepted_chars(){
  if(!this->isCreated()) create();
  return lv_ta_get_accepted_chars(this->obj);
}

void GTextArea::insert_replace(String text){
  if(!this->isCreated()) create();
  lv_ta_set_insert_replace(this->obj, text.c_str());
}

void GTextArea::placeholder(String txt){
  if(txt == "") return;  // เนื่องจาก ไปชนอะไรสักอย่างทำให้ค้างกรณี placeholder เป็น "" เลบต้องใส่กัน

  if(!this->isCreated()) create();
  lv_ta_set_placeholder_text(this->obj, txt.c_str());
  lv_ta_set_style(this->obj, LV_TA_STYLE_PLACEHOLDER, &style_sb);
#if defined (LAOS_TEXT)
  if( style_sb.text.font == &prasanmit_25 || style_sb.text.font == &prasanmit_30 ||
      style_sb.text.font == &prasanmit_35 || style_sb.text.font == &prasanmit_40 )
    this->placeholder_font(saysettha_25);
#elif defined (VIETNAM_TEXT)
  if( style_sb.text.font == &prasanmit_25 || style_sb.text.font == &prasanmit_30 ||
      style_sb.text.font == &prasanmit_35 || style_sb.text.font == &prasanmit_40 )
    this->placeholder_font(arial_vietnam_30);
#endif
}

String GTextArea::placeholder(){
  if(!this->isCreated()) create();
  const char* txt = lv_ta_get_placeholder_text(this->obj);
  return (txt==NULL)? String(txt): "";
}

void GTextArea::placeholder_font(font_t& font) {
  if(!this->isCreated()) create();
  
  GTextArea_ext_t* ext = (GTextArea_ext_t*) this->ext_attr();
  if( ext->ta.placeholder == NULL) return;

  style_sb.text.font = &font;
  lv_ta_set_style(this->obj, LV_TA_STYLE_PLACEHOLDER, &style_sb);
}

void GTextArea::placeholder_font(font_t& font, color_t color){
  if(!this->isCreated()) create();

  GTextArea_ext_t* ext = (GTextArea_ext_t*) this->ext_attr();
  if( ext->ta.placeholder == NULL) return;

  style_sb.text.font        = &font;
  style_sb.text.color.full  = color;
  lv_ta_set_style(this->obj, LV_TA_STYLE_PLACEHOLDER, &style_sb);
}

void  GTextArea::placeholder_color(color_t color, uint8_t opa) {
  if(!this->isCreated()) create();

  GTextArea_ext_t* ext = (GTextArea_ext_t*) this->ext_attr();
  if( ext->ta.placeholder == NULL) return;

  style_sb.text.color.full = color;
  style_sb.text.opa        = opa;
  lv_ta_set_style(this->obj, LV_TA_STYLE_PLACEHOLDER, &style_sb);
}

color_t GTextArea::placeholder_color() {
  if(!this->isCreated()) create();
  return style_sb.text.color.full;
}


void GTextArea::oneline(bool enable){
  if(!this->isCreated()) create();
  lv_ta_set_one_line(this->obj, enable);
}

bool GTextArea::oneline() {
  if(!this->isCreated()) create();
  return lv_ta_get_one_line(this->obj);
}


void GTextArea::password_mode(bool enable){
  if(!this->isCreated()) create();
  lv_ta_set_pwd_mode(this->obj, enable);
}

bool GTextArea::password_mode(){
  if(!this->isCreated()) create();
  return lv_ta_get_pwd_mode(this->obj);
}

void GTextArea::password_show_time(uint16_t time){    // เวลาในหน่วย milliseconds (ms) ที่จะให้ password แสดงก่อนจะกลายเป็น ***
  if(!this->isCreated()) create();
  lv_ta_set_pwd_show_time(this->obj, time);
}

uint16_t GTextArea::password_show_time(){
  if(!this->isCreated()) create();
  return lv_ta_get_pwd_show_time(this->obj);
}

void GTextArea::max_length(uint16_t len) {
  if(!this->isCreated()) create();
  lv_ta_set_max_length(this->obj, len);
}

uint16_t GTextArea::max_length(){
  if(!this->isCreated()) create();
  return lv_ta_get_max_length(this->obj);
}


void GTextArea::text_select(bool enable){
  if(!this->isCreated()) create();
  return lv_ta_set_text_sel(this->obj, enable);
}

bool GTextArea::text_select(){
  if(!this->isCreated()) create();
  return lv_ta_get_text_sel_en(this->obj);
}

bool GTextArea::text_isSelected(){
  if(!this->isCreated()) create();
  return lv_ta_text_is_selected(this->obj);
}

void GTextArea::scrollbar(scrollbar_t sb_mode){
  if(!this->isCreated()) create();
  lv_ta_set_sb_mode(this->obj, sb_mode);
}

scrollbar_t GTextArea::scrollbar(){
  if(!this->isCreated()) create();
  return lv_ta_get_sb_mode(this->obj);
}

void GTextArea::scroll_propagation(bool enable){
  if(!this->isCreated()) create();
  lv_ta_set_scroll_propagation(this->obj, enable);
}

bool GTextArea::scroll_propagation(){
  if(!this->isCreated()) create();
  return lv_ta_get_scroll_propagation(this->obj);
}

GTextArea& GTextArea::operator =(String label){
  this->text(label);
  return *this;
}

GTextArea& GTextArea::operator =(const char* cstr){
  this->text(cstr);
  return *this;
}

GTextArea& GTextArea::operator =(int num){
  this->text(String(num));
  return *this;
}

#if TEXTAREA_USE_LABEL
GTextArea& GTextArea::operator =(GLabel& label){
  this->text(label.text());
  return *this;
}
#endif

static void gtextarea_event_cb(obj_t *obj, event_t event){
  GTextArea* pTA = (GTextArea*) lv_obj_get_user_data(obj);
  GTextArea_ext_t* ext = (GTextArea_ext_t*) pTA->ext_attr();

  // screen_timer_update();

  switch(event) {
    case LV_EVENT_CLICKED:
      if( !pTA->clickable()) return;
#if BLYNKGO_USE_KBD
      if(ext->default_kbd) {
        // kbd = make_unique<GKeyboard>();
        // if(kbd != NULL) { delete kbd; kbd = NULL; }
        // kbd = (GKeyboard*) esp32_malloc(sizeof(GKeyboard));
        // new (kbd) GKeyboard();
        // kbd->onApply([](){ 
        //   // kbd->del();
        //   delete kbd; kbd = NULL;
        // });
        // kbd->setSource(pTA);
#if defined(LAOS_TEXT)
        kbd_create(pTA, KBD_LAOS);
#elif defined(VIETNAM_TEXT)
        kbd_create(pTA, KBD_VIETNAM);
        // kbd_create(pTA, KBD_THAI);
#else
        kbd_create(pTA, KBD_THAI);
#endif
        // MEM_CHECK;
        // kbd_delete();  // เรียกก่อน เผื่อลบของเดิมที่เคยประกาศไป
        // kb = lv_keyboard_create(lv_scr_act(), NULL);
        // lv_keyboard_set_src(kb, pTA->obj, NULL);
        // lv_keyboard_set_apply_cb(kb, kbd_delete);
        // Serial.println("edit mode start");
        // MEM_CHECK;
      }
#endif //BLYNKGO_USE_KBD

      break;
    // case LV_EVENT_VALUE_CHANGED:
    //   if(pTA->fn_onvaluechanged) {
    //     pTA->fn_onvaluechanged(pTA);
    //   }
    //   break;
  }
  main_event_cb(obj, event);
}

