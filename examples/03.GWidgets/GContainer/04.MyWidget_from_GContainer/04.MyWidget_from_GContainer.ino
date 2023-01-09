/********************************************************
 * ตัวอย่างการสร้างวิตเจ็ตส่วนตัว MyWidget
 * สำหรับ Sensor 3 ตัว เพื่อใช้แสดงข้อมูล  อุณหภูมิ / ความชื้น
 * โดยประยุกต์สร้างจาก GContainer สำหรับรวบรวม วิตเจ็ต GLabel 2 ชุด
 * ด้วยการสร้างเป็น struct 
 * แล้วสร้างเป็น array ขึ้นมาเป็น my_widgets 3 ชุด
 * 
 ********************************************************/

#include <BlynkGOv3.h>

typedef struct {
  GContainer cont;
  GLabel     temp;
  GLabel     humid;
} MyWidget_t;

MyWidget_t my_widgets[3];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  for(int i=0; i< 3; i++){
    my_widgets[i].cont.fit(FIT_TIGHT);        // ให้ขนาดปรับให้พอดีกับลูกๆ
    my_widgets[i].cont.layout(LAYOUT_COL_L);  // ให้วิตเจ็ตลูกๆ จัดเรียงแบบไล่แนวตั้งซ้ายไล่ลงมา
    my_widgets[i].cont.padding(10);           // จัดช่องไฟทุกอย่างเป็น 10 pixel
    my_widgets[i].cont.padding_inner(-10);    // จัดช่องไฟระหว่างลูกๆเป็น -10 pixel
    my_widgets[i].cont.color(TFT_SILVER);
    my_widgets[i].cont.border(2, TFT_BLUE);
    my_widgets[i].cont.corner_radius(15);
    my_widgets[i].cont.font(prasanmit_25, TFT_BLACK); // ฟอนต์ของลูกๆเป็น prasanmit_25 สีดำ

    // กำหนดตำแหน่งของ cont ของ my_widgets[i]
    if(i==0)  my_widgets[i].cont.position(30,10);
    else      my_widgets[i].cont.align(my_widgets[i-1].cont, ALIGN_BOTTOM_LEFT,0,5);

    my_widgets[i].temp.parent(my_widgets[i].cont);
    my_widgets[i].humid.parent(my_widgets[i].cont);

    my_widgets[i].temp  = "Temp  : " + String(random(2000,3000)/100.0);
    my_widgets[i].humid = "Humid : " + String(random(4000,9000)/100.0);
  }
}

void loop() {
  BlynkGO.update();
}


