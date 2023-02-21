/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GWidgets_init.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GWidgets.h"
#include <Ticker.h>
#define LVGL_CHECK_PERIOD 30000  // ms

extern "C" {
  uint8_t lv_lvgl_go();
  const char* blynkgo_serial_no();
}

extern  String blynkgo_board_name;

static GObject *obj_message = NULL;
static GImage  *logo        = NULL;        //(blynkgo_logo);
static GLabel  *lb_message  = NULL; //( _ctn_message);
static GLabel  *lb_serialno = NULL;//( _ctn_message );
static GLabel  *lb_contact  = NULL;
static Ticker ticker_checker;

static void lvgl_checker(void*);
static void lvgl_checker_del(void*);

IMG_DECLARE(blynkgo_logo);
IMG_DECLARE(beenext_logo);

GObject GScreen(NULL);              // _par ไม่ต้องกำหนด 
void GWidgets_init(){
  GScreen.setObject(lv_scr_act());  // กำหนด obj เท่าน้น และให้เป็น created ไปเลย จะได้ใช้ style ได้
  GScreen.padding(0);
  GScreen.color(TFT_BLACK);
  GScreen.text_color(TFT_WHITE);
  GScreen.line_color(TFT_WHITE);

  GStyle_init();
  style_sb_init();
  // Serial.println("[GWidget] inited");
  
  // อย่าลืมเปิด
  // ticker_checker.once_ms(5000L, [](){
  //   lvgl_checker(NULL);
  // });
}

static void blynkgo_unregisted_info(){
  bool is_beenext = blynkgo_board_name.startsWith("BeeNeXT");

  if(is_beenext) {
    Serial.println("**********************************************************");
    Serial.println(String("[BeeNeXT] SN.") + blynkgo_serial_no());
    Serial.println("[BeeNeXT] This Board is UNREGISTED.");
    Serial.println();
    Serial.println("   Regist at beenext@gmail.com");
    // Serial.println("       or at https://web.facebook.com/profile.php?id=100087122164195");
    Serial.println("**********************************************************");
    Serial.println();
  }else{
    Serial.println("**********************************************************");
    Serial.println(String("[BlynkGO] SN.") + blynkgo_serial_no());
    Serial.println("[BlynkGO] This Board is UNREGISTED.");
    Serial.println();
    Serial.println("   Regist at blynkgo@gmail.com");
    Serial.println("       or at https://www.facebook.com/blynkgo.os.1");
    Serial.println("**********************************************************");
    Serial.println();
  }
  if( obj_message == NULL) {
    obj_message = new_GObject();
    logo        = new_GImage(obj_message);
    lb_message  = new_GLabel(obj_message);
    lb_serialno = new_GLabel(obj_message);
    lb_contact  = new_GLabel(obj_message);

    if(GScreen.width() <= 240){
      obj_message->corner_radius(10);
      obj_message->color((is_beenext)? TFT_COLOR_HSV(random(360),40,100) : TFT_SILVER);
      obj_message->border(2, TFT_DARKGRAY);
      obj_message->font(prasanmit_20,TFT_BLACK);
      
      *logo         = (is_beenext)?  beenext_logo : blynkgo_logo;
      *lb_message   = "UNREGISTED!!!";
      *lb_serialno  =  String("SN.")+ blynkgo_serial_no();
      *lb_contact   = (is_beenext)?  "regist at #0000ff beenext@gmail.com#" : "regist at #0000ff blynkgo@gmail.com#";

      lb_message->font(prasanmit_25,TFT_RED);
      lb_contact->colorful(true);

      logo->position( 10, 10 );
      lb_message->align(logo, ALIGN_RIGHT, 5);
      lb_serialno->align(ALIGN_TOP, 0, logo->bottomY() );
      lb_contact->align(lb_serialno, ALIGN_BOTTOM);
      obj_message->size(220, lb_contact->bottomY()+10);
      lb_contact->realign();
    }
    else if( GScreen.width() == 320){
      obj_message->corner_radius(10);
      obj_message->color((is_beenext)? TFT_COLOR_HSV(random(360),40,100) : TFT_SILVER);
      obj_message->border(2, TFT_DARKGRAY);
      obj_message->font(prasanmit_25,TFT_BLACK);
          
      *logo         = (is_beenext)?  beenext_logo : blynkgo_logo;
      *lb_message   = "UNREGISTED!!!";
      *lb_serialno  =  String("SN.")+ blynkgo_serial_no();
      *lb_contact   =  (is_beenext)?  "regist at #0000ff beenext@gmail.com#" : "regist at #0000ff blynkgo@gmail.com#";

      lb_message->font(prasanmit_40,TFT_RED);
      lb_contact->colorful(true);

      logo->position( 10, 10 );
      lb_message->align(logo, ALIGN_RIGHT, 5);
      lb_serialno->align(ALIGN_TOP, 0, logo->bottomY() );
      lb_contact->align(lb_serialno, ALIGN_BOTTOM);
      obj_message->size(300, lb_contact->bottomY()+10);
      lb_contact->realign();

    }else if( GScreen.width() >= 480){
      obj_message->corner_radius(10);
      obj_message->color((is_beenext)? TFT_COLOR_HSV(random(360),40,100) : TFT_SILVER);
      obj_message->border(2, TFT_DARKGRAY);
        
    //  logo->setImage(blynkgo_logo);    
      *logo         = (is_beenext)?  beenext_logo : blynkgo_logo;
      *lb_message   = "UNREGISTED!!!";
      *lb_serialno  =  String("SN.")+ blynkgo_serial_no();
      *lb_contact   =  (is_beenext)?  "regist at #0000ff beenext@gmail.com#" : "regist at #0000ff blynkgo@gmail.com#";

      obj_message->font(prasanmit_30,TFT_BLACK);
      lb_message->font(prasanmit_40,TFT_RED);
      lb_contact->colorful(true);

      logo->position( 30, 10 );
      lb_message->align(logo, ALIGN_RIGHT, 10);
      lb_serialno->align(ALIGN_TOP, 0, logo->bottomY() );
      lb_contact->align(lb_serialno, ALIGN_BOTTOM);
      obj_message->size(360, lb_contact->bottomY()+10);
      lb_contact->realign();
    }

  }else{
    obj_message->hidden(false);
    obj_message->toForeground();
  }
}

