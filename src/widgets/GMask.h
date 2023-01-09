/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GMask.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GMASK_H__
#define __GMASK_H__

#include "GObject.h"
#include "objx/lv_mask.h"

class GMask : public GObject {
  public:
    GMask(GWidget& parent=GScreen);                                 // ไม่มีการ alloc buffer ต้อง setBuffer(..) เองภายหลัง
    GMask(GWidget* parent);                                         // ไม่มีการ alloc buffer ต้อง setBuffer(..) เองภายหลัง
    GMask(uint16_t w, uint16_t h, GWidget& parent=GScreen);         // จะ alloc buffer อัตโนมัติ จาก  w, h
    GMask(uint16_t w, uint16_t h, GWidget* parent);                 // จะ alloc buffer อัตโนมัติ จาก  w, h
    GMask(const img_t & src_img, GWidget& parent= GScreen);         // จะ alloc buffer อัตโนมัติ จาก  ขนาดของรูป
    GMask(const img_t & src_img, GWidget* parent);                  // จะ alloc buffer อัตโนมัติ จาก  ขนาดของรูป

    virtual ~GMask();   // มีการล้าง buffer ให้อัตโนมัติหากมีจอง

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);
            void del();

    void    size(uint16_t w, uint16_t h)        { this->allocBuffer(w,h); }
    void    allocBuffer(uint16_t w, uint16_t h);  // alloc & set ด้วย LV_IMG_CF_ALPHA_8BIT
    void    setBuffer(uint8_t* buffer, uint16_t w, uint16_t h);
    uint8_t* getBuffer(uint16_t*w=NULL, uint16_t*h=NULL);       // pointer ที่เป็น buffer ของ mask's canvas ที่ใช้งานโดยตรง ไม่ใช่ ext->cbuf
    void    linkBuffer(GMask &mask_src);

    void    fill(uint8_t alpha=255);
    void    fill(uint8_t alpha, uint8_t alpha_gradian);
    void    clear()    { fill(0); }
    void    invert();

    /* มุมแบบตามระบบมุมแกน x,y (หมุนทวนเข็มจาก x+) */
    void    arc(uint8_t alpha,Point c, uint16_t r, uint16_t thick, 
                              int start_angle, int end_angle, 
                              bool round_ending= false);
    /* มุมแบบตามเข็ม จาก offset_angle ไป */
    void    arc(uint8_t alpha,Point c, uint16_t r, uint16_t thick, 
                              int offset_angle, int start_angle, int end_angle, 
                              bool round_ending= false);
    void    line(uint8_t alpha,Point p1, Point p2, uint16_t thick, bool round_ending);

    void    line_aa(uint8_t alpha,Point p1, Point p2, uint16_t thick);

    void    polygon(uint8_t alpha, Point* points, size_t point_num);
    void    rect(uint8_t alpha, Point topleft, uint16_t w, uint16_t h, uint16_t r=0);
    void    rect(uint8_t alpha, uint8_t alpha_gradian, Point topleft, uint16_t w, uint16_t h, uint16_t r=0);
    void    circle(uint8_t alpha, Point c, uint16_t r);
    void    triangle(uint8_t alpha, Point p1, Point p2, Point p3);

    /*
    #define TL_DATUM 0 // Top left (default)
    #define TC_DATUM 1 // Top centre
    #define TR_DATUM 2 // Top right
    #define ML_DATUM 3 // Middle left
    #define CL_DATUM 3 // Centre left, same as above
    #define MC_DATUM 4 // Middle centre
    #define CC_DATUM 4 // Centre centre, same as above
    #define MR_DATUM 5 // Middle right
    #define CR_DATUM 5 // Centre right, same as above
    #define BL_DATUM 6 // Bottom left
    #define BC_DATUM 7 // Bottom centre
    #define BR_DATUM 8 // Bottom right
    #define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
    #define C_BASELINE 10 // Centre character baseline
    #define R_BASELINE 11 // Right character baseline
    */
    void    text(uint8_t alpha, const char* txt, Point p, lv_datum_t datum = TL_DATUM, font_t& font=prasanmit_40);

    void    rect(Point topleft, uint16_t w, uint16_t h, GStyle& style );
    void    circle(Point c, uint16_t r, GStyle& style );

    void    image(uint8_t alpha, const void * src, Point topleft = Point(0,0));
    void    image(uint8_t alpha, const img_t & src, Point topleft = Point(0,0));


    uint8_t* _mask_buffer = NULL;       // สำหรับ alloc ภายใน
  private:
    uint16_t _w = 0;
    uint16_t _h = 0;
    void* _img_src = NULL;              // สำหรับ ชี้ไปยัง img_src ภายนอก
};

#endif //__GMASK_H__

