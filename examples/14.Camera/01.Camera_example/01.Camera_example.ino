/**********************************************
 * 
 * ตัวอย่าง การแสดงภาพจาก camara ให้ วิตเจ็ต GImage
 * สำหรับ บอร์ด BlynkGO Board V1.1
 * 
 **********************************************/

#include <BlynkGOv3.h>
#include "esp_camera.h"

// กำหนดให้ กล้องไปอยู่หลังจอ  TFT หรือไม่  true / false
//    - หาก กำหนดเป็น true  ให้ เอนสายแพของ กล้อง โค้งไปด้านหลัง เพื่อทำให้กล้องไปอยู่ด้านหลัง จอ TFT
//    - หาก กำหนดเป็น false ให้ กล้องมาอยู๋ในทิศทางเดียวกับจอ TFT
#define CAMARA_REAR             true

GImage camera_img;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  camera_init();        // เริ่มการใช้งาน camera

}

void loop() {
  BlynkGO.update();
  
  // รับรูปจาก camera แล้วทำการกลับ byte พร้อมกำหนด ให้วิตเจ็ตรูปภาพ camera_img
  camera_fb_t *fb = esp_camera_fb_get();
  camara_swapbyte(fb);
  camera_img.setImage(fb->buf, fb->width, fb->height);
}

//Camera setting
void camera_init() {
  //camera config
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;

  config.pixel_format = PIXFORMAT_RGB565;
  config.frame_size   = FRAMESIZE_QVGA;  // QVGA 320*240
  config.jpeg_quality = 10;
  config.fb_count     = 1;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("[Camera] init failed with error 0x%x\n", err);
    while (1);
  }

  sensor_t *s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV2640_PID)  // 2MP Camera
  {
    s->set_vflip(s, !CAMARA_REAR);      // flip it back
    s->set_brightness(s, 0);            //  -2 .. 2  : up the blightness just a bit
    s->set_saturation(s, 1);            //  -2 .. 2  : lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);
  Serial.println("[Camera] inited" );
}

uint8_t* camara_swapbyte(camera_fb_t* fb) {
  if(fb==NULL) return NULL;
  uint16_t* fb_buf16 = (uint16_t*) fb->buf;
  for (int16_t j = 0; j < fb->height; j++){
    for (int16_t i = 0; i < fb->width; i++) {
      fb_buf16[j * fb->width + i]  = (fb_buf16[j * fb->width + i] << 8) | (fb_buf16[j * fb->width + i] >> 8);
    }
  }
  return (uint8_t*) fb->buf;
}

