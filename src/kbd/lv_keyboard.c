/**
 * @file lv_keyboard.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_keyboard.h"
#include "keyboard.h"
#include <stdio.h>
#include "../utils/BlynkGO_Type.h"
#include "../config/blynkgo_config.h"

/*********************
 *      DEFINES
 *********************/
#if defined (LAOS_TEXT)
LV_FONT_DECLARE(saysettha_15);
LV_FONT_DECLARE(saysettha_25);
#endif

#if defined (VIETNAM_TEXT)
LV_FONT_DECLARE(arial_vietnam_20);
LV_FONT_DECLARE(arial_vietnam_30);
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_res_t lv_keyboard_signal(lv_obj_t * keyboard, lv_signal_t sign, void * param);
static void kbd_eye_event_cb(lv_obj_t* kbd_eye, lv_event_t event);
static void kbd_eye_anim_ready_cb(lv_anim_t*a);
static void kbd_ta_cont_create(lv_obj_t * keyboard, lv_obj_t* ta_copy);
static void kbd_ta_cont_del(lv_obj_t * keyboard);
static bool is_objx_type(lv_obj_t* obj, const char* type);

// static void kbd_ta_inner_event_cb(lv_obj_t* ta_inner, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_cb_t ancestor_signal;
static lv_style_t kb_style_pr;
static lv_style_t kb_style_rel;
static lv_style_t kbd_eye_lb_style;
static lv_style_t ta_cont_style;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/

// lv_style_t style_placeholder;
extern lv_style_t style_sb;


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/**
 * Create a TridentTD's keyboard objects for ENG and THAI keyboard system
 * @param par pointer to an object, it will be the parent of the new keyboard
 * @param copy pointer to a keyboard object, always NULL
 * @return pointer to the created keyboard
 */
