/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GImage.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GIMAGE_H__
#define __GIMAGE_H__

// img_t image;  image.header.w --> width; image.header.h --> height ;

#include "GObject.h"

class GImage : public GObject {
  public:
    GImage(GWidget& parent=GScreen);
    GImage(GWidget* parent);
    GImage( const img_t & src_img, GWidget& parent= GScreen);   // create GImage with specific parent
    GImage( const img_t & src_img, GWidget* parent);            // create GImage with specific parent
    GImage( const uint8_t* img_data, uint16_t w, uint16_t h, GWidget& parent= GScreen); // CF_TRUE_COLOR
    GImage( const uint8_t* img_data, uint16_t w, uint16_t h, color_format_t cf, GWidget& parent= GScreen);

    // parameter : file_path เป็นได้ ทั้ง png, jpb, bmp
    // เช่น "sd://images/filename.png" หรือ "spiffs://images/filename.png"
    // หากเป็น "/images/filename.png"  จะอ่านจาก sd อัตโนมัติ
    GImage( String file_path, GWidget& parent= GScreen ); 
    GImage( String file_path, GWidget* widget ); 
    virtual ~GImage();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    GImage& operator =(const img_t * src_img);
    GImage& operator =(const img_t & src_img);
    GImage& operator =(String file_path);

    inline bool operator ==(const img_t * src_img) {
      if(!this->isCreated()) this->create();
      return (src_img ==(const img_t*) this->getSource() );
    }

    inline bool operator ==(const img_t & src_img) {
      if(!this->isCreated()) this->create();
      return (&src_img ==(const img_t*) this->getSource() );
    }
    
    void        setImage(String file_path);
    void        setImage(const img_t & src_img);
    void        setImage(const img_t * src_img);
    void        setImage(const uint8_t* img_data, uint16_t w, uint16_t h, color_format_t cf= CF_TRUE_COLOR); // by PROGMEM image data
    const void* getSource();

    const uint8_t* getImageInfo(uint16_t* w=NULL, uint16_t *h=NULL, bool* isalpha=NULL);

    void        angle(int16_t angle);           // มุมจาก 12 นาฬิกาตามเข็ม
    uint16_t    angle();
    void        zoom(float zoom_percentage);    // 100 : ขนาดเดิม ; < 100 zoom out ; > 100 zoom in
    void        antialias(bool en);
    void        offset(uint16_t x, uint16_t y);

    void        opa(uint8_t opa);
    uint8_t     opa();
    void        filter(color_t color, uint8_t intense=128);  // ใส่ filter สีที่ต้องการลงในรูป
    void        reset();                                     // reset ที่เคย opa() หรือ filter() ออกไป

    void        hidden(bool enable);
    bool        hidden();

    uint16_t    gif_delay();
    void        gif_delay(uint16_t delay);

    // don't use , inner function/var
    void        cleanup_src_dsc();  // inner used for free decoded image mem ... don't call
    img_t       src_dsc;            /* img_t ที่ decode จาก png/jpg/bmp  หมายเหตุ สร้างแบบ alloc ไม่ได้ Why?? */
    gd_GIF      *gifdec=NULL;
    TaskHandle_t gif_task=NULL;

  private:
    const img_t * img = NULL;   /* pointer img_t ไปยัง c file */
    String _file_path= "";      /* ชื่อไฟล์หาก src เป็น png/jpg/bmp */

    bool src_dsc_created = false;
    bool gif_decode_created = false;
};


#endif //__GIMAGE_H__
