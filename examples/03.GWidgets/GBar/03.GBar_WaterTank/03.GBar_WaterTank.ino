#include <BlynkGOv3.h>

GBar   water_tank;              // ประกาศวิตเจ็ต GBar
GLabel lb_water;
GLabel lb_unit("Litres");

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  water_tank.size(180, 240);                      // กำหนดขนาด
  water_tank.range(0,2000);                       // กำหนด range ให้ water_tank
  water_tank.color(TFT_BLUE);                     // กำหนดสี
  water_tank.align(ALIGN_LEFT, 50);               // จัดวางตำแหน่ง ชิดซ้ายจอ โดยขยับเลื่อนไปทางขวา 50 pixels

  water_tank.style[BG].corner_radius(20);         // customize ให้ พื้นหลัง  มี รัศมีมุมโค้ง 20 pixels
  water_tank.style[INDIC].corner_radius(20);      // customize ให้ indicator มี รัศมีมุมโค้ง 20 pixels

  water_tank.onValueChanged([](GWidget*widget){   // หาก water_tank มีการกำหนดเปลี่ยนค่า
    lb_water = water_tank.value();                // lb_water จะถูกกำหนดค่าตามด้วย
    lb_unit.align(lb_water, ALIGN_RIGHT,10, 5);   // จัดวาง lb_unit ไว้ด้านขวาของ lb_water โดย ปรับเลื่อนไปทางขวา 10 px ลงล่าง 5 px
  });
  lb_water.font(BebasNeueBold_num_45);            // กำหนด font
  lb_water.align(water_tank, ALIGN_RIGHT, 50);    // จัดวางตำแหน่ง ไว้ด้านขวา ของ water_tank โดย ปรับเลื่อนไปทางขวา 50 px
  
  water_tank = 1548;                              // กำหนดค่าให้ water_tank

}

void loop(){
  BlynkGO.update();
}



