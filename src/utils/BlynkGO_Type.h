/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ BlynkGO_Type.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __BLYNKGO_TYPE_H__
#define __BLYNKGO_TYPE_H__

#include "./lvgl-dev-7.0/lvgl.h"
#include "./lvgl-dev-7.0/src/lv_draw/TridentUnicode.h"

#define SIZE_PRINTLN(widget)    Serial.printf("[Size] %d,%d\n", widget.width(), widget.height())

typedef lv_obj_t            obj_t;
typedef lv_font_t           font_t;
typedef lv_style_t          style_t;
typedef lv_img_dsc_t        img_t;
typedef lv_point_t          point_t;
typedef lv_area_t           area_t;
typedef lv_chart_series_t*  serie_t;
typedef lv_chart_type_t     chart_type_t;
// typedef lv_event_cb_t   event_cb_t;
typedef lv_signal_cb_t      signal_cb_t;
typedef lv_design_cb_t      design_cb_t;
typedef lv_task_t           task_t;
typedef lv_res_t            res_t;

typedef uint16_t            color_t;
typedef uint8_t             datum_t;
typedef lv_color_hsv_t      color_hsv_t;


#define PROJECT_NAME        String(__FILE__).substring(String(__FILE__).lastIndexOf("\\")+1, String(__FILE__).lastIndexOf(".ino"))

#define RADIUS_CIRCLE       LV_RADIUS_CIRCLE
#define CURSOR_LAST         LV_TA_CURSOR_LAST
#define NO_TXT_SEL          LV_DRAW_LABEL_NO_TXT_SEL

#define GRect               GObject
#define new_GRect           new_GObject
#define StringX             GLabel
#define MATH_MAX            LV_MATH_MAX
#define MATH_MIN            LV_MATH_MIN

#define ACTIVE_LOW          0
#define ACTIVE_HIGH         1


#define obj_get_user_data(obj)    lv_obj_get_user_data(obj)
#define obj_get_parent(obj)       lv_obj_get_parent(obj)

#define TEXT_NEXT(str, i)         lv_txt_encoded_next(str, &i)
#define TEXT_PREV(str, i)         lv_txt_encoded_prev(str, &i)

enum {
  DESIGN_RES_OK,          /**< Draw ready */
  DESIGN_RES_COVER,       /**< Returned on `DESIGN_COVER_CHK` if the areas is fully covered*/
  DESIGN_RES_NOT_COVER,   /**< Returned on `DESIGN_COVER_CHK` if the areas is not covered*/
  DESIGN_RES_MASKED,      /**< Returned on `DESIGN_COVER_CHK` if the areas is masked out (children also not cover)*/
};
typedef lv_design_res_t     design_res_t;


/** Design modes */
enum {
  DESIGN_DRAW_MAIN, /**< Draw the main portion of the object */
  DESIGN_DRAW_POST, /**< Draw extras on the object */
  DESIGN_COVER_CHK, /**< Check if the object fully covers the 'mask_p' area */
};
typedef lv_design_mode_t    design_mode_t;


enum {
  ALARM_IVALID=0,
  ALARM_SUNDAY=1, 
  ALARM_MONDAY=2, 
  ALARM_TUESDAY=3, 
  ALARM_WEDNESDAY=4,
  ALARM_THURSDAY=5, 
  ALARM_FRIDAY=6,
  ALARM_SATURDAY=7,
};
typedef uint8_t alarm_dow_t;


enum {
  IMG_CF_UNKNOWN = 0,

  IMG_CF_RAW,              /**< Contains the file as it is. Needs custom decoder function*/
  IMG_CF_RAW_ALPHA,        /**< Contains the file as it is. The image has alpha. Needs custom decoder
                                   function*/
  IMG_CF_RAW_CHROMA_KEYED, /**< Contains the file as it is. The image is chroma keyed. Needs
                                   custom decoder function*/

  IMG_CF_TRUE_COLOR,              /**< Color format and depth should match with LV_COLOR settings*/
  IMG_CF_TRUE_COLOR_ALPHA,        /**< Same as `LV_IMG_CF_TRUE_COLOR` but every pixel has an alpha byte*/
  IMG_CF_TRUE_COLOR_CHROMA_KEYED, /**< Same as `LV_IMG_CF_TRUE_COLOR` but LV_COLOR_TRANSP pixels
                                          will be transparent*/

  IMG_CF_INDEXED_1BIT, /**< Can have 2 different colors in a palette (always chroma keyed)*/
  IMG_CF_INDEXED_2BIT, /**< Can have 4 different colors in a palette (always chroma keyed)*/
  IMG_CF_INDEXED_4BIT, /**< Can have 16 different colors in a palette (always chroma keyed)*/
  IMG_CF_INDEXED_8BIT, /**< Can have 256 different colors in a palette (always chroma keyed)*/

  IMG_CF_ALPHA_1BIT, /**< Can have one color and it can be drawn or not*/
  IMG_CF_ALPHA_2BIT, /**< Can have one color but 4 different alpha value*/
  IMG_CF_ALPHA_4BIT, /**< Can have one color but 16 different alpha value*/
  IMG_CF_ALPHA_8BIT, /**< Can have one color but 256 different alpha value*/

  IMG_CF_RESERVED_15,              /**< Reserved for further use. */
  IMG_CF_RESERVED_16,              /**< Reserved for further use. */
  IMG_CF_RESERVED_17,              /**< Reserved for further use. */
  IMG_CF_RESERVED_18,              /**< Reserved for further use. */
  IMG_CF_RESERVED_19,              /**< Reserved for further use. */
  IMG_CF_RESERVED_20,              /**< Reserved for further use. */
  IMG_CF_RESERVED_21,              /**< Reserved for further use. */
  IMG_CF_RESERVED_22,              /**< Reserved for further use. */
  IMG_CF_RESERVED_23,              /**< Reserved for further use. */

  IMG_CF_USER_ENCODED_0,          /**< User holder encoding format. */
  IMG_CF_USER_ENCODED_1,          /**< User holder encoding format. */
  IMG_CF_USER_ENCODED_2,          /**< User holder encoding format. */
  IMG_CF_USER_ENCODED_3,          /**< User holder encoding format. */
  IMG_CF_USER_ENCODED_4,          /**< User holder encoding format. */
  IMG_CF_USER_ENCODED_5,          /**< User holder encoding format. */
  IMG_CF_USER_ENCODED_6,          /**< User holder encoding format. */
  IMG_CF_USER_ENCODED_7,          /**< User holder encoding format. */
};
typedef lv_img_cf_t img_cf_t;  // image color format type

// typedef struct
// {
//     lv_coord_t * points;
//     lv_color_t color;
//     uint16_t start_point;
// } chart_series_t;

enum {
  ALIGN_CENTER = 0,
  ALIGN_IN_TOP_LEFT,
  ALIGN_IN_TOP_MID,
  ALIGN_IN_TOP_RIGHT,
  ALIGN_IN_BOTTOM_LEFT,
  ALIGN_IN_BOTTOM_MID,
  ALIGN_IN_BOTTOM_RIGHT,
  ALIGN_IN_LEFT_MID,
  ALIGN_IN_RIGHT_MID,
  ALIGN_OUT_TOP_LEFT,
  ALIGN_OUT_TOP_MID,
  ALIGN_OUT_TOP_RIGHT,
  ALIGN_OUT_BOTTOM_LEFT,
  ALIGN_OUT_BOTTOM_MID,
  ALIGN_OUT_BOTTOM_RIGHT,
  ALIGN_OUT_LEFT_TOP,
  ALIGN_OUT_LEFT_MID,
  ALIGN_OUT_LEFT_BOTTOM,
  ALIGN_OUT_RIGHT_TOP,
  ALIGN_OUT_RIGHT_MID,
  ALIGN_OUT_RIGHT_BOTTOM,
  ALIGN_MAX,

  ALIGN_TOP_LEFT,   // = ALIGN_OUT_TOP_LEFT,
  ALIGN_TOP_MID,    // = ALIGN_OUT_TOP_MID,
  ALIGN_TOP_RIGHT,  // = ALIGN_OUT_TOP_RIGHT,
  ALIGN_IN_TOP,     // = ALIGN_IN_TOP_MID,
  ALIGN_OUT_TOP,    // = ALIGN_OUT_TOP_MID,
  ALIGN_BOTTOM_LEFT, // = ALIGN_OUT_BOTTOM_LEFT,
  ALIGN_BOTTOM_MID, // = ALIGN_OUT_BOTTOM_MID,
  ALIGN_BOTTOM_RIGHT, // = ALIGN_OUT_BOTTOM_RIGHT,
  ALIGN_IN_BOTTOM,  // = ALIGN_IN_BOTTOM_MID,
  ALIGN_OUT_BOTTOM, // = ALIGN_OUT_BOTTOM_MID,
  ALIGN_LEFT_TOP, // = ALIGN_OUT_LEFT_TOP,
  ALIGN_LEFT_MID, // = ALIGN_OUT_LEFT_MID,
  ALIGN_LEFT_BOTTOM, // = ALIGN_OUT_LEFT_BOTTOM,
  ALIGN_IN_LEFT,  // = ALIGN_IN_LEFT_MID,
  ALIGN_OUT_LEFT, // = ALIGN_OUT_LEFT_MID,
  ALIGN_RIGHT_TOP, // = ALIGN_OUT_RIGHT_TOP,
  ALIGN_RIGHT_MID, // = ALIGN_OUT_RIGHT_MID,
  ALIGN_RIGHT_BOTTOM, // = ALIGN_OUT_RIGHT_BOTTOM,
  ALIGN_IN_RIGHT,  // = ALIGN_IN_RIGHT_MID,
  ALIGN_OUT_RIGHT, // = ALIGN_OUT_RIGHT_MID,
  ALIGN_TOP,    // = ALIGN_IN/OUT_TOP_MID
  ALIGN_BOTTOM,  // = ALIGN_IN/OUT_BOTTOM_MID,
  ALIGN_LEFT,  // = ALIGN_IN/OUT_LEFT_MID,
  ALIGN_RIGHT,// = ALIGN_IN/OUT_RIGHT_MID,
}; 
typedef uint8_t align_t;

typedef enum{
  CLOCKWISE,
  ANTICLOCKWISE,
} rotate_direction_t;

enum {
  TASK_PRIO_OFF = 0,
  TASK_PRIO_LOWEST,
  TASK_PRIO_LOW,
  TASK_PRIO_MID,
  TASK_PRIO_HIGH,
  TASK_PRIO_HIGHEST,
  _TASK_PRIO_NUM,
};
typedef uint8_t task_prio_t;


/*Image color format*/
enum {
    CF_UNKNOWN = 0,

    CF_RAW,              /**< Contains the file as it is. Needs custom decoder function*/
    CF_RAW_ALPHA,        /**< Contains the file as it is. The image has alpha. Needs custom decoder
                                   function*/
    CF_RAW_CHROMA_KEYED, /**< Contains the file as it is. The image is chroma keyed. Needs
                                   custom decoder function*/

    CF_TRUE_COLOR,              /**< Color format and depth should match with LV_COLOR settings*/
    CF_TRUE_COLOR_ALPHA,        /**< Same as `LV_IMG_CF_TRUE_COLOR` but every pixel has an alpha byte*/
    CF_TRUE_COLOR_CHROMA_KEYED, /**< Same as `LV_IMG_CF_TRUE_COLOR` but LV_COLOR_TRANSP pixels
                                          will be transparent*/

    CF_INDEXED_1BIT, /**< Can have 2 different colors in a palette (always chroma keyed)*/
    CF_INDEXED_2BIT, /**< Can have 4 different colors in a palette (always chroma keyed)*/
    CF_INDEXED_4BIT, /**< Can have 16 different colors in a palette (always chroma keyed)*/
    CF_INDEXED_8BIT, /**< Can have 256 different colors in a palette (always chroma keyed)*/

    CF_ALPHA_1BIT, /**< Can have one color and it can be drawn or not*/
    CF_ALPHA_2BIT, /**< Can have one color but 4 different alpha value*/
    CF_ALPHA_4BIT, /**< Can have one color but 16 different alpha value*/
    CF_ALPHA_8BIT, /**< Can have one color but 256 different alpha value*/
};
typedef uint8_t color_format_t;

// style enum 
#define BG          0
#define INDIC       1
#define JOY         1
#define ARC         2
#define KNOB        2
#define KNOB_OFF    2
#define KNOB_ON     3
#define PLACEHOLDER 2
#define BTNM_BG     5
#define KEYPAD_BG   5
#define LIST_BG     6

#define TAB_BG              2
#define TAB_RELEASED        3
#define TAB_PRESSED         4
#define TAB_TOGGLE_RELEASED 5
#define TAB_TOGGLE_PRESSED  6

// #define LIST_RELEASED   0
// #define LIST_PRESSED    1
#define LIST_SCRL       2
#define TILEVIEW_SCRL   0
#define SELECT          1


#define RES_WIDTH   LV_HOR_RES
#define RES_HEIGHT  LV_VER_RES

enum {
  GRAD_DIR_NONE,
  GRAD_DIR_VER,
  GRAD_DIR_HOR,
};
typedef uint8_t grad_dir_t;

enum {
  FIT_NONE,  /**< Do not change the size automatically*/
  FIT_TIGHT, /**< Shrink wrap around the children */
  FIT_FLOOD, /**< Align the size to the parent's edge*/
  FIT_FILL,  /**< Align the size to the parent's edge first but if there is an object out of it then get larger */
  _FIT_NUM
};
typedef uint8_t fit_t;

#define FIT_CHILDREN  FIT_TIGHT
#define FIT_PARENT    FIT_FLOOD
#define FIT_MAX       FIT_FILL

enum {
  LAYOUT_OFF = 0, /**< No layout */
  LAYOUT_NONE = 0,
  LAYOUT_CENTER, /**< Center objects */
  LAYOUT_COL_L,  /**< Column left align*/
  LAYOUT_COL_M,  /**< Column middle align*/
  LAYOUT_COL_R,  /**< Column right align*/
  LAYOUT_ROW_T,  /**< Row top align*/
  LAYOUT_ROW_M,  /**< Row middle align*/
  LAYOUT_ROW_B,  /**< Row bottom align*/
  LAYOUT_PRETTY, /**< Put as many object as possible in row and begin a new row*/
  LAYOUT_GRID,   /**< Align same-sized object into a grid*/
  LAYOUT_NUM,
};
typedef uint8_t layout_t;

/** Scrollbar modes: shows when should the scrollbars be visible*/
enum {
  SCROLLBAR_OFF    = 0x0, /**< Never show scrollbars*/
  SCROLLBAR_ON     = 0x1, /**< Always show scrollbars*/
  SCROLLBAR_DRAG   = 0x2, /**< Show scrollbars when page is being dragged*/
  SCROLLBAR_AUTO   = 0x3, /**< Show scrollbars when the scrollable container is large enough to be scrolled*/
  SCROLLBAR_HIDE   = 0x4, /**< Hide the scroll bar temporally*/
  SCROLLBAR_UNHIDE = 0x5, /**< Unhide the previously hidden scrollbar. Recover it\'s type too*/
  SCROLLBAR_AUTOHIDE = 0x2,
};
typedef uint8_t scrollbar_t;


enum {
  BLEND_MODE_NORMAL,
  BLEND_MODE_ADDITIVE,
  BLEND_MODE_SUBTRACTIVE,
};
typedef uint8_t blend_mode_t;

enum
{
  /*The first 4 values are stored in `lv_obj_t` on 2 bits*/
  BIDI_DIR_LTR      = 0x00,
  BIDI_DIR_RTL      = 0x01,
  BIDI_DIR_AUTO     = 0x02,
  BIDI_DIR_INHERIT  = 0x03,

  BIDI_DIR_NEUTRAL  = 0x20,
  BIDI_DIR_WEAK     = 0x21,
};

typedef uint8_t bidi_dir_t;

/**Styles*/
enum {
  RELEASED = 0,
  BUTTON_RELEASED = 0,
  PRESSED = 1,
  BUTTON_PRESSED = 1,
  TOGGLE_RELEASED =2,
  TOGGLE_PRESSED = 3,
  INACTIVE=4,
  BUTTON_INACTIVE = 4,
};
typedef uint8_t button_state_t;

enum {
  BUTTON_PUSH,
  BUTTON_SWITCH,
};
typedef uint8_t button_type_t;

enum {
  BUTTON_MODE_NORMAL,
  BUTTON_MODE_TOGGLE,
  BUTTON_MODE_INACTIVE,
};
typedef uint8_t button_mode_t;

/** Long mode behaviors. Used in 'lv_label_ext_t' */
enum {
  LABEL_LONG_EXPAND,      /**< Expand the object size to the text size*/
  LABEL_LONG_BREAK,       /**< Keep the object width, break the too long lines and expand the object
                                height*/
  LABEL_LONG_DOT,         /**< Keep the size and write dots at the end if the text is too long*/
  LABEL_LONG_SROLL,       /**< Keep the size and roll the text back and forth*/
  LABEL_LONG_SROLL_CIRC,  /**< Keep the size and roll the text circularly*/
  LABEL_LONG_CROP,        /**< Keep the size and crop the text out of it*/
  // LABEL_LONG_AUTO,     /** โหมด Auto สลับให้ระหว่าง LABEL_LONG_EXPAND กับ LABEL_LONG_SROLL_CIRC หากยาวเกิน */
};
typedef uint8_t long_mode_t;

/*Border types (Use 'OR'ed values)*/
enum {
  BORDER_NONE     = 0x00,
  BORDER_BOTTOM   = 0x01,
  BORDER_TOP      = 0x02,
  BORDER_LEFT     = 0x04,
  BORDER_RIGHT    = 0x08,
  BORDER_FULL     = 0x0F,
  BORDER_INTERNAL = 0x10, /**< FOR matrix-like objects (e.g. Button matrix)*/
};
typedef uint8_t border_part_t;

enum {
  KB_MODE_TEXT_LOWER,
  KB_MODE_TEXT_UPPER,
  KB_MODE_SPECIAL,
  KB_MODE_NUM,
};
typedef uint8_t kb_mode_t;

