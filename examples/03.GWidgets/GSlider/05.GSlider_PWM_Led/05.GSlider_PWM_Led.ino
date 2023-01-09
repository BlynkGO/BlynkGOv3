#include <BlynkGOv3.h>

#define PWM_PIN       5                             // ขา pin ที่จะปรับ PWM
#define PWM_CHANNEL   ((uint8_t)3)                  // channel ที่ใช้ในการปรับ PWM (0-14)

GSlider slider;
GLabel  lb_slider;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  slider.size(400,24);
  slider.range(0,100);
  slider.color(TFT_RED, TFT_ORANGE);
  slider.align(ALIGN_BOTTOM,0,-40);

  // ปรับ style ทำการ customize หน้าตาเพิ่มเติมเอง
  slider.style[KNOB].color(TFT_BLUE, TFT_CYAN);     // สีตัว ปุ่มเลื่อน slider
  slider.style[KNOB].padding_left_right(30);        // ปรับเพิ่มระยะแนวนอน ของปุ่มเลื่อน slider
  slider.style[KNOB].padding_top_bottom(15);        // ปรับเพิ่มระยะแนวตั้ง ของปุ่มเลื่อน slider  
  slider.style[KNOB].corner_radius(20);             // รัศมีมุมโค้งของ ปุ่มเลื่อน slider
  slider.style[INDIC].padding_top_bottom(5);        // ระยะช่องไฟบนล่าง ระหว่าง พื้นหลัง slider กับ indic
  slider.style[BG].opa(30);                         // ปรับความโปร่งใสของ พื้นหลัง slider
  
  lb_slider.align(slider, ALIGN_TOP,0,-20);
  
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);
  ledcSetup(PWM_CHANNEL, 12000, 8);
  slider.onValueChanged([](GWidget*widget){
    lb_slider = slider.value();
    
    static GTimer timer_pwm;
    timer_pwm.setOnce(100,[](){                     // ตั้งเวลาหน่วงในการปรับ PWM จะได้ไม่ต้องมีการปรับ PWM ถี่ ทำให้ระบบสัมผัสมีเวลามาทำงานได้มากขึ้น
      ledcWrite(PWM_CHANNEL, constrain(map(slider.value(),0,100,255,0),0,255));
    });
  });
  slider = 50;
  slider.event_send(EVENT_VALUE_CHANGED);           // สั่งให้ slider ทำงานภายใน onValueChanged(..) ที่กำหนดไป
}

void loop() {
  BlynkGO.update();
}

