/***********************************************************************
 * ตัวอย่าง โปรแกรมกล้อง BlynkGO Camara32
 *   version 1.0
 *     - ปรับค่า brightnest, contrast, saturation, 
 *        sharpness, gain ceiling, quality, effect ได้
 *        
 *     - สลับกล้องหน้า/หลังได้  (ปรับปรับกล้องหลัง ให้เอนสายแพ กล้องไปด้านหลังจอ)
 *     
 ***********************************************************************/

#include <BlynkGOv3.h>
#include "esp_camera.h"

/**********************************************
 * function prototypes
 **********************************************/
bool      camera_init(bool pixel_gray, bool camera_rear);
void      camera_update(void *param=NULL);
uint16_t  camera_width();
uint16_t  camera_height();

/**********************************************
 * global variables
 **********************************************/
bool camera_rear  = true;
enum{ CMR_ST_NONE, CMR_ST_BRIGHTNESS, CMR_ST_CONTRAST, 
      CMR_ST_SATURATION, CMR_ST_SHARPNESS, CMR_ST_DENOISE, 
      CMR_ST_GAINCEILING, CMR_ST_QUALITY, CMR_ST_EFFECT } 
camera_status_type = CMR_ST_NONE;

/**********************************************
 * BlynkGO Widgets
 **********************************************/
GRect   camera_rect;      // วิตเจ็ตสี่เหลี่ยม สำหรับทำเป็นกรอบ
GImage  camera_img;       // วิตเจ็ตรูปภาพ สำหรับแสดงภาพจากกล้อง
GButton btn_rear(BUTTON_PUSH, (camera_rear)? "กล้องหลัง" : "กล้องหน้า");   // วิตเจ็ตปุ่มกด สลับ กล้อง หน้า/หลัง
GButton btn_brightness(BUTTON_PUSH, "ความสว่าง");
GButton btn_contrast(BUTTON_PUSH, "contrast");
GButton btn_saturation(BUTTON_PUSH, "saturation");
GButton btn_sharpness(BUTTON_PUSH, "ความคมชัด");
GButton btn_denoise(BUTTON_PUSH, "ลด Noise");
GButton btn_gainceiling(BUTTON_PUSH, "gain ceiling");
GButton btn_quality(BUTTON_PUSH, "quality");
GButton btn_effect(BUTTON_PUSH, "effect");

GSliderH slider_h;
GTask   camera_task;

/**********************************************
 * setup() & loop()
 **********************************************/
void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  camera_init(false, camera_rear);      // เริ่มการใช้งาน camera

  camera_img.align(ALIGN_TOP,0,2);  
  camera_task.setInterval(100L, camera_update);             // ให้มีการ update ภาพ ขึ้น วิตเจ็ตรูปภาพ เป็นระยะๆ ทุก 100ms

  // วิตเจ็ดปุ่มกด สลับ กลอ้งหน้า/กล้องหลัง
  btn_rear.align(ALIGN_RIGHT_TOP,-5,2);
  btn_rear.font(prasanmit_20);
  btn_rear.size(70,30);
  btn_rear.color((camera_rear)? TFT_BLUE: TFT_GREEN);       // กำหนดสีปุ่ม
  btn_rear.onClicked([](GWidget*w){                         // เมื่อมีการ clicked เข้ามา
    camera_rear = !camera_rear;                             // สลับสถานะ กล้องหน้า <--> กล้องหลัง
    btn_rear = (camera_rear)? "กล้องหลัง" : "กล้องหน้า";       // เปลี่ยนข้อความปุ่ม
    btn_rear.color((camera_rear)? TFT_BLUE: TFT_GREEN);     // เปลี่ยนสีปุ่ม
    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s, !camera_rear);                          // เปลี่ยนสถานะกล้องหน้า/หลัง
  });

  // วิตเจ็ดปุ่มกด ปรับความสว่าง
  btn_brightness.align(ALIGN_LEFT_TOP,5,2);
  btn_brightness.size(btn_rear);
  btn_brightness.font(prasanmit_20);
  btn_brightness.color(TFT_BLUE);
  btn_brightness.onClicked([](GWidget*w){
    if( camera_status_type == CMR_ST_BRIGHTNESS){
      slider_h.hidden(true);
      camera_status_type = CMR_ST_NONE;
    }else {
      slider_h.hidden(false);
      slider_h.toForeground();
      slider_h.title("ความสว่าง");
      slider_h.range(-2,2);
      sensor_t *s = esp_camera_sensor_get();
      slider_h =slider_h = s->status.brightness;
      camera_status_type = CMR_ST_BRIGHTNESS;
    }
  });

  // วิตเจ็ดปุ่มกด ปรับ contrast
  btn_contrast.align(btn_brightness, ALIGN_BOTTOM,0,5);
  btn_contrast.size(btn_rear);
  btn_contrast.font(prasanmit_20);
  btn_contrast.color(TFT_BLUE);
  btn_contrast.onClicked([](GWidget*w){
    if( camera_status_type == CMR_ST_CONTRAST){
      slider_h.hidden(true);
      camera_status_type = CMR_ST_NONE;
    }else {
      slider_h.hidden(false);
      slider_h.toForeground();
      slider_h.title("Contrast");
      slider_h.range(-2,2);
      sensor_t *s = esp_camera_sensor_get();
      slider_h =slider_h = s->status.contrast;
      camera_status_type = CMR_ST_CONTRAST;
    }
  });

  // วิตเจ็ดปุ่มกด ปรับ saturation
  btn_saturation.align(btn_contrast, ALIGN_BOTTOM,0,5);
  btn_saturation.size(btn_rear);
  btn_saturation.font(prasanmit_20);
  btn_saturation.color(TFT_BLUE);
  btn_saturation.onClicked([](GWidget*w){
    if( camera_status_type == CMR_ST_SATURATION){
      slider_h.hidden(true);
      camera_status_type = CMR_ST_NONE;
    }else {
      slider_h.hidden(false);
      slider_h.toForeground();
      slider_h.title("Saturation");
      slider_h.range(-2,2);
      sensor_t *s = esp_camera_sensor_get();
      slider_h =slider_h = s->status.saturation;
      camera_status_type = CMR_ST_SATURATION;
    }
  });

  // วิตเจ็ดปุ่มกด ปรับ sharpness
  btn_sharpness.align(btn_saturation, ALIGN_BOTTOM,0,5);
  btn_sharpness.size(btn_rear);
  btn_sharpness.font(prasanmit_20);
  btn_sharpness.color(TFT_BLUE);
  btn_sharpness.onClicked([](GWidget*w){
    if( camera_status_type == CMR_ST_SHARPNESS){
      slider_h.hidden(true);
      camera_status_type = CMR_ST_NONE;
    }else {
      slider_h.hidden(false);
      slider_h.toForeground();
      slider_h.title("ความคมชัด");
      slider_h.range(-2,2);
      sensor_t *s = esp_camera_sensor_get();
      slider_h =slider_h = s->status.sharpness;
      camera_status_type = CMR_ST_SHARPNESS;
    }
  });

  // วิตเจ็ดปุ่มกด ปรับ gain ceiling
  btn_gainceiling.align(btn_sharpness, ALIGN_BOTTOM,0,5);
  btn_gainceiling.size(btn_rear);
  btn_gainceiling.font(prasanmit_20);
  btn_gainceiling.color(TFT_BLUE);
  btn_gainceiling.onClicked([](GWidget*w){
    if( camera_status_type == CMR_ST_GAINCEILING){
      slider_h.hidden(true);
      camera_status_type = CMR_ST_NONE;
    }else {
      slider_h.hidden(false);
      slider_h.toForeground();
      slider_h.title("Effect");
      slider_h.range(0,6);
      sensor_t *s = esp_camera_sensor_get();
      slider_h =slider_h = s->status.gainceiling;
      camera_status_type = CMR_ST_GAINCEILING;
    }
  });

  // วิตเจ็ดปุ่มกด ปรับ quality
  btn_quality.align(btn_gainceiling, ALIGN_BOTTOM,0,5);
  btn_quality.size(btn_rear);
  btn_quality.font(prasanmit_20);
  btn_quality.color(TFT_BLUE);
  btn_quality.onClicked([](GWidget*w){
    if( camera_status_type == CMR_ST_QUALITY){
      slider_h.hidden(true);
      camera_status_type = CMR_ST_NONE;
    }else {
      slider_h.hidden(false);
      slider_h.toForeground();
      slider_h.title("Quality");
      slider_h.range(0,63);
      sensor_t *s = esp_camera_sensor_get();
      slider_h =slider_h = s->status.quality;
      camera_status_type = CMR_ST_QUALITY;
    }
  });

  // วิตเจ็ดปุ่มกด ปรับ special effect
  btn_effect.align(btn_quality, ALIGN_BOTTOM,0,5);
  btn_effect.size(btn_rear);
  btn_effect.font(prasanmit_20);
  btn_effect.color(TFT_BLUE);
  btn_effect.onClicked([](GWidget*w){
    if( camera_status_type == CMR_ST_EFFECT){
      slider_h.hidden(true);
      camera_status_type = CMR_ST_NONE;
    }else {
      slider_h.hidden(false);
      slider_h.toForeground();
      slider_h.title("Special Effect");
      slider_h.range(0,6);
      sensor_t *s = esp_camera_sensor_get();
      slider_h =slider_h = s->status.special_effect;
      camera_status_type = CMR_ST_EFFECT;
    }
  });

  // วิตเจ็ด slider สำหรับใช้ปรับค่าต่างๆของ กล้อง
  slider_h.size(478,60);
  slider_h.range(-2,2);
  slider_h.title("brightness");
  slider_h.align(ALIGN_BOTTOM,0,-1);
  slider_h.hidden(true);
  slider_h = 0;
  slider_h.onValueChanged([](GWidget*w){
    // ตัวอย่างปรับค่า ต่างๆ ของกล้อง ESP32 จากค่า slider_h ที่มีการเปลี่ยนแปลง
    // สามารถปรับแต่งเพิ่มเติมโดย ดู API ได้จาก sensor.h ของ esp32 ( search ได้จาก google)
    sensor_t *s = esp_camera_sensor_get();
    switch(camera_status_type){
      case CMR_ST_BRIGHTNESS:
        s->set_brightness(s, slider_h.value()); 
        break;
      case CMR_ST_CONTRAST:
        s->set_contrast(s, slider_h.value()); 
        break;
      case CMR_ST_SATURATION:
        s->set_saturation(s, slider_h.value());
        break;
      case CMR_ST_SHARPNESS:
        s->set_sharpness(s, slider_h.value());
        break;
      case CMR_ST_DENOISE:
        s->set_denoise(s, slider_h.value());
        break;
      case CMR_ST_GAINCEILING:
        s->set_gainceiling(s, (gainceiling_t) slider_h.value());
        break;
      case CMR_ST_QUALITY:
        s->set_quality(s, slider_h.value());
        break;
      case CMR_ST_EFFECT:
        s->set_special_effect(s, slider_h.value());
        break;
    }
  });

  btn_brightness.event_send(EVENT_CLICKED); // จำลองให้ ปุ่ม btn_brightness เสมือน มีการถูก clicked
}

void loop() {
  BlynkGO.update();
}