/*Protect some attributes (max. 8 bit)*/
enum {
  PROTECT_NONE      = 0x00,
  PROTECT_CHILD_CHG = 0x01,   /**< Disable the child change signal. Used by the library*/
  PROTECT_PARENT    = 0x02,   /**< Prevent automatic parent change (e.g. in lv_page)*/
  PROTECT_POS       = 0x04,   /**< Prevent automatic positioning (e.g. in lv_cont layout)*/
  PROTECT_FOLLOW    = 0x08,   /**< Prevent the object be followed in automatic ordering (e.g. in
                                      lv_cont PRETTY layout)*/
  PROTECT_PRESS_LOST = 0x10,  /**< If the `indev` was pressing this object but swiped out while
                                      pressing do not search other object.*/
  PROTECT_CLICK_FOCUS = 0x20, /**< Prevent focusing the object by clicking on it*/
};
typedef uint8_t protect_t;

enum {
  SW3D_TYPE_RECT=0,
  SW3D_TYPE_ROUND,
};
typedef uint8_t sw3d_type_t;

enum {
  CTRL_HIDDEN    = 0x0008,
  CTRL_NO_REPEAT = 0x0010,
  CTRL_INACTIVE  = 0x0020,
  CTRL_TOGGLE    = 0x0040,
};
typedef uint16_t button_ctrl_t;

enum {
  LABEL_ALIGN_LEFT, /**< Align text to left */
  LABEL_ALIGN_CENTER, /**< Align text to center */
  LABEL_ALIGN_RIGHT, /**< Align text to right */
};
typedef uint8_t label_align_t;

#define TEXT_ALIGN_LEFT     LABEL_ALIGN_LEFT
#define TEXT_ALIGN_CENTER   LABEL_ALIGN_CENTER
#define TEXT_ALIGN_RIGHT    LABEL_ALIGN_RIGHT

enum {
  TL_DATUM =0, //Top left
  TC_DATUM =1, //Top centre
  TR_DATUM =2, //Top right
  ML_DATUM =3, //Middle left
  MC_DATUM =4, //Middle centre
  MR_DATUM =5, //Middle right
  BL_DATUM =6, //Bottom left
  BC_DATUM =7, //Bottom centre
  BR_DATUM =8, //Bottom right
};
typedef uint8_t label_datum_t;

#define TOP_LEFT_DATUM        TL_DATUM
#define TOP_CENTER_DATUM      TC_DATUM
#define TOP_RIGHT_DATUM       TR_DATUM
#define MID_LEFT_DATUM        ML_DATUM
#define MID_CENTER_DATUM      MC_DATUM
#define MID_RIGHT_DATUM       MR_DATUM
#define BOTTOM_LEFT_DATUM     BL_DATUM
#define BOTTOM_CENTER_DATUM   BC_DATUM
#define BOTTOM_RIGHT_DATUM    BR_DATUM

enum {
  CURSOR_NONE, /**< No cursor */
  CURSOR_LINE, /**< Vertical line */
  CURSOR_BLOCK, /**< Rectangle */
  CURSOR_OUTLINE, /**< Outline around character */
  CURSOR_UNDERLINE, /**< Horizontal line under character */
  CURSOR_HIDDEN = 0x08, /**< This flag can be ORed to any of the other values to temporarily hide the cursor */
};
typedef uint8_t cursor_type_t;

enum {
  MODE_NORMAL,
  MODE_PASSWORD,
};
typedef uint8_t textarea_mode_t;

typedef struct
{
  uint16_t year;
  int8_t month;
  int8_t day;
} calendar_date_t;

enum {
  CHART_TYPE_NONE          = 0x00, /**< Don't draw the series*/
  CHART_TYPE_LINE          = 0x01, /**< Connect the points with lines*/
  CHART_TYPE_COLUMN        = 0x02, /**< Draw columns*/
  CHART_TYPE_POINT         = 0x04, /**< Draw circles on the points*/
  CHART_TYPE_VERTICAL_LINE = 0x08, /**< Draw vertical lines on points (useful when chart width == point count)*/
  CHART_TYPE_AREA          = 0x10, /**< Draw area chart*/
  CHART_TYPE_AREA_FADED    = 0x20, /**< Draw area chart by fading out the bottom of the area*/ //dev-7.0
  CHART_TYPE_SPLINE        = 0x40, /**< Draw spline chart */ // TridentTD
};

// enum {
//   DRAG_DIR_HOR = 0x1, /**< Object can be dragged horizontally. */
//   DRAG_DIR_VER = 0x2, /**< Object can be dragged vertically. */
//   DRAG_DIR_ALL = 0x3, /**< Object can be dragged in all directions. */
// };
// typedef uint8_t drag_dir_t;

enum {
  DRAG_DIR_NONE = 0x0, /**< Both directions are disabled */
  DRAG_DIR_HOR = 0x1, /**< Object can be dragged horizontally. */
  DRAG_DIR_VER = 0x2, /**< Object can be dragged vertically. */
  DRAG_DIR_BOTH = 0x3, /**< Object can be dragged in all directions. */
  DRAG_DIR_ONE = 0x4, /**< Object can be dragged only one direction (the first move). */
};
typedef uint8_t drag_dir_t;

enum {
  SLIDER_TYPE_NORMAL,
  SLIDER_TYPE_SYM,
  SLIDER_TYPE_RANGE
};
typedef uint8_t slider_type_t;

enum {
  OPA_TRANSP = 0,
  OPA_0      = 0,
  OPA_10     = 25,
  OPA_20     = 51,
  OPA_30     = 76,
  OPA_40     = 102,
  OPA_50     = 127,
  OPA_60     = 153,
  OPA_70     = 178,
  OPA_80     = 204,
  OPA_90     = 229,
  OPA_100    = 255,
  OPA_COVER  = 255,
};

enum {
  /*General signals*/
  SIGNAL_CLEANUP, /**< Object is being deleted */
  SIGNAL_CHILD_CHG, /**< Child was removed/added */
  SIGNAL_CORD_CHG, /**< Object coordinates/size have changed */
  SIGNAL_PARENT_SIZE_CHG, /**< Parent's size has changed */
  SIGNAL_STYLE_CHG,    /**< Object's style has changed */
  SIGNAL_BASE_DIR_CHG, /**<The base dir has changed*/
  SIGNAL_REFR_EXT_DRAW_PAD, /**< Object's extra padding has changed */
  SIGNAL_GET_TYPE, /**< LittlevGL needs to retrieve the object's type */

  /*Input device related*/
  SIGNAL_PRESSED,           /**< The object has been pressed*/
  SIGNAL_PRESSING,          /**< The object is being pressed (called continuously while pressing)*/
  SIGNAL_PRESS_LOST,        /**< User is still pressing but slid cursor/finger off of the object */
  SIGNAL_RELEASED,          /**< User pressed object for a short period of time, then released it. Not called if dragged. */
  SIGNAL_LONG_PRESS,        /**< Object has been pressed for at least `LV_INDEV_LONG_PRESS_TIME`.  Not called if dragged.*/
  SIGNAL_LONG_PRESS_REP,    /**< Called after `LV_INDEV_LONG_PRESS_TIME` in every `LV_INDEV_LONG_PRESS_REP_TIME` ms.  Not called if dragged.*/
  SIGNAL_DRAG_BEGIN, 
  SIGNAL_DRAG_THROW_BEGIN,
  SIGNAL_DRAG_END,                                   
  SIGNAL_GESTURE,           /**< Signal Gesture >*/

  /*Group related*/
  SIGNAL_FOCUS,
  SIGNAL_DEFOCUS,
  SIGNAL_CONTROL,
  SIGNAL_GET_EDITABLE,
};
typedef uint8_t signal_t;

enum {
  EVENT_PRESSED,             /**< The object has been pressed*/
  EVENT_PRESSING,            /**< The object is being pressed (called continuously while pressing)*/
  EVENT_PRESS_LOST,          /**< User is still pressing but slid cursor/finger off of the object */
  EVENT_SHORT_CLICKED,       /**< User pressed object for a short period of time, then released it. Not called if dragged. */
  EVENT_LONG_PRESSED,        /**< Object has been pressed for at least `LV_INDEV_LONG_PRESS_TIME`.  Not called if dragged.*/
  EVENT_LONG_PRESSED_REPEAT, /**< Called after `LV_INDEV_LONG_PRESS_TIME` in every
                                     `LV_INDEV_LONG_PRESS_REP_TIME` ms.  Not called if dragged.*/
  EVENT_CLICKED,             /**< Called on release if not dragged (regardless to long press)*/
  EVENT_RELEASED,            /**< Called in every cases when the object has been released*/                                    
  EVENT_DRAG_BEGIN,      
  EVENT_DRAG_END,
  EVENT_DRAG_THROW_BEGIN,
  EVENT_GESTURE,          /**<Gesture> */
  EVENT_KEY,
  EVENT_FOCUSED,
  EVENT_DEFOCUSED,
  EVENT_VALUE_CHANGED,    /**< The object's value has changed (i.e. slider moved) */
  EVENT_INSERT,
  EVENT_REFRESH,
  EVENT_APPLY,  /**< "Ok", "Apply" or similar specific button has clicked*/
  EVENT_CANCEL, /**< "Close", "Cancel" or similar specific button has clicked*/
  EVENT_DELETE, /**< Object is being deleted */
};
typedef uint8_t event_t; /**< Type of event being sent to the object. */

enum {
  GESTURE_DIR_NONE    = -1,  /**< Gesture dir none. */
  GESTURE_DIR_TOP     = 0,   /**< Gesture dir up. */
  GESTURE_DIR_BOTTOM  = 1,   /**< Gesture dir down. */
  GESTURE_DIR_LEFT    = 2,   /**< Gesture dir left. */
  GESTURE_DIR_RIGHT   = 3,   /**< Gesture dir right. */
};
typedef uint8_t gesture_dir_t;

// New color definitions use for all my libraries
// extern uint16_t tft_black;

#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGRAY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGRAY    0x7BEF      /* 128, 128, 128 */ //0xBDD7      // 
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_SKYBLUE     0x867D      /* RGB(135, 206, 235) */
#define TFT_BLUE_LVGL   0x54BB
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_LIGHTGREEN  0x77F3      /* 113, 253, 115 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_LIGHTYELLOW 0xFF93      /* 255, 241, 165 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xF9E0      /* 250,  62,   0 */   // 0xFAE2 /* 255,  94,  19 */   0xFDA0 /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFC9F
#define TFT_SADDLEBROWN 0x8A22      /* #8B4513 RGB(139, 69, 19) */
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */

#define TFT_GREENBLYNK      0x2631
#define TFT_REDBLYNK        0xD20B
#define TFT_YELLOWBLYNK     0xF483
#define TFT_BLUEBLYNK       0x529F  //0x061F
#define TFT_VIOLETBLYNK     0x5BD9

#define TFT_DARKGREENBLYNK  0x00c2
#define TFT_DARKREDBLYNK    0x1841
#define TFT_DARKYELLOWBLYNK 0x2080
#define TFT_DARKBLUEBLYNK   0x0844
#define TFT_DARKVIOLETBLYNK 0x0883

#define TFT_GRAY            0x8410
#define TFT_DIMGRAY         0x6B4D
#define TFT_GRAYBLYNK       0x2106
#define TFT_DARKGRAYBLYNK   0x10C3
#define TFT_DARKGRAYBLYNK2  0x1082
#define TFT_SILVER          0xC618

#define TFT_VIOLET          0x6013
#define TFT_DARKVIOLET      0x2004
#define TFT_PIGBLOOD        0x7804
#define TFT_DARKPIGBLOOD    0x2802
#define TFT_DARKYELLOW      0x2940
#define TFT_DARKRED         0x2800
#define TFT_REDGARNET       0x7802  //91A8 //71A6
#define TFT_EMERALD         0x564F  // 0x2B86
#define TFT_BROWN           0x9A60  /* 150,  75,   0 */

