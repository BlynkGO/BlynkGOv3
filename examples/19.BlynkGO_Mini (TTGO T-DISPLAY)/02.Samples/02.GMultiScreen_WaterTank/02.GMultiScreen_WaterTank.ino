#include <BlynkGOv3.h>

typedef struct _smartfarm {
  GLabel label;
  GGauge gauge_temp;
  GGauge gauge_humid;
} smartfarm_t;

GMultiScreen multiscreen;
smartfarm_t smartfarm[4];
String name[] = { "ผักกาดแก้ว", "เรดโอ๊ค",  "กรีนโอ๊ค", "ร็อคเก็ต"};

extern GRect watertank_water;
void WaterTank_create(GWidget&parent=GScreen);
void WaterTank_WaterHeight(float h );

void setup() {
  Serial.begin(115200); Serial.println();     // เริ่มใช้งาน Serial Monitor
  BlynkGO.begin();                            // เริ่มใช้งาน BlynkGO
  BlynkGO.fillScreen(TFT_BLACK);              // เติมสีหน้าจอเป็นสี ดำ

  multiscreen.addScreen(5);
  multiscreen.indic().align(ALIGN_BOTTOM_RIGHT,-5,15);
  multiscreen.indic().padding_inner(6);

  for(int i=0; i < 4; i++){
    smartfarm[i].label = name[i];
    smartfarm[i].label.parent(multiscreen[i]);
    smartfarm[i].label.font(prasanmit_20, TFT_WHITE);
    
    
    smartfarm[i].gauge_temp.size(120,120); 
    smartfarm[i].gauge_temp.parent(multiscreen[i]);
    smartfarm[i].gauge_temp.title("อุณหภูมิ (°C)");
    smartfarm[i].gauge_temp.color( TFT_COLOR_HEX(0xF45757) );
    smartfarm[i].gauge_temp.decimal(1);
    smartfarm[i].gauge_temp.align(ALIGN_LEFT);
    smartfarm[i].gauge_temp = random(1000)/10.0;

    smartfarm[i].gauge_humid.size(120,120); 
    smartfarm[i].gauge_humid.parent(multiscreen[i]);
    smartfarm[i].gauge_humid.title("ความชื้น (%)");
    smartfarm[i].gauge_humid.color( TFT_COLOR_HEX(0x0455BF) );
    smartfarm[i].gauge_humid.decimal(1);
    smartfarm[i].gauge_humid.align(ALIGN_RIGHT);
    smartfarm[i].gauge_humid = random(1000)/10.0;


    smartfarm[i].label.align(smartfarm[i].gauge_temp, ALIGN_BOTTOM,0,-20);
  }

  WaterTank_create(*multiscreen[4]);
  

  // เมื่อ BTN1 มีการ click ให้ทำการเลื่อนหน้าจอ multiscreen ถอยย้อนหน้าก่อน
  BTN1.onClicked([](ButtonISR* btn) {
    Serial.println("btn1 click\n");
    if( multiscreen.current_screen_id() > 0)
      multiscreen.current_screen( multiscreen.current_screen_id()-1);
  });

  // เมื่อ BTN2 มีการ click ให้ทำการเลื่อนหน้าจอ multiscreen ไปหน้าถัดไป
  BTN2.onClicked([](ButtonISR* btn) {
    Serial.println("btn2 click\n");
    if( multiscreen.current_screen_id() < multiscreen.num()-1)
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
  });
}

int16_t h;
int8_t step=1;

void loop() {
  BlynkGO.update();                           // ให้ BlynkGO ทำการ update สถานะต่างๆ
 
  WaterTank_WaterHeight(h);
  
  h += step;
  if( h > 100){ h = 100; step = -step; }
  if( h < 0 ) { h = 0;  step = -step; }
}
