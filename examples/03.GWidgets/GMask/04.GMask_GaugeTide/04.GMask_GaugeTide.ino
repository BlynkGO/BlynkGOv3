#include <BlynkGOv3.h>

FONT_DECLARE(BebasNeueBold_num_60);

GCircle cir_body;                               // วิตเจ็ตวงกลม
GMask   mask_gauge( cir_body);                  // วิตเจ็ต Mask มีแม่คือ cir_bory
GRect   rect_gauge(mask_gauge);                 // วิตเจ็ต สี่เหลี่ยม มีแม่คือ mask_gauge
GLabel  lb_gauge_title("Humidity", cir_body);   // วิตเจ็ตข้อความ มีแม่คือ cir_body
GLabel  lb_value("0%",cir_body);                // วิตเจ็ตข้อความ มีแม่คือ cir_body

GAnimation anim_gauge;

// ออกแบบสร้าง gauge ขึ้นลง
void gauge_create(){
  cir_body.size(100,100);                       // กำหนดขนาดของวงกลม
  cir_body.color(TFT_COLOR_HEX(0xD4D4D4));      // กำหนดสีวงกลม

  int mask_w = cir_body.width()*88/100;         // ขนาดของ mask
  mask_gauge.allocBuffer(mask_w,mask_w);        // จองพื้นที่เม็มด้วยขนาด mask_w  x  mask_w
  mask_gauge.circle(255, mask_gauge.centerpoint(), mask_w/2);     // วาด วงกลม รัศมี 90 ด้วย alpha 255 ลงบน mask
  
  rect_gauge.width(mask_gauge.width());         // กำหนดสี่เหลี่ยมสำหรับเอาไว้บอกระดับ gauge ให้มีความกว้างเท่า mask_gauge
  rect_gauge.color(TFT_ORANGE);
  rect_gauge.align(ALIGN_BOTTOM);               // จัดไว้ชิดในด้านล่างของ mask_gauge
  
  lb_value.font(BebasNeueBold_num_60, TFT_BLACK);
  lb_value.align(ALIGN_CENTER,0,-10);
  
  lb_gauge_title.font(prasanmit_40, TFT_BLACK);
  lb_gauge_title.align(lb_value, ALIGN_BOTTOM,0,-10);

  gauge_value(50);
}

// สำหรับกำหนดค่า ให้ gauge ระหว่าง 0-100
void gauge_value(int8_t value){  // 0-100
  int16_t h = map_f(value, 0,100, 0, mask_gauge.height());
  h = constrain( h, 0,  mask_gauge.height() );
  rect_gauge.height(h);
  rect_gauge.align(ALIGN_BOTTOM);
  lb_value = String(value) + "%";
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  gauge_create();
  gauge_value(48);


  // ใช้ GAnimation ทำจำลอง gauge ขึ้นลง จาก 0->100 ในเวลา 4000ms
  anim_gauge.init( cir_body, [](GWidget*w, int16_t val){
    gauge_value(val);
  }, 0, 100, 4000);
  anim_gauge.playback(true);  // ให้เล่นย้อนกลับค่าด้วย
  anim_gauge.repeat(true);    // ให้เล๋นซ้ำวนไปเรื่อยๆ
  anim_gauge.create();        // เริ่มทำงาน
}

void loop() {
  BlynkGO.update();
}