#define TFT_OLDROSE   0xFC10
#define TFT_PINK1     0xFC18
#define TFT_OLDGREEN  0x958B
#define TFT_TRANSPARENT 0x07E0


#define PALETTE_0   0
#define PALETTE_1   1
#define PALETTE_2   2
#define PALETTE_3   3
#define PALETTE_4   4
#define PALETTE_5   5
#define PALETTE_6   6
#define PALETTE_7   7
#define PALETTE_8   8
#define PALETTE_9   9
#define PALETTE_10  10
#define PALETTE_11  11
#define PALETTE_12  12
#define PALETTE_13  13
#define PALETTE_14  14
#define PALETTE_15  15
#define PALETTE_16  16
#define PALETTE_17  17
#define PALETTE_18  18
#define PALETTE_19  19
#define PALETTE_20  20
#define PALETTE_21  21
#define PALETTE_22  22
#define PALETTE_23  23
#define PALETTE_24  24
#define PALETTE_25  25
#define PALETTE_26  26
#define PALETTE_27  27
#define PALETTE_28  28
#define PALETTE_29  29
#define PALETTE_30  30
#define PALETTE_31  31
#define PALETTE_32  32
#define PALETTE_33  33
#define PALETTE_34  34
#define PALETTE_35  35
#define PALETTE_36  36
#define PALETTE_37  37
#define PALETTE_38  38
#define PALETTE_39  39
#define PALETTE_40  40
#define PALETTE_41  41
#define PALETTE_42  42
#define PALETTE_43  43
#define PALETTE_44  44
#define PALETTE_45  45
#define PALETTE_46  46
#define PALETTE_47  47
#define PALETTE_48  48
#define PALETTE_49  49
#define PALETTE_50  50
#define PALETTE_51  51
#define PALETTE_52  52
#define PALETTE_53  53
#define PALETTE_54  54
#define PALETTE_55  55
#define PALETTE_56  56
#define PALETTE_57  57
#define PALETTE_58  58
#define PALETTE_59  59
#define PALETTE_60  60
#define PALETTE_61  61
#define PALETTE_62  62
#define PALETTE_63  63
#define PALETTE_64  64
#define PALETTE_65  65
#define PALETTE_66  66
#define PALETTE_67  67
#define PALETTE_68  68
#define PALETTE_69  69
#define PALETTE_70  70
#define PALETTE_71  71
#define PALETTE_72  72
#define PALETTE_73  73
#define PALETTE_74  74
#define PALETTE_75  75
#define PALETTE_76  76
#define PALETTE_77  77
#define PALETTE_78  78
#define PALETTE_79  79
#define PALETTE_80  80
#define PALETTE_81  81
#define PALETTE_82  82
#define PALETTE_83  83
#define PALETTE_84  84
#define PALETTE_85  85
#define PALETTE_86  86
#define PALETTE_87  87
#define PALETTE_88  88
#define PALETTE_89  89
#define PALETTE_90  90
#define PALETTE_91  91
#define PALETTE_92  92
#define PALETTE_93  93
#define PALETTE_94  94
#define PALETTE_95  95
#define PALETTE_96  96
#define PALETTE_97  97
#define PALETTE_98  98
#define PALETTE_99  99
#define PALETTE_100 100
#define PALETTE_101 101
#define PALETTE_102 102
#define PALETTE_103 103
#define PALETTE_104 104
#define PALETTE_105 105
#define PALETTE_106 106
#define PALETTE_107 107
#define PALETTE_108 108
#define PALETTE_109 109
#define PALETTE_110 110
#define PALETTE_111 111
#define PALETTE_112 112
#define PALETTE_113 113
#define PALETTE_114 114
#define PALETTE_115 115
#define PALETTE_116 116
#define PALETTE_117 117
#define PALETTE_118 118
#define PALETTE_119 119
#define PALETTE_120 120
#define PALETTE_121 121
#define PALETTE_122 122
#define PALETTE_123 123
#define PALETTE_124 124
#define PALETTE_125 125
#define PALETTE_126 126
#define PALETTE_127 127
#define PALETTE_128 128
#define PALETTE_129 129
#define PALETTE_130 130
#define PALETTE_131 131
#define PALETTE_132 132
#define PALETTE_133 133
#define PALETTE_134 134
#define PALETTE_135 135
#define PALETTE_136 136
#define PALETTE_137 137
#define PALETTE_138 138
#define PALETTE_139 139
#define PALETTE_140 140
#define PALETTE_141 141
#define PALETTE_142 142
#define PALETTE_143 143
#define PALETTE_144 144
#define PALETTE_145 145
#define PALETTE_146 146
#define PALETTE_147 147
#define PALETTE_148 148
#define PALETTE_149 149
#define PALETTE_150 150
#define PALETTE_151 151
#define PALETTE_152 152
#define PALETTE_153 153
#define PALETTE_154 154
#define PALETTE_155 155
#define PALETTE_156 156
#define PALETTE_157 157
#define PALETTE_158 158
#define PALETTE_159 159
#define PALETTE_160 160
#define PALETTE_161 161
#define PALETTE_162 162
#define PALETTE_163 163
#define PALETTE_164 164
#define PALETTE_165 165
#define PALETTE_166 166
#define PALETTE_167 167
#define PALETTE_168 168
#define PALETTE_169 169
#define PALETTE_170 170
#define PALETTE_171 171
#define PALETTE_172 172
#define PALETTE_173 173
#define PALETTE_174 174
#define PALETTE_175 175
#define PALETTE_176 176
#define PALETTE_177 177
#define PALETTE_178 178
#define PALETTE_179 179
#define PALETTE_180 180
#define PALETTE_181 181
#define PALETTE_182 182
#define PALETTE_183 183
#define PALETTE_184 184
#define PALETTE_185 185
#define PALETTE_186 186
#define PALETTE_187 187
#define PALETTE_188 188
#define PALETTE_189 189
#define PALETTE_190 190
#define PALETTE_191 191
#define PALETTE_192 192
#define PALETTE_193 193
#define PALETTE_194 194
#define PALETTE_195 195
#define PALETTE_196 196
#define PALETTE_197 197
#define PALETTE_198 198
#define PALETTE_199 199
#define PALETTE_200 200
#define PALETTE_201 201
#define PALETTE_202 202
#define PALETTE_203 203
#define PALETTE_204 204
#define PALETTE_205 205
#define PALETTE_206 206
#define PALETTE_207 207
#define PALETTE_208 208
#define PALETTE_209 209
#define PALETTE_210 210
#define PALETTE_211 211
#define PALETTE_212 212
#define PALETTE_213 213
#define PALETTE_214 214
#define PALETTE_215 215
#define PALETTE_216 216
#define PALETTE_217 217
#define PALETTE_218 218
#define PALETTE_219 219
#define PALETTE_220 220
#define PALETTE_221 221
#define PALETTE_222 222
#define PALETTE_223 223
#define PALETTE_224 224
#define PALETTE_225 225
#define PALETTE_226 226
#define PALETTE_227 227
#define PALETTE_228 228
#define PALETTE_229 229
#define PALETTE_230 230
#define PALETTE_231 231
#define PALETTE_232 232
#define PALETTE_233 233
#define PALETTE_234 234
#define PALETTE_235 235
#define PALETTE_236 236
#define PALETTE_237 237
#define PALETTE_238 238
#define PALETTE_239 239
#define PALETTE_240 240
#define PALETTE_241 241
#define PALETTE_242 242
#define PALETTE_243 243
#define PALETTE_244 244
#define PALETTE_245 245
#define PALETTE_246 246
#define PALETTE_247 247
#define PALETTE_248 248
#define PALETTE_249 249
#define PALETTE_250 250
#define PALETTE_251 251
#define PALETTE_252 252
#define PALETTE_253 253
#define PALETTE_254 254
#define PALETTE_255 255

