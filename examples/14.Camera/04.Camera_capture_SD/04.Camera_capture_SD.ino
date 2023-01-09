#include <BlynkGOv3.h>
#include "esp_camera.h"

IMG_DECLARE(ico_capture_sd);

// กำหนดให้ กล้องไปอยู่หลังจอ  TFT หรือไม่  true / false
//    - หาก กำหนดเป็น true  ให้ เอนสายแพของ กล้อง โค้งไปด้านหลัง เพื่อทำให้กล้องไปอยู่ด้านหลัง จอ TFT
//    - หาก กำหนดเป็น false ให้ กล้องมาอยู๋ในทิศทางเดียวกับจอ TFT
#define CAMARA_REAR             false

#define CAMERA_FOLDER       "/camera/"            // ให้สร้าง โฟลเดอร์นี้ ไว้ที่ sd card เตรียมไว้ก่อน
#define CAPTURE_TEMPLATE    "capture_%05d.jpg"


GImage camera_img;
//GButton btn_postimage;
GImageButton btn_capture_sd(ico_capture_sd);

GRect   rect_info;
GLabel  lb_info(rect_info);
GAnimation anim_info;

void camera_init();
uint8_t* camara_swapbyte(camera_fb_t* fb);
uint8_t* jpg_data;
size_t   jpg_len;

bool capture = false;

uint16_t capture_id = 1;


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  camera_init();        // เริ่มการใช้งาน camera

  if( BlynkGO.flashMem_exists("CAPTURE_ID") )
    capture_id = (uint16_t) BlynkGO.flashMem_Int("CAPTURE_ID");  // โหลดค่า capture_id สุดท้าย ที่เคย save

  camera_img.align(ALIGN_LEFT,30);
//  btn_capture_sd = "บันทึกภาพ";
  btn_capture_sd.align(ALIGN_RIGHT,-30);
  btn_capture_sd.onClicked([](GWidget*w){
    capture = true;
    lb_info = "กำลังบันทึกลง SD";
    rect_info.toForeground();
    anim_info.init(rect_info, [](GWidget*w, int16_t v){
      rect_info.align(ALIGN_BOTTOM, 0, rect_info.height() - v);
    }, 0, rect_info.height(), 1000);
    anim_info.create();
  });

  rect_info.size(480,80);
  rect_info.corner_radius(5);
  rect_info.border(2, TFT_DARKGRAY);
  rect_info.font_color(TFT_BLACK);
  rect_info.align(ALIGN_BOTTOM,0,rect_info.height());
}

void loop() {
  BlynkGO.update();
  
  // รับรูปจาก camera แล้วทำการกลับ byte พร้อมกำหนด ให้วิตเจ็ตรูปภาพ camera_img
  camera_fb_t *fb = esp_camera_fb_get();
  if(capture){
    capture = false;
    if( fmt2jpg(fb->buf, fb->len, fb->width, fb->height, PIXFORMAT_RGB565, 100, &jpg_data, &jpg_len)){
      Serial.println("JPG Data done!");
      char jpg_filename[64];
      snprintf( jpg_filename, sizeof(jpg_filename), CAMERA_FOLDER  CAPTURE_TEMPLATE, capture_id+1);
      Serial.println( jpg_filename);
      
      File jpg_file = SD.open(jpg_filename, FILE_WRITE);
      if(jpg_file){
        jpg_file.write( jpg_data, jpg_len);
        jpg_file.close();

        capture_id++;
        BlynkGO.flashMem("CAPTURE_ID", capture_id); 
        
        lb_info = StringX::printf( "บันทึกเรียบร้อย " CAPTURE_TEMPLATE, capture_id) ;
      }

      anim_info.init(rect_info, [](GWidget*w, int16_t v){
        rect_info.align(ALIGN_BOTTOM, 0, v);
      }, 0, rect_info.height(), 1000);
      anim_info.delay(2000);
      anim_info.create();
    }    
  }else{ 
    camara_swapbyte(fb);
    camera_img.setImage(fb->buf, fb->width, fb->height);
  }
}