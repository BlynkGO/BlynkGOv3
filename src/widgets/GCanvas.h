/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GCanvas.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * version 3.0.0  @25/03/2023
 * --------------
 *   - fix ให้สร้าง style ภายในแต่ต้น
 *   - เพิ่ม drawQRCode(..) ให้วาด qrcode ได้ด้วย
 *
 *********************************************************************
 */


#ifndef __GCANVAS_H__
#define __GCANVAS_H__

#include "GObject.h"

#define TRUECOLOR           0
#define TRUECOLOR_ALPHA     1

class GCanvas : public GObject {
  public:
    GCanvas(GWidget& parent=GScreen);
    GCanvas(GWidget* parent);
    GCanvas(bool alpha, GWidget& parent=GScreen);
    GCanvas(bool alpha, GWidget* parent);
    GCanvas(uint16_t w, uint16_t h, GWidget& parent=GScreen);             // True Color
    GCanvas(uint16_t w, uint16_t h, GWidget* parent);                     // True Color
    GCanvas(uint16_t w, uint16_t h, bool alpha, GWidget& parent=GScreen); // True Color Alpha
    GCanvas(uint16_t w, uint16_t h, bool alpha, GWidget* parent);         // True Color Alpha
    virtual ~GCanvas();

    virtual void create();
            void create(GObject& parent);
            void create(GObject* parent);

    void fillScreen(uint16_t color, uint8_t opa=255);
    inline void clear()             { this->fillScreen(TFT_BLACK); }
    // void fillAlpha(uint8_t alpha);

    // use style จาก canvas' style ในการวาด text
    void drawText(String text, Point topleft, uint16_t max_width, label_align_t text_align=TEXT_ALIGN_LEFT);

    #define QR_VERSION   15     // ใช้ได้ถึง 426 AlphaNumerics
    void drawQRCode(String text, Point topleft, uint16_t width, uint8_t qr_version = QR_VERSION);

    void drawLine(Point p1, Point p2, uint16_t color);
    void drawLine(Point p1, Point p2, float thick , uint16_t color );
    void drawLine(float ax, float ay, float bx, float by, uint16_t color,  float r=0.8);
    void drawHLine(Point topleft, uint16_t w, uint16_t color);
    void drawVLine(Point topleft, uint16_t h, uint16_t color);

    void drawRect(Point topleft, uint16_t w, uint16_t h, uint16_t color);
    void drawRect(Point topleft, uint16_t w, uint16_t h, uint8_t thick, uint16_t color);
    void drawRect(Point topleft, uint16_t w, uint16_t h, GStyle *style=NULL); // style จาก canvas' style ในการวาด rect

    void fillRect(Point topleft, uint16_t w, uint16_t h, uint16_t color);
    // void maskRect(Point topleft, uint16_t w, uint16_t h, bool invert_mask=false);

    void drawRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t conner_radius, uint16_t color);
    void drawRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t thick, uint8_t conner_radius, uint16_t color);
    void fillRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t conner_radius, uint16_t color);
    // void maskRoundRect(Point topleft, uint16_t w, uint16_t h, uint8_t conner_radius, bool invert_mask=false);

    void drawCircle(Point center, uint16_t r, uint16_t color);
    void drawCircle(Point center, uint16_t r, uint8_t thick, uint16_t color);
    void fillCircle(Point center, uint16_t r, uint16_t color);
    // void maskCircle(Point center, uint16_t r, bool invert_mask=false);

    void drawTriangle(Point p1, Point p2, Point p3, uint16_t color);
    void drawTriangle(Point p1, Point p2, Point p3, uint8_t thick, uint16_t color);
    void fillTriangle(Point p1, Point p2, Point p3, uint16_t color);
    // void maskTriangle(Point p1, Point p2, Point p3, bool invert_mask=false);

    void drawPolygon(Point* points, size_t point_num, uint16_t color);
    void drawPolygon(Point* points, size_t point_num, uint8_t thick, uint16_t color);
    void fillPolygon(Point* points, size_t point_num, uint16_t color);
    // void maskPolygon(Point* points, size_t point_num, bool invert_mask=false);

    void drawArc(Point center, uint16_t r, 
                int32_t angle_deg_start, int32_t angle_deg_end, // กวาดมุมทวนเข็มตามระนาบพิกัด xy
                uint16_t color);
    void drawArc(Point center, uint16_t r, uint8_t thick, 
                int32_t angle_deg_start, int32_t angle_deg_end, 
                uint16_t color);
    // void maskArc(Point center, uint16_t r, uint8_t thick, 
    //             int32_t angle_deg_start, int32_t angle_deg_end, // กวาดมุมทวนเข็มตามระนาบพิกัด xy
    //             bool invert_mask=false);

    void drawImage(String file_path, uint16_t x=0, uint16_t y=0);

    color_t& pixel(uint16_t x, uint16_t y);
    // uint8_t&  alpha(uint16_t x, uint16_t y);


    // void drawEllipse(Point center, uint16_t a, uint16_t b, uint16_t color);
    // void drawEllipse(Point center, uint16_t a, uint16_t b, uint8_t thick, uint16_t color);
    // void fillEllipse(Point center, uint16_t a, uint16_t b, uint16_t color);

    void plotLine(int x0, int y0, int x1, int y1, uint16_t color);
    void plotLineAA(int x0, int y0, int x1, int y1, uint16_t color);
    void plotLineWidth(int x0, int y0, int x1, int y1, int th, uint16_t color);
    void plotCircleAA(int xm, int ym, int r, uint16_t color);
    void plotCircleWidth(int x, int y, float r, int th, uint16_t color);
    void plotEllipseRect(int x0, int y0, int x1, int y1, uint16_t color);
    void plotEllipseRectAA(int x0, int y0, int x1, int y1, uint16_t color);
    void plotEllipseRectWidth(int x0, int y0, int x1, int y1, int th, uint16_t color);

    void plotRotatedEllipseRect(int x0, int y0, int x1, int y1, float zd, uint16_t color);
    void plotRotatedEllipse(int x, int y, int a, int b, float angle_deg, uint16_t color); // angle = degree
    void plotRotatedEllipseRectAA(int x0, int y0, int x1, int y1, float zd, uint16_t color);
    void plotRotatedEllipseAA(int x, int y, int a, int b, float angle_deg, uint16_t color);
    void plotRotatedEllipseWidth(int x, int y, int a, int b, float angle_deg, int th, uint16_t color);
    void plotRotatedEllipseRectWidth(int x0, int y0, int x1, int y1, float zd, int th, uint16_t color);

    void plotQuadRationalBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color);
    void plotQuadRationalBezier(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color);
    void plotQuadRationalBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color);
    void plotQuadRationalBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, float w, uint16_t color);
    void plotQuadRationalBezierWidthSeg(int x0, int y0, int x1, int y1, int x2, int y2, float w, int th, uint16_t color);
    void plotQuadRationalBezierWidth(int x0, int y0, int x1, int y1, int x2, int y2, float w, int th, uint16_t color);
    void plotQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void plotQuadBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void plotQuadBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
    void plotCubicBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color);
    void plotCubicBezier(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color);
    void plotCubicBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color);
    void plotCubicBezierAA(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color);
    void plotCubicBezierWidth(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int th, uint16_t color);
    void plotCubicBezierSegWidth(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int th, uint16_t color);
    void plotQuadSpline(int n, int x[], int y[], int thickness, uint16_t color);
    void plotCubicSpline(int n, int x[], int y[], int thickness, uint16_t color);

    void getThermalPrinterBuffer(uint8_t* &buffer, bool invert_color=true);

    inline color_t* buffer()        { return cbuf; }

    color_t *cbuf = NULL;
  protected:
    uint16_t _w=0, _h=0;
    bool _has_alpha;
    bool _fill_transp=false;
    img_t src_dsc;
    bool src_dsc_created = false;

    void setPixel(int x, int y, uint16_t color);
    void setPixelAA(int x, int y, uint16_t color, uint8_t aa);
    // void draw_rect_border_corner(Point topleft, uint16_t width, uint16_t height, uint16_t bwidth, uint16_t radius, uint16_t color);
};

#endif //__GCANVAS_H__