// #define STYLE_SCREEN                 &lv_style_scr
// #define STYLE_TRANSPARENT            &lv_style_transp
// #define STYLE_TRANSPARENT_FIT        &lv_style_transp_fit
// #define STYLE_TRANSPARENT_TIGHT      &lv_style_transp_tight
// #define STYLE_PLAIN                  &lv_style_plain
// #define STYLE_PLAIN_COLOR            &lv_style_plain_color
// #define STYLE_PRETTY                 &lv_style_pretty
// #define STYLE_PRETTY_COLOR           &lv_style_pretty_color
// #define STYLE_BUTTON_RELEASED        &lv_style_btn_rel
// #define STYLE_BUTTON_PRESSED         &lv_style_btn_pr
// #define STYLE_BUTTON_TOGGLE_RELEASED &lv_style_btn_tgl_rel
// #define STYLE_BUTTON_TOGGLE_PRESSED  &lv_style_btn_tgl_pr
// #define STYLE_BUTTON_INACTIVATE      &lv_style_btn_ina
// #define STYLE_SCREEN                 &lv_style_scr

#define STYLE_TRANSPARENT             GStyle_transp
#define STYLE_TRANSPARENT_FIT         GStyle_transp_fit
#define STYLE_TRANSPARENT_TIGHT       GStyle_transp_tight
#define STYLE_PLAIN                   GStyle_plain
#define STYLE_PLAIN_COLOR             GStyle_plain_color
#define STYLE_PRETTY                  GStyle_pretty
#define STYLE_PRETTY_COLOR            GStyle_pretty_color
#define STYLE_BUTTON_RELEASED         GStyle_btn_rel
#define STYLE_BUTTON_PRESSED          GStyle_btn_pr
#define STYLE_BUTTON_TOGGLE_RELEASED  GStyle_btn_tgl_rel
#define STYLE_BUTTON_TOGGLE_PRESSED   GStyle_btn_tgl_pr
#define STYLE_BUTTON_INACTIVATE       GStyle_btn_ina


#define SYMBOL_AUDIO           "\xef\x80\x81"   // \uf001
#define SYMBOL_VIDEO           "\xef\x80\x88"   // \uf008
#define SYMBOL_LIST            "\xef\x80\x8b"   // \uf00b
#define SYMBOL_OK              "\xef\x80\x8c"   // \uf00c
#define SYMBOL_CLOSE           "\xef\x80\x8d"   // \uf00d
#define SYMBOL_POWER           "\xef\x80\x91"   // \uf011
#define SYMBOL_SETTINGS        "\xef\x80\x93"   // \uf013
#define SYMBOL_TRASH           "\xef\x8B\xAD"   // \uf2ed   old "\xef\x80\x94"   // \uf014
#define SYMBOL_HOME            "\xef\x80\x95"   // \uf015
#define SYMBOL_DOWNLOAD        "\xef\x80\x99"   // \uf019

#define SYMBOL_DRIVE           "\xef\x80\x9c"   // \uf01c
#define SYMBOL_REFRESH         "\xef\x80\xa1"   // \uf021
#define SYMBOL_MUTE            "\xef\x80\xa6"   // \uf026
#define SYMBOL_VOLUME_MID      "\xef\x80\xa7"   // \uf027
#define SYMBOL_VOLUME_MAX      "\xef\x80\xa8"   // \uf028
#define SYMBOL_IMAGE           "\xef\x80\xbe"   // \uf03e
#define SYMBOL_EDIT            "\xef\x81\x80"   // \uf040
#define SYMBOL_PREV            "\xef\x81\x88"   // \uf048
#define SYMBOL_PLAY            "\xef\x81\x8b"   // \uf04b
#define SYMBOL_PAUSE           "\xef\x81\x8c"   // \uf04c
#define SYMBOL_STOP            "\xef\x81\x8d"   // \uf04d
#define SYMBOL_NEXT            "\xef\x81\x91"   // \uf051
#define SYMBOL_EJECT           "\xef\x81\x92"   // \uf052
#define SYMBOL_LEFT            "\xef\x81\x93"   // \uf053
#define SYMBOL_RIGHT           "\xef\x81\x94"   // \uf054
#define SYMBOL_ARROW_LEFT      "\xef\x81\xa0"   // \uf060
#define SYMBOL_ARROW_RIGHT     "\xef\x81\xa1"   // \uf061
#define SYMBOL_ARROW_UP        "\xef\x81\xa2"   // \uf062
#define SYMBOL_ARROW_DOWN      "\xef\x81\xa3"   // \uf063
#define SYMBOL_PLUS            "\xef\x81\xa7"   // \uf067
#define SYMBOL_MINUS           "\xef\x81\xa8"   // \uf068