// enum {
//   UNREGIST                  = 0,
//   BLYNKGO_NORMAL            = 1,
//   BLYNKGO_BOARD_V1          = 1,    // 480x320
//   BLYNKGO_EMMA_II           = 1,    // 480x320
//   BLYNKGO_BOARD_S3_PARALLEL = 1,    // 480x320
//   BLYNKGO_BOARD_S3_SPI      = 1,    // 480x320
//   BLYNKGO_MINI              = 2,    // 240x135
//   BLYNKGO_ENTERPRISE_5      = 3,    // 800x480
//   BLYNKGO_ENTERPRISE_7      = 4,    // 800x480
//   BLYNKGO_BOARD_3_5_S3P     = 5,    // 480x320  ESP32S3 Parallel
//   BLYNKGO_EMMA_PRO          = 6,    // 480x320  ESP32S3-485
//   BLYNKGO_BEENEXT_2_8       = 7,    // 320x240
//   BLYNKGO_BEENEXT_3_5       = 8,    // 480x320
//   BLYNKGO_BEENEXT_4_3       = 9,    // 480x272
//   BLYNKGO_BEENEXT_4_3_IPS   = 10,   // 800x480
//   BLYNKGO_ESP32DEVKIT_OLED  = 11,   
//   BLYNKGO_ESP32DEVKIT_TFT   = 12,
//   BLYNKGO_TYPE13 = 13,
//   BLYNKGO_TYPE14 = 14,
//   BLYNKGO_TYPE15 = 15,
//   BLYNKGO_TYPE16 = 16,
//   BLYNKGO_TYPE17 = 17,
//   BLYNKGO_TYPE18 = 18,
//   BLYNKGO_TYPE19 = 19,
//   BLYNKGO_TYPE20 = 20,
// };

static void lvgl_checker(void*){
  bool registed = false;

  uint8_t type = lv_lvgl_go();
  // Serial.println(type);
  String sn_no = blynkgo_serial_no();
  if( sn_no == "2AA8-34455CAB-9568-D169" ){  // 7 นิ้ว ตัวแรก ที่ปล่อยออกไป
    registed = true;
  }else{
    switch(type){
      case 0:
        break;
      case 1:  // BLYNKGO_BOARD_V1, 
        if( (GScreen.width() == 480 && GScreen.height()==320) || 
            (GScreen.width() == 320 && GScreen.height()==480)  )   {  registed = true; }
        break;
      case 2:  // BLYNKGO_MINI
        if( (GScreen.width() == 240 && GScreen.height()==135) || 
            (GScreen.width() == 135 && GScreen.height()==240)  )   {  registed = true; }
        break;
      case 3:  // BLYNKGO_ENTERPRISE_5  , LOLIN_D32_PRO_OLED_SSD1306, LOLIN_D32_PRO_OLED_SH1106
        if(((GScreen.width() == 800 && GScreen.height()==480) || 
            (GScreen.width() == 480 && GScreen.height()==800)) && 
          blynkgo_board_name == "BlynkGO Enterprise 5.0\" Capacitive Touch" )   {  registed = true; }
        if( blynkgo_board_name.startsWith("Lolin D32Pro OLED"))                 {  registed = true; }
        break;
      case 4:  // BLYNKGO_ENTERPRISE_7, LOLIN_D32_PRO_OLED_SSD1306, LOLIN_D32_PRO_OLED_SH1106
        if(((GScreen.width() == 800 && GScreen.height()==480) || 
            (GScreen.width() == 480 && GScreen.height()==800)) && 
          blynkgo_board_name == "BlynkGO Enterprise 7.0\" Capacitive Touch" )   {  registed = true; }
        if( blynkgo_board_name.startsWith("Lolin D32Pro OLED"))                 {  registed = true; }
        break;
      case 5:  // BLYNKGO_BOARD_3_5_S3P BLYNKGO_BOARD_S3_PARALLEL, BLYNKGO_BOARD_S3_SPI
        if( (GScreen.width() == 480 && GScreen.height()==320) || 
            (GScreen.width() == 320 && GScreen.height()==480)  )   {  registed = true; }
        break;
      case 6:  // BLYNKGO_EMMA_PRO 485
        if( (GScreen.width() == 480 && GScreen.height()==320) || 
            (GScreen.width() == 320 && GScreen.height()==480)  )   {  registed = true; }
        break;
      case 7:  // BeeNeXT 2.8  320x240
        if( (GScreen.width() == 320 && GScreen.height()==240) || 
            (GScreen.width() == 240 && GScreen.height()==320) &&
          blynkgo_board_name.startsWith("BeeNeXT 2.8")         )   {  registed = true; }
        break;
      case 8:  // BeeNeXT 3.5  480x320
        if( (GScreen.width() == 480 && GScreen.height()==320) || 
            (GScreen.width() == 320 && GScreen.height()==480) &&
          blynkgo_board_name.startsWith("BeeNeXT 3.5")         )   {  registed = true; }
        break;
      case 9:   // BeeNeXT 4.3  480x272
        if( (GScreen.width() == 480 && GScreen.height()==272) || 
            (GScreen.width() == 272 && GScreen.height()==480) &&
          blynkgo_board_name.startsWith("BeeNeXT 4.3")         )   {  registed = true; }
        break;
      case 10:  // BeeNeXT 4.3 IPS 800x480
        if( (GScreen.width() == 800 && GScreen.height()==480) || 
            (GScreen.width() == 480 && GScreen.height()==800) &&
          blynkgo_board_name.startsWith("BeeNeXT 4.3")         )   {  registed = true; }
        break;
      case 11:  // BeeNeXT 5.0 800x480
        if( (GScreen.width() == 800 && GScreen.height()==480) || 
            (GScreen.width() == 480 && GScreen.height()==800) &&
          blynkgo_board_name.startsWith("BeeNeXT 5.0")         )   {  registed = true; }
        break;
      case 12:  // BeeNeXT 7.0 800x480
        if( (GScreen.width() == 800 && GScreen.height()==480) || 
            (GScreen.width() == 480 && GScreen.height()==800) &&
          blynkgo_board_name.startsWith("BeeNeXT 7.0")         )   {  registed = true; }
        break;
      case 13:  // BLYNKGO_ESP32DEVBOARD_OLED_SSD1306, BLYNKGO_ESP32DEVBOARD_OLED_SH1106 128x64
        if( (GScreen.width() <= 240 && GScreen.height()<=240)  )   {  registed = true; }
        break;
      case 14:  // BLYNKGO_ESP32DEVBOARD_TFT_2_4_INCH, .. BLYNKGO_ESP32DEVBOARD_TFT_4_0_INCH
        if( (GScreen.width() <= 480 && GScreen.height()<=480) )    {  registed = true; }
        break;
      case 15:
        break;
      case 16:
        break;
      case 17:
        break;
      case 18:
        break;
      case 19:
        break;
      case 20:
        break;
    }
  }
  if(registed) {
    Serial.println("[BlynkGO] registed");
    return;
  }

  blynkgo_unregisted_info();

  // bool is_beenext = blynkgo_board_name.startsWith("BeeNeXT");

  ticker_checker.detach();
  // if(is_beenext) {
  //   ticker_checker.once_ms(5000L, [](){
  //     ESP.restart();
  //   });
  // }else{
    ticker_checker.once_ms(5000L, [](){
      lvgl_checker(NULL);
    });
  // }
}

// static void lvgl_checker_del(void*) {
//   obj_message->hidden(true);

//   ticker_checker.detach();
//   ticker_checker.once_ms(5000L, [](){
//     lvgl_checker(NULL);
//   });
// }

