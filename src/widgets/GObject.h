/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GObject.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GOBJECT_H__
#define __GOBJECT_H__

#include "GStyle.h"
#include "GUtils.h"
#include "Point.h"
#include "../utils/TridentTD_ESP32NVS/TridentTD_ESP32NVS.h"  // ESP32 NVS


class GObject;
extern GObject GScreen;

#define GWidget  GObject
#define GParam   GWidget* widget

typedef void (*event_cb_t)      (GWidget* widget, event_t event);
typedef void (*event_handler_t) (GWidget* parent);
void main_event_cb(lv_obj_t *obj, lv_event_t event);

// API สำหรับสร้าง Trident Keyboard (Eng-Thai)
// void kbd_create(GWidget* widget);  // GWidget เป็นได้ คือ GLabel กับ GTextArea
void kbd_create(GWidget* widget, kbd_lang_t lang2nd);
void kbd_delete();
void kbd_display_numpad();

class GObject {
  public:
    GObject(GWidget& parent=GScreen);
    GObject(GWidget* parent);
    virtual ~GObject();
    
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            
    void        del();          /* ลบแบบปกติสำหรับ GWidget ใดๆที่สร้างแบบปกติ ไม่ได้สร้างด้วย new */
    void        del_async();    /* ลบแบบปกติ async() */
    void        clean();        /* ลบ child ออกทั้งหมด */

    String      type();
    bool        isType(String type);
    bool        isCreated()             {return _created;}
    bool        hasLocalStyle()         {return _has_localstyle;}
    bool        isLinkStyle()           {return _is_linkstyle;}
    void        setObject(obj_t * obj);

    bool        sameWith(GWidget &base);
    bool        sameWith(GWidget *base);
    bool        operator ==(GWidget &base);
    bool        operator ==(GWidget *base);
    bool        operator !=(GWidget &base);

    /* --------- Parent & Child API ------- */
    void        parent(GWidget* parent);
    void        parent(GWidget& parent);
    GWidget&    parent();
    GWidget*    child(bool start=false);
    GWidget*    child_back(bool start=false);
    uint16_t    child_num();
    uint16_t    child_num_recursive();
    void        child_del_recursive();

    /* -------- Call Back API ------------ */
    void        event_main(event_t event);
    void        event_cb(event_cb_t event_cb);
    event_cb_t  event_cb();
    void        signal_cb(signal_cb_t signal_cb);
    signal_cb_t signal_cb();
    void        design_cb(design_cb_t design_cb);
    design_cb_t design_cb();

    void        user_data(void* user_data);
    void*       user_data();
    void*       ext_attr();
    void*       alloc_ext_attr(uint16_t ext_size);

    void        invalidate();
    void        refreshStyle();

    /* -------- POSITION API --------  */
    void        auto_realign(bool enable);
    bool        auto_realign();
    void        realign();
    void        reset_align();

    void        align(align_t Align, int16_t offset_x=0, int16_t offset_y=0);
    void        align(align_t Align, Point offset);
    void        align(GWidget &base, align_t Align, int16_t offset_x=0, int16_t offset_y=0);
    void        align(GWidget &base, align_t Align, Point offset);
    void        align(GWidget *base, align_t Align, int16_t offset_x=0, int16_t offset_y=0);
    void        align(GWidget *base, align_t Align, Point offset);
    
    void        align_center();
    void        align_center(int16_t offset_x, int16_t offset_y);
    void        align_center(Point offset);
    void        align_center(GWidget &base, int16_t offset_x=0, int16_t offset_y=0);
    void        align_center(GWidget *base, int16_t offset_x, int16_t offset_y);

    void        c_align(align_t Align, int16_t offset_x =0, int16_t offset_y =0);
    void        c_align(align_t Align, Point offset);
    void        c_align(GWidget &base, align_t Align, int16_t offset_x=0, int16_t offset_y=0);
    void        c_align(GWidget &base, align_t Align, Point offset);

