#include <BlynkGOv3.h>

GThermo thermo(0,100);    /* ประกาศตัวแปรพร้อมกำหนดช่วง */

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  thermo = 43;              /* กำหนดค่าให้ thermo */
  thermo.color(TFT_ORANGE);
  
  thermo.onValueChanged([](GWidget* widget){/* หากค่ามีการเปลี่ยนแปลง */
    GThermo* pThermo = (GThermo*) widget;   /* cast ให้กลายเป็น pointer ของ GThermo */
    /* สีเปลี่ยนตาม ค่า ของ thermo */
    float temp = pThermo->value();
    if(temp < 20)       pThermo->color(TFT_BLUE);
    else if(temp < 40)  pThermo->color(TFT_DARKGREEN);
    else if(temp < 60)  pThermo->color(TFT_ORANGE);
    else                pThermo->color(TFT_RED);
  });

}

int8_t temp = 0;
int8_t step = 1;

void loop() {
  BlynkGO.update();

  thermo = temp;
  temp += step;
  if(temp > 100) { temp = 100; step = -step; }
  if(temp < 0 )  { temp = 0;   step = -step; }
}