lv_obj_t * lv_keyboard_create(lv_obj_t * parent, const lv_obj_t * copy)
{
  lv_obj_t* new_keyboard = lv_obj_create(parent, NULL);
    LV_ASSERT_MEM(new_keyboard);
    if(new_keyboard == NULL) return NULL;
    
    keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_allocate_ext_attr(new_keyboard, sizeof(keyboard_ext_t));
    LV_ASSERT_MEM(ext);
    if(ext == NULL) return NULL;

    if(ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(new_keyboard);
    
    lv_obj_set_size(new_keyboard, lv_obj_get_width(lv_obj_get_parent(new_keyboard)),lv_obj_get_height(lv_obj_get_parent(new_keyboard)));
    lv_obj_set_hidden(new_keyboard, true);
    
    static lv_style_t keyboard_style; 
      lv_style_copy(&keyboard_style, &lv_style_transp_tight);
      lv_obj_set_style(new_keyboard,  &keyboard_style);
      keyboard_style.body.main_color      = LV_COLOR_BLACK;
      keyboard_style.body.grad_color      = LV_COLOR_BLACK;
      keyboard_style.body.opa             = 255;

    lv_style_copy( &kb_style_rel,     &lv_style_btn_rel);
    lv_style_copy( &kb_style_pr,      &lv_style_btn_pr);
      kb_style_rel.body.radius        = 0;
      kb_style_rel.body.border.width  = 1;
      kb_style_rel.body.opa           = 0;
      kb_style_rel.text.color         = LV_COLOR_BLACK;

      kb_style_pr.body.radius         = 0;
      kb_style_pr.body.border.width   = 1;
      kb_style_pr.body.border.color   = LV_COLOR_SILVER;

    ext->kb         = NULL;
    ext->kb_event_cb= NULL;
    ext->kb_apply_cb= NULL;
    ext->ta_cont    = NULL;
    ext->ta_inner   = NULL;
    ext->kbd_eye    = NULL;
    ext->kbd_eye_lb = NULL;
    ext->kbd_obj_outside = NULL;
    ext->language = KBD_ENG;
    ext->lang2nd  = KBD_THAI;

    ext->kb_bg = lv_obj_create(new_keyboard,NULL);

      int scr_w = lv_obj_get_width(lv_obj_get_parent(ext->kb_bg));
      int scr_h = lv_obj_get_height(lv_obj_get_parent(ext->kb_bg));
      // printf("%d,%d\n",scr_w,scr_h);
      int kb_ctrl_w = (scr_w == 800)? 181 : (scr_w == 480)? 110 : (scr_w == 320)? 72 : (scr_w == 240)? 54 : 54;
      int kb_ctrl_h = (scr_w*scr_h >= 800*480 )? 240: 180;

      lv_obj_set_size(ext->kb_bg, scr_w -1, kb_ctrl_h);
      lv_obj_align(ext->kb_bg, NULL, LV_ALIGN_IN_BOTTOM_MID,0,0);
      static lv_style_t kb_bg_style; 
        lv_style_copy(&kb_bg_style, &lv_style_plain);
        lv_obj_set_style(ext->kb_bg,  &kb_bg_style);
        kb_bg_style.body.main_color     = LV_COLOR_WHITE;
        kb_bg_style.body.grad_color     = LV_COLOR_WHITE;

    ext->kb_ctrl1_bg = lv_obj_create(ext->kb_bg,NULL);
      lv_obj_set_size(ext->kb_ctrl1_bg, kb_ctrl_w ,kb_ctrl_h);
      lv_obj_align(ext->kb_ctrl1_bg, NULL, LV_ALIGN_IN_BOTTOM_LEFT,(scr_w == 800)? -1:0,0);
      static lv_style_t kb_ctrl_style; 
        lv_style_copy(&kb_ctrl_style, &lv_style_plain);
        lv_obj_set_style(ext->kb_ctrl1_bg,  &kb_ctrl_style);
        kb_ctrl_style.body.main_color     = LV_COLOR_BLUE;
        kb_ctrl_style.body.grad_color     = LV_COLOR_CYAN;

    ext->kb_ctrl2_bg = lv_obj_create(ext->kb_bg, ext->kb_ctrl1_bg);
      lv_obj_set_size(ext->kb_ctrl2_bg, kb_ctrl_w ,kb_ctrl_h);
      lv_obj_align(ext->kb_ctrl2_bg, NULL, LV_ALIGN_IN_BOTTOM_RIGHT,0,0);

    ext->kb_ok_bg = lv_obj_create(ext->kb_bg, NULL);
      lv_obj_set_size(ext->kb_ok_bg, kb_ctrl_w ,kb_ctrl_h/4);
      lv_obj_align(ext->kb_ok_bg, NULL, LV_ALIGN_IN_TOP_LEFT,0,0);
      static lv_style_t kb_ok_style; 
        lv_style_copy(&kb_ok_style, &lv_style_plain);
        lv_obj_set_style(ext->kb_ok_bg,  &kb_ok_style);
        kb_ok_style.body.main_color     = LV_COLOR_YELLOW;
        kb_ok_style.body.grad_color     = LV_COLOR_CYAN;

    lv_style_copy(&ta_cont_style, &lv_style_plain);
      ta_cont_style.body.radius = 10;
      ta_cont_style.body.main_color     = LV_COLOR_WHITE;
      ta_cont_style.body.grad_color     = LV_COLOR_SILVER;
      ta_cont_style.body.padding.top    = 0;
      ta_cont_style.body.padding.bottom = 0;
      ta_cont_style.body.padding.left   = 0;
      ta_cont_style.body.padding.right  = 0;
      ta_cont_style.body.padding.inner  = 0;
      ta_cont_style.body.border.width   = 1;
      ta_cont_style.body.border.color   = LV_COLOR_GRAY;
        
    lv_style_copy(&kbd_eye_lb_style, &lv_style_plain);
      kbd_eye_lb_style.text.color = LV_COLOR_BLACK;
      kbd_eye_lb_style.text.opa   = LV_OPA_90;

  lv_obj_set_signal_cb(new_keyboard, lv_keyboard_signal);
  
  return new_keyboard;
}

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the source for editing by the keyboard 
 * @param keyboard pointer to the keyboard
 * @param source pointer to the source ( only lv_label or lv_ta )
 */
void lv_keyboard_set_src(lv_obj_t *keyboard, lv_obj_t * source, const char* manual_placeholder) {
  
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  // lv_ta_ext_t * ta_inner_ext = (lv_ta_ext_t *)lv_obj_get_ext_attr(ext->ta_inner);

  if(ext->kb == NULL) {
    ext->kb = lv_kb_create(ext->kb_bg, NULL);
    if(ext->kb == NULL) return;

      int scr_w = lv_obj_get_width(lv_obj_get_parent(ext->kb_bg));
      int scr_h = lv_obj_get_height(lv_obj_get_parent(ext->kb_bg));
      // printf("lv_keyboard_set_src : %d, %d\n",scr_w, scr_h);
      int kb_ctrl_w = (scr_w == 800)? 181 : (scr_w == 480)? 110 : (scr_w == 320)? 72 : (scr_w == 240)? 54 : 54;
      int kb_ctrl_h = (scr_w*scr_h >= 800*480 )? 240: 180;

    lv_obj_set_size(ext->kb, lv_obj_get_width(lv_obj_get_parent(lv_obj_get_parent(ext->kb))) +3 ,kb_ctrl_h);
    lv_obj_align(ext->kb, NULL, LV_ALIGN_IN_BOTTOM_MID,0,0);
    lv_obj_set_event_cb(ext->kb, keyboard_def_event_cb);

    lv_kb_set_style(ext->kb, LV_KB_STYLE_BG,      &lv_style_transp_tight);//
    lv_kb_set_style(ext->kb, LV_KB_STYLE_BTN_REL, &kb_style_rel);
    lv_kb_set_style(ext->kb, LV_KB_STYLE_BTN_PR,  &kb_style_pr);
  }

  /* โหลด keyboard language ปัจจุบัน */
  kbd_lang_t lang = lv_keyboard_get_language(keyboard);
  switch(lang){
    case KBD_ENG:
      lv_kb_load_en_kbd(ext->kb);
      break;
    case KBD_THAI:
      lv_kb_load_th_kbd(ext->kb);
      break;
#if defined (LAOS_TEXT)
    case KBD_LAOS:
      lv_kb_load_la_kbd(ext->kb);
      break;
#endif
#if defined (VIETNAM_TEXT)
    case KBD_VIETNAM:
      // lv_kb_load_vn_kbd(ext->kb);
      lv_kb_load_th_kbd(ext->kb);
      break;
#endif
  }



  // lv_obj_type_t obj_type;
  // lv_obj_get_type(source, &obj_type);

  // if(strcmp(obj_type.type[0], "lv_label")==0) {
  if( is_objx_type(source, "lv_label") ) {
    ext->kbd_obj_outside = source;
    lv_obj_set_hidden(keyboard, false);
    lv_obj_move_foreground(keyboard);

      // สร้าง ta_cont ชุดใหม่ โดยลบของเก่าออก แบบไม่ต้อง copy จากอะไรเลย
      kbd_ta_cont_create(keyboard, NULL);

      lv_ta_ext_t * ta_inner_ext = (lv_ta_ext_t *)lv_obj_get_ext_attr(ext->ta_inner);

      // source เป็น lv_label เลยให้ copy มาลงที่ ta_inner
      lv_ta_set_text(ext->ta_inner, lv_label_get_text(source));

      // placeholder
      if(manual_placeholder != NULL)
        lv_ta_set_placeholder_text(ext->ta_inner, manual_placeholder);
      // หาก copy มาแล้วมี placeholder ติดมาด้วย ให้ปรับ style ให้ placeholder ด้วย
      if(ta_inner_ext->placeholder!= NULL) {
        const char* placeholder_txt = lv_ta_get_placeholder_text(ext->ta_inner);
        if(placeholder_txt != NULL) {
          lv_obj_set_style(ta_inner_ext->placeholder, &style_sb);
        }
      }

      // เลื่อน cursor ไปไว้ตัวท้ายเสมอ
      lv_ta_set_cursor_pos(ext->ta_inner, LV_TA_CURSOR_LAST);

      // ซ่อนดวงตาออกไป
      lv_obj_set_hidden(ext->kbd_eye, true);
      lv_obj_set_width(ext->ta_inner, lv_obj_get_width(lv_obj_get_parent(ext->ta_inner)) );      

  }else
  // if(strcmp(obj_type.type[0], "lv_ta")==0){
  if( is_objx_type(source, "lv_ta") ) {
    ext->kbd_obj_outside = source;
    lv_obj_set_hidden(keyboard, false);
    lv_obj_move_foreground(keyboard);

    // เนื้องจาก source เป็น lv_ta ให้ทำการ copyมา
    kbd_ta_cont_create(keyboard, source);

    lv_ta_ext_t * ta_inner_ext = (lv_ta_ext_t *)lv_obj_get_ext_attr(ext->ta_inner);

    if(manual_placeholder != NULL)
      lv_ta_set_placeholder_text(ext->ta_inner, manual_placeholder);

    // หาก copy มาแล้วมี placeholder ติดมาด้วย ให้ปรับ style ให้ placeholder ด้วย
    if(ta_inner_ext->placeholder!= NULL) {
      const char* placeholder_txt = lv_ta_get_placeholder_text(ext->ta_inner);
      if(placeholder_txt != NULL) {
        lv_obj_set_style(ta_inner_ext->placeholder, &style_sb);
      }
    }

    lv_ta_set_one_line(ext->ta_inner, true);
    lv_ta_set_cursor_pos(ext->ta_inner, LV_TA_CURSOR_LAST);
  
    if(lv_ta_get_pwd_mode(ext->ta_inner)){
      //กรณี ta ที่เข้ามาเป็น password mode
      lv_obj_set_hidden(ext->kbd_eye, false);
      lv_obj_set_width(ext->ta_inner, lv_obj_get_width(lv_obj_get_parent(ext->ta_inner)) - 40 );

    }else{
      //กรณี ta ที่เข้ามาไม่ใช่ password mode
      lv_obj_set_hidden(ext->kbd_eye, true);
      lv_obj_set_width(ext->ta_inner, lv_obj_get_width(lv_obj_get_parent(ext->ta_inner)) );      
    }
  }
}

/**
 * Set the language for the keyboard 
 * @param keyboard pointer to the keyboard
 * @param lang to the language of keyboard
 */
void lv_keyboard_set_language(lv_obj_t *keyboard, kbd_lang_t lang) {
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  ext->language = lang;
}

void lv_keyboard_set_language2nd(lv_obj_t *keyboard, kbd_lang_t lang2nd) {
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  ext->lang2nd = lang2nd;
  switch(ext->lang2nd){
    case KBD_THAI:
      ta_cont_style.text.font     = &prasanmit_30;
      kb_style_rel.text.font      = &prasanmit_30;
      kb_style_pr.text.font       = &prasanmit_30;
      break;
#if defined (LAOS_TEXT)
    case KBD_LAOS:
      ta_cont_style.text.font     = &saysettha_25;
      kb_style_rel.text.font      = &saysettha_25;
      kb_style_pr.text.font       = &saysettha_25;
      break;
#elif defined (VIETNAM_TEXT)
    case KBD_VIETNAM:
      ta_cont_style.text.font     = &arial_vietnam_30;
      kb_style_rel.text.font      = &arial_vietnam_30;
      kb_style_pr.text.font       = &arial_vietnam_30;
      break;
#endif
    default:
      ta_cont_style.text.font     = &prasanmit_30;
      kb_style_rel.text.font      = &prasanmit_30;
      kb_style_pr.text.font       = &prasanmit_30;

      break;
  }
  lv_obj_set_style(ext->ta_cont,  &ta_cont_style);
}


void lv_keyboard_set_event_cb(lv_obj_t *keyboard, lv_event_cb_t event_cb){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  ext->kb_event_cb = event_cb;
}


void lv_keyboard_set_apply_cb(lv_obj_t *keyboard, lv_apply_cb_t apply_cb){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  ext->kb_apply_cb = apply_cb;
}

void lv_keyboard_set_user_data(lv_obj_t *keyboard, void* user_data){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  ext->user_data = user_data;
}

void lv_keyboard_display_numpad(lv_obj_t *keyboard){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  lv_kb_load_en_kbd_numpad(ext->kb);
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the language for the keyboard 
 * @param keyboard pointer to the keyboard
 * @return language of 'kbd_lang_t'
 */
kbd_lang_t lv_keyboard_get_language(lv_obj_t *keyboard) {
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  return ext->language;
}

lv_obj_t* lv_keyboard_get_src(lv_obj_t *keyboard){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  return ext->kbd_obj_outside;
}

void * lv_keyboard_get_user_data(lv_obj_t *keyboard){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  return ext->user_data;
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
 * Signal function of the keyboard
 * @param keyboard pointer to a keyboard object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_keyboard_signal(lv_obj_t * keyboard, lv_signal_t sign, void * param){
  lv_res_t res;
  
  /* Include the ancient signal function */
  res = ancestor_signal(keyboard, sign, param);
  if(res != LV_RES_OK) return res;
  
  if(sign == LV_SIGNAL_GET_TYPE) {
    lv_obj_type_t * buf = (lv_obj_type_t *)param;
    uint8_t i;
    for(i = 0; i < LV_MAX_ANCESTOR_NUM - 1; i++) { /*Find the last set data*/
      if(buf->type[i] == NULL) break;
    }
    buf->type[i] = "lv_keyboard";
  }
}

/**
 * keyboard_ext's kbd_eye event when is clicked. (password-mode ta_inner only)
 * @param kbd_eye pointer to a  keyboard_ext's kbd_eye obj
 * @param event the triggering event  only LV_EVENT_CLICKED
 */
static void kbd_eye_event_cb(lv_obj_t* kbd_eye, lv_event_t event){
  if(event != LV_EVENT_CLICKED) return;
  // printf("kbd_eye clicked\n");
  
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(lv_obj_get_parent(lv_obj_get_parent(kbd_eye)));

  lv_ta_ext_t * ta_inner_ext = (lv_ta_ext_t*) lv_obj_get_ext_attr(ext->ta_inner);

  ext->kbd_eye_state = !ext->kbd_eye_state;
  if(ext->kbd_eye_state){
    lv_label_set_text(ext->kbd_eye_lb, SYMBOL_EYE_CLOSE);
    lv_ta_set_pwd_mode(ext->ta_inner, false);
    // if(ta_inner_ext->pwd_tmp != NULL)
    //   printf("eye open : %s\n",ta_inner_ext->pwd_tmp);

  }else{
    lv_label_set_text(ext->kbd_eye_lb, SYMBOL_EYE_OPEN);
    lv_ta_set_pwd_mode(ext->ta_inner, true);
    // if(ta_inner_ext->pwd_tmp != NULL)
    //   printf("eye close : %s\n",ta_inner_ext->pwd_tmp);
  }
}

/**
 * keyboard_ext's kbd_eye anim_ready_cb when anim for coping text is done. (password-mode ta_inner only)
 * @param a pointer to a animation variable
 */
static void kbd_eye_anim_ready_cb(lv_anim_t*a) { 
  keyboard_ext_t * ext = (keyboard_ext_t *)a->var;
  ext->kbd_eye_state = false;
  lv_label_set_text(ext->kbd_eye_lb, SYMBOL_EYE_OPEN);
}

/**
 * สำหรับสร้าง ta_inner ให้ keyboard 
 * @param keyboard pointer to the keyboard
 * @param ta_copy pointer to a source-textarea for copying
 */

static void kbd_ta_cont_create(lv_obj_t * keyboard, lv_obj_t* ta_copy){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);

  if(ext->ta_cont != NULL) kbd_ta_cont_del(keyboard);

  ext->ta_cont = lv_cont_create(keyboard, NULL);
    lv_cont_set_layout(ext->ta_cont, LV_LAYOUT_ROW_M);
    lv_cont_set_fit2(ext->ta_cont, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_obj_set_width(ext->ta_cont, lv_obj_get_width(lv_obj_get_parent(ext->ta_cont))-6);
    lv_obj_align(ext->ta_cont, NULL, LV_ALIGN_IN_TOP_LEFT, 3, -6);
    lv_obj_set_style(ext->ta_cont,  &ta_cont_style);

  ext->ta_inner = lv_ta_create(ext->ta_cont, ta_copy);
    lv_page_set_style(ext->ta_inner, LV_PAGE_STYLE_BG, &lv_style_transp);
    lv_obj_set_width(ext->ta_inner, lv_obj_get_width(lv_obj_get_parent(ext->ta_inner)) - 40 );
    lv_ta_set_cursor_type(ext->ta_inner, LV_CURSOR_LINE);
    lv_ta_set_one_line(ext->ta_inner, true);
    lv_ta_set_text_sel(ext->ta_inner, true); // เพิ่ม
    lv_obj_set_event_cb(ext->ta_inner, NULL);
    // lv_obj_set_event_cb(ext->ta_inner, kbd_ta_inner_event_cb);
    // static lv_style_t ta_inner_style; 
    //   lv_style_copy(&ta_inner_style, &lv_style_transp_tight);
    //   lv_obj_set_style(ext->ta_inner,  &ta_inner_style);
    //   ta_inner_style.body.main_color      = LV_COLOR_BLACK;
    //   ta_inner_style.body.grad_color      = LV_COLOR_BLACK;
    //   ta_inner_style.body.opa             = 255;
    
    ext->kbd_eye = lv_obj_create(ext->ta_cont, NULL);
      lv_obj_set_size(ext->kbd_eye,36,36);
      lv_obj_set_style(ext->kbd_eye, &lv_style_transp);
      lv_obj_set_event_cb( ext->kbd_eye, kbd_eye_event_cb );
  
      ext->kbd_eye_state = false;
      ext->kbd_eye_lb = lv_label_create(ext->kbd_eye, NULL);
        lv_label_set_text(ext->kbd_eye_lb, SYMBOL_EYE_OPEN);
        lv_obj_align(ext->kbd_eye_lb, NULL, LV_ALIGN_CENTER,0,0);
        lv_obj_set_click(ext->kbd_eye_lb,false);
        lv_obj_set_style(ext->kbd_eye_lb, &kbd_eye_lb_style);

  // จับ kb ให้มา link กับ ta_inner
  lv_kb_set_ta(ext->kb, ext->ta_inner);
  // printf("ta_inner created\n");
}

// static void kbd_ta_inner_event_cb(lv_obj_t* ta_inner, lv_event_t event)
// {
//   // lv_obj_t* keyboard = lv_obj_get_parent(lv_obj_get_parent(ta_inner));
//   // keyboard_ext_t * keyboard_ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
//   lv_ta_ext_t* ext = (lv_ta_ext_t*) lv_obj_get_ext_attr(ta_inner);
//   switch(event){
//     case LV_EVENT_RELEASED:
//       if( lv_ta_text_is_selected(ta_inner)){
//         printf("[keyboard] ta_inner is selected.  pos : %d - %d\n", ext->sel.start, ext->sel.end);
//       }
//       break;
//   }

// }

/**
 * สำหรับลบ ta_inner ให้ keyboard 
 * @param keyboard pointer to the keyboard
 */
static void kbd_ta_cont_del(lv_obj_t * keyboard){
  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(keyboard);
  lv_obj_del(ext->kbd_eye_lb);
  lv_obj_del(ext->kbd_eye);
  lv_obj_del(ext->ta_inner);
  lv_obj_del(ext->ta_cont);
  ext->ta_cont = NULL;
}

/*=====================
 * Other functions
 *====================*/

/**
 * Default keyboard event after kb is applied and send edited text to the outside obj.
 * @param kb pointer to a  keyboard_ext's kb obj
 * @param event the triggering event  only LV_EVENT_APPLY and LV_EVENT_CANCEL
 */
void keyboard_def_event_cb(lv_obj_t* kb, lv_event_t event){

  keyboard_ext_t * ext = (keyboard_ext_t*) lv_obj_get_ext_attr(lv_obj_get_parent(lv_obj_get_parent(kb)));
  
  if(event == LV_EVENT_APPLY){
    if(ext->kbd_obj_outside != NULL) {
      // lv_obj_type_t obj_type;
      // lv_obj_get_type(ext->kbd_obj_outside, &obj_type);
      // if(strcmp(obj_type.type[0], "lv_label")==0) {
      if( is_objx_type(ext->kbd_obj_outside, "lv_label") ) {
        lv_label_set_text(ext->kbd_obj_outside, lv_ta_get_text(ext->ta_inner)); 
        // lv_res_t res = lv_event_send(ext->kbd_obj_outside,  LV_EVENT_VALUE_CHANGED,  NULL);
        // if(res != LV_RES_OK) return;

      }else
      // if(strcmp(obj_type.type[0], "lv_ta")==0){
      if( is_objx_type(ext->kbd_obj_outside, "lv_ta") ) {
        const char* ta_inner_txt = lv_ta_get_text(ext->ta_inner);
        if( ta_inner_txt != NULL)
          lv_ta_set_text(ext->kbd_obj_outside, ta_inner_txt);
      }
      ext->kbd_obj_outside = NULL;
      lv_obj_set_hidden(lv_obj_get_parent(lv_obj_get_parent(kb)), true);
    }
    if(ext->kb_apply_cb!=NULL){
      ext->kb_apply_cb();
    }
  }else
  if(event == LV_EVENT_CANCEL){
    ext->kbd_obj_outside = NULL;
    lv_obj_set_hidden( lv_obj_get_parent(lv_obj_get_parent(kb)), true);
  }
}


static bool is_objx_type(lv_obj_t* obj, const char* type)
{
  lv_obj_type_t obj_type;

  lv_obj_get_type(obj, &obj_type);
  uint8_t cnt;
  for(cnt = 0; cnt < LV_MAX_ANCESTOR_NUM; cnt++) {
    if(obj_type.type[cnt] == NULL) break;
    if(!strcmp(obj_type.type[cnt], type)) return true;
  }
  return false;
}