    void        offset(int16_t offset_x, int16_t offset_y); // set Offset by cur align
    void        offset(Point offset);                       // set Offset by cur align
    void        offsetX(int16_t offset_x);
    int16_t     offsetX();
    void        offsetY(int16_t offset_y);
    int16_t     offsetY();

    Point       centerpoint();          // จุดศก.วิตเจ็ต (พิกัดที่อยู่ภายใน วิตเจ็ต)
    Point       center();               // center of the widget  relative to parent (จุดศก.ของ widget อยู่พิกัดไหนเทียบจาก parent)
    void        center(Point p);        // set center of the widget relative to parent
    void        center(int16_t x, int16_t y);   // set center of the widget relative to parent
    Point       position();   // return topleft point of the object
    void        position(Point p);
    void        position(int16_t x, int16_t y); //topleft

    void        pos(int16_t x, int16_t y);
    void        posX(int16_t x);
    void        posY(int16_t y);
    int16_t     posX();     // top-left x
    int16_t     posY();     // top-left y
    area_t      coordinate();
    int16_t     leftX();    // left x
    int16_t     rightX();   // right x
    int16_t     topY();     // top y
    int16_t     bottomY();  // bottom y
    int16_t     centerX();
    int16_t     centerY();

    inline      Point top_left()                { return Point(this->leftX(), this->topY()); }
    inline      Point top_right()               { return Point(this->rightX(), this->topY()); }
    inline      Point bottom_left()             { return Point(this->leftX(), this->bottomY()); }
    inline      Point bottom_right()            { return Point(this->rightX(), this->bottomY()); }


    void        top(bool enable);
    bool        top();
    void        toForeground();
    void        toBackground();

    /* -------- SIZE API --------  */
    void        size(uint16_t w, uint16_t h);
    void        size(GWidget& widget);
    void        size(GWidget* widget);
    inline Point size()                         { return Point(this->width(), this->height()); }
    void        size_fit(GWidget& widget);
    void        size_fit(GWidget* widget);
    void        width(uint16_t w);
    void        width(GWidget& widget);
    void        width(GWidget* widget);
    uint16_t    width();
    uint16_t    width_fit();
    void        height(uint16_t h);
    void        height(GWidget& widget);
    void        height(GWidget* widget);
    uint16_t    height();
    uint16_t    height_fit();


    /* -------- STYLE API --------  */
    void        createLocalStyle(style_t *base_style=NULL); // for inner use.. don't call (alloc & copy)
    void        freeLocalStyle();          // for inner use .. don't call (free alloc mem)
    void        copyStyle(style_t &style); // alloc & copy 
    void        copyStyle(style_t *style); // alloc & copy

    /* ใช้ API เหล่านี้แทน */
    void        copyStyle(GStyle &style);  // alloc & copy  ถ้ายังไม่ alloc จะ alloc&copy ถ้า alloc แล้วแค่ copy
    void        copyStyle(GStyle *style);  // alloc & copy
    void        copyStyle(GWidget &object);
    void        copyStyle(GWidget *object);
    void        linkStyle(GStyle &style);  // link from static GStyle จำไม่สามารถ แก้ไข GStyle ได้ หาก link
    void        linkStyle(GStyle *style);  // link from static GStyle
    void        linkStyle(GWidget &object);  // link from another Widget
    void        linkStyle(GWidget *object);  // link from another Widget
    GStyle*     getStyle();

    void        color(color_t color);
    void        color(color_t color, color_t gradian_color);
    color_t     color();
    void        grad_color(uint16_t gradian_color);
    uint16_t    grad_color();
    void        grad_dir(grad_dir_t type);      // GRAD_DIR_NONE, GRAD_DIR_VER (default), GRAD_DIR_HOR
    grad_dir_t  grad_dir();
    void        main_color_stop(uint8_t stop); // 0..255
    uint8_t     main_color_stop();
    void        grad_color_stop(uint8_t stop);
    uint8_t     grad_color_stop();
    void        opa(uint8_t opa);  // 0..255
    uint8_t     opa();

    void        opa_scalable(bool enable);  // ทำให้ child ทุก child ก้ปรับ opa ตาม แม่
    bool        opa_scalable();
    void        opa_scale(uint8_t opa_scale);
    uint8_t     opa_scale();

    void        padding(int16_t padding);  // for all paddings
    int16_t     padding();
    void        padding_inner(int16_t padding_inner);
    void        padding_top(int16_t padding_top);
    void        padding_bottom(int16_t padding_bottom);
    void        padding_left(int16_t padding_left);
    void        padding_right(int16_t padding_right);
    int16_t     padding_inner();
    int16_t     padding_top();
    int16_t     padding_bottom();
    int16_t     padding_left();
    int16_t     padding_right();
    inline void padding(uint16_t padding_top,uint16_t padding_bottom, uint16_t padding_left,uint16_t padding_right, uint16_t padding_inner) 
                {  
                    this->padding_top(padding_top);
                    this->padding_bottom(padding_bottom);
                    this->padding_left(padding_left);
                    this->padding_right(padding_right);
                    this->padding_inner(padding_inner);
                }
    inline void padding_top_bottom(uint16_t padding)
                {
                    this->padding_top(padding);
                    this->padding_bottom(padding);
                }

    inline void padding_top_bottom(uint16_t padding_top, uint16_t padding_bottom)
                {
                    this->padding_top(padding_top);
                    this->padding_bottom(padding_bottom);
                }

    inline void padding_left_right(uint16_t padding)
                {
                    this->padding_left(padding);
                    this->padding_right(padding);
                }

    inline void padding_left_right(uint16_t padding_left, uint16_t padding_right)
                {
                    this->padding_left(padding_left);
                    this->padding_right(padding_right);
                }

    void        setCircle(bool enable);
    void        corner_radius(uint16_t r);
    inline void corner_radius(uint16_t r, bool corner_mask)  { this->corner_radius(r); this->corner_mask(corner_mask); }
    uint16_t    corner_radius();
    void        corner_mask(bool enable);
    bool        corner_mask();

    inline void        radius(uint16_t r)               { this->corner_radius(r);       }
    inline void        radius(uint16_t r, bool mask)    { this->corner_radius(r, mask); }
    inline uint16_t    radius()                         { return this->corner_radius(); }

    void        border(uint16_t width);
    void        border(uint16_t width, color_t color);
    uint16_t    border();
    void        border_color(color_t color);
    color_t     border_color();
    void        border_part(border_part_t part_type); /* BORDER_NONE, BORDER_BOTTOM, BORDER_TOP, BORDER_LEFT, BORDER_RIGHT, BORDER_FULL, BORDER_INTERNAL*/
    border_part_t border_part();
    void        border_opa(uint8_t opa);
    uint8_t     border_opa();

    void        shadow(uint16_t width);
    void        shadow(uint16_t width, color_t color);
    uint16_t    shadow();
    void        shadow_color(color_t color);
    color_t     shadow_color();
    void        shadow_opa(uint8_t opa);
    uint8_t     shadow_opa();
    void        shadow_offset(point_t point);
    void        shadow_offset(int16_t offset_x, int16_t offset_y);
    point_t     shadow_offset();
    void        shadow_spread(uint16_t spread);
    uint16_t    shadow_spread();

    /* line style */
    void        line_width(uint16_t width);
    void        line_width(uint16_t width, color_t color);
    uint16_t    line_width();
    void        line_color(color_t color);
    color_t     line_color();
    void        line_opa(uint8_t opa);
    uint8_t     line_opa();
    void        line_rounded(bool enable);
    bool        line_rounded();
    void        image_color(color_t color);
    color_t     image_color();
    void        image_intense(uint8_t opa);
    uint8_t     image_intense();
    void        image_opa(uint8_t opa);
    uint8_t     image_opa();
    inline void image_filter(color_t color, uint8_t intense)    { this->image_color(color); this->image_intense(intense); }
    inline void image_reset()                                   { this->image_intense(0); }

