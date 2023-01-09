#include <BlynkGOv3.h>

/* ประกาศ BlynkGO Widgets */
GMultiScreen MultiScreen;
GSwitch3D sw3d[6];
GLabel    label[6]      = { "ห้องนอน", "ห้องครัว", "ห้องนั่งเล่น", "ห้องรับแขก", "ห้องทานข้าว", "ห้องน้ำ"};

/* สี และ ขา gpio ของ sw3d */
color_t   sw3d_color[6] = { TFT_BLUE, TFT_RED, TFT_GREEN, TFT_ORANGE, TFT_VIOLET, TFT_PINK };

#if defined(BLYNKGO_BOARD_V1_3)
uint8_t   relay_pin[6]  = { 5, 21, 23, 25, 22, 32};
#elif defined(BLYNKGO_ENTERPRISE)
uint8_t   relay_pin[6]  = { 13, 12,14, 26, 25, 4};
#else
uint8_t   relay_pin[6]  = { 22, 21, 5, 25, 13, 15};
#endif

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  MultiScreen.addScreen(6);                 // ให้ MultiScreen เพิ่ม 6 Screen

  for(int i=0; i < 6; i++){
    sw3d[i].parent(MultiScreen[i]);         // สร้าง sw3d[i] ให้มี parent คือ MultiScreen[i] (คือ Screen หน้าที่ i )
    sw3d[i].color(sw3d_color[i]);
    sw3d[i].gpio(relay_pin[i], ACTIVE_LOW); // กำหนดขา gpio ที่ต่อกับ relay และ เป็น relay แบบ ACIIVE_LOW / ACTIVE_HIGH
    
    label[i].parent(MultiScreen[i]);        // สร้าง label[i] ให้มี parent คือ MultiScreen[i] (คือ Screen หน้าที่ i )
    label[i].align(sw3d[i], ALIGN_BOTTOM);
  }
}

void loop() {
  BlynkGO.update();
}

