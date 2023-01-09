#include <BlynkGOv3.h>

/* ประกาศ BlynkGO Widgets */
GMultiScreen multiscreen;
GSwitch3D sw3d[6];
GLabel    label[6]      = { "ห้องนอน", "ห้องครัว", "ห้องนั่งเล่น", "ห้องรับแขก", "ห้องทานข้าว", "ห้องน้ำ"};

/* สี และ ขา gpio ของ sw3d */
color_t   sw3d_color[6] = { TFT_BLUE, TFT_RED, TFT_GREEN, TFT_ORANGE, TFT_VIOLET, TFT_PINK };
uint8_t   relay_pin[6]  = { 22, 21, 5, 25, 13, 15};


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);


  multiscreen.addScreen(6);                  // ให้ MultiScreen เพิ่ม 6 Screen
  multiscreen.anim_time(300);
  multiscreen.indic().align(ALIGN_BOTTOM,0,22);
  multiscreen.indic().padding_inner(3);  
  multiscreen_btn_ctrl();
  
  for(int i=0; i < 6; i++){
    sw3d[i].parent(multiscreen[i]);         // สร้าง sw3d[i] ให้มี parent คือ MultiScreen[i] (คือ Screen หน้าที่ i )
    sw3d[i].size(60,75);
    sw3d[i].color(sw3d_color[i]);
    sw3d[i].align_center(0,-10);
    sw3d[i].gpio(relay_pin[i], ACTIVE_LOW); // กำหนดขา gpio ที่ต่อกับ relay และ เป็น relay แบบ ACIIVE_LOW / ACTIVE_HIGH
    
    label[i].parent(multiscreen[i]);        // สร้าง label[i] ให้มี parent คือ MultiScreen[i] (คือ Screen หน้าที่ i )
    label[i].font(prasanmit_25,TFT_BLACK);
    label[i].align(sw3d[i], ALIGN_BOTTOM);
  }  
}

void loop() {
  BlynkGO.update();
}