#define SYMBOL_WARNING         "\xef\x81\xb1"   // \uf071
#define SYMBOL_SHUFFLE         "\xef\x81\xb4"   // \uf074
#define SYMBOL_UP              "\xef\x81\xb7"   // \uf077
#define SYMBOL_DOWN            "\xef\x81\xb8"   // \uf078
#define SYMBOL_LOOP            "\xef\x81\xb9"   // \uf079
#define SYMBOL_DIRECTORY       "\xef\x81\xbb"   // \uf07b
#define SYMBOL_UPLOAD          "\xef\x82\x93"   // \uf093
#define SYMBOL_CALL            "\xef\x82\x95"   // \uf095
#define SYMBOL_CUT             "\xef\x83\x84"   // \uf0c4
#define SYMBOL_COPY            "\xef\x83\x85"   // \uf0c5
#define SYMBOL_SAVE            "\xef\x83\x87"   // \uf0c7
#define SYMBOL_CHARGE          "\xef\x83\xa7"   // \uf0e7
#define SYMBOL_BELL            "\xef\x83\xb3"   // \uf0f3
#define SYMBOL_KEYBOARD        "\xef\x84\x9c"   // \uf11c
#define SYMBOL_GPS             "\xef\x84\xa4"   // \uf124
#define SYMBOL_FILE            "\xef\x85\x9b"   // \uf15b
#define SYMBOL_ARROW_LONG_LEFT "\xef\x85\xb7"   // \uf177
#define SYMBOL_WIFI            "\xef\x87\xab"   // \uf1eb
#define SYMBOL_BATTERY_FULL    "\xef\x89\x80"   // \uf240
#define SYMBOL_BATTERY_3       "\xef\x89\x81"   // \uf241
#define SYMBOL_BATTERY_2       "\xef\x89\x82"   // \uf242
#define SYMBOL_BATTERY_1       "\xef\x89\x83"   // \uf243
#define SYMBOL_BATTERY_EMPTY   "\xef\x89\x84"   // \uf244
#define SYMBOL_BLUETOOTH       "\xef\x8a\x93"   // \uf293
#define SYMBOL_THERMO_4        "\xef\x8b\x87"   // \uf2c7
#define SYMBOL_THERMO_3        "\xef\x8b\x88"   // \uf2c8
#define SYMBOL_THERMO_2        "\xef\x8b\x89"   // \uf2c9
#define SYMBOL_THERMO_1        "\xef\x8b\x8a"   // \uf2ca
#define SYMBOL_THERMO_0        "\xef\x8b\x8b"   // \uf2cb
#define SYMBOL_THERMO          SYMBOL_THERMO_4 
#define SYMBOL_HUMID           "\xEF\x81\x83"   // \uf043
#define SYMBOL_SHOWER          "\xef\x8b\x8c"   // \uf2cc
#define SYMBOL_BATHROOM        "\xef\x8b\x8d"   // \uf2cd
#define SYMBOL_WIFI_1          "\xef\x8b\xa1"   // \uf2e1
#define SYMBOL_WIFI_2          "\xef\x8b\xa2"   // \uf2e2
#define SYMBOL_WIFI_3          "\xef\x8b\xa3"   // \uf2e3
#define SYMBOL_WIFI_4          "\xef\x8b\xa4"   // \uf2e4
#define SYMBOL_WIFI_NO_CON_1   "\xef\x8b\xa5"   // \uf2e5
#define SYMBOL_WIFI_NO_CON_2   "\xef\x8b\xa6"   // \uf2e6
#define SYMBOL_ENTER           "\xef\x8b\xa7"   // \uf2e7     "\xef\x8b\xa1"
#define SYMBOL_SD_CARD         "\xef\x9F\x82"   /*63426, 0xF7C2*/
#define SYMBOL_NEW_LINE        "\xef\xA2\xA2"   /*63650, 0xF8A2*/
#define SYMBOL_DUMMY           "\xEF\xA3\xBF"   // \uf8ff
#define SYMBOL_OPTION          "\xef\x85\x82"   // \uf142
#define SYMBOL_ALARM           "\xef\x8b\xa8"   // \uf2e8
#define SYMBOL_EYE_OPEN        "\xef\x81\xae"   // \uf06e
#define SYMBOL_EYE_CLOSE       "\xef\x81\xb0"   // \uf070
#define SYMBOL_DEGREE          "°"
#define SYMBOL_MICRO           "µ"
#define SYMBOL_UMBRELLA        "\xef\x83\xa9"   // \uf0e9
#define SYMBOL_WALKING         "\xef\x8b\xa9"   // \uf2e9
#define SYMBOL_BACKSPACE       "\xef\x8b\xaa"   // \uf2ea


#ifndef FONT_DECLARE
#define FONT_DECLARE          LV_FONT_DECLARE
#endif

#ifndef IMG_DECLARE
#define IMG_DECLARE           LV_IMG_DECLARE
#endif

#ifndef EVENT_CB_DECLARE
#define EVENT_CB_DECLARE      EVENT_CB_DECLARE
#endif

#ifndef COLOR_MAKE
#define COLOR_MAKE(r,g,b)     ({ lv_color_make(r,g,b).full; })
#endif

#ifndef COLOR_MIX
#define COLOR_MIX(c1,c2,a)    ({ lv_color_mix(c1,c2,a); })
#endif

#ifndef COLOR_TO1
#define COLOR_TO1(c)          ({ lv_color_t _c; _c.full = c; lv_color_to1(_c); })
#endif

#ifndef COLOR_TO8
#define COLOR_TO8(c)          ({ lv_color_t _c; _c.full = c; lv_color_to8(_c); })
#endif


#ifndef BEZIER3
#define BEZIER3(t,u0,u1,u2,u3)    ({lv_bezier3(t,u0,u1,u2,u3); })
#endif



#endif //__BLYNKGO_TYPE_H__