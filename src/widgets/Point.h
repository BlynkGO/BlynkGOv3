/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ Point.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef _POINT_H__
#define _POINT_H__

#include <stdint.h>
#include <cmath>
#include <initializer_list>
#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_area.h"  // point_t;
#include "../utils/BlynkGO_Type.h"
#include "../utils/esp32_utils.h"
#include "vector"

struct Point;
typedef std::vector<Point> Points;

class GArea;

//typedef float GMatrix[9];
class GMatrix;

// ในที่นี้คือ พิกัด Pixel ของ screen TFT   
//  (0,0) ---------> (320,0)
//    |
//    |
//    v (0,240)----> (320,240)
//
//  angle_rad และ angle_deg ให้นับ จากแกน X+ ตามทวนเข็มนาฬิกา 

struct Point {
  float x;
  float y;
  Point();
  Point(float a, float b);
  Point(float r, float angle_deg, Point c, rotate_direction_t rotate=ANTICLOCKWISE);  // ทวนเข็ม
  Point(float r, float angle_deg, float angle_deg_offset, Point c, rotate_direction_t rotate=ANTICLOCKWISE); // ทวนเข็ม
  Point(const Point &p);
  Point(point_t p);
  Point(float t, Point *control_points, uint8_t num) ;   // bezier by t : 0..1  with control points
  Point& operator=(const Point &p);
  Point& operator=(const point_t &p);
  void   operator+=(const Point &p);
  void   operator-=(const Point &p);
  Point  operator+(const Point &p);
  Point  operator-(const Point &p);  // ยังไม่ work?
  bool   operator==(const Point p);
  bool   operator!=(const Point p);
  
  // Point * scalar
  Point  operator*(double k);  // OK
  Point  operator*(float k);   // OK
  Point  operator*(int k);     // OK

  // Point * GMatrix3x3
  Point  operator*(GMatrix m); 

  // Point / scalar
  Point  operator/(double k);  // OK
  Point  operator/(float k);
  Point  operator/(int k);

  Point  rotate(float angle_deg, rotate_direction_t rotate= ANTICLOCKWISE);
  Point  rotate(float angle_deg, Point c, rotate_direction_t rotate= ANTICLOCKWISE);
  Point  opposite();
  Point  opposite(Point c);
  Point  flipX();
  Point  flipX(Point c);
  Point  flipY();
  Point  flipY(Point c);
  Point  scale(float scale, Point c );
  Point  round();  // ปรับค่า x, y ให้เป็นจำนวนเต็ม ปัดแบบ 0.5 ขึ้นลง
  Point  ceil();   // ปรับค่า x, y ให้เป็นจำนวนเต็ม ปัดขึ้น
  Point  floor();  // ปรับค่า x, y ให้เป็นจำนวนเต็ม ปัดลง
  float  angle_rad(Point c , rotate_direction_t rotate= ANTICLOCKWISE);
  float  angle_deg(Point c , rotate_direction_t rotate= ANTICLOCKWISE);
  float  distance(Point c );
  void   setCenterPoint(Point c);
  Point  getCenterPoint();
  Point  transform(GMatrix m);
  Point& bezier(float t, Point *control_points, uint8_t num) ;
  void   swap(Point &b);
  Point  unitify(Point c);      // คืนค่า Point ขนาดความยาว 1 หน่วย ในทิศ c->p

  // จุดอยู่ในพื้นที่ GArea ที่กำหนดหรือไม่
  bool   is_in(GArea a , uint16_t corner_radius=0);
  bool   is_in(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t corner_radius=0);
  bool   is_in(Point p, uint16_t w, uint16_t h, uint16_t corner_radius=0);
  bool   is_in(Point p1, Point p2, uint16_t corner_radius=0);  // อยู่บนพื้นที่ระหว่าง p1, p2 หรือไม่
  bool   is_in(Points pts);

  point_t to_point_t();
};

#ifndef POINT_OPERATOR
#define POINT_OPERATOR
// scalar * Point
Point operator *( int k, const Point& p );
Point operator *( float k, const Point& p );
Point operator *( double k, const Point& p );
// Point operator =-(const& p);

// GMatrix 3x3  * Point
Point operator *( GMatrix m, const Point& p);

#endif //POINT_OPERATOR

class GArea {
  public:
    Point p[2];
    lv_area_t _area;

