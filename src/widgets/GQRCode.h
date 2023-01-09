/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GQRCode.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#ifndef __GQRCODE_H__
#define __GQRCODE_H__

#include "GObject.h"
#include "GImage.h"

#define QR_VERSION   15     // ใช้ได้ถึง 426 AlphaNumerics

typedef enum {
  WIFI_NOPASS,
  WIFI_WEB,
  WIFI_WPA,
} wifi_type_t;

typedef struct {
  lv_canvas_ext_t canvas;
  uint8_t *cbuf;
  GImage  *img;
  lv_color_t palette_c[2];
} gqrcode_ext_t;

class GQRCode : public GObject{
  public:
    GQRCode(GWidget& parent=GScreen);
    GQRCode(GWidget* parent);
    virtual ~GQRCode();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    void    version(uint8_t version); // กำหนด version ของ qrcode
    uint8_t version();

    void color(color_t color);
    void encode(String txt, const img_t* logo=NULL);
    void encode(String txt, const img_t& logo);
    void encode(String txt, String file_path);
    void encodeWiFi(String ssid, String password="", wifi_type_t wifi_type=WIFI_WPA);
    void encodeLine(String line_id);
    void encodePromptPay(String id, float amount, bool multi_use=true); // สำหรับโอนเงิน
    void encodePromptPay(String biller_id, String reference_id1, String reference_id2, float amount, bool multi_use=false, String terminal_label= "");
    void encodeKPlusShop(String MerchantID, String bill_id, float amount, bool multi_use=false, String terminal_label= "");

    void      size(uint16_t w, uint16_t h);
    void      width(uint16_t w);
    uint16_t  width();
    void      height(uint16_t h);
    uint16_t  height();

    uint16_t w=231;
  private:
    uint8_t _qr_version = QR_VERSION;

};


// QR Version  Q Alphanumeric (max length)
// 1(21)            16
// 2(25)            29
// 3(29)            47
// 4(33)            67
// 5(37)            87
// 6(41)           108
// 7(45)           125
// 8(49)           157
// 9(53)           189
// 10(57)          221
// 11(61)          259
// 12(65)          296
// 13(69)          352
// 14(73)          376
// 15(77)          426
// 16(81)          470
// 17(85)          531
// 18(89)          574
// 19(93)          644
// 20(97)          702
// 21(101)         742
// 22(105)         823

#endif //__GQRCODE_H__
