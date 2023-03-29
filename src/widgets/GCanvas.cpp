/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GCanvas.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GCanvas.h"
extern "C" {
#include "../qrcode/qrcodegen.h" 
}
#include "../qrcode/promptpay.h"

#define LV_OBJX_NAME  "GCanvas"

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
#if BLYNGO_DEBUG
    Serial.println("[GCanvas] signal cleanup");
#endif
    GCanvas* pCanvas = (GCanvas*) lv_obj_get_user_data(obj);
    if(pCanvas->hasLocalStyle()) { pCanvas->freeLocalStyle(); }
    if(pCanvas->cbuf != NULL) { free(pCanvas->cbuf); pCanvas->cbuf = NULL; }
    pCanvas->_created = false;
  }
  return LV_RES_OK;
}

GCanvas::GCanvas(GWidget& parent) : _has_alpha(false), GObject(&parent) {}
GCanvas::GCanvas(GWidget* parent) : _has_alpha(false), GObject(parent)  {}
GCanvas::GCanvas(bool alpha, GWidget& parent) : _has_alpha(alpha), GObject(&parent) {}
GCanvas::GCanvas(bool alpha, GWidget* parent) : _has_alpha(alpha), GObject(parent)  {}

GCanvas::GCanvas(uint16_t w, uint16_t h, GWidget& parent)
: _w(w), _h(h), _has_alpha(false), GObject(&parent) {}

GCanvas::GCanvas(uint16_t w, uint16_t h, GWidget* parent)
: _w(w), _h(h), _has_alpha(false), GObject(parent)  {}

GCanvas::GCanvas(uint16_t w, uint16_t h, bool alpha, GWidget& parent)
: _w(w), _h(h), _has_alpha(alpha), GObject(&parent) {}

GCanvas::GCanvas(uint16_t w, uint16_t h, bool alpha, GWidget* parent)
: _w(w), _h(h), _has_alpha(alpha), GObject(parent)  {}

GCanvas::~GCanvas() { this->del(); }

void GCanvas::create() {
  if(this->_par != NULL && !this->_par->isCreated()) this->_par->create();
  if(this->_par->obj == NULL) return;

  if(!this->isCreated()) {
    this->obj = lv_canvas_create(this->_par->obj, NULL);
    if(this->obj != NULL) {
      this->_created  = true;
      lv_obj_set_user_data(this->obj, this);
      lv_obj_set_event_cb (this->obj, main_event_cb);

      if( this->_w == 0 )   this->_w = GScreen.width();
      if( this->_h == 0 )   this->_h = GScreen.height();  

      if(this->_has_alpha) {
        this->cbuf  = (color_t*) esp32_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(this->_w,this->_h));
        lv_canvas_set_buffer(this->obj, this->cbuf, this->_w, this->_h, LV_IMG_CF_TRUE_COLOR_ALPHA);
      }else {
        this->cbuf  = (color_t*) esp32_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR_CHROMA_KEYED(this->_w,this->_h));
        lv_canvas_set_buffer(this->obj, this->cbuf, this->_w, this->_h, LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED);
      }
      this->fillScreen(TFT_BLACK);
      // if(!this->hasLocalStyle()) this->createLocalStyle();
      this->font_color(TFT_WHITE);
      this->align(ALIGN_CENTER);
    }
  }
}

void GCanvas::create(GObject& parent) {
  this->_par = &parent;
  create();
}

void GCanvas::create(GObject* parent) {
  this->_par = parent;
  create();
}


void GCanvas::fillScreen(uint16_t color, uint8_t opa){
  if(!this->isCreated()) create();
  lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  if(color == TFT_TRANSPARENT) {
    // if(ext->dsc.header.cf != LV_IMG_CF_TRUE_COLOR_ALPHA)
    if(ext->dsc.header.cf == LV_IMG_CF_TRUE_COLOR_ALPHA){
      // this->fillAlpha(0);
    }
    else{
#if LV_VERSION_CHECK(7,0,0)
      lv_canvas_fill_bg(this->obj, LV_COLOR_TRANSP, LV_OPA_COVER);
#else
      lv_canvas_fill_bg(this->obj, LV_COLOR_TRANSP);
#endif
    }

    this->_fill_transp = true;
  }else{
    lv_color_t _color;  _color.full = color;
#if LV_VERSION_CHECK(7,0,0)
    lv_canvas_fill_bg(this->obj, _color, LV_OPA_COVER);
#else
    lv_canvas_fill_bg(this->obj, _color);
#endif

    // this->fillAlpha(opa);

    this->_fill_transp = false;
  }
  this->invalidate();
}


// void GCanvas::fillAlpha(uint8_t alpha){
//   if(!this->isCreated()) create();
//   lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();
//   if(ext->dsc.header.cf != LV_IMG_CF_TRUE_COLOR_ALPHA) return;

//   uint8_t  * buf_u8  = (uint8_t *)ext->dsc.data;

//   for(uint16_t y=0; y < ext->dsc.header.h; y++) {
//     for(uint16_t x=0; x < ext->dsc.header.w; x++) {
//       uint32_t px = ext->dsc.header.w * y * LV_IMG_PX_SIZE_ALPHA_BYTE + x * LV_IMG_PX_SIZE_ALPHA_BYTE;
//       buf_u8[px + LV_IMG_PX_SIZE_ALPHA_BYTE - 1] = alpha;
//     }
//   }
//   lv_obj_invalidate(this->obj);
// }

float _capsuleSDF(float px, float py, float ax, float ay, float bx, float by, float r) {
  float pax = px - ax, pay = py - ay, bax = bx - ax, bay = by - ay;
  float h = fmaxf(fminf((pax * bax + pay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
  float dx = pax - bax * h, dy = pay - bay * h;
  return sqrtf(dx * dx + dy * dy) - r;
}

void GCanvas::drawText(String text, Point topleft, uint16_t max_width, label_align_t text_align){
  if(!this->isCreated()) create();
  if(!this->hasLocalStyle()) this->createLocalStyle();
  // lv_style_t *_style = (this->_style==NULL)?  &lv_style_plain : this->_style->raw();

  // lv_canvas_draw_text(this->obj, topleft.x, topleft.y, max_width, (const lv_style_t * ) _style,
                         // text.c_str(), text_align);

  lv_canvas_draw_text(this->obj, topleft.x, topleft.y, max_width, (const lv_style_t * ) this->getStyle()->raw(),
                         text.c_str(), text_align);
}

void GCanvas::drawQRCode(String txt, Point topleft, uint16_t qr_width, uint8_t qr_version){
  if(!this->isCreated()) create();

  bool round = this->corner_radius()? true : false;

  color_t c = this->color();
  // color_t bg_c = (c== TFT_WHITE)? TFT_BLACK : TFT_WHITE;

  this->color(TFT_WHITE);
  this->corner_radius(0);
  this->drawRect(topleft,qr_width,qr_width);
  
  this->color(c);
  size_t buf_len = qrcodegen_BUFFER_LEN_FOR_VERSION(qr_version);
  uint8_t qrcode[buf_len];
  uint8_t tempBuffer[buf_len];

  bool ok = qrcodegen_encodeText(txt.c_str(), tempBuffer, qrcode,
              qrcodegen_Ecc_QUARTILE, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_4, true);
  if (ok) {
    // Serial.println("encode ok");
    int size = qrcodegen_getSize(qrcode);  // 29
    int border = 2;  // 29+2+2 = 33 ส่วนรวมกรอบ
    float sqr_w = qr_width*1.0/(size+2*border);
    // Serial.printf("[sqr_w] %.2f\n", sqr_w);
    if( round ) {
      this->corner_radius(RADIUS_CIRCLE);
    }

    for (int y = -border; y < size + border; y++) {
      for (int x = -border; x < size + border; x++) {
        if( qrcodegen_getModule(qrcode, x, y) )
          lv_canvas_draw_rect(this->obj, topleft.x + (x+ border) *sqr_w, topleft.y + (y+border)*sqr_w, ceil(sqr_w), ceil(sqr_w), this->_style->raw());
      }
    }

    if( round ) {
      this->corner_radius(0);
      this->color(TFT_WHITE);
      this->drawRect(topleft, (border+7+1)*sqr_w , (border+7+1)*sqr_w);
      this->drawRect(topleft + Point(qr_width - (border+7+1)*sqr_w, 0), (border+7+1)*sqr_w , (border+7+1)*sqr_w);
      this->drawRect(topleft + Point(0, qr_width - (border+7+1)*sqr_w), (border+7+1)*sqr_w , (border+7+1)*sqr_w);

      this->corner_radius(RADIUS_CIRCLE);
      this->color(TFT_WHITE);
      this->border(sqr_w , c);
      Point p = topleft + Point(border*sqr_w,border*sqr_w);
      this->drawRect(p, 7*sqr_w, 7*sqr_w);
      this->color(c);
      this->border(0);
      this->drawRect(p + Point(2*sqr_w,2*sqr_w), 3*sqr_w, 3*sqr_w);

      this->color(TFT_WHITE);
      this->border(sqr_w , c);
      p = topleft + Point(qr_width,0) - Point((border+7)*sqr_w, -border*sqr_w);
      this->drawRect(p, 7*sqr_w, 7*sqr_w);
      this->color(c);
      this->border(0);
      this->drawRect(p + Point(2*sqr_w,2*sqr_w), 3*sqr_w, 3*sqr_w);

      this->color(TFT_WHITE);
      this->border(sqr_w , c);
      p = topleft + Point(0,qr_width) - Point(-border*sqr_w, (border+7)*sqr_w);
      this->drawRect(p, 7*sqr_w, 7*sqr_w);
      this->color(c);
      this->border(0);
      this->drawRect(p + Point(2*sqr_w,2*sqr_w), 3*sqr_w, 3*sqr_w);

      this->corner_radius(0);
    }

  }

}


void GCanvas::drawLine(float ax, float ay, float bx, float by, uint16_t color,  float r) {
  if(!this->isCreated()) create();
  lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  int x0 = (int)floorf(fminf(ax, bx) - r);
  int x1 = (int) ceilf(fmaxf(ax, bx) + r);
  int y0 = (int)floorf(fminf(ay, by) - r);
  int y1 = (int) ceilf(fmaxf(ay, by) + r);
  for (int y = y0; y <= y1; y++) {
    for (int x = x0; x <= x1; x++) {
      if( x < 0 || y < 0 || x >= ext->dsc.header.w || y >= ext->dsc.header.h ) continue;
      uint16_t alpha = 255*fmaxf(fminf(0.5f - _capsuleSDF(x, y, ax, ay, bx, by, r), 1.0f), 0.0f);
      if( alpha == 0) continue;

      if(this->_fill_transp) {
        this->pixel(x,y) = color;//alphaBlend( alpha, color, TFT_WHITE );
        if(ext->dsc.header.cf == LV_IMG_CF_TRUE_COLOR_ALPHA){
          // this->alpha(x,y) = alpha; // ให้เตรียมผสมสีพื้นหลังอื่นๆต่อไป
        }
      }
      else{
        this->pixel(x,y) = alphaBlend( alpha, color, this->pixel(x,y)  );
        // this->alpha(x,y) = 255; // ไม่ต้องผสมสีอีกแล้ว ใช้แบบสีนี้เต็มๆ
      }
    }
  }
  lv_obj_invalidate(this->obj);
}

void GCanvas::drawLine(Point p1, Point p2, uint16_t color){
  if(!this->isCreated()) create();
  this->drawLine(p1.x, p1.y, p2.x, p2.y, color, 0.8);
}

void GCanvas::drawLine(Point p1, Point p2, float thick , uint16_t color){
  if(!this->isCreated()) create();
  this->drawLine(p1.x, p1.y, p2.x, p2.y, color, thick/2.0);
}

void GCanvas::drawRect(Point topleft, uint16_t w, uint16_t h, uint16_t color){
  this->drawRect(topleft, w, h, 1, color);
}

void GCanvas::drawHLine(Point topleft, uint16_t w, uint16_t color) {
  if(!this->isCreated()) create();
  lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  if( topleft.y < 0 || topleft.y >= ext->dsc.header.h) return;

  int x1 = topleft.x;
  int x2 = x1+ w -1;
  int y = topleft.y;
  for(int x = x1 ; x <= x2; x++ ){
    if( x < 0 || x >=  ext->dsc.header.w) continue;
    this->pixel(x, y) = color;
    if(ext->dsc.header.cf == LV_IMG_CF_TRUE_COLOR_ALPHA){
      // this->alpha(x,y) = 255;
    }
  }
}

void GCanvas::drawVLine(Point topleft, uint16_t h, uint16_t color) {
  if(!this->isCreated()) create();
  lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  if( topleft.x < 0 || topleft.x >= ext->dsc.header.w) return;

  int y1 = topleft.y;
  int y2 = y1 + h -1;
  int x = topleft.x;
  for(int y = y1 ; y <= y2; y++ ){
    if( y < 0 || y >=  ext->dsc.header.h) continue;
    this->pixel(x, y) = color;   
    if(ext->dsc.header.cf == LV_IMG_CF_TRUE_COLOR_ALPHA){
      // this->alpha(x,y) = 255;
    }
  }
}


void GCanvas::drawRect(Point topleft, uint16_t w, uint16_t h, uint8_t thick, uint16_t color){
  if(!this->isCreated()) create();
  // lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  style_t style;
  lv_style_copy(&style, &lv_style_transp);
  style.body.border.color.full= color;
  style.body.border.width = thick;

  lv_canvas_draw_rect(this->obj, topleft.x, topleft.y, w, h, &style);
}

void GCanvas::drawRect(Point topleft, uint16_t w, uint16_t h, GStyle *style){
  if(!this->isCreated()) create();
  

  lv_canvas_draw_rect(this->obj, topleft.x, topleft.y, w, h, 
    ( style == NULL)? (const lv_style_t * ) this->getStyle()->raw():  (const lv_style_t * ) style->raw() );
}

void GCanvas::fillRect(Point topleft, uint16_t w, uint16_t h, uint16_t color){
  if(!this->isCreated()) create();
  // lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.main_color.full = color;
  style.body.grad_color.full = color;
  lv_canvas_draw_rect(this->obj, topleft.x, topleft.y, w, h, &style);
}

// void GCanvas::maskRect(Point topleft, uint16_t w, uint16_t h, bool invert_mask){
//   if(!this->isCreated()) create();

//   lv_canvas_mask_rect(this->obj, topleft.x, topleft.y, w, h,
//                          &lv_style_plain, invert_mask);
// }

void GCanvas::drawRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t thick, uint8_t conner_radius, uint16_t color) {
  if(!this->isCreated()) create();
  // lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  style_t style;
  lv_style_copy(&style, &lv_style_transp);
  style.body.border.width= thick;
  style.body.border.color.full= color;
  style.body.radius = conner_radius;

  lv_canvas_draw_rect(this->obj, topleft.x, topleft.y, w, h, &style);
}

void GCanvas::drawRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t conner_radius, uint16_t color) {
  this->drawRoundRect(topleft, w, h, 1, conner_radius, color);
}


void GCanvas::fillRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t conner_radius, uint16_t color){
  if(!this->isCreated()) create();
  style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.main_color.full = color;
  style.body.grad_color.full = color;
  style.body.radius = conner_radius;
  lv_canvas_draw_rect(this->obj, topleft.x, topleft.y, w, h, &style);
}

// void GCanvas::maskRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t conner_radius, bool invert_mask)
// {
//   if(!this->isCreated()) create();
//   style_t style;
//   lv_style_copy(&style, &lv_style_plain);
//   style.body.radius = conner_radius;

//   lv_canvas_mask_rect(this->obj, topleft.x, topleft.y, w, h,
//                          &style, invert_mask);
// }


void GCanvas::drawCircle(Point center, uint16_t r, uint16_t color){
  this->drawCircle(center, r, 1, color);
}

void GCanvas::drawCircle(Point center, uint16_t r, uint8_t thick, uint16_t color) {
  if(!this->isCreated()) create();
  style_t style;
  lv_style_copy(&style, &lv_style_transp);
  style.body.border.width= thick;
  style.body.border.color.full= color;
  style.body.radius = LV_RADIUS_CIRCLE;

  lv_canvas_draw_rect(this->obj, center.x-r, center.y-r, 2*r, 2*r, &style);
}

void GCanvas::fillCircle(Point center, uint16_t r, uint16_t color) {
  if(!this->isCreated()) create();
  style_t style;
  lv_style_copy(&style, &lv_style_plain);

  style.body.main_color.full = color;
  style.body.grad_color.full = color;
  style.body.radius = LV_RADIUS_CIRCLE;

  lv_canvas_draw_rect(this->obj, center.x-r, center.y-r, 2*r, 2*r, &style);
}

// void GCanvas::maskCircle(Point center, uint16_t r, bool invert_mask){
//   if(!this->isCreated()) create();
//   style_t style;
//   lv_style_copy(&style, &lv_style_plain);
//   style.body.radius = LV_RADIUS_CIRCLE;

//   lv_canvas_mask_rect(this->obj, center.x-r, center.y-r, 2*r, 2*r,
//                          &style, invert_mask);
// }

// void GCanvas::drawEllipse(Point center, uint16_t a, uint16_t b, uint16_t color) {
//   if(!this->isCreated()) create();

//   style_t style;
//   lv_style_copy(&style, &lv_style_transp);
//   style.body.border.width= 2;
//   style.body.border.color.full= color;
//   style.body.radius = LV_RADIUS_CIRCLE;

//   lv_canvas_draw_rect(this->obj, center.x-a, center.y-b, 2*a, 2*b, &style);
// }

// void GCanvas::drawEllipse(Point center, uint16_t a, uint16_t b, uint8_t thick, uint16_t color) {
//   if(!this->isCreated()) create();

//   style_t style;
//   lv_style_copy(&style, &lv_style_transp);
//   style.body.border.width= thick;
//   style.body.border.color.full= color;
//   style.body.radius = LV_RADIUS_CIRCLE;

//   lv_canvas_draw_rect(this->obj, center.x-a, center.y-b, 2*a, 2*b, &style);
// }


// void GCanvas::fillEllipse(Point center, uint16_t a, uint16_t b, uint16_t color){
//   if(!this->isCreated()) create();
//   style_t style;
//   lv_style_copy(&style, &lv_style_plain);

//   style.body.main_color.full = color;
//   style.body.grad_color.full = color;
//   style.body.radius = LV_RADIUS_CIRCLE;

//   lv_canvas_draw_rect(this->obj, center.x-a, center.y-b, 2*a, 2*b, &style);
// }

void GCanvas::drawTriangle(Point p1, Point p2, Point p3, uint16_t color) {
  if(!this->isCreated()) create();
  const lv_point_t _points[] =  {  (const lv_point_t){p1.x, p1.y}, 
                                  (const lv_point_t){p2.x, p2.y}, 
                                  (const lv_point_t){p3.x, p3.y},
                                  (const lv_point_t){p1.x, p1.y} };
  style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.line.width = 2;
  style.line.color.full = color;
  style.line.rounded = true;

  lv_canvas_draw_line(this->obj, _points, 4, &style );
}

void GCanvas::drawTriangle(Point p1, Point p2, Point p3, uint8_t thick, uint16_t color){
  if(!this->isCreated()) create();
  const lv_point_t _points[] =  {  (const lv_point_t){p1.x, p1.y}, 
                                  (const lv_point_t){p2.x, p2.y}, 
                                  (const lv_point_t){p3.x, p3.y},
                                  (const lv_point_t){p1.x, p1.y} };
  style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.line.width = thick;
  style.line.color.full = color;
  style.line.rounded = true;

  lv_canvas_draw_line(this->obj, _points, 4, &style );
}

void GCanvas::fillTriangle(Point p1, Point p2, Point p3, uint16_t color){
  if(!this->isCreated()) create();
  const lv_point_t _points[3] = {  (const lv_point_t){p1.x, p1.y}, 
                                  (const lv_point_t){p2.x, p2.y}, 
                                  (const lv_point_t){p3.x, p3.y} };
  style_t style;
  lv_style_copy(&style, &lv_style_plain);

  style.body.main_color.full = color;
  style.body.grad_color.full = color;

  lv_canvas_draw_polygon(this->obj, _points, 3, &style );
}

// void GCanvas::maskTriangle(Point p1, Point p2, Point p3, bool invert_mask){
//   if(!this->isCreated()) create();

//   const lv_point_t _points[] =  {  (const lv_point_t){p1.x, p1.y}, 
//                                   (const lv_point_t){p2.x, p2.y}, 
//                                   (const lv_point_t){p3.x, p3.y},
//                                   (const lv_point_t){p1.x, p1.y} };

//   lv_canvas_mask_polygon(this->obj, _points, 4, &lv_style_plain, invert_mask);
// }

void GCanvas::drawPolygon(Point* points, size_t point_num, uint16_t color){
  this->drawPolygon(points,point_num, 1, color);
}

void GCanvas::drawPolygon(Point* points, size_t point_num, uint8_t thick, uint16_t color) {
  if(!this->isCreated()) create();
  lv_point_t *_points = (lv_point_t*) esp32_malloc(point_num*sizeof(lv_point_t));
  for(int i=0; i < point_num; i++){
    _points[i].x = points[i].x;
    _points[i].y = points[i].y;
  }

  style_t style;
  lv_style_copy(&style, &lv_style_transp);
  style.body.border.width= thick;
  style.body.border.color.full= color;

  lv_canvas_draw_polygon(this->obj, _points, point_num, &style );
  free(_points);
}

void GCanvas::fillPolygon(Point* points, size_t point_num, uint16_t color){
  if(!this->isCreated()) create();
  lv_point_t *_points = (lv_point_t*) esp32_malloc(point_num*sizeof(lv_point_t));
  for(int i=0; i < point_num; i++){
    _points[i].x = points[i].x;
    _points[i].y = points[i].y;
  }

  style_t style;
  lv_style_copy(&style, &lv_style_plain);

  style.body.main_color.full = color;
  style.body.grad_color.full = color;

  lv_canvas_draw_polygon(this->obj, _points, point_num, &style );
  free(_points);

}

// void GCanvas::maskPolygon(Point* points, size_t point_num, bool invert_mask)
// {
//   if(!this->isCreated()) create();
//   lv_point_t *_points = (lv_point_t*) esp32_malloc(point_num*sizeof(lv_point_t));
//   for(int i=0; i < point_num; i++){
//     _points[i].x = points[i].x;
//     _points[i].y = points[i].y;
//   }

//   lv_canvas_mask_polygon(this->obj, _points, point_num, &lv_style_plain, invert_mask );
//   free(_points);
// }

void GCanvas::drawArc(Point center, uint16_t r, 
            int32_t angle_deg_start, int32_t angle_deg_end, 
            uint16_t color)
{
  this->drawArc(center, r, 1, angle_deg_start, angle_deg_end, color);
}

void GCanvas::drawArc(Point center, uint16_t r, uint8_t thick, 
            int32_t angle_deg_start, int32_t angle_deg_end, 
            uint16_t color)
{
  if(!this->isCreated()) create();

  style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.line.width= thick;
  style.line.color.full= color;
  style.line.rounded = true;

  lv_canvas_draw_arc(this->obj, center.x, center.y, r, 
                      angle_deg_start+90, angle_deg_end+90, &style);
}

// void GCanvas::maskArc(Point center, uint16_t r, uint8_t thick, 
//                 int32_t angle_deg_start, int32_t angle_deg_end, // กวาดมุมทวนเข็มตามระนาบพิกัด xy
//                 bool invert_mask)
// {
//   if(!this->isCreated()) create();

//   style_t style;
//   lv_style_copy(&style, &lv_style_plain);
//   style.line.width= thick;

//   lv_canvas_mask_arc(this->obj, center.x, center.y, r, 
//                       angle_deg_start+90, angle_deg_end+90, &style, invert_mask);

// }

color_t& GCanvas::pixel(uint16_t x, uint16_t y){
  static uint16_t buf = 0;
  if(!this->isCreated()) create();
  if( x < 0 || y < 0 || x >= this->width() || y >= this->height() )return buf;

  lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();
  const lv_style_t * style = lv_canvas_get_style(this->obj, LV_CANVAS_STYLE_MAIN);

  uint8_t  * buf_u8  = (uint8_t *)ext->dsc.data;

#if LV_VERSION_CHECK(7,0,0)
  uint8_t px_size = lv_img_cf_get_px_size(ext->dsc.header.cf) >> 3;
#else
  uint8_t px_size = lv_img_color_format_get_px_size(ext->dsc.header.cf) >> 3;
#endif
  uint32_t px     =  (x + ext->dsc.header.w * y) * px_size;
  return *(uint16_t*)&buf_u8[px];
}

// uint8_t& GCanvas::alpha(uint16_t x, uint16_t y) {
//   static uint8_t buf = 0;
//   if(!this->isCreated()) create();
//   if( x < 0 || y < 0 || x >= this->width() || y >= this->height() )return buf;

//   lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();
//   if(ext->dsc.header.cf != LV_IMG_CF_TRUE_COLOR_ALPHA) return buf;

//   uint8_t  * buf_u8  = (uint8_t *)ext->dsc.data;
//   uint8_t  px_size = lv_img_color_format_get_px_size(ext->dsc.header.cf) >> 3;
//   uint32_t px     = (x + ext->dsc.header.w * y) * px_size;
//   return  buf_u8[px + px_size - 1];
// }

void GCanvas::drawImage(String file_path, uint16_t x, uint16_t y){
  if(!this->isCreated()) create();
  lv_canvas_ext_t * ext    = (lv_canvas_ext_t *) this->ext_attr();

  String f_path = file_path;
  bool spiffs_fs = false;

#if BLYNKGO_USE_SD
    // เปลี่ยน file_path เป็น /sd/... แทน
    if( file_path.startsWith("sd://") ||
        file_path.startsWith("SD://") ||
        file_path.startsWith(BLYNKGO_SD_DRIVE) ||
        file_path.startsWith("/") )
    {
      f_path.replace("sd://", "/");
      f_path.replace("SD://", "/");
      f_path.replace(BLYNKGO_SD_DRIVE, "/");
      f_path = String("/sd") + f_path;
    } else 
#endif
    // เปลี่ยน file_path เป็น /spiffs/... แทน
    if( file_path.startsWith("spiffs://")  ||
        file_path.startsWith("SPIFFS://")  ||
        file_path.startsWith(BLYNKGO_SPIFFS_DRIVE) )
    {
      spiffs_fs = true;
      f_path.replace("spiffs://", "/");
      f_path.replace("SPIFFS://", "/");
      f_path.replace(BLYNKGO_SPIFFS_DRIVE, "/");
      f_path = String("/spiffs") + f_path;
    }

  // Serial.println(f_path);
  // Serial.println(access( f_path.c_str(), F_OK ) );
  if ( access( f_path.c_str(), F_OK ) != -1 || spiffs_fs) {
    // file exists
    // decode image to this->sec_dsc
    if ( LV_RES_OK == png_to_img_dsc(f_path.c_str() , &this->src_dsc)) {
    } else if ( LV_RES_OK == bmp_to_img_dsc(f_path.c_str() , &this->src_dsc)) {
    } else if ( LV_RES_OK == jpg_to_img_dsc(f_path.c_str() , &this->src_dsc)) {
    } else {
      Serial.println("[GCanvas] File is unknown format.");
      return;
    }
    lv_img_cache_invalidate_src(&this->src_dsc);
    // this->full_img = &this->src_dsc;
    // lv_img_set_src(this->obj, &this->src_dsc);
    if(this->_has_alpha){
      int px; int py;
      uint8_t pixel_size = (this->src_dsc.header.cf == LV_IMG_CF_TRUE_COLOR_ALPHA)? 3 : 2;
#if BLYNGO_DEBUG
      Serial.println("[GCanvas] has alpha");
#endif
      for(int j=0; j < src_dsc.header.h; j++){
        for(int i=0; i < src_dsc.header.w; i++){
          px = x + i; py = y + j;
          if(px < 0 || py < 0 || px >= ext->dsc.header.w || py >= ext->dsc.header.h) continue;

          if(this->_fill_transp) {
            this->pixel(px,py) = *(uint16_t*) &src_dsc.data[ (i+j*src_dsc.header.w)*pixel_size];
            if(pixel_size == 3) {
              // this->alpha(px,py) = src_dsc.data[ (i+j*src_dsc.header.w)*pixel_size + pixel_size -1];
            }else{
              // this->alpha(px,py) = 255;
            }
          }else{
            // กรณี canvas มี alpha แลรูป มี alpha ด้วย แต่พื้นหลังเดิม canvas ไม่ใช่ transparents
            if(pixel_size == 3) {
              uint8_t a = src_dsc.data[ (i+j*src_dsc.header.w)*pixel_size + pixel_size -1];
              this->pixel(px,py) = alphaBlend( a, *(uint16_t*) &src_dsc.data[ (i+j*src_dsc.header.w)*pixel_size],
                                      this->pixel(px,py));
              // this->alpha(px,py) = 255;
            }else{
            // กรณี canvas มี alpha แต่รูปไม่มี alpha 
              this->pixel(px,py) = *(uint16_t*) &src_dsc.data[ (i+j*src_dsc.header.w)*pixel_size];
              // this->alpha(px,py) = 255;              
            }
          }
        }
      }
    }else{
      // หาก canvas ไม่มี alpha ให้วาดตามปกติด้วย api ของ canvas
      lv_canvas_draw_img(this->obj, x, y, &this->src_dsc, &lv_style_plain);
    }
    // this->src_dsc_created = true;
    free((void*)src_dsc.data);
    // this->hidden(false);
    this->realign();
  } else {
    // file doesn't exist
    Serial.println("[GCanvas] File not Exist");
    this->hidden(true);
    return;
  }

}

void GCanvas::setPixel(int x, int y, uint16_t color) {
  // if(this->_bpp==16)      this->pixel16(x,y) = color;
  // else if(this->_bpp==8)  this->pixel8(x,y)  = color16to8(color);
  this->pixel(x,y) = color;
}

