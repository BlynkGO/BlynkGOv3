#include <BlynkGOv3.h>

GSlider slider_v;
GLabel  lb_slider_v;

GSlider slider_h;
GLabel  lb_slider_h;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  //-----------------------------------
  slider_v.size(20,160);
  slider_v.range(0,100);
  slider_v.align(ALIGN_RIGHT,-30);
  slider_v.style[KNOB].padding(8);
  slider_v.style[INDIC].padding(1);  
  slider_v.onValueChanged([](GWidget*widget){
    /* ปรับสี ปุ่ม knob และ แถบ indic ตามค่าปัจจุบันของ slider_v ตามสีรุ้ง */
    int16_t color_degree = map(slider_v.value(),slider_v.min_value(), slider_v.max_value(), 0,240);
    auto my_color = TFT_COLOR_HSV(color_degree,100,100);
    slider_v.style[KNOB] .color( my_color );
    slider_v.style[INDIC].color( my_color );
    
    lb_slider_v = slider_v.value();
    lb_slider_v.font(prasanmit_25, TFT_BLACK);

    /* ปรับตำแหน่ง lb_slider_v ให้อยู่กลาง ปุ่ม knob */
    int16_t knob_pos_y = map(slider_v.value(),slider_v.min_value(), slider_v.max_value(), slider_v.height(), 0);
    lb_slider_v.c_align(slider_v, ALIGN_IN_TOP, 0, knob_pos_y);
  });
  slider_v = 79;
  slider_v.event_send(EVENT_VALUE_CHANGED);  /* ให้ทำใน onValueChanged(..) ที่ได้กำหนดด้วย หลังกำหนดค่าเอง */

  //-----------------------------------
  slider_h.size(180,20);
  slider_h.range(0,100);
  slider_h.align(ALIGN_LEFT, 50 );
  slider_h.style[KNOB].padding(8);
  slider_h.style[INDIC].padding(1);  
  slider_h.onValueChanged([](GWidget*widget){
    /* ปรับสี ปุ่ม knob และ แถบ indic ตามค่าปัจจุบันของ slider_v ตามสีรุ้ง */
    int16_t color_degree = map(slider_h.value(),slider_h.min_value(), slider_h.max_value(), 0,240);
    auto my_color = TFT_COLOR_HSV(color_degree,100,100);
    slider_h.style[KNOB] .color( my_color );
    slider_h.style[INDIC].color( my_color );
    
    lb_slider_h = slider_h.value();
    lb_slider_h.font(prasanmit_25, TFT_BLACK);

    /* ปรับตำแหน่ง lb_slider_h ให้อยู่กลาง ปุ่ม knob */
    int16_t knob_pos_x = map(slider_h.value(),slider_h.min_value(), slider_h.max_value(), 0, slider_h.width());
    lb_slider_h.c_align(slider_h, ALIGN_IN_LEFT, knob_pos_x, 0);
  });
  slider_h = 94;
  slider_h.event_send(EVENT_VALUE_CHANGED);  /* ให้ทำใน onValueChanged(..) ที่ได้กำหนดด้วย หลังกำหนดค่าเอง */
  //-----------------------------------
}

void loop() {
  BlynkGO.update();
}
