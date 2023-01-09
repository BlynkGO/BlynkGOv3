/**********************************************
 * 
 * ตัวอย่าง QR Code Scanner 
 * สำหรับ บอร์ด BlynkGO Board V1.1
 * 
 **********************************************/

#include <BlynkGOv3.h>
#include "esp_camera.h"
#include "src/quirc.h"

/**********************************************
 * define / typedef
 **********************************************/
// กำหนดให้ กล้องไปอยู่หลังจอ  TFT หรือไม่  true / false
//    - หาก กำหนดเป็น true  ให้ เอนกล้องไปอยู่ด้านหลัง จอ TFT
//    - หาก กำหนดเป็น false ให้ เอนกล้องมาอยู๋ในทิศทางเดียวกับจอ TFT
#define CAMERA_REAR             true

/**********************************************
 * function prototypes
 **********************************************/
bool      camera_init(bool pixel_gray, bool camera_rear);
void      camera_update(void *param=NULL);
uint16_t  camera_width();
uint16_t  camera_height();

void      qr_scanner(void* param);

/**********************************************
 * global variables
 **********************************************/
bool camera_rear  = CAMERA_REAR;
bool format_gray  = false;

/**********************************************
 * BlynkGO Widgets
 **********************************************/
GLabel  lb_title = "QR Code Scanner";
GRect   camera_rect;      // วิตเจ็ตสี่เหลี่ยม สำหรับทำเป็นกรอบ
GImage  camera_img;       // วิตเจ็ตรูปภาพ สำหรับแสดงภาพจากกล้อง
GButton btn_rear(BUTTON_PUSH, (camera_rear)? "กล้องหลัง" : "กล้องหน้า");   // วิตเจ็ตปุ่มกด สลับ กล้อง หน้า/หลัง
GLabel  lb_qrcode(camera_rect);
GTask   camera_task;


/**********************************************
 * Arduino's setup() & loop()
 **********************************************/
void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  camera_init(format_gray, camera_rear);      // เริ่มการใช้งาน camera
  camera_update();                            // ให้จับภาพจาก camera แสดงขึ้น วิตเจ็ตรูปภาพ

  // สร้างกรอบ ล้อม วิตเจ็ตรูปภาพ ด้วย วิตเจ็ตสีเหลี่ยม camera_rect แต่ทำเป็นกรอบสีฟ้า และสามารถลากเลื่อนไปมาได้
  camera_rect.size( camera_width()+20, camera_height() +20); 
  camera_rect.border(10, TFT_WHITE);
  camera_rect.opa(0);
  camera_rect.corner_radius(20);
  camera_rect.align(ALIGN_LEFT,10,15);
  camera_rect.font(prasanmit_30, TFT_RED);
  lb_qrcode.align(ALIGN_BOTTOM,0,-20);

  // ให้ วิตเจ็ตรูปภาพพ อยู่กลาง กรอบ camera_rect
  camera_img.align(camera_rect, ALIGN_CENTER);
  camera_img.line_color(TFT_WHITE);

  // วิตเจ็ดปุ่มกด สลับ กลอ้งหน้า/กล้องหลัง
  btn_rear.align(ALIGN_RIGHT_TOP,-15,45);
  btn_rear.font(prasanmit_25);
  btn_rear.width(100);
  btn_rear.color((camera_rear)? TFT_BLUE: TFT_GREEN);       // กำหนดสีปุ่ม
  btn_rear.onClicked([](GWidget*w){                         // เมื่อมีการ clicked เข้ามา
    camera_rear = !camera_rear;                             // สลับสถานะ กล้องหน้า <--> กล้องหลัง
    btn_rear = (camera_rear)? "กล้องหลัง" : "กล้องหน้า";       // เปลี่ยนข้อความปุ่ม
    btn_rear.color((camera_rear)? TFT_BLUE: TFT_GREEN);     // เปลี่ยนสีปุ่ม
    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s, !camera_rear);                          // เปลี่ยนสถานะกล้องหน้า/หลัง
  });

  lb_title.align(ALIGN_TOP);
  lb_title.font(prasanmit_40, TFT_BLACK);
  
  camera_task.setInterval(100L, camera_update);             // ให้มีการ update ภาพ ขึ้น วิตเจ็ตรูปภาพ เป็นระยะๆ ทุก 100ms
  xTaskCreate(qr_scanner, "qr_scanner", 111500, NULL, 5, NULL);
}

void loop() {
  BlynkGO.update();
}

