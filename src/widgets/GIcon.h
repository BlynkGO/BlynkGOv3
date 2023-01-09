
/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GIcon.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 * History
 *   V.2.0.0 @ 05/09/22
 *     - fix ให้ event ภายในรองรับคำสั่ง font_color(..), font(..) ได้ด้วย
 *
 *********************************************************************
 */

#ifndef __GICON_H__
#define __GICON_H__

#include "GContainer.h"
#include "GLabel.h"
#include "GImage.h"

typedef struct {
  lv_cont_ext_t cont;
  GImage *img;
  GLabel *label;
  bool body_draw;
} gicon_ext_t;

class GIcon : public GContainer {
  public:
    GIcon(GWidget& parent=GScreen);
    GIcon(GWidget* parent);
    GIcon(const img_t & src_img, String name, GWidget& parent=GScreen);
    GIcon(const img_t & src_img, String name, GWidget* parent);
    GIcon(String file_path, String name, GWidget& parent=GScreen);
    GIcon(String file_path, String name, GWidget* parent);
    virtual ~GIcon();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void    setImage(String file_path);
    void    setImage(const img_t & src_img);
    void    setImage(const img_t * src_img);
    GImage* getImage();
    GLabel* getLabel();

    void    body_draw(bool enable);     // กำหนด ให้แสดงพื้นที่ส่วนพื้นหลังด้วย (ปกติจะเป็น โปร่งใส)
    bool    body_draw();

    void    text(String txt);
    String  text();
    void    text_hidden(bool enable);
    // long_mode_t : LABEL_LONG_EXPAND, LABEL_LONG_BREAK, LABEL_LONG_DOT, LABEL_LONG_SROLL, LABEL_LONG_SROLL_CIRC, LABEL_LONG_CROP
    // long_mode() ใช้คู่กับ width(xx) ให้กำหนดความกว้างด้วย
    void    long_mode(long_mode_t long_mode); 

  protected:
    const img_t * _src_img = NULL;
    String _name = "";
    String _file_path= "";
};

#endif // __GICON_H__