    /* text style */
    void        font(font_t &font);                         // กำหนด ฟอนต์
    void        font(font_t &font, color_t color);          // กำหนด ฟอนต์พร้อมสี

    inline void font(font_t *font)                              { this->font(*font); }
    inline void font(font_t *font, color_t color)               { this->font(*font, color); }

    const font_t* font();                                   // คืนค่า  ฟอนต์
    int16_t     font_height();                              // คืนค่า  ความสูงฟอนต์
    void        text_color(color_t color);                  // กำหนด สีข้อความ  (ใช้ font_color(..) ก็ได้)
    color_t     text_color();                               // คืนค่า  สีข้อความ   (ใช้ font_color() ก็ได้)
    void        text_opa(uint8_t opa);                      // กำหนด ความโปร่งใสข้อความ ; opa : 0-255
    uint8_t     text_opa();                                 // คืนค่า  ความโปร่งใสข้อความ
    void        text_sel_color(color_t color);              // กำหนด สีสำหรับเลือกข้อความ
    color_t     text_sel_color();                           // คืนค่า  สีสำหรับเลือกข้อความ
    void        text_line_space(int16_t line_space);        // กำหนด ระยะห่างระหว่างบรรทัด
    int16_t     text_line_space();                          // คืนค่า  ระยะห่างระหว่างบรรทัด
    void        text_letter_space(int16_t letter_space);    // กำหนด ระยะห่างระหว่างอักษร
    int16_t     text_letter_space();                        // คืนค่า  ระยะห่างระหว่างอักษร
    void        text_blend_mode(uint8_t blend_mode);        // กำหนด text blend mode  ; blend_mode : BLEND_MODE_NORMAL, BLEND_MODE_ADDITIVE, BLEND_MODE_SUBTRACTIVE
    uint8_t     text_blend_mode();                          // คืนค่า  text blend mode  
    void        underline(bool enable);                     // กำหนด มีเส้น underline
    bool        underline();                                // คืนค่า  มีเส้น underline หรือไม่
    void        strikethrough(bool enable);                 // กำหนด มีเส้นขีดคร่อมกลางข้อความ
    bool        strikethrough();                            // คืนค่า  มีเส้นขีดคร่อมกลางข้อความ หรือไม่

    /* text style alias */
    inline void        font_color(color_t color)         { this->text_color(color); }
    inline color_t     font_color()                      { return this->text_color(); }
    inline void        font_opa(uint8_t opa)             { this->text_opa(opa); }
    inline uint8_t     font_opa()                        { return this->text_opa(); }
    inline void        line_space(uint16_t line_space)   { this->text_line_space(line_space); }
    inline uint16_t    line_space()                      { return this->text_letter_space(); }
    inline void        letter_space(uint16_t letter_space) { this->text_letter_space(letter_space); }
    inline uint16_t    letter_space()                    { return this->text_letter_space(); }

    /* -------------- event part 1-------------*/
    void        clickable(bool enable);
    bool        clickable();
    void        dragable(bool enable);
    bool        dragable();
    void        drag_throw(bool enable);
    bool        drag_throw();
    void        drag_direction(drag_dir_t direction); // DRAG_DIR_NONE, DRAG_DIR_HOR, DRAG_DIR_VER, DRAG_DIR_BOTH, DRAG_DIR_ONE
    drag_dir_t  drag_direction();
    void        drag_parent(bool enable);  // ส่งการ scroll ไปให้ parent ที่เป็น page ให้สามารถ scroll ได้
    bool        drag_parent();
    inline bool isDraging()                         { return _is_draging; }
    void        gesture(bool enable);
    bool        gesture();
    void        gesture_parent(bool enable);
    bool        gesture_parent();

    /* etc API */
    point_t     touch_point(bool absolute=false);  /* ตำแหน่ง touch ล่าสุด absolute/relative point*/
    point_t     touch_vector();            /* ทิศทางของ touch */
    gesture_dir_t touch_gesture();         // gesture ของ touch  ต้องเปิด gesture(true); ก่อนเรียกคำสั่งนี้ได้   GESTURE_DIR_NONE, GESTURE_DIR_TOP, GESTURE_DIR_BOTTOM, GESTURE_DIR_LEFT, GESTURE_DIR_RIGHT
    void        finish_drag();             /* ให้หยุดการ drag เมื่อมีการ drag เกิดขึ้น */
    void        page_glue(bool enable);    /* ให้ติดไปกับ page ด้วยไหม */
    void        hidden(bool enable);
    bool        hidden();
    inline void visible(bool enable)                {  hidden(!enable);  }
    inline bool visible()                           {  return !hidden(); }
    inline void show(bool enable)                   {  hidden(!enable);  }
    inline bool show()                              {  return !hidden(); }
    void        protect(protect_t type, bool enable=true);   // PROTECT_NONE, PROTECT_CHILD_CHG, PROTECT_PARENT, PROTECT_POS, PROTECT_FOLLOW, PROTECT_PRESS_LOST, PROTECT_CLICK_FOCUS
    bool        isProtected(protect_t type);



    // bool        capture(const char* filename=NULL);

    /* -------------- event part 2-------------*/
    bool        event_send(event_t event, const void * event_data=NULL);
    const void *event_data();
    void        parent_event(bool enable);  // ส่ง event ไปให้ parent ด้วย
    inline void event_parent(bool enable)       { this->parent_event(enable); }

    
    event_handler_t  fn_onclicked = NULL;
    event_handler_t  fn_onpressed = NULL;
    event_handler_t  fn_onpressing = NULL;
    event_handler_t  fn_onreleased = NULL;
    event_handler_t  fn_onlongpressed = NULL;
    event_handler_t  fn_onlongpressed_repeat = NULL;
    event_handler_t  fn_onvaluechanged = NULL;
    event_handler_t  fn_ondelete  = NULL;
    event_handler_t  fn_ongesture = NULL;

    void onClicked(event_handler_t fn)          { if(!this->isCreated()) this->create();  fn_onclicked = fn; }
    void onPressed(event_handler_t fn)          { if(!this->isCreated()) this->create();  fn_onpressed = fn; }
    void onPressing(event_handler_t fn)         { if(!this->isCreated()) this->create();  fn_onpressing = fn; }
    void onReleased(event_handler_t fn)         { if(!this->isCreated()) this->create();  fn_onreleased = fn; }
    void onLongPressed(event_handler_t fn)      { if(!this->isCreated()) this->create();  fn_onlongpressed = fn; }
    void onLongPressedRepeat(event_handler_t fn){ if(!this->isCreated()) this->create();  fn_onlongpressed_repeat = fn; }
    void onValueChanged(event_handler_t fn)     { if(!this->isCreated()) this->create();  fn_onvaluechanged = fn; }
    void onDelete(event_handler_t fn)           { if(!this->isCreated()) this->create();  fn_ondelete = fn; }
    void onGesture(event_handler_t fn)          { if(!this->isCreated()) this->create();  fn_ongesture = fn; }



    // สำหรับ temp ชั่วคราว โดยการ link obj จากภายนอกมาไว้ไม่ได้มีการสร้างจริง
    // เรียก temp_mode(true); ก่อนใช้ temp(..) และ เลิกใช้ด้วย temp_mode(false);
    inline void temp_mode(bool enable)     { if(enable) { this->_created = true; } else { this->obj = NULL; this->_created = false; } }
    inline void temp(GWidget& link_obj)    { if(this->isCreated()) this->obj = link_obj.obj; }
    inline void temp(GWidget* link_obj)    { if(this->isCreated()) this->obj = link_obj->obj; }

    obj_t*   obj            = NULL;
    GWidget*_par            = NULL;
    bool    _created        = false;
    bool    _has_localstyle = false;
    bool    _is_draging     = false;
  protected:
    void*   _user_data      = NULL;
    GStyle* _style          = NULL;
    obj_t*  _child          = NULL;
    event_cb_t _event_cb    = NULL;


    bool    _is_linkstyle = false;


};



#endif //__GOBJECT_H__