    GArea();
    GArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)   { this->setPoints(Point(x1,y1),Point(x2,y2)); }
    GArea(Point a, Point b)                   { this->setPoints(a,b); }
    GArea(Point topleft, uint16_t w, uint16_t h)  { this->setPoints(topleft, topleft + Point(w-1,h-1)); }
    GArea(lv_area_t area)                     { this->setPoints(area); }
    GArea& operator=(const GArea& area)       { this->setPoints( area.p[0], area.p[1] );  return *this; }

    inline void setPoints(Point a, Point b)   { this->p[0] = a; this->p[1]=b; _area.x1 = p[0].x; _area.y1 = p[0].y; _area.x2 = p[1].x; _area.y2 = p[1].y; }    
    inline void setPoints(lv_area_t area)     { this->setPoints(Point(area.x1,area.y1), Point(area.x2, area.y2)); }

    inline uint16_t width()                   { return lv_area_get_width( (const lv_area_t *) &_area);}
    inline uint16_t height()                  { return lv_area_get_height( (const lv_area_t *) &_area);}
    inline void width(uint16_t w)             { lv_area_set_width( &_area, w); }
    inline void height(uint16_t h)            { lv_area_set_height(&_area, h); }
    inline void position(Point p)             { lv_area_set_pos(&_area, p.x, p.y); }
    inline uint32_t size()                    { return lv_area_get_size( (const lv_area_t *) &_area); }
    
    bool intersect(const GArea &a, const GArea&b, GArea* res_area=NULL)  { 
      lv_area_t _res_area;  
      bool res = lv_area_intersect(&_res_area, &a._area, &b._area );   
      if( res_area !=NULL) {
        res_area->setPoints(_res_area);
      }
      return res; 
    }

    GArea join(const GArea&a, const GArea&b) {
      lv_area_t _res_area;  
      lv_area_join(&_res_area, &a._area, &b._area );   
      return GArea(_res_area);
    }

    // พื้นที่อยู่ ทับกันไหม
    inline bool is_on(const GArea& a ) {
      return lv_area_is_on( (const lv_area_t * ) &this->_area, (const lv_area_t * ) &a._area);
    }

    inline  bool is_on(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)  {
      lv_area_t a;   lv_area_set(&a, x1, y1, x2, y2);
      return  lv_area_is_on( (const lv_area_t * ) &this->_area, (const lv_area_t * ) &a );
    }


    // พื้นที่อยู่ภายในอีกพื้นที่ไหม
    inline bool is_in(const GArea& a ) {
      return lv_area_is_in( (const lv_area_t * ) &this->_area, (const lv_area_t * ) &a._area);
    }

    inline  bool is_in(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)  {
      lv_area_t a;   lv_area_set(&a, x1, y1, x2, y2);
      return  lv_area_is_in( (const lv_area_t * ) &this->_area, (const lv_area_t * ) &a );
    }

    lv_area_t to_area_t()                     { return _area; }
  private:
};


class GMatrix {
public:
  GMatrix(uint8_t Rows=3, uint8_t Cols=3);
  GMatrix(float* buf, uint8_t Rows, uint8_t Cols);
  GMatrix(std::initializer_list<float> m);
  GMatrix(std::initializer_list<float> m , uint8_t Rows, uint8_t Cols);
  ~GMatrix();

  GMatrix& operator=(const GMatrix &m);

  uint8_t rows;   // จน. row
  uint8_t cols;   // จน. column
  float  *buffer=NULL;   // ข้อมูลใน matrix

  float  det();   // สำหรับ N x N  matrix only

  void   freeMem();
  float& element(uint8_t r , uint8_t c) {
    if(c == 0 || c > cols || r==0 || r > rows ) return buffer[rows*cols];
    return buffer[(c-1) + (r-1)* cols];
  }

  class GMatrixHelper {
    public:
      GMatrixHelper( GMatrix & m, uint8_t r) : _m(m), _r(r) {}
      float& operator[](uint8_t c) {   return _m.element(_r,c); }
    private:
      GMatrix& _m;
      uint8_t _r;
  };

  GMatrixHelper operator[](uint8_t r)      { return GMatrixHelper(*this, r); }

private:
  bool inited = false;
  void   _getCofactor(GMatrix &temp, int p, int q);
};


GMatrix operator *( GMatrix m, GMatrix n);

//----------------------------------
class Grid {
  public:
    Grid() {}
    Grid( uint16_t screen_width, uint16_t screen_height, 
          uint16_t grid_width, uint16_t grid_height, 
          Point topleft=Point(0,0) );
    void begin( uint16_t screen_width, uint16_t screen_height, 
                uint16_t grid_width, uint16_t grid_height, 
                Point topleft=Point(0,0) );
    Point snap(Point p);

    Point gridPoint(uint16_t x, uint16_t y); 
    uint16_t max_coordinate_x()                          { return _max_x; }
    uint16_t max_coordinate_y()                          { return _max_y; }
  private:
    Point    _topleft;
    uint16_t _grid_w;
    uint16_t _grid_h;
    uint16_t _max_x;
    uint16_t _max_y;
};


int combination(int n, int r);

#define POINT_PRINTLN(point)    Serial.printf("[Point] %d,%d\n", (int)point.x, (int)point.y)
#define POINT_PRINTLN_F(point)  Serial.printf("[Point] %.2f,%.2f\n", point.x, point.y)

#endif