void GCanvas::setPixelAA(int x, int y, uint16_t color16, uint8_t aa) {
  // if(this->_bpp==16)      this->pixel16(x,y) = alphaBlend( aa, this->pixel16(x,y), color);
  // else if(this->_bpp==8)  this->pixel8(x,y)  = alphaBlend8(aa, this->pixel8 (x,y), color16to8(color));
  if(!this->isCreated()) create();
  this->pixel(x,y) = alphaBlend( aa, this->pixel(x,y), color16);
}

void GCanvas::plotLine(int x0, int y0, int x1, int y1, uint16_t color)
{
  if(!this->isCreated()) create();

  float dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  float dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  float err = dx + dy, e2;                                 /* error value e_xy */

  for (;;) {                                    /* loop */
    setPixel(x0, y0, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;  /* x step */
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;  /* y step */
      y0 += sy;
    }
  }
  lv_obj_invalidate(this->obj);
}

void GCanvas::plotLineAA(int x0, int y0, int x1, int y1, uint16_t color)
{ /* draw a black (0) anti-aliased line on white (255) background */
  if(!this->isCreated()) create();

  float dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  float dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  float err = dx - dy, e2, x2;                             /* error value e_xy */
  float ed = dx + dy == 0 ? 1 : sqrt(dx * dx + dy * dy);

  for ( ; ; ) {                                                /* pixel loop */
    setPixelAA(x0, y0, color, 255 * abs(err - dx + dy) / ed);
    e2 = err; x2 = x0;
    if (2 * e2 >= -dx) {                                          /* x step */
      if (x0 == x1) break;
      if (e2 + dy < ed) setPixelAA(x0, y0 + sy, color, 255 * (e2 + dy) / ed);
      err -= dy; x0 += sx;
    }
    if (2 * e2 <= dy) {                                           /* y step */
      if (y0 == y1) break;
      if (dx - e2 < ed) setPixelAA(x2 + sx, y0, color, 255 * (dx - e2) / ed);
      err += dx; y0 += sy;
    }
  }
  lv_obj_invalidate(this->obj);
}

void GCanvas::plotLineWidth(int x0, int y0, int x1, int y1, int th, uint16_t color)
{ /* plot an anti-aliased line of width th pixel */
  if(!this->isCreated()) create();

  float dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  float dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  float err, e2 = sqrt(dx * dx + dy * dy);                      /* length */

  if (th <= 1 || e2 == 0) return plotLineAA(x0, y0, x1, y1, color);       /* assert */
  dx *= 255 / e2; dy *= 255 / e2; th = 255 * (th - 1);       /* scale values */

  if (dx < dy) {                                               /* steep line */
    x1 = round((e2 + th / 2) / dy);                    /* start offset */
    err = x1 * dy - th / 2;            /* shift error value to offset width */
    for (x0 -= x1 * sx; ; y0 += sy) {
      setPixelAA(x1 = x0, y0, color, err);                  /* aliasing pre-pixel */
      for (e2 = dy - err - th; e2 + dy < 255; e2 += dy)
        setPixel(x1 += sx, y0, color);                      /* pixel on the line */
      setPixelAA(x1 + sx, y0, color, e2);                  /* aliasing post-pixel */
      if (y0 == y1) break;
      err += dx;                                                 /* y-step */
      if (err > 255) {
        err -= dy;  /* x-step */
        x0 += sx;
      }
    }
  } else {                                                      /* flat line */
    y1 = round((e2 + th / 2) / dx);                    /* start offset */
    err = y1 * dx - th / 2;            /* shift error value to offset width */
    for (y0 -= y1 * sy; ; x0 += sx) {
      setPixelAA(x0, y1 = y0, color, err);                  /* aliasing pre-pixel */
      for (e2 = dx - err - th; e2 + dx < 255; e2 += dx)
        setPixel(x0, y1 += sy, color);                      /* pixel on the line */
      setPixelAA(x0, y1 + sy, color, e2);                  /* aliasing post-pixel */
      if (x0 == x1) break;
      err += dy;                                                 /* x-step */
      if (err > 255) {
        err -= dx;  /* y-step */
        y0 += sy;
      }
    }
  }
  lv_obj_invalidate(this->obj);
}

void GCanvas::plotCircleAA(int xm, int ym, int r, uint16_t color)
{ /* draw a black anti-aliased circle on white background */
  if(!this->isCreated()) create();

  float x = r, y = 0;            /* II. quadrant from bottom left to top right */
  float i, x2, e2, err = 2 - 2 * r;                         /* error of 1.step */
  r = 1 - err;
  for ( ; ; ) {
    i = 255 * abs(err + 2 * (x + y) - 2) / r; /* get blend value of pixel */
    setPixelAA(xm + x, ym - y, color, i);                         /*   I. Quadrant */
    setPixelAA(xm + y, ym + x, color, i);                         /*  II. Quadrant */
    setPixelAA(xm - x, ym + y, color, i);                         /* III. Quadrant */
    setPixelAA(xm - y, ym - x, color, i);                         /*  IV. Quadrant */
    if (x == 0) break;
    e2 = err; x2 = x;                                    /* remember values */
    if (err > y) {                                                /* x step */
      i = 255 * (err + 2 * x - 1) / r;                    /* outward pixel */
      if (i < 255) {
        setPixelAA(xm + x, ym - y + 1, color, i);
        setPixelAA(xm + y - 1, ym + x, color, i);
        setPixelAA(xm - x, ym + y - 1, color, i);
        setPixelAA(xm - y + 1, ym - x, color, i);
      }
      err -= --x * 2 - 1;
    }
    if (e2 <= x2--) {                                             /* y step */
      i = 255 * (1 - 2 * y - e2) / r;                      /* inward pixel */
      if (i < 255) {
        setPixelAA(xm + x2, ym - y, color, i);
        setPixelAA(xm + y, ym + x2, color, i);
        setPixelAA(xm - x2, ym + y, color, i);
        setPixelAA(xm - y, ym - x2, color, i);
      }
      err -= --y * 2 - 1;
    }
  }
  lv_obj_invalidate(this->obj);
}

void GCanvas::plotCircleWidth(int x, int y, float r, int th, uint16_t color){
  if(!this->isCreated()) create();
  plotEllipseRectWidth( x-r, y-r, x+r,y+r, th, color); //OK คมเนียน หนา
  lv_obj_invalidate(this->obj);
}

void GCanvas::plotEllipseRect(int x0, int y0, int x1, int y1, uint16_t color)
{ /* rectangular parameter enclosing the ellipse */
  if(!this->isCreated()) create();

  float a = abs(x1 - x0), b = abs(y1 - y0); int b1 = ((int)b) & 1;  /* diameter */
  float dx = 4 * (1.0 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
  float err = dx + dy + b1 * a * a, e2;                     /* error of 1.step */

  if (x0 > x1) {
    x0 = x1;  /* if called with swapped pofloats */
    x1 += a;
  }
  if (y0 > y1) y0 = y1;                                  /* .. exchange them */
  y0 += ((int)b + 1) >> 1; y1 = y0 - b1;                        /* starting pixel */
  a = 8 * a * a; b1 = 8 * b * b;

  do {
    setPixel(x1, y0, color);                                      /*   I. Quadrant */
    setPixel(x0, y0, color);                                      /*  II. Quadrant */
    setPixel(x0, y1, color);                                      /* III. Quadrant */
    setPixel(x1, y1, color);                                      /*  IV. Quadrant */
    e2 = 2 * err;
    if (e2 <= dy) {
      y0++;  /* y step */
      y1--;
      err += dy += a;
    }
    if (e2 >= dx || 2 * err > dy) {
      x0++;  /* x */
      x1--;
      err += dx += b1;
    }
  } while (x0 <= x1);

  while (y0 - y1 <= b) {              /* too early stop of flat ellipses a=1 */
    setPixel(x0 - 1, y0, color);                       /* -> finish tip of ellipse */
    setPixel(x1 + 1, y0++, color);
    setPixel(x0 - 1, y1, color);
    setPixel(x1 + 1, y1--, color);
  }
  lv_obj_invalidate(this->obj);
}

void GCanvas::plotEllipseRectAA(int x0, int y0, int x1, int y1, uint16_t color)
{ /* draw a black anti-aliased rectangular ellipse on white background */
  if(!this->isCreated()) create();

  float a = abs(x1 - x0), b = abs(y1 - y0); int b1 = (int)b & 1;  /* diameter */
  float dx = 4 * (a - 1) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
  float f, ed, i, err = b1 * a * a - dx + dy;               /* error of 1.step */

  if (a == 0 || b == 0) return plotLine(x0, y0, x1, y1, color);
  if (x0 > x1) {
    x0 = x1;  /* if called with swapped pofloats */
    x1 += a;
  }
  if (y0 > y1) y0 = y1;                                  /* .. exchange them */
  y0 += ((int)b + 1) >> 1; y1 = y0 - b1;                        /* starting pixel */
  a = 8 * a * a; b1 = 8 * b * b;

  for (;;) {                        /* approximate ed=sqrt(dx*dx+dy*dy) */
    i = min(dx, dy); ed = max(dx, dy);
    if (y0 == y1 + 1 && err > dy && a > b1) ed = 255 * 4 / a;      /* x-tip */
    else ed = 255 / (ed + 2 * ed * i * i / (4 * ed * ed + i * i)); /* approximation */
    i = ed * abs(err + dx - dy);   /* get floatensity value by pixel err */
    setPixelAA(x0, y0, color, i); setPixelAA(x0, y1, color, i);
    setPixelAA(x1, y0, color, i); setPixelAA(x1, y1, color, i);

    if (f = 2 * err + dy >= 0) {              /* x step, remember condition */
      if (x0 >= x1) break;
      i = ed * (err + dx);
      if (i < 256) {
        setPixelAA(x0, y0 + 1, color, i); setPixelAA(x0, y1 - 1, color, i);
        setPixelAA(x1, y0 + 1, color, i); setPixelAA(x1, y1 - 1, color, i);
      }          /* do error increment later since values are still needed */
    }
    if (2 * err <= dx) {                                          /* y step */
      i = ed * (dy - err);
      if (i < 256) {
        setPixelAA(x0 + 1, y0, color, i); setPixelAA(x1 - 1, y0, color, i);
        setPixelAA(x0 + 1, y1, color, i); setPixelAA(x1 - 1, y1, color, i);
      }
      y0++; y1--; err += dy += a;
    }
    if (f) {
      x0++;  /* x error increment */
      x1--;
      err -= dx -= b1;
    }
  }
  if (--x0 == x1++)                       /* too early stop of flat ellipses */
    while (y0 - y1 < b) {
      i = 255 * 4 * abs(err + dx) / b1;   /* -> finish tip of ellipse */
      setPixelAA(x0, ++y0, color, i); setPixelAA(x1, y0, color, i);
      setPixelAA(x0, --y1, color, i); setPixelAA(x1, y1, color, i);
      err += dy += a;
    }
  lv_obj_invalidate(this->obj);
}

void GCanvas::plotEllipseRectWidth(int x0, int y0, int x1, int y1, int th, uint16_t color)
{ /* draw anti-aliased ellipse inside rectangle with thick line */
  if(!this->isCreated()) create();

  float a = abs(x1 - x0), b = abs(y1 - y0); int b1 = (int)b & 1; /* outer diameter */
  float a2 = a - 2 * th, b2 = b - 2 * th;                    /* inner diameter */
  float dx = 4 * (a - 1) * b * b, dy = 4 * (b1 - 1) * a * a; /* error increment */
  float i = a + b2, err = b1 * a * a, dx2, dy2, e2, ed;
  /* thick line correction */
  if (th < 1.5) return plotEllipseRectAA(x0, y0, x1, y1, color);
  if ((th - 1) * (2 * b - th) > a * a) b2 = sqrt(a * (b - a) * i * a2) / (a - th);
  if ((th - 1) * (2 * a - th) > b * b) {
    a2 = sqrt(b * (a - b) * i * b2) / (b - th);
    th = (a - a2) / 2;
  }
  if (a == 0 || b == 0) return plotLine(x0, y0, x1, y1, color);
  if (x0 > x1) {
    x0 = x1;  /* if called with swapped pofloats */
    x1 += a;
  }
  if (y0 > y1) y0 = y1;                                  /* .. exchange them */
  if (b2 <= 0) th = a;                                     /* filled ellipse */
  e2 = th - floor(th); th = x0 + th - e2;
  dx2 = 4 * (a2 + 2 * e2 - 1) * b2 * b2; dy2 = 4 * (b1 - 1) * a2 * a2; e2 = dx2 * e2;
  y0 += ((int)b + 1) >> 1; y1 = y0 - b1;                        /* starting pixel */
  a = 8 * a * a; b1 = 8 * b * b; a2 = 8 * a2 * a2; b2 = 8 * b2 * b2;

  do {
    for (;;) {
      if (err < 0 || x0 > x1) {
        i = x0;
        break;
      }
      i = min(dx, dy); ed = max(dx, dy);
      if (y0 == y1 + 1 && 2 * err > dx && a > b1) ed = a / 4;     /* x-tip */
      else ed += 2 * ed * i * i / (4 * ed * ed + i * i + 1) + 1; /* approx ed=sqrt(dx*dx+dy*dy) */
      i = 255 * err / ed;                         /* outside anti-aliasing */
      setPixelAA(x0, y0, color, i); setPixelAA(x0, y1, color, i);
      setPixelAA(x1, y0, color, i); setPixelAA(x1, y1, color, i);
      if (err + dy + a < dx) {
        i = x0 + 1;
        break;
      }
      x0++; x1--; err -= dx; dx -= b1;                /* x error increment */
    }
    for (; i < th && 2 * i <= x0 + x1; i++) {            /* fill line pixel */
      setPixel(i, y0, color); setPixel(x0 + x1 - i, y0, color);
      setPixel(i, y1, color); setPixel(x0 + x1 - i, y1, color);
    }
    while (e2 > 0 && x0 + x1 >= 2 * th) {           /* inside anti-aliasing */
      i = min(dx2, dy2); ed = max(dx2, dy2);
      if (y0 == y1 + 1 && 2 * e2 > dx2 && a2 > b2) ed = a2 / 4;   /* x-tip */
      else  ed += 2 * ed * i * i / (4 * ed * ed + i * i); /* approximation */
      i = 255 - 255 * e2 / ed;       /* get floatensity value by pixel error */
      setPixelAA(th, y0, color, i); setPixelAA(x0 + x1 - th, color, y0, i);
      setPixelAA(th, y1, color, i); setPixelAA(x0 + x1 - th, color, y1, i);
      if (e2 + dy2 + a2 < dx2) break;
      th++; e2 -= dx2; dx2 -= b2;                     /* x error increment */
    }
    e2 += dy2 += a2;
    y0++; y1--; err += dy += a;                                   /* y step */
  } while (x0 < x1);

  if (y0 - y1 <= b)
  {
    if (err > dy + a) {
      y0--;
      y1++;
      err -= dy -= a;
    }
    for (; y0 - y1 <= b; err += dy += a) { /* too early stop of flat ellipses */
      i = 255 * 4 * err / b1;                  /* -> finish tip of ellipse */
      setPixelAA(x0, y0, color, i); setPixelAA(x1, y0++, color, i);
      setPixelAA(x0, y1, color, i); setPixelAA(x1, y1--, color, i);
    }
  }
  lv_obj_invalidate(this->obj);
}



//------------------------------------------------------------

void GCanvas::plotRotatedEllipseRect(int x0, int y0, int x1, int y1, float zd, uint16_t color)
{ /* rectangle enclosing the ellipse, floateger rotation angle */
  if(!this->isCreated()) create();

  float xd = x1 - x0, yd = y1 - y0, w = xd * yd;
  if (zd == 0) return plotEllipseRect(x0, y0, x1, y1, color);        /* looks nicer */   //plotEllipseRectAA(x0, y0, x1, y1);
  if (w != 0.0) w = (w - zd) / (w + w);              /* squared weight of P1 */
  //  assert(w <= 1.0 && w >= 0.0);                /* limit angle to |zd|<=xd*yd */
  xd = floor(xd * w + 0.5); yd = floor(yd * w + 0.5); /* snap to float */
  plotQuadRationalBezierSeg(x0, y0 + yd, x0, y0, x0 + xd, y0, 1.0 - w, color);
  plotQuadRationalBezierSeg(x0, y0 + yd, x0, y1, x1 - xd, y1, w, color);
  plotQuadRationalBezierSeg(x1, y1 - yd, x1, y1, x1 - xd, y1, 1.0 - w, color);
  plotQuadRationalBezierSeg(x1, y1 - yd, x1, y0, x0 + xd, y0, w, color);

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotRotatedEllipse(int x, int y, int a, int b, float angle_deg, uint16_t color) // angle = degree
{ /* plot ellipse rotated by angle (radian) */
  if(!this->isCreated()) create();

  float angle = radians(angle_deg);
  float xd = a * a, yd = b * b;
  float s = sin(angle), zd = (xd - yd) * s;           /* ellipse rotation */
  xd = sqrt(xd - zd * s), yd = sqrt(yd + zd * s); /* surrounding rect */
  a = floor(xd + 0.5); b = floor(yd + 0.5); zd = zd * a * b / (xd * yd);
  plotRotatedEllipseRect(x - a, y - b, x + a, y + b, (4 * zd * cos(angle)), color);

  lv_obj_invalidate(this->obj);
}

//--------------------------------------------------------

void GCanvas::plotRotatedEllipseRectAA(int x0, int y0, int x1, int y1, float zd, uint16_t color)
{ /* rectangle enclosing the ellipse, floateger rotation angle */
  if(!this->isCreated()) create();

  float xd = x1 - x0, yd = y1 - y0, w = xd * yd;
  if (w != 0.0) w = (w - zd) / (w + w);              /* squared weight of P1 */
  //   assert(w <= 1.0 && w >= 0.0);                /* limit angle to |zd|<=xd*yd */
  xd = floor(xd * w + 0.5); yd = floor(yd * w + 0.5); /* snap to float */
  plotQuadRationalBezierSegAA(x0, y0 + yd, x0, y0, x0 + xd, y0, 1.0 - w, color);
  plotQuadRationalBezierSegAA(x0, y0 + yd, x0, y1, x1 - xd, y1, w, color);
  plotQuadRationalBezierSegAA(x1, y1 - yd, x1, y1, x1 - xd, y1, 1.0 - w, color);
  plotQuadRationalBezierSegAA(x1, y1 - yd, x1, y0, x0 + xd, y0, w, color);

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotRotatedEllipseAA(int x, int y, int a, int b, float angle_deg, uint16_t color)
{ /* plot ellipse rotated by angle (radian) */
  if(!this->isCreated()) create();

  float angle = radians(angle_deg);
  float xd = a * a, yd = b * b;
  float s = sin(angle), zd = (xd - yd) * s;           /* ellipse rotation */
  xd = sqrt(xd - zd * s), yd = sqrt(yd + zd * s); /* surrounding rect*/
  a = floor(xd + 0.5); b = floor(yd + 0.5); zd = zd * a * b / (xd * yd);
  plotRotatedEllipseRectAA(x - a, y - b, x + a, y + b, (4 * zd * cos(angle)), color);

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotRotatedEllipseWidth(int x, int y, int a, int b, float angle_deg, int th, uint16_t color)
{ /* plot ellipse rotated by angle (radian) */
  if(!this->isCreated()) create();

  float angle = radians(angle_deg);
  float xd = a * a, yd = b * b;
  float s = sin(angle), zd = (xd - yd) * s;           /* ellipse rotation */
  xd = sqrt(xd - zd * s), yd = sqrt(yd + zd * s); /* surrounding rect*/
  a = floor(xd + 0.5); b = floor(yd + 0.5); zd = zd * a * b / (xd * yd);
  plotRotatedEllipseRectWidth(x - a, y - b, x + a, y + b, (4 * zd * cos(angle)), th, color);

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotRotatedEllipseRectWidth(int x0, int y0, int x1, int y1, float zd, int th, uint16_t color)
{ /* rectangle enclosing the ellipse, floateger rotation angle */
  if(!this->isCreated()) create();

  float xd = x1 - x0, yd = y1 - y0, w = xd * yd;
  if (w != 0.0) w = (w - zd) / (w + w);              /* squared weight of P1 */
  assert(w <= 1.0 && w >= 0.0);                /* limit angle to |zd|<=xd*yd */
  xd = floor(xd * w + 0.5); yd = floor(yd * w + 0.5); /* snap to float */
  plotQuadRationalBezierWidthSeg(x0, y0 + yd, x0, y0, x0 + xd, y0, 1.0 - w, th, color);
  plotQuadRationalBezierWidthSeg(x0, y0 + yd, x0, y1, x1 - xd, y1, w, th, color);
  plotQuadRationalBezierWidthSeg(x1, y1 - yd, x1, y1, x1 - xd, y1, 1.0 - w, th, color);
  plotQuadRationalBezierWidthSeg(x1, y1 - yd, x1, y0, x0 + xd, y0, w, th, color);

  lv_obj_invalidate(this->obj);
}

//------------------------------------------------
void GCanvas::plotQuadRationalBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color)
{ /* plot a limited rational Bezier segment, squared weight */
  if(!this->isCreated()) create();

  float sx = x2 - x1, sy = y2 - y1;               /* relative values for checks */
  float dx = x0 - x2, dy = y0 - y2, xx = x0 - x1, yy = y0 - y1;
  float xy = xx * sy + yy * sx, cur = xx * sy - yy * sx, err;      /* curvature */

  //  assert(xx*sx <= 0.0 && yy*sy <= 0.0);   /* sign of gradient must not change */

  if (cur != 0.0 && w > 0.0) {                            /* no straight line */
    if (sx * sx + sy * sy > xx * xx + yy * yy) {   /* begin with shorter part */
      x2 = x0; x0 -= dx; y2 = y0; y0 -= dy; cur = -cur;         /* swap P0 P2 */
    }
    xx = 2.0 * (4.0 * w * sx * xx + dx * dx);       /* differences 2nd degree */
    yy = 2.0 * (4.0 * w * sy * yy + dy * dy);
    sx = x0 < x2 ? 1 : -1;                                /* x step direction */
    sy = y0 < y2 ? 1 : -1;                                /* y step direction */
    xy = -2.0 * sx * sy * (2.0 * w * xy + dx * dy);

    if (cur * sx * sy < 0.0) {                          /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0 * w * (x1 - x0) * sy * cur + xx / 2.0 + xy; /* differences 1st degree */
    dy = 4.0 * w * (y0 - y1) * sx * cur + yy / 2.0 + xy;

    if (w < 0.5 && (dy > xy || dx < xy)) {   /* flat ellipse, algorithm fails */
      cur = (w + 1.0) / 2.0; w = sqrt(w); xy = 1.0 / (w + 1.0);
      sx = floor((x0 + 2.0 * w * x1 + x2) * xy / 2.0 + 0.5); /*subdivide curve in half */
      sy = floor((y0 + 2.0 * w * y1 + y2) * xy / 2.0 + 0.5);
      dx = floor((w * x1 + x0) * xy + 0.5); dy = floor((y1 * w + y0) * xy + 0.5);
      plotQuadRationalBezierSeg(x0, y0, dx, dy, sx, sy, cur, color); /* plot separately */
      dx = floor((w * x1 + x2) * xy + 0.5); dy = floor((y1 * w + y2) * xy + 0.5);
      plotQuadRationalBezierSeg(sx, sy, dx, dy, x2, y2, cur, color);
      return;
    }
    err = dx + dy - xy;                                       /* error 1.step */
    do {
      setPixel(x0, y0, color);                                         /* plot curve */
      if (x0 == x2 && y0 == y2) return;       /* last pixel -> curve finished */
      x1 = 2 * err > dy; y1 = 2 * (err + yy) < -dy; /* save value for test of x step */
      if (2 * err < dx || y1) {
        y0 += sy;  /* y step */
        dy += xy;
        err += dx += xx;
      }
      if (2 * err > dx || x1) {
        x0 += sx;  /* x step */
        dx += xy;
        err += dy += yy;
      }
    } while (dy <= xy && dx >= xy);    /* gradient negates -> algorithm fails */
  }
  plotLine(x0, y0, x2, y2, color);                   /* plot remaining needle to end */

  lv_obj_invalidate(this->obj);
}


void GCanvas::plotQuadRationalBezier(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color)
{ /* plot any quadratic rational Bezier curve */
  if(!this->isCreated()) create();

  int x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 + y2;
  float xx = x0 - x1, yy = y0 - y1, ww, t, q;

  //   assert(w >= 0.0);

  if (xx * (x2 - x1) > 0) {                         /* horizontal cut at P4? */
    if (yy * (y2 - y1) > 0)                      /* vertical cut at P6 too? */
      if (fabs(xx * y) > fabs(yy * x)) {                   /* which first? */
        x0 = x2; x2 = xx + x1; y0 = y2; y2 = yy + y1;      /* swap points */
      }                            /* now horizontal cut at P4 comes first */
    if (x0 == x2 || w == 1.0) t = (x0 - x1) / (float)x;
    else {                                 /* non-rational or rational case */
      q = sqrt(4.0 * w * w * (x0 - x1) * (x2 - x1) + (x2 - x0) * (long)(x2 - x0));
      if (x1 < x0) q = -q;
      t = (2.0 * w * (x0 - x1) - x0 + x2 + q) / (2.0 * (1.0 - w) * (x2 - x0)); /* t at P4 */
    }
    q = 1.0 / (2.0 * t * (1.0 - t) * (w - 1.0) + 1.0);   /* sub-divide at t */
    xx = (t * t * (x0 - 2.0 * w * x1 + x2) + 2.0 * t * (w * x1 - x0) + x0) * q; /* = P4 */
    yy = (t * t * (y0 - 2.0 * w * y1 + y2) + 2.0 * t * (w * y1 - y0) + y0) * q;
    ww = t * (w - 1.0) + 1.0; ww *= ww * q;            /* squared weight P3 */
    w = ((1.0 - t) * (w - 1.0) + 1.0) * sqrt(q);               /* weight P8 */
    x = floor(xx + 0.5); y = floor(yy + 0.5);                         /* P4 */
    yy = (xx - x0) * (y1 - y0) / (x1 - x0) + y0;    /* intersect P3 | P0 P1 */
    plotQuadRationalBezierSeg(x0, y0, x, floor(yy + 0.5), x, y, ww, color);
    yy = (xx - x2) * (y1 - y2) / (x1 - x2) + y2;    /* intersect P4 | P1 P2 */
    y1 = floor(yy + 0.5); x0 = x1 = x; y0 = y;          /* P0 = P4, P1 = P8 */
  }
  if ((y0 - y1) * (long)(y2 - y1) > 0) {              /* vertical cut at P6? */
    if (y0 == y2 || w == 1.0) t = (y0 - y1) / (y0 - 2.0 * y1 + y2);
    else {                                 /* non-rational or rational case */
      q = sqrt(4.0 * w * w * (y0 - y1) * (y2 - y1) + (y2 - y0) * (long)(y2 - y0));
      if (y1 < y0) q = -q;
      t = (2.0 * w * (y0 - y1) - y0 + y2 + q) / (2.0 * (1.0 - w) * (y2 - y0)); /* t at P6 */
    }
    q = 1.0 / (2.0 * t * (1.0 - t) * (w - 1.0) + 1.0);   /* sub-divide at t */
    xx = (t * t * (x0 - 2.0 * w * x1 + x2) + 2.0 * t * (w * x1 - x0) + x0) * q; /* = P6 */
    yy = (t * t * (y0 - 2.0 * w * y1 + y2) + 2.0 * t * (w * y1 - y0) + y0) * q;
    ww = t * (w - 1.0) + 1.0; ww *= ww * q;            /* squared weight P5 */
    w = ((1.0 - t) * (w - 1.0) + 1.0) * sqrt(q);               /* weight P7 */
    x = floor(xx + 0.5); y = floor(yy + 0.5);                         /* P6 */
    xx = (x1 - x0) * (yy - y0) / (y1 - y0) + x0;    /* intersect P6 | P0 P1 */
    plotQuadRationalBezierSeg(x0, y0, floor(xx + 0.5), y, x, y, ww, color);
    xx = (x1 - x2) * (yy - y2) / (y1 - y2) + x2;    /* intersect P7 | P1 P2 */
    x1 = floor(xx + 0.5); x0 = x; y0 = y1 = y;          /* P0 = P6, P1 = P7 */
  }
  plotQuadRationalBezierSeg(x0, y0, x1, y1, x2, y2, w * w, color);     /* remaining */

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotQuadRationalBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color)
{ /* draw an anti-aliased rational quadratic Bezier segment, squared weight */
  if(!this->isCreated()) create();

  float sx = x2 - x1, sy = y2 - y1;              /* relative values for checks */
  float dx = x0 - x2, dy = y0 - y2, xx = x0 - x1, yy = y0 - y1;
  float xy = xx * sy + yy * sx, cur = xx * sy - yy * sx, err, ed, f; /* curvature */

  //   assert(xx*sx <= 0.0 && yy*sy <= 0.0);  /* sign of gradient must not change */

  if (cur != 0.0 && w > 0.0) {                           /* no straight line */
    if (sx * sx + sy * sy > xx * xx + yy * yy) { /* begin with shorter part */
      x2 = x0; x0 -= dx; y2 = y0; y0 -= dy; cur = -cur;      /* swap P0 P2 */
    }
    xx = 2.0 * (4.0 * w * sx * xx + dx * dx);     /* differences 2nd degree */
    yy = 2.0 * (4.0 * w * sy * yy + dy * dy);
    sx = x0 < x2 ? 1 : -1;                              /* x step direction */
    sy = y0 < y2 ? 1 : -1;                              /* y step direction */
    xy = -2.0 * sx * sy * (2.0 * w * xy + dx * dy);

    if (cur * sx * sy < 0) {                          /* negated curvature? */
      xx = -xx; yy = -yy; cur = -cur; xy = -xy;
    }
    dx = 4.0 * w * (x1 - x0) * sy * cur + xx / 2.0 + xy; /* differences 1st degree */
    dy = 4.0 * w * (y0 - y1) * sx * cur + yy / 2.0 + xy;

    if (w < 0.5 && dy > dx) {              /* flat ellipse, algorithm fails */
      cur = (w + 1.0) / 2.0; w = sqrt(w); xy = 1.0 / (w + 1.0);
      sx = floor((x0 + 2.0 * w * x1 + x2) * xy / 2.0 + 0.5); /* subdivide curve */
      sy = floor((y0 + 2.0 * w * y1 + y2) * xy / 2.0 + 0.5);
      dx = floor((w * x1 + x0) * xy + 0.5); dy = floor((y1 * w + y0) * xy + 0.5);
      plotQuadRationalBezierSegAA(x0, y0, dx, dy, sx, sy, cur, color); /* plot apart */
      dx = floor((w * x1 + x2) * xy + 0.5); dy = floor((y1 * w + y2) * xy + 0.5);
      return plotQuadRationalBezierSegAA(sx, sy, dx, dy, x2, y2, cur, color);
    }
    err = dx + dy - xy;                                   /* error 1st step */
    do {                                                      /* pixel loop */
      cur = min(dx - xy, xy - dy); ed = max(dx - xy, xy - dy);
      ed += 2 * ed * cur * cur / (4.*ed * ed + cur * cur); /* approximate error distance */
      x1 = 255 * abs(err - dx - dy + xy) / ed; /* get blend value by pixel error */
      if (x1 < 256) setPixelAA(x0, y0, color, x1);                  /* plot curve */
      if (f = 2 * err + dy < 0) {                                /* y step */
        if (y0 == y2) return;             /* last pixel -> curve finished */
        if (dx - err < ed) setPixelAA(x0 + sx, y0, color, 255 * abs(dx - err) / ed);
      }
      if (2 * err + dx > 0) {                                    /* x step */
        if (x0 == x2) return;             /* last pixel -> curve finished */
        if (err - dy < ed) setPixelAA(x0, y0 + sy, color, 255 * abs(err - dy) / ed);
        x0 += sx; dx += xy; err += dy += yy;
      }
      if (f) {
        y0 += sy;  /* y step */
        dy += xy;
        err += dx += xx;
      }
    } while (dy < dx);               /* gradient negates -> algorithm fails */
  }
  plotLineAA(x0, y0, x2, y2, color);                /* plot remaining needle to end */

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotQuadRationalBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color)
{ /* plot any anti-aliased quadratic rational Bezier curve */
  if(!this->isCreated()) create();

  float x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 + y2;
  float xx = x0 - x1, yy = y0 - y1, ww, t, q;

  //   assert(w >= 0.0);

  if (xx * (x2 - x1) > 0) {                         /* horizontal cut at P4? */
    if (yy * (y2 - y1) > 0)                      /* vertical cut at P6 too? */
      if (abs(xx * y) > abs(yy * x)) {           /* which first? */
        x0 = x2; x2 = xx + x1; y0 = y2; y2 = yy + y1;      /* swap pofloats */
      }                            /* now horizontal cut at P4 comes first */
    if (x0 == x2 || w == 1.0) t = (x0 - x1) / x;
    else {                                 /* non-rational or rational case */
      q = sqrt(4.0 * w * w * (x0 - x1) * (x2 - x1) + (x2 - x0) * (x2 - x0));
      if (x1 < x0) q = -q;
      t = (2.0 * w * (x0 - x1) - x0 + x2 + q) / (2.0 * (1.0 - w) * (x2 - x0)); /* t at P4 */
    }
    q = 1.0 / (2.0 * t * (1.0 - t) * (w - 1.0) + 1.0);   /* sub-divide at t */
    xx = (t * t * (x0 - 2.0 * w * x1 + x2) + 2.0 * t * (w * x1 - x0) + x0) * q; /* = P4 */
    yy = (t * t * (y0 - 2.0 * w * y1 + y2) + 2.0 * t * (w * y1 - y0) + y0) * q;
    ww = t * (w - 1.0) + 1.0; ww *= ww * q;            /* squared weight P3 */
    w = ((1.0 - t) * (w - 1.0) + 1.0) * sqrt(q);          /* weight P8 */
    x = floor(xx + 0.5); y = floor(yy + 0.5);               /* P4 */
    yy = (xx - x0) * (y1 - y0) / (x1 - x0) + y0;    /* floatersect P3 | P0 P1 */
    plotQuadRationalBezierSegAA(x0, y0, x, floor(yy + 0.5), x, y, ww, color);
    yy = (xx - x2) * (y1 - y2) / (x1 - x2) + y2;    /* floatersect P4 | P1 P2 */
    y1 = floor(yy + 0.5); x0 = x1 = x; y0 = y;     /* P0 = P4, P1 = P8 */
  }
  if ((y0 - y1) * (y2 - y1) > 0) {                    /* vertical cut at P6? */
    if (y0 == y2 || w == 1.0) t = (y0 - y1) / (y0 - 2.0 * y1 + y2);
    else {                                 /* non-rational or rational case */
      q = sqrt(4.0 * w * w * (y0 - y1) * (y2 - y1) + (y2 - y0) * (y2 - y0));
      if (y1 < y0) q = -q;
      t = (2.0 * w * (y0 - y1) - y0 + y2 + q) / (2.0 * (1.0 - w) * (y2 - y0)); /* t at P6 */
    }
    q = 1.0 / (2.0 * t * (1.0 - t) * (w - 1.0) + 1.0);   /* sub-divide at t */
    xx = (t * t * (x0 - 2.0 * w * x1 + x2) + 2.0 * t * (w * x1 - x0) + x0) * q; /* = P6 */
    yy = (t * t * (y0 - 2.0 * w * y1 + y2) + 2.0 * t * (w * y1 - y0) + y0) * q;
    ww = t * (w - 1.0) + 1.0; ww *= ww * q;            /* squared weight P5 */
    w = ((1.0 - t) * (w - 1.0) + 1.0) * sqrt(q);          /* weight P7 */
    x = floor(xx + 0.5); y = floor(yy + 0.5);               /* P6 */
    xx = (x1 - x0) * (yy - y0) / (y1 - y0) + x0;    /* floatersect P6 | P0 P1 */
    plotQuadRationalBezierSegAA(x0, y0, floor(xx + 0.5), y, x, y, ww, color);
    xx = (x1 - x2) * (yy - y2) / (y1 - y2) + x2;    /* floatersect P7 | P1 P2 */
    x1 = floor(xx + 0.5); x0 = x; y0 = y1 = y;     /* P0 = P6, P1 = P7 */
  }
  plotQuadRationalBezierSegAA(x0, y0, x1, y1, x2, y2, w * w, color); /* remaining part */

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotQuadRationalBezierWidthSeg(int x0, int y0, int x1, int y1, int x2, int y2, float w, int th, uint16_t color)
{ /* plot a limited rational Bezier segment of thickness th, squared weight */
  if(!this->isCreated()) create();

  float sx = x2 - x1, sy = y2 - y1;              /* relative values for checks */
  float dx = x0 - x2, dy = y0 - y2, xx = x0 - x1, yy = y0 - y1;
  float xy = xx * sy + yy * sx, cur = xx * sy - yy * sx, err, e2, ed; /* curvature */

  //   assert(xx*sx <= 0.0 && yy*sy <= 0.0);  /* sign of gradient must not change */

  if (cur != 0.0 && w > 0.0) {                           /* no straight line */
    if (sx * sx + sy * sy > xx * xx + yy * yy) {  /* begin with longer part */
      x2 = x0; x0 -= dx; y2 = y0; y0 -= dy; cur = -cur;      /* swap P0 P2 */
    }
    xx = 2.0 * (4.0 * w * sx * xx + dx * dx);     /* differences 2nd degree */
    yy = 2.0 * (4.0 * w * sy * yy + dy * dy);
    sx = x0 < x2 ? 1 : -1;                              /* x step direction */
    sy = y0 < y2 ? 1 : -1;                              /* y step direction */
    xy = -2.0 * sx * sy * (2.0 * w * xy + dx * dy);

    if (cur * sx * sy < 0) {                          /* negated curvature? */
      xx = -xx; yy = -yy; cur = -cur; xy = -xy;
    }
    dx = 4.0 * w * (x1 - x0) * sy * cur + xx / 2.0; /* differences 1st degree */
    dy = 4.0 * w * (y0 - y1) * sx * cur + yy / 2.0;

    if (w < 0.5 && (dx + xx <= 0 || dy + yy >= 0)) { /* flat ellipse, algo fails */
      cur = (w + 1.0) / 2.0; w = sqrt(w); xy = 1.0 / (w + 1.0); //w = fsqrt(w); xy = 1.0 / (w + 1.0);
      sx = floor((x0 + 2.0 * w * x1 + x2) * xy / 2.0 + 0.5); /* subdivide curve  */
      sy = floor((y0 + 2.0 * w * y1 + y2) * xy / 2.0 + 0.5); /* plot separately */
      dx = floor((w * x1 + x0) * xy + 0.5); dy = floor((y1 * w + y0) * xy + 0.5);
      plotQuadRationalBezierWidthSeg(x0, y0, dx, dy, sx, sy, cur, th, color);
      dx = floor((w * x1 + x2) * xy + 0.5); dy = floor((y1 * w + y2) * xy + 0.5);
      return plotQuadRationalBezierWidthSeg(sx, sy, dx, dy, x2, y2, cur, th, color);
    }
fail:
    for (err = 0; dy + 2 * yy < 0 && dx + 2 * xx > 0; ) /* loop of steep/flat curve */
      if (dx + dy + xy < 0) {                               /* steep curve */
        do {
          ed = -dy - 2 * dy * dx * dx / (4.*dy * dy + dx * dx); /* approximate sqrt */
          w = (th - 1) * ed;                         /* scale line width */
          x1 = floor((err - ed - w / 2) / dy);      /* start offset */
          e2 = err - x1 * dy - w / 2;           /* error value at offset */
          x1 = x0 - x1 * sx;                              /* start pofloat */
          setPixelAA(x1, y0, color, 255 * e2 / ed);       /* aliasing pre-pixel */
          for (e2 = -w - dy - e2; e2 - dy < ed; e2 -= dy)
            setPixel(x1 += sx, y0, color);              /* pixel on thick line */
          setPixelAA(x1 + sx, y0, color, 255 * e2 / ed); /* aliasing post-pixel */
          if (y0 == y2) return;          /* last pixel -> curve finished */
          y0 += sy; dy += xy; err += dx; dx += xx;             /* y step */
          if (2 * err + dy > 0) {                        /* e_x+e_xy > 0 */
            x0 += sx; dx += xy; err += dy; dy += yy;          /* x step */
          }
          if (x0 != x2 && (dx + 2 * xx <= 0 || dy + 2 * yy >= 0))
            if (abs(y2 - y0) > abs(x2 - x0)) goto fail;
            else break;                             /* other curve near */
        } while (dx + dy + xy < 0);              /* gradient still steep? */
        /* change from steep to flat curve */
        for (cur = err - dy - w / 2, y1 = y0; cur < ed; y1 += sy, cur += dx) {
          for (e2 = cur, x1 = x0; e2 - dy < ed; e2 -= dy)
            setPixel(x1 -= sx, y1, color);              /* pixel on thick line */
          setPixelAA(x1 - sx, y1, color, 255 * e2 / ed); /* aliasing post-pixel */
        }
      } else {                                               /* flat curve */
        do {
          ed = dx + 2 * dx * dy * dy / (4.*dx * dx + dy * dy); /* approximate sqrt */
          w = (th - 1) * ed;                         /* scale line width */
          y1 = floor((err + ed + w / 2) / dx);      /* start offset */
          e2 = y1 * dx - w / 2 - err;           /* error value at offset */
          y1 = y0 - y1 * sy;                              /* start pofloat */
          setPixelAA(x0, y1, color, 255 * e2 / ed);       /* aliasing pre-pixel */
          for (e2 = dx - e2 - w; e2 + dx < ed; e2 += dx)
            setPixel(x0, y1 += sy, color);              /* pixel on thick line */
          setPixelAA(x0, y1 + sy, color, 255 * e2 / ed); /* aliasing post-pixel */
          if (x0 == x2) return;          /* last pixel -> curve finished */
          x0 += sx; dx += xy; err += dy; dy += yy;             /* x step */
          if (2 * err + dx < 0)  {                       /* e_y+e_xy < 0 */
            y0 += sy; dy += xy; err += dx; dx += xx;          /* y step */
          }
          if (y0 != y2 && (dx + 2 * xx <= 0 || dy + 2 * yy >= 0))
            if (abs(y2 - y0) <= abs(x2 - x0)) goto fail;
            else break;                             /* other curve near */
        } while (dx + dy + xy >= 0);              /* gradient still flat? */
        /* change from flat to steep curve */
        for (cur = -err + dx - w / 2, x1 = x0; cur < ed; x1 += sx, cur -= dy) {
          for (e2 = cur, y1 = y0; e2 + dx < ed; e2 += dx)
            setPixel(x1, y1 -= sy, color);              /* pixel on thick line */
          setPixelAA(x1, y1 - sy, color, 255 * e2 / ed); /* aliasing post-pixel */
        }
      }
  }
  plotLineWidth(x0, y0, x2, y2, th, color);          /* confusing error values  */

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotQuadRationalBezierWidth(int x0, int y0, int x1, int y1, int x2, int y2, float w, int th, uint16_t color)
{ /* plot any anti-aliased quadratic rational Bezier curve */
  if(!this->isCreated()) create();

  float x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 + y2;
  float xx = x0 - x1, yy = y0 - y1, ww, t, q;

  //   assert(w >= 0.0);

  if (xx * (x2 - x1) > 0) {                         /* horizontal cut at P4? */
    if (yy * (y2 - y1) > 0)                      /* vertical cut at P6 too? */
      if (abs(xx * y) > abs(yy * x)) {           /* which first? */
        x0 = x2; x2 = xx + x1; y0 = y2; y2 = yy + y1;      /* swap pofloats */
      }                            /* now horizontal cut at P4 comes first */
    if (x0 == x2 || w == 1.0) t = (x0 - x1) / x;
    else {                                 /* non-rational or rational case */
      q = sqrt(4.0 * w * w * (x0 - x1) * (x2 - x1) + (x2 - x0) * (x2 - x0));
      if (x1 < x0) q = -q;
      t = (2.0 * w * (x0 - x1) - x0 + x2 + q) / (2.0 * (1.0 - w) * (x2 - x0)); /* t at P4 */
    }
    q = 1.0 / (2.0 * t * (1.0 - t) * (w - 1.0) + 1.0);   /* sub-divide at t */
    xx = (t * t * (x0 - 2.0 * w * x1 + x2) + 2.0 * t * (w * x1 - x0) + x0) * q; /* = P4 */
    yy = (t * t * (y0 - 2.0 * w * y1 + y2) + 2.0 * t * (w * y1 - y0) + y0) * q;
    ww = t * (w - 1.0) + 1.0; ww *= ww * q;            /* squared weight P3 */
    w = ((1.0 - t) * (w - 1.0) + 1.0) * sqrt(q);          /* weight P8 */
    x = floor(xx + 0.5); y = floor(yy + 0.5);               /* P4 */
    yy = (xx - x0) * (y1 - y0) / (x1 - x0) + y0;    /* floatersect P3 | P0 P1 */
    plotQuadRationalBezierWidthSeg(x0, y0, x, floor(yy + 0.5), x, y, ww, th, color);
    yy = (xx - x2) * (y1 - y2) / (x1 - x2) + y2;    /* floatersect P4 | P1 P2 */
    y1 = floor(yy + 0.5); x0 = x1 = x; y0 = y;     /* P0 = P4, P1 = P8 */
  }
  if ((y0 - y1) * (y2 - y1) > 0) {                    /* vertical cut at P6? */
    if (y0 == y2 || w == 1.0) t = (y0 - y1) / (y0 - 2.0 * y1 + y2);
    else {                                 /* non-rational or rational case */
      q = sqrt(4.0 * w * w * (y0 - y1) * (y2 - y1) + (y2 - y0) * (y2 - y0));
      if (y1 < y0) q = -q;
      t = (2.0 * w * (y0 - y1) - y0 + y2 + q) / (2.0 * (1.0 - w) * (y2 - y0)); /* t at P6 */
    }
    q = 1.0 / (2.0 * t * (1.0 - t) * (w - 1.0) + 1.0);   /* sub-divide at t */
    xx = (t * t * (x0 - 2.0 * w * x1 + x2) + 2.0 * t * (w * x1 - x0) + x0) * q; /* = P6 */
    yy = (t * t * (y0 - 2.0 * w * y1 + y2) + 2.0 * t * (w * y1 - y0) + y0) * q;
    ww = t * (w - 1.0) + 1.0; ww *= ww * q;            /* squared weight P5 */
    w = ((1.0 - t) * (w - 1.0) + 1.0) * sqrt(q);          /* weight P7 */
    x = floor(xx + 0.5); y = floor(yy + 0.5);               /* P6 */
    xx = (x1 - x0) * (yy - y0) / (y1 - y0) + x0;    /* floatersect P6 | P0 P1 */
    plotQuadRationalBezierWidthSeg(x0, y0, floor(xx + 0.5), y, x, y, ww, th, color);
    xx = (x1 - x2) * (yy - y2) / (y1 - y2) + x2;    /* floatersect P7 | P1 P2 */
    x1 = floor(xx + 0.5); x0 = x; y0 = y1 = y;     /* P0 = P6, P1 = P7 */
  }
  plotQuadRationalBezierWidthSeg(x0, y0, x1, y1, x2, y2, w * w, th, color);

  lv_obj_invalidate(this->obj);
}
//------------------------------------------
void GCanvas::plotQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{ /* plot a limited quadratic Bezier segment */
  if(!this->isCreated()) create();

  float sx = x2 - x1, sy = y2 - y1;
  float xx = x0 - x1, yy = y0 - y1, xy;           /* relative values for checks */
  float dx, dy, err, cur = xx * sy - yy * sx;                      /* curvature */

  //  assert(xx*sx <= 0 && yy*sy <= 0);       /* sign of gradient must not change */

  if (sx * sx + sy * sy > xx * xx + yy * yy) {     /* begin with shorter part */
    x2 = x0; x0 = sx + x1; y2 = y0; y0 = sy + y1; cur = -cur;   /* swap P0 P2 */
  }
  if (cur != 0) {                                         /* no straight line */
    xx += sx; xx *= sx = x0 < x2 ? 1 : -1;                /* x step direction */
    yy += sy; yy *= sy = y0 < y2 ? 1 : -1;                /* y step direction */
    xy = 2 * xx * yy; xx *= xx; yy *= yy;           /* differences 2nd degree */
    if (cur * sx * sy < 0) {                            /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0 * sy * cur * (x1 - x0) + xx - xy;      /* differences 1st degree */
    dy = 4.0 * sx * cur * (y0 - y1) + yy - xy;
    xx += xx; yy += yy; err = dx + dy + xy;                 /* error 1st step */
    do {
      setPixel(x0, y0, color);                                         /* plot curve */
      if (x0 == x2 && y0 == y2) return;       /* last pixel -> curve finished */
      y1 = 2 * err < dx;                     /* save value for test of y step */
      if (2 * err > dy) {
        x0 += sx;  /* x step */
        dx -= xy;
        err += dy += yy;
      }
      if (    y1    ) {
        y0 += sy;  /* y step */
        dy -= xy;
        err += dx += xx;
      }
    } while (dy < 0 && dx > 0);        /* gradient negates -> algorithm fails */
  }
  plotLine(x0, y0, x2, y2, color);                     /* plot remaining part to end */

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{ /* plot any quadratic Bezier curve */
  if(!this->isCreated()) create();

  float x = x0 - x1, y = y0 - y1, t = x0 - 2 * x1 + x2, r;

  if (x * (x2 - x1) > 0) {                          /* horizontal cut at P4? */
    if (y * (y2 - y1) > 0)                       /* vertical cut at P6 too? */
      if (abs((y0 - 2 * y1 + y2) / t * x) > abs(y)) { /* which first? */
        x0 = x2; x2 = x + x1; y0 = y2; y2 = y + y1;        /* swap pofloats */
      }                            /* now horizontal cut at P4 comes first */
    t = (x0 - x1) / t;
    r = (1 - t) * ((1 - t) * y0 + 2.0 * t * y1) + t * t * y2;   /* By(t=P4) */
    t = (x0 * x2 - x1 * x1) * t / (x0 - x1);           /* gradient dP4/dx=0 */
    x = floor(t + 0.5); y = floor(r + 0.5);
    r = (y1 - y0) * (t - x0) / (x1 - x0) + y0;      /* floatersect P3 | P0 P1 */
    plotQuadBezierSeg(x0, y0, x, floor(r + 0.5), x, y, color);
    r = (y1 - y2) * (t - x2) / (x1 - x2) + y2;      /* floatersect P4 | P1 P2 */
    x0 = x1 = x; y0 = y; y1 = floor(r + 0.5);      /* P0 = P4, P1 = P8 */
  }
  if ((y0 - y1) * (y2 - y1) > 0) {                    /* vertical cut at P6? */
    t = y0 - 2 * y1 + y2; t = (y0 - y1) / t;
    r = (1 - t) * ((1 - t) * x0 + 2.0 * t * x1) + t * t * x2;   /* Bx(t=P6) */
    t = (y0 * y2 - y1 * y1) * t / (y0 - y1);           /* gradient dP6/dy=0 */
    x = floor(r + 0.5); y = floor(t + 0.5);
    r = (x1 - x0) * (t - y0) / (y1 - y0) + x0;      /* floatersect P6 | P0 P1 */
    plotQuadBezierSeg(x0, y0, floor(r + 0.5), y, x, y, color);
    r = (x1 - x2) * (t - y2) / (y1 - y2) + x2;      /* floatersect P7 | P1 P2 */
    x0 = x; x1 = floor(r + 0.5); y0 = y1 = y;      /* P0 = P6, P1 = P7 */
  }
  plotQuadBezierSeg(x0, y0, x1, y1, x2, y2, color);               /* remaining part */

  lv_obj_invalidate(this->obj);
}
//-----------------------------------------------------------------------------
void GCanvas::plotQuadBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{ /* draw an limited anti-aliased quadratic Bezier segment */
  if(!this->isCreated()) create();

  float sx = x2 - x1, sy = y2 - y1;
  float xx = x0 - x1, yy = y0 - y1, xy;          /* relative values for checks */
  float dx, dy, err, ed, cur = xx * sy - yy * sx;                 /* curvature */

  //   assert(xx*sx <= 0 && yy*sy <= 0);      /* sign of gradient must not change */

  if (sx * sx + sy * sy > xx * xx + yy * yy) {    /* begin with shorter part */
    x2 = x0; x0 = sx + x1; y2 = y0; y0 = sy + y1; cur = -cur; /* swap P0 P2 */
  }
  if (cur != 0)
  { /* no straight line */
    xx += sx; xx *= sx = x0 < x2 ? 1 : -1;              /* x step direction */
    yy += sy; yy *= sy = y0 < y2 ? 1 : -1;              /* y step direction */
    xy = 2 * xx * yy; xx *= xx; yy *= yy;         /* differences 2nd degree */
    if (cur * sx * sy < 0) {                          /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0 * sy * (x1 - x0) * cur + xx - xy;    /* differences 1st degree */
    dy = 4.0 * sx * (y0 - y1) * cur + yy - xy;
    xx += xx; yy += yy; err = dx + dy + xy;               /* error 1st step */
    do {
      cur = min(dx + xy, -xy - dy);
      ed = max(dx + xy, -xy - dy);      /* approximate error distance */
      ed += 2 * ed * cur * cur / (4 * ed * ed + cur * cur);
      setPixelAA(x0, y0, color, 255 * abs(err - dx - dy - xy) / ed); /* plot curve */
      if (x0 == x2 || y0 == y2) break;     /* last pixel -> curve finished */
      x1 = x0; cur = dx - err; y1 = 2 * err + dy < 0;
      if (2 * err + dx > 0) {                                    /* x step */
        if (err - dy < ed) setPixelAA(x0, y0 + sy, color, 255 * abs(err - dy) / ed);
        x0 += sx; dx -= xy; err += dy += yy;
      }
      if (y1) {                                                  /* y step */
        if (cur < ed) setPixelAA(x1 + sx, y0, color, 255 * abs(cur) / ed);
        y0 += sy; dy -= xy; err += dx += xx;
      }
    } while (dy < dx);                  /* gradient negates -> close curves */
  }
  plotLineAA(x0, y0, x2, y2, color);                /* plot remaining needle to end */

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotQuadBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color)
{ /* plot any anti-aliased quadratic Bezier curve */
  if(!this->isCreated()) create();

  float x = x0 - x1, y = y0 - y1, t = x0 - 2 * x1 + x2, r;

  if (x * (x2 - x1) > 0) {                          /* horizontal cut at P4? */
    if (y * (y2 - y1) > 0)                       /* vertical cut at P6 too? */
      if (abs((y0 - 2 * y1 + y2) / t * x) > abs(y)) { /* which first? */
        x0 = x2; x2 = x + x1; y0 = y2; y2 = y + y1;        /* swap pofloats */
      }                            /* now horizontal cut at P4 comes first */
    t = (x0 - x1) / t;
    r = (1 - t) * ((1 - t) * y0 + 2.0 * t * y1) + t * t * y2;   /* By(t=P4) */
    t = (x0 * x2 - x1 * x1) * t / (x0 - x1);           /* gradient dP4/dx=0 */
    x = floor(t + 0.5); y = floor(r + 0.5);
    r = (y1 - y0) * (t - x0) / (x1 - x0) + y0;      /* floatersect P3 | P0 P1 */
    plotQuadBezierSegAA(x0, y0, x, floor(r + 0.5), x, y, color);
    r = (y1 - y2) * (t - x2) / (x1 - x2) + y2;      /* floatersect P4 | P1 P2 */
    x0 = x1 = x; y0 = y; y1 = floor(r + 0.5);      /* P0 = P4, P1 = P8 */
  }
  if ((y0 - y1) * (y2 - y1) > 0) {                    /* vertical cut at P6? */
    t = y0 - 2 * y1 + y2; t = (y0 - y1) / t;
    r = (1 - t) * ((1 - t) * x0 + 2.0 * t * x1) + t * t * x2;   /* Bx(t=P6) */
    t = (y0 * y2 - y1 * y1) * t / (y0 - y1);           /* gradient dP6/dy=0 */
    x = floor(r + 0.5); y = floor(t + 0.5);
    r = (x1 - x0) * (t - y0) / (y1 - y0) + x0;      /* floatersect P6 | P0 P1 */
    plotQuadBezierSegAA(x0, y0, floor(r + 0.5), y, x, y, color);
    r = (x1 - x2) * (t - y2) / (y1 - y2) + x2;      /* floatersect P7 | P1 P2 */
    x0 = x; x1 = floor(r + 0.5); y0 = y1 = y;      /* P0 = P6, P1 = P7 */
  }
  plotQuadBezierSegAA(x0, y0, x1, y1, x2, y2, color);             /* remaining part */

  lv_obj_invalidate(this->obj);
}
//---------------------------------------------
// Cubic Bezier

void GCanvas::plotCubicBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color)
{ /* plot limited cubic Bezier segment */
  if(!this->isCreated()) create();

  float f, fx, fy, leg = 1;
  float sx = x0 < x3 ? 1 : -1, sy = y0 < y3 ? 1 : -1;        /* step direction */
  float xc = -abs(x0 + x1 - x2 - x3), xa = xc - 4 * sx * (x1 - x2), xb = sx * (x0 - x1 - x2 + x3);
  float yc = -abs(y0 + y1 - y2 - y3), ya = yc - 4 * sy * (y1 - y2), yb = sy * (y0 - y1 - y2 + y3);
  float ab, ac, bc, cb, xx, xy, yy, dx, dy, ex, pxy, EP = 0.01;
  /* check for curve restrains */
  /* slope P0-P1 == P2-P3    and  (P0-P3 == P1-P2      or  no slope change)  */
  //   assert((x1-x0)*(x2-x3) < EP && ((x3-x0)*(x1-x2) < EP || xb*xb < xa*xc+EP));
  //   assert((y1-y0)*(y2-y3) < EP && ((y3-y0)*(y1-y2) < EP || yb*yb < ya*yc+EP));

  if (xa == 0 && ya == 0)                                /* quadratic Bezier */
    return plotQuadBezierSeg(x0, y0, (int)(3 * x1 - x0) >> 1, (int)(3 * y1 - y0) >> 1, x3, y3, color);
  x1 = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + 1;    /* line lengths */
  x2 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3) + 1;

  do {                                                /* loop over both ends */
    ab = xa * yb - xb * ya; ac = xa * yc - xc * ya; bc = xb * yc - xc * yb;
    ex = ab * (ab + ac - 3 * bc) + ac * ac; /* P0 part of self-floatersection loop? */
    f = ex > 0 ? 1 : floor(sqrt(1 + 1024 / x1)); /* calc resolution */
    ab *= f; ac *= f; bc *= f; ex *= f * f;          /* increase resolution */
    xy = 9 * (ab + ac + bc) / 8; cb = 8 * (xa - ya); /* init differences of 1st degree */
    dx = 27 * (8 * ab * (yb * yb - ya * yc) + ex * (ya + 2 * yb + yc)) / 64 - ya * ya * (xy - ya);
    dy = 27 * (8 * ab * (xb * xb - xa * xc) - ex * (xa + 2 * xb + xc)) / 64 - xa * xa * (xy + xa);
    /* init differences of 2nd degree */
    xx = 3 * (3 * ab * (3 * yb * yb - ya * ya - 2 * ya * yc) - ya * (3 * ac * (ya + yb) + ya * cb)) / 4;
    yy = 3 * (3 * ab * (3 * xb * xb - xa * xa - 2 * xa * xc) - xa * (3 * ac * (xa + xb) + xa * cb)) / 4;
    xy = xa * ya * (6 * ab + 6 * ac - 3 * bc + cb); ac = ya * ya; cb = xa * xa;
    xy = 3 * (xy + 9 * f * (cb * yb * yc - xb * xc * ac) - 18 * xb * yb * ab) / 8;

    if (ex < 0) {         /* negate values if inside self-floatersection loop */
      dx = -dx; dy = -dy; xx = -xx; yy = -yy; xy = -xy; ac = -ac; cb = -cb;
    }                                     /* init differences of 3rd degree */
    ab = 6 * ya * ac; ac = -6 * xa * ac; bc = 6 * ya * cb; cb = -6 * xa * cb;
    dx += xy; ex = dx + dy; dy += xy;                  /* error of 1st step */
    //exit:
    for (pxy = 0, fx = fy = f; x0 != x3 && y0 != y3; ) {
      setPixel(x0, y0, color);                                      /* plot curve */
      do {                                  /* move sub-steps of one pixel */
        if (pxy == 0) if (dx > xy || dy < xy) goto out_exit1;    /* confusing */
        if (pxy == 1) if (dx > 0 || dy < 0) goto out_exit1;         /* values */
        y1 = 2 * ex - dy;                /* save value for test of y step */
        if (2 * ex >= dx) {                                 /* x sub-step */
          fx--; ex += dx += xx; dy += xy += ac; yy += bc; xx += ab;
        } else if (y1 > 0) goto out_exit1;
        if (y1 <= 0) {                                      /* y sub-step */
          fy--; ex += dy += yy; dx += xy += bc; xx += ac; yy += cb;
        }
      } while (fx > 0 && fy > 0);                       /* pixel complete? */
      if (2 * fx <= f) {
        x0 += sx;  /* x step */
        fx += f;
      }
      if (2 * fy <= f) {
        y0 += sy;  /* y step */
        fy += f;
      }
      if (pxy == 0 && dx < 0 && dy > 0) pxy = 1;      /* pixel ahead valid */
    }
out_exit1:
    xx = x0; x0 = x3; x3 = xx; sx = -sx; xb = -xb;             /* swap legs */
    yy = y0; y0 = y3; y3 = yy; sy = -sy; yb = -yb; x1 = x2;
  } while (leg--);                                          /* try other end */
  plotLine(x0, y0, x3, y3, color);     /* remaining part in case of cusp or crunode */

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotCubicBezier(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color)
{ /* plot any cubic Bezier curve */
  if(!this->isCreated()) create();

  int n = 0, i = 0;
  float xc = x0 + x1 - x2 - x3, xa = xc - 4 * (x1 - x2);
  float xb = x0 - x1 - x2 + x3, xd = xb + 4 * (x1 + x2);
  float yc = y0 + y1 - y2 - y3, ya = yc - 4 * (y1 - y2);
  float yb = y0 - y1 - y2 + y3, yd = yb + 4 * (y1 + y2);
  float fx0 = x0, fx1, fx2, fx3, fy0 = y0, fy1, fy2, fy3;
  float t1 = xb * xb - xa * xc, t2;
  float t[5];
  /* sub-divide curve at gradient sign changes */
  if (xa == 0) {                                               /* horizontal */
    if (abs(xc) < 2 * abs(xb)) t[n++] = xc / (2.0 * xb); /* one change */
  } else if (t1 > 0.0) {                                      /* two changes */
    t2 = sqrt(t1);
    t1 = (xb - t2) / xa; if (abs(t1) < 1.0) t[n++] = t1;
    t1 = (xb + t2) / xa; if (abs(t1) < 1.0) t[n++] = t1;
  }
  t1 = yb * yb - ya * yc;
  if (ya == 0) {                                                 /* vertical */
    if (abs(yc) < 2 * abs(yb)) t[n++] = yc / (2.0 * yb); /* one change */
  } else if (t1 > 0.0) {                                      /* two changes */
    t2 = sqrt(t1);
    t1 = (yb - t2) / ya; if (abs(t1) < 1.0) t[n++] = t1;
    t1 = (yb + t2) / ya; if (abs(t1) < 1.0) t[n++] = t1;
  }
  for (i = 1; i < n; i++)                         /* bubble sort of 4 pofloats */
    if ((t1 = t[i - 1]) > t[i]) {
      t[i - 1] = t[i];
      t[i] = t1;
      i = 0;
    }

  t1 = -1.0; t[n] = 1.0;                                /* begin / end pofloat */
  for (i = 0; i <= n; i++) {                 /* plot each segment separately */
    t2 = t[i];                                /* sub-divide at t[i-1], t[i] */
    fx1 = (t1 * (t1 * xb - 2 * xc) - t2 * (t1 * (t1 * xa - 2 * xb) + xc) + xd) / 8 - fx0;
    fy1 = (t1 * (t1 * yb - 2 * yc) - t2 * (t1 * (t1 * ya - 2 * yb) + yc) + yd) / 8 - fy0;
    fx2 = (t2 * (t2 * xb - 2 * xc) - t1 * (t2 * (t2 * xa - 2 * xb) + xc) + xd) / 8 - fx0;
    fy2 = (t2 * (t2 * yb - 2 * yc) - t1 * (t2 * (t2 * ya - 2 * yb) + yc) + yd) / 8 - fy0;
    fx0 -= fx3 = (t2 * (t2 * (3 * xb - t2 * xa) - 3 * xc) + xd) / 8;
    fy0 -= fy3 = (t2 * (t2 * (3 * yb - t2 * ya) - 3 * yc) + yd) / 8;
    x3 = floor(fx3 + 0.5); y3 = floor(fy3 + 0.5); /* scale bounds */
    if (fx0 != 0.0) {
      fx1 *= fx0 = (x0 - x3) / fx0;
      fx2 *= fx0;
    }
    if (fy0 != 0.0) {
      fy1 *= fy0 = (y0 - y3) / fy0;
      fy2 *= fy0;
    }
    if (x0 != x3 || y0 != y3)                            /* segment t1 - t2 */
      plotCubicBezierSeg(x0, y0, x0 + fx1, y0 + fy1, x0 + fx2, y0 + fy2, x3, y3, color);
    x0 = x3; y0 = y3; fx0 = fx3; fy0 = fy3; t1 = t2;
  }
}
//----------------------------------------
void GCanvas::plotCubicBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color)
{ /* plot limited anti-aliased cubic Bezier segment */
  if(!this->isCreated()) create();

  float f, fx, fy, leg = 1;
  float sx = x0 < x3 ? 1 : -1, sy = y0 < y3 ? 1 : -1;        /* step direction */
  float xc = -abs(x0 + x1 - x2 - x3), xa = xc - 4 * sx * (x1 - x2), xb = sx * (x0 - x1 - x2 + x3);
  float yc = -abs(y0 + y1 - y2 - y3), ya = yc - 4 * sy * (y1 - y2), yb = sy * (y0 - y1 - y2 + y3);
  float ab, ac, bc, ba, xx, xy, yy, dx, dy, ex, px, py, ed, ip, EP = 0.01;
  /* check for curve restrains */
  /* slope P0-P1 == P2-P3    and  (P0-P3 == P1-P2      or   no slope change) */
  //   assert((x1-x0)*(x2-x3) < EP && ((x3-x0)*(x1-x2) < EP || xb*xb < xa*xc+EP));
  //   assert((y1-y0)*(y2-y3) < EP && ((y3-y0)*(y1-y2) < EP || yb*yb < ya*yc+EP));

  if (xa == 0 && ya == 0)                                /* quadratic Bezier */
    return plotQuadBezierSegAA(x0, y0, (int)(3 * x1 - x0) >> 1, (int)(3 * y1 - y0) >> 1, x3, y3, color);
  x1 = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + 1;    /* line lengths */
  x2 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3) + 1;
  //exit:
  do {                                                /* loop over both ends */
    ab = xa * yb - xb * ya; ac = xa * yc - xc * ya; bc = xb * yc - xc * yb;
    ip = 4 * ab * bc - ac * ac;           /* self floatersection loop at all? */
    ex = ab * (ab + ac - 3 * bc) + ac * ac; /* P0 part of self-floatersection loop? */
    f = ex > 0 ? 1 : floor(sqrt(1 + 1024 / x1)); /* calc resolution */
    ab *= f; ac *= f; bc *= f; ex *= f * f;          /* increase resolution */
    xy = 9 * (ab + ac + bc) / 8; ba = 8 * (xa - ya); /* init differences of 1st degree */
    dx = 27 * (8 * ab * (yb * yb - ya * yc) + ex * (ya + 2 * yb + yc)) / 64 - ya * ya * (xy - ya);
    dy = 27 * (8 * ab * (xb * xb - xa * xc) - ex * (xa + 2 * xb + xc)) / 64 - xa * xa * (xy + xa);
    /* init differences of 2nd degree */
    xx = 3 * (3 * ab * (3 * yb * yb - ya * ya - 2 * ya * yc) - ya * (3 * ac * (ya + yb) + ya * ba)) / 4;
    yy = 3 * (3 * ab * (3 * xb * xb - xa * xa - 2 * xa * xc) - xa * (3 * ac * (xa + xb) + xa * ba)) / 4;
    xy = xa * ya * (6 * ab + 6 * ac - 3 * bc + ba); ac = ya * ya; ba = xa * xa;
    xy = 3 * (xy + 9 * f * (ba * yb * yc - xb * xc * ac) - 18 * xb * yb * ab) / 8;

    if (ex < 0) {         /* negate values if inside self-floatersection loop */
      dx = -dx; dy = -dy; xx = -xx; yy = -yy; xy = -xy; ac = -ac; ba = -ba;
    }                                     /* init differences of 3rd degree */
    ab = 6 * ya * ac; ac = -6 * xa * ac; bc = 6 * ya * ba; ba = -6 * xa * ba;
    dx += xy; ex = dx + dy; dy += xy;                  /* error of 1st step */
    //loop:
    for (fx = fy = f; ; ) {
      if (x0 == x3 || y0 == y3) goto out_exit;
      y1 = min(abs(xy - dx), abs(dy - xy));
      ed = max(abs(xy - dx), abs(dy - xy)); /* approximate err */
      ed = f * (ed + 2 * ed * y1 * y1 / (4 * ed * ed + y1 * y1));
      y1 = 255 * abs(ex - (f - fx + 1) * dx - (f - fy + 1) * dy + f * xy) / ed;
      if (y1 < 256) setPixelAA(x0, y0, color, y1);                  /* plot curve */
      px = abs(ex - (f - fx + 1) * dx + (fy - 1) * dy); /* pixel varensity x move */
      py = abs(ex + (fx - 1) * dx - (f - fy + 1) * dy); /* pixel varensity y move */
      y2 = y0;
      do {                                  /* move sub-steps of one pixel */
        if (ip >= -EP)               /* floatersection possible? -> check.. */
          if (dx + xx > xy || dy + yy < xy) goto out_loop; /* two x or y steps */
        y1 = 2 * ex + dx;                /* save value for test of y step */
        if (2 * ex + dy > 0) {                              /* x sub-step */
          fx--; ex += dx += xx; dy += xy += ac; yy += bc; xx += ab;
        } else if (y1 > 0) goto out_loop;                /* tiny nearly cusp */
        if (y1 <= 0) {                                      /* y sub-step */
          fy--; ex += dy += yy; dx += xy += bc; xx += ac; yy += ba;
        }
      } while (fx > 0 && fy > 0);                       /* pixel complete? */
      if (2 * fy <= f) {                         /* x+ anti-aliasing pixel */
        if (py < ed) setPixelAA(x0 + sx, y0, color, 255 * py / ed); /* plot curve */
        y0 += sy; fy += f;                                      /* y step */
      }
      if (2 * fx <= f) {                         /* y+ anti-aliasing pixel */
        if (px < ed) setPixelAA(x0, y2 + sy, color, 255 * px / ed); /* plot curve */
        x0 += sx; fx += f;                                      /* x step */
      }
    }
out_loop:
    if (2 * ex < dy && 2 * fy <= f + 2) {   /* round x+ approximation pixel */
      if (py < ed) setPixelAA(x0 + sx, y0, color, 255 * py / ed);   /* plot curve */
      y0 += sy;
    }
    if (2 * ex > dx && 2 * fx <= f + 2) {   /* round y+ approximation pixel */
      if (px < ed) setPixelAA(x0, y2 + sy, color, 255 * px / ed);   /* plot curve */
      x0 += sx;
    }
    xx = x0; x0 = x3; x3 = xx; sx = -sx; xb = -xb;             /* swap legs */
    yy = y0; y0 = y3; y3 = yy; sy = -sy; yb = -yb; x1 = x2;
  } while (leg--);                                          /* try other end */

out_exit:
  plotLineAA(x0, y0, x3, y3, color);   /* remaining part in case of cusp or crunode */
}

void GCanvas::plotCubicBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color)
{ /* plot any cubic Bezier curve */
  if(!this->isCreated()) create();

  int n = 0, i = 0;
  float xc = x0 + x1 - x2 - x3, xa = xc - 4 * (x1 - x2);
  float xb = x0 - x1 - x2 + x3, xd = xb + 4 * (x1 + x2);
  float yc = y0 + y1 - y2 - y3, ya = yc - 4 * (y1 - y2);
  float yb = y0 - y1 - y2 + y3, yd = yb + 4 * (y1 + y2);
  float fx0 = x0, fx1, fx2, fx3, fy0 = y0, fy1, fy2, fy3;
  float t1 = xb * xb - xa * xc, t2, t[5];
  /* sub-divide curve at gradient sign changes */
  if (xa == 0) {                                               /* horizontal */
    if (abs(xc) < 2 * abs(xb)) t[n++] = xc / (2.0 * xb); /* one change */
  } else if (t1 > 0.0) {                                      /* two changes */
    t2 = sqrt(t1);
    t1 = (xb - t2) / xa; if (abs(t1) < 1.0) t[n++] = t1;
    t1 = (xb + t2) / xa; if (abs(t1) < 1.0) t[n++] = t1;
  }
  t1 = yb * yb - ya * yc;
  if (ya == 0) {                                                 /* vertical */
    if (abs(yc) < 2 * abs(yb)) t[n++] = yc / (2.0 * yb); /* one change */
  } else if (t1 > 0.0) {                                      /* two changes */
    t2 = sqrt(t1);
    t1 = (yb - t2) / ya; if (abs(t1) < 1.0) t[n++] = t1;
    t1 = (yb + t2) / ya; if (abs(t1) < 1.0) t[n++] = t1;
  }
  for (i = 1; i < n; i++)                         /* bubble sort of 4 pofloats */
    if ((t1 = t[i - 1]) > t[i]) {
      t[i - 1] = t[i];
      t[i] = t1;
      i = 0;
    }

  t1 = -1.0; t[n] = 1.0;                                /* begin / end pofloat */
  for (i = 0; i <= n; i++) {                 /* plot each segment separately */
    t2 = t[i];                                /* sub-divide at t[i-1], t[i] */
    fx1 = (t1 * (t1 * xb - 2 * xc) - t2 * (t1 * (t1 * xa - 2 * xb) + xc) + xd) / 8 - fx0;
    fy1 = (t1 * (t1 * yb - 2 * yc) - t2 * (t1 * (t1 * ya - 2 * yb) + yc) + yd) / 8 - fy0;
    fx2 = (t2 * (t2 * xb - 2 * xc) - t1 * (t2 * (t2 * xa - 2 * xb) + xc) + xd) / 8 - fx0;
    fy2 = (t2 * (t2 * yb - 2 * yc) - t1 * (t2 * (t2 * ya - 2 * yb) + yc) + yd) / 8 - fy0;
    fx0 -= fx3 = (t2 * (t2 * (3 * xb - t2 * xa) - 3 * xc) + xd) / 8;
    fy0 -= fy3 = (t2 * (t2 * (3 * yb - t2 * ya) - 3 * yc) + yd) / 8;
    x3 = floor(fx3 + 0.5); y3 = floor(fy3 + 0.5); /* scale bounds */
    if (fx0 != 0.0) {
      fx1 *= fx0 = (x0 - x3) / fx0;
      fx2 *= fx0;
    }
    if (fy0 != 0.0) {
      fy1 *= fy0 = (y0 - y3) / fy0;
      fy2 *= fy0;
    }
    if (x0 != x3 || y0 != y3)                            /* segment t1 - t2 */
      plotCubicBezierSegAA(x0, y0, x0 + fx1, y0 + fy1, x0 + fx2, y0 + fy2, x3, y3, color);
    x0 = x3; y0 = y3; fx0 = fx3; fy0 = fy3; t1 = t2;
  }

  lv_obj_invalidate(this->obj);
}
//----------------------------------------------------------------
void GCanvas::plotCubicBezierWidth(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int th, uint16_t color)
{ /* plot any cubic Bezier curve */
  if(!this->isCreated()) create();

  int n = 0, i = 0;
  float xc = x0 + x1 - x2 - x3, xa = xc - 4 * (x1 - x2);
  float xb = x0 - x1 - x2 + x3, xd = xb + 4 * (x1 + x2);
  float yc = y0 + y1 - y2 - y3, ya = yc - 4 * (y1 - y2);
  float yb = y0 - y1 - y2 + y3, yd = yb + 4 * (y1 + y2);
  float fx0 = x0, fx1, fx2, fx3, fy0 = y0, fy1, fy2, fy3;
  float t1 = xb * xb - xa * xc, t2, t[7];
  /* sub-divide curve at gradient sign changes */
  if (xa == 0) {                                               /* horizontal */
    if (abs(xc) < 2 * abs(xb)) t[n++] = xc / (2.0 * xb); /* one change */
  } else if (t1 > 0.0) {                                      /* two changes */
    t2 = sqrt(t1);
    t1 = (xb - t2) / xa; if (abs(t1) < 1.0) t[n++] = t1;
    t1 = (xb + t2) / xa; if (abs(t1) < 1.0) t[n++] = t1;
  }
  t1 = yb * yb - ya * yc;
  if (ya == 0) {                                                 /* vertical */
    if (abs(yc) < 2 * abs(yb)) t[n++] = yc / (2.0 * yb); /* one change */
  } else if (t1 > 0.0) {                                      /* two changes */
    t2 = sqrt(t1);
    t1 = (yb - t2) / ya; if (abs(t1) < 1.0) t[n++] = t1;
    t1 = (yb + t2) / ya; if (abs(t1) < 1.0) t[n++] = t1;
  }
  t1 = 2 * (xa * yb - xb * ya); t2 = xa * yc - xc * ya; /* divide at inflection pofloat */
  i = t2 * t2 - 2 * t1 * (xb * yc - xc * yb);
  if (i > 0) {
    i = sqrt(i);
    t[n] = (t2 + i) / t1; if (abs(t[n]) < 1.0) n++;
    t[n] = (t2 - i) / t1; if (abs(t[n]) < 1.0) n++;
  }
  for (i = 1; i < n; i++)                         /* bubble sort of 4 pofloats */
    if ((t1 = t[i - 1]) > t[i]) {
      t[i - 1] = t[i];
      t[i] = t1;
      i = 0;
    }

  t1 = -1.0; t[n] = 1.0;                               /* begin / end pofloats */
  for (i = 0; i <= n; i++) {                 /* plot each segment separately */
    t2 = t[i];                                /* sub-divide at t[i-1], t[i] */
    fx1 = (t1 * (t1 * xb - 2 * xc) - t2 * (t1 * (t1 * xa - 2 * xb) + xc) + xd) / 8 - fx0;
    fy1 = (t1 * (t1 * yb - 2 * yc) - t2 * (t1 * (t1 * ya - 2 * yb) + yc) + yd) / 8 - fy0;
    fx2 = (t2 * (t2 * xb - 2 * xc) - t1 * (t2 * (t2 * xa - 2 * xb) + xc) + xd) / 8 - fx0;
    fy2 = (t2 * (t2 * yb - 2 * yc) - t1 * (t2 * (t2 * ya - 2 * yb) + yc) + yd) / 8 - fy0;
    fx0 -= fx3 = (t2 * (t2 * (3 * xb - t2 * xa) - 3 * xc) + xd) / 8;
    fy0 -= fy3 = (t2 * (t2 * (3 * yb - t2 * ya) - 3 * yc) + yd) / 8;
    x3 = floor(fx3 + 0.5); y3 = floor(fy3 + 0.5); /* scale bounds */
    if (fx0 != 0.0) {
      fx1 *= fx0 = (x0 - x3) / fx0;
      fx2 *= fx0;
    }
    if (fy0 != 0.0) {
      fy1 *= fy0 = (y0 - y3) / fy0;
      fy2 *= fy0;
    }
    if (x0 != x3 || y0 != y3)                            /* segment t1 - t2 */
      plotCubicBezierSegWidth(x0, y0, x0 + fx1, y0 + fy1, x0 + fx2, y0 + fy2, x3, y3, th, color);
    x0 = x3; y0 = y3; fx0 = fx3; fy0 = fy3; t1 = t2;
  }

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotCubicBezierSegWidth(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int th, uint16_t color)
{ /* split cubic Bezier segment in two quadratic segments */
  if(!this->isCreated()) create();

  float x = floor((x0 + 3 * x1 + 3 * x2 + x3 + 4) / 8),
         y = floor((y0 + 3 * y1 + 3 * y2 + y3 + 4) / 8);
  plotQuadRationalBezierWidthSeg(x0, y0,
                                 floor((x0 + 3 * x1 + 2) / 4), floor((y0 + 3 * y1 + 2) / 4), x, y, 1, th, color);
  plotQuadRationalBezierWidthSeg(x, y,
                                 floor((3 * x2 + x3 + 2) / 4), floor((3 * y2 + y3 + 2) / 4), x3, y3, 1, th, color);

  lv_obj_invalidate(this->obj);
}


void GCanvas::plotQuadSpline(int n, int x[], int y[], int thickness, uint16_t color)
{ /* plot quadratic spline, destroys input arrays x,y */
  if(!this->isCreated()) create();

  int xx[n];memcpy( xx, x, n * sizeof(int));
  int yy[n];memcpy( yy, y, n * sizeof(int));

#define M_MAX 6
  float mi = 1, m[M_MAX];                    /* diagonal constants of matrix */
  int i, x0, y0, x1, y1, x2 = xx[n-1], y2 = yy[n-1];

  //   assert(n > 1);                        /* need at least 3 points P[0]..P[n] */

  xx[1] = x0 = 8 * xx[1] - 2 * xx[0];                    /* first row of matrix */
  yy[1] = y0 = 8 * yy[1] - 2 * yy[0];

  for (i = 2; i < n; i++) {                                 /* forward sweep */
    if (i - 2 < M_MAX) m[i - 2] = mi = 1.0 / (6.0 - mi);
    xx[i] = x0 = floor(8 * xx[i] - x0 * mi + 0.5);                /* store yi */
    yy[i] = y0 = floor(8 * yy[i] - y0 * mi + 0.5);
  }
  x1 = floor((x0 - 2 * x2) / (5.0 - mi) + 0.5);       /* correction last row */
  y1 = floor((y0 - 2 * y2) / (5.0 - mi) + 0.5);

  for (i = n - 2; i > 0; i--) {                         /* back substitution */
    if (i <= M_MAX) mi = m[i - 1];
    x0 = floor((xx[i] - x1) * mi + 0.5);                      /* next corner */
    y0 = floor((yy[i] - y1) * mi + 0.5);
//    plotQuadBezierAA((x0 + x1) / 2, (y0 + y1) / 2, x1, y1, x2, y2);
//    plotQuadRationalBezierAA((x0 + x1) / 2, (y0 + y1) / 2, x1, y1, x2, y2, 1);
    plotQuadRationalBezierWidth((x0 + x1) / 2, (y0 + y1) / 2, x1, y1, x2, y2, 1, thickness, color);
    x2 = (x0 + x1) / 2; x1 = x0;
    y2 = (y0 + y1) / 2; y1 = y0;
  }
//  plotQuadBezierAA(xx[0], yy[0], x1, y1, x2, y2);
//  plotQuadRationalBezierAA(xx[0], yy[0], x1, y1, x2, y2,1);
  plotQuadRationalBezierWidth(xx[0], yy[0], x1, y1, x2, y2,1, thickness, color);

  lv_obj_invalidate(this->obj);
}

void GCanvas::plotCubicSpline(int n, int x[], int y[], int thickness, uint16_t color)
{ /* plot cubic spline, destroys input arrays x,y */
  if(!this->isCreated()) create();

  int xx[n];memcpy( xx, x, n * sizeof(int));
  int yy[n];memcpy( yy, y, n * sizeof(int));

#define M_MAX 6
  float mi = 0.25, m[M_MAX];                 /* diagonal constants of matrix */
//  int x3 = x[n - 1], y3 = y[n - 1], x4 = x[n], y4 = y[n];
  int x3 = xx[n - 1], y3 = yy[n - 1], x4 = xx[n-1], y4 = yy[n-1];
  int i, x0, y0, x1, y1, x2, y2;

  //   assert(n > 2);                        /* need at least 4 points P[0]..P[n] */

  xx[1] = x0 = 12 * xx[1] - 3 * xx[0];                   /* first row of matrix */
  yy[1] = y0 = 12 * yy[1] - 3 * yy[0];

  for (i = 2; i < n; i++) {                                /* foreward sweep */
    if (i - 2 < M_MAX) m[i - 2] = mi = 0.25 / (2.0 - mi);
    xx[i] = x0 = floor(12 * xx[i] - 2 * x0 * mi + 0.5);
    yy[i] = y0 = floor(12 * yy[i] - 2 * y0 * mi + 0.5);
  }
  x2 = floor((x0 - 3 * x4) / (7 - 4 * mi) + 0.5);        /* correct last row */
  y2 = floor((y0 - 3 * y4) / (7 - 4 * mi) + 0.5);
//  plotCubicBezierAA(x3, y3, (x2 + x4) / 2, (y2 + y4) / 2, x4, y4, x4, y4);
  plotCubicBezierWidth(x3, y3, (x2 + x4) / 2, (y2 + y4) / 2, x4, y4, x4, y4, thickness, color);

  if (n - 3 < M_MAX) mi = m[n - 3];
  x1 = floor((xx[n - 2] - 2 * x2) * mi + 0.5);
  y1 = floor((yy[n - 2] - 2 * y2) * mi + 0.5);
  for (i = n - 3; i > 0; i--) {                         /* back substitution */
    if (i <= M_MAX) mi = m[i - 1];
    x0 = floor((xx[i] - 2 * x1) * mi + 0.5);
    y0 = floor((yy[i] - 2 * y1) * mi + 0.5);
    x4 = floor((x0 + 4 * x1 + x2 + 3) / 6.0);           /* reconstruct P[i] */
    y4 = floor((y0 + 4 * y1 + y2 + 3) / 6.0);
//    plotCubicBezierAA(x4, y4,
//                    floor((2 * x1 + x2) / 3 + 0.5), floor((2 * y1 + y2) / 3 + 0.5),
//                    floor((x1 + 2 * x2) / 3 + 0.5), floor((y1 + 2 * y2) / 3 + 0.5),
//                    x3, y3);
    plotCubicBezierWidth(x4, y4,
                    floor((2 * x1 + x2) / 3 + 0.5), floor((2 * y1 + y2) / 3 + 0.5),
                    floor((x1 + 2 * x2) / 3 + 0.5), floor((y1 + 2 * y2) / 3 + 0.5),
                    x3, y3, thickness, color);
    x3 = x4; y3 = y4; x2 = x1; y2 = y1; x1 = x0; y1 = y0;
  }
  x0 = xx[0]; x4 = floor((3 * x0 + 7 * x1 + 2 * x2 + 6) / 12.0); /* reconstruct P[1] */
  y0 = yy[0]; y4 = floor((3 * y0 + 7 * y1 + 2 * y2 + 6) / 12.0);
//  plotCubicBezierAA(x4, y4, floor((2 * x1 + x2) / 3 + 0.5), floor((2 * y1 + y2) / 3 + 0.5),
//                  floor((x1 + 2 * x2) / 3 + 0.5), floor((y1 + 2 * y2) / 3 + 0.5), x3, y3);
//  plotCubicBezierAA(x0, y0, x0, y0, (x0 + x1) / 2, (y0 + y1) / 2, x4, y4);
  plotCubicBezierWidth(x4, y4, floor((2 * x1 + x2) / 3 + 0.5), floor((2 * y1 + y2) / 3 + 0.5),
                  floor((x1 + 2 * x2) / 3 + 0.5), floor((y1 + 2 * y2) / 3 + 0.5), x3, y3, thickness, color);
  plotCubicBezierWidth(x0, y0, x0, y0, (x0 + x1) / 2, (y0 + y1) / 2, x4, y4, thickness, color);

  lv_obj_invalidate(this->obj);

}


void GCanvas::getThermalPrinterBuffer(uint8_t* &buffer, bool invert_color){
  if(!this->isCreated()) create();
  if(buffer!=NULL) { free(buffer); buffer=NULL; }

  uint16_t w = this->width();
  uint16_t h = this->height();
  buffer = (uint8_t*) esp32_malloc((w + 7) / 8 * h);

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      uint16_t color16 = canvas.cbuf[y * w + x];
      uint8_t brightness = (uint8_t)((0.299 * ((color16 >> 11) & 0x1F) + 0.587 * ((color16 >> 5) & 0x3F) + 0.114 * (color16 & 0x1F)) / 255.0 * 7.0);
      buffer[y * ((w + 7) / 8) + x / 8] |= (brightness << (7 - x % 8));
    }
  }
  if(invert_color) {
    for (int i = 0; i < (w + 7) / 8 * h; i++)  buffer[i] = ~buffer[i];
  }
}
