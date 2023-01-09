#include <BlynkGOv3.h>

GRect   mywidget;
  GArc    arc[5];
    color_t arc_color[] = { TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN, TFT_BLUE };
    int     arc_step[]  = {  10, 20, 40, 70, 100 }; // ระยะที่จะเป็นโซนสำหรับ arc ช่วงย่อต่างๆ

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  mywidget.size(240,240);
  mywidget.color(TFT_DARKGRAY);

  int angle_offset = 150;                         // มุมสำหรับเป็น offset เริ่มต้น วัดจากแกน x+ ตามเข็ม
  int angle_total  = (180-angle_offset)*2 + 180;  // มุมทั้งหมด สำหรับ 5 arcs รวมกัน
  float angle_start = 0;
  for(int i=0; i< 5; i++){
    arc[i].parent(mywidget);                      // ให้เป็น layer ลูกของ mywidget
    arc[i].radius( mywidget.width()/2);           // กำหนดรัศมี
    arc[i].thickness(15);                         // กำหนดความหนา
    arc[i].color( arc_color[i]);                  // กำหนดสี
    arc[i].direction(ARC_CLOCKWISE);              // กำหนดมุมให้วัดแบบตามเข็ม
    arc[i].angle_offset(angle_offset);            // มุม offset เริ่มต้น
    arc[i].clickable(false);                      // ไม่ต้องรับสัญญาณสัมผัส
    arc[i].align(ALIGN_CENTER,0,30);              // กำหนดตำแหน่งให้เลื่อนจากตรงกลาง mywidget ลงไป 30 pixels

    float angle_end = map_f( arc_step[i], 0, 100, 0, angle_total );  // เปลี่ยน ระยะ โซน ให้เป็นมุม
    
    arc[i].angle( angle_start, angle_end );       // กำหนดมุม arc เริ่มต้นและ มุมสิ้นสุด ของแต่ละ arc
    angle_start = angle_end;                      // มุมสิ้นสุด จะกลายเป็น มุมเริ่มต้น สำหรับ arc ถัดๆไป
  }

  // หากจะย้ายวิตเจ็ตที่ออกแบบเองนี้ไปไหนให้จัด align(...) ให้ mywidget
  // mywidget.align(ALIGN_TOP,0,20);
}

void loop() {
  BlynkGO.update();
}


