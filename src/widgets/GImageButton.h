/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GImageButton.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GIMAGEBUTTON_H__
#define __GIMAGEBUTTON_H__

#include "GContainer.h"
#include "GLabel.h"

class GImageButton : public GContainer {
  public:
    GImageButton(GWidget& parent=GScreen);
    GImageButton(GWidget* parent);
    GImageButton( const void *src, GWidget& parent=GScreen);
    GImageButton( const void *src_btn_rel, 
                  const void *src_btn_pr=NULL,
                  const void *src_toggle_rel=NULL,
                  const void *src_toggle_pr=NULL,
                  const void *src_btn_inactive=NULL, 
                  GWidget& parent=GScreen);
    GImageButton( const lv_img_dsc_t &src, GWidget& parent=GScreen);
    GImageButton( const lv_img_dsc_t &src, GWidget* parent);
    GImageButton( const lv_img_dsc_t &src_btn_rel, 
                  const lv_img_dsc_t &src_btn_pr,
                  const lv_img_dsc_t &src_toggle_rel,
                  const lv_img_dsc_t &src_toggle_pr,
                  const lv_img_dsc_t &src_btn_inactive, 
                  GWidget& parent=GScreen);

    GImageButton( button_type_t type,         // BUTTON_PUSH or BUTTON_SWITCH
                  const lv_img_dsc_t &src_btn_rel, 
                  const lv_img_dsc_t &src_btn_pr,
                  GWidget& parent=GScreen);

    GImageButton( button_type_t type, 
                  const lv_img_dsc_t &src_btn_rel, 
                  GWidget& parent=GScreen);

    virtual ~GImageButton();
    virtual void create();
            void create(GWidget *parent);
            void create(GWidget &parent);
            void createImageButtonStyle();
            void freeImageButtonStyle();

    /* button's mode : BUTTON_MODE_NORMAL or BUTTON_MODE_TOGGLE or BUTTON_MODE_INACTIVE */
    void          mode(button_mode_t mode);  
    button_mode_t mode();

    /* Toggle API : set  mode(BUTTON_MODE_TOGGLE) first */
    void     toggle();      // change toggle pressed -> released or released -> pressed
    void     ON();          // turn on
    void     OFF();         // turn off
    bool     isON();        // is turn on?
    bool     isOFF();       // is turn off?

                      //สวิทซ์ ปล่อย  กด       สวิทซ์ปล่อย  ในโหมด Toggle
    /* button's state : RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE, */
    void     state(button_state_t button_state); 
    button_state_t state();

    void setImage(const void *src_btn_rel, const void *src_btn_pr=NULL,const void *src_toggle_rel=NULL,const void *src_toggle_pr=NULL,const void *src_btn_inactive=NULL);
    void setImage(const lv_img_dsc_t & src_btn);
    void setImage(const lv_img_dsc_t & src_btn_rel, 
                  const lv_img_dsc_t & src_btn_pr,
                  const lv_img_dsc_t & src_toggle_rel,
                  const lv_img_dsc_t & src_toggle_pr,
                  const lv_img_dsc_t & src_btn_inactive);

    // button_state_t : RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE
    void setImage(button_state_t btn_type, const void *src);

    GImageButton& operator =(const img_t * src_img);
    GImageButton& operator =(const img_t & src_img);

    void    gpio(uint8_t pin, bool active_high = HIGH);
    uint8_t gpio();
    void    active_high(bool active_high);
    bool    active_high();

#if BLYNKGO_USE_BLYNK
    void    virtualPin(uint8_t vpin);
    uint8_t virtualPin();
#endif
    
    GStyle   style[5]; // RELEASED,  PRESSED,  TOGGLE_RELEASED,  TOGGLE_PRESSED,  INACTIVE,

  protected:
    const void * img_btn_rel = NULL;
    const void * img_btn_pr  = NULL;
    const void * img_tgl_rel = NULL;
    const void * img_tgl_pr  = NULL;
    const void * img_btn_ina = NULL;
    button_type_t _type = BUTTON_PUSH;
    
    uint8_t gpin;
    uint8_t gpin_active=HIGH;
#if BLYNKGO_USE_BLYNK
    uint8_t vpin;
#endif
};



#endif // __GIMAGEBUTTON_H__