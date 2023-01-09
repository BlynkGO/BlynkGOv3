/********************************************************************
 * ตัวอย่าง การสร้าง HomePlan Switch (สวิตซ์ จาก แปลนบ้าน)
 * โดยสามารถ ใช้แอพ Blynk เปิดปิด ไปพร้อมกับ TFT ได้
 * 
 *  Resource รูปแปลนบ้าน ให้ดูใน โฟลเดอร์  BlynkBlynkGO_HomePlan_Switch/resource
 *  สามารถทำการแปลงเป็น รูป c file ด้วย Tool เครื่องมือ BlynkGO_ImgConv (ติดต่อ admin)
 *  ด้วยคำสั่ง 
 *    1. สำหรับรูปทั่วไป 
 *         BlynkGO_ImgConv  <รูป>  <ชื่อ>
 *
 *    2. สำหรับรูปที่มีพื้นที่โปร่งใส
 *         BlynkGO_ImgConv  <รูป>  <ชื่อ>   t
 *
 * --------------------------------------------
 * ที่แอพ Blynk ให้สร้าง Widget Button
 * โดยกำหนด virtual pin ตามใน code นี้
 * ------------------------------------------------------------------
 * หมายเหตุ 
 * ก่อนใช้งาน ร่วมกับแอพ Blynk
 *  1. ให้ copy ไฟล์ ในโฟลเดอร์ BlynkGOv3/BlynkBlynkGO ให้นำโฟลเดอร์ src ไปวางไว้ที่ 
 *     โฟลเดอร์ไลบรารี่ Blynk หากมีไฟล์ซ้ำให้ copy ทับ
 *
 *  2. ให้กำหนด เปิด การใช้งาน Blynk ให้ BlynkGO ได้ที่
 *     ไฟล์ config/blynkgo_config.h
 *     โดยกำหนดเป็น
 *       #define BLYNKGO_USE_WIFI           1
 *       #define BLYNKGO_USE_NTP            1
 *       #define BLYNKGO_USE_BLYNK          1
 *
 **************************************************************/
 
#include <BlynkGOv3.h>

/*****************************************
 * Resources Declare
 *****************************************/
IMG_DECLARE(img_balcony);     // ประกาศรูป  c file ก่อนใช้ใน วิตเจ็ต
IMG_DECLARE(img_bathroom);
IMG_DECLARE(img_bedroom);
IMG_DECLARE(img_closet);
IMG_DECLARE(img_kitchen);
IMG_DECLARE(img_path);
IMG_DECLARE(img_table);

/*****************************************
 * Widgets
 *****************************************/
GWiFiManager wifi_manager;
GBlynkManager blynk_manager;
 
GImageButton balcony  (BUTTON_SWITCH, img_balcony);   // กำหนดให้เป็นปุ่มกดแบบ Switch( กดติดกดดับ)
GImageButton bathroom (BUTTON_SWITCH, img_bathroom);
GImageButton bedroom  (BUTTON_SWITCH, img_bedroom);
GImageButton closet   (BUTTON_SWITCH, img_closet);
GImageButton kitchen  (BUTTON_SWITCH, img_kitchen);
GImageButton path     (BUTTON_SWITCH, img_path);
GImageButton table    (BUTTON_SWITCH, img_table);


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  wifi_manager  .align(ALIGN_TOP_RIGHT, -5, 5);
  blynk_manager .align( wifi_manager, ALIGN_BOTTOM, 0, 10);

  balcony .align(ALIGN_TOP_LEFT, 61, 10);
  bedroom .align(balcony  , ALIGN_RIGHT_TOP);
  table   .align(bedroom  , ALIGN_BOTTOM);
  bathroom.align(bedroom  , ALIGN_RIGHT_TOP);
  closet  .align(bathroom , ALIGN_BOTTOM);
  path    .align(closet   , ALIGN_BOTTOM);
  kitchen .align(path     , ALIGN_BOTTOM_RIGHT);

  balcony .gpio(5 , HIGH);    // set GPIO with relay ACTIVE_HIGH type ; if relay is ACTIVE_LOW set LOW 
  bedroom .gpio(12, HIGH);
  table   .gpio(13, HIGH);
  bathroom.gpio(14, HIGH);
  closet  .gpio(22, HIGH);
  path    .gpio(23, HIGH);
  kitchen .gpio(25, HIGH);

  balcony .virtualPin(V11);   // set Virtual Pin that is same with Blynk App.
  bedroom .virtualPin(V12);
  table   .virtualPin(V13);
  bathroom.virtualPin(V14);
  closet  .virtualPin(V15);
  path    .virtualPin(V16);
  kitchen .virtualPin(V17);
}

void loop() {
  BlynkGO.update();
}

/*****************************************
 * SmartWiFi
 *****************************************/

WIFI_CONNECTED(){
  Serial.print("[WiFi] connected. IP : ");
  Serial.println(WiFi.localIP());
}

WIFI_DISCONNECTED(){
  Serial.println("[WiFi] disconnected.");
}

/*****************************************
 * Blynk
 *****************************************/
BLYNK_CONNECTED(){
  Serial.println("[Blynk] connected.");  
  BlynkGO.syncAll();
}
BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] disconnected.");
}
