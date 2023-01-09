#include <BlynkGOv3.h>

#define KEYLOCK_NUM             4           // จำหนวนหลักของ keylock
#define KEYLOCK_PASSWORD        4321        // ค่า keylock ปัจจุบัน สำหรับปลดล็อคหน้าจอ
#define KEYLOCK_SAVER_INTERVAL  30000       // 30วินาที หากหน้า multiscreen ไม่มีการกดอะไร ให้กลับไปหน้า keylock

FONT_DECLARE(BebasNeueBold_num_40);

//------------------------------
GMultiScreen multiscreen;
GGauge    gauge;
GSwitch3D sw3d[3];
GLabel    label[3]      = { "ห้องนอน", "ห้องครัว", "ห้องนั่งเล่น" };
color_t   sw3d_color[3] = { TFT_BLUE, TFT_RED,TFT_GREEN };
uint8_t   relay_pin[3]  = { 22, 21, 5 };

//------------------------------
GRect   app_keylock;
GContainer cont_keylock (app_keylock);
GLabel  lb_key[KEYLOCK_NUM];
GLabel  lb_ok(app_keylock);
GRect   msg_box(app_keylock);
GLabel  lb_msg(msg_box);

uint8_t key[KEYLOCK_NUM];
int8_t  key_id = 0;
bool    keylock_edit_mode = false;
GTimer  timer_keylock;
GTimer  timer_msg_box;

//------------------------------

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  multiscreen_design();
  app_keylock_design();

  // ตัวควบคุมปุ่มกด สำหรับ app_keylock
  app_keylock_btn_ctrl();   
}

void loop() {
  BlynkGO.update();
}

void multiscreen_design(){
  multiscreen.addScreen(4);                  // ให้ MultiScreen เพิ่ม 6 Screen
  multiscreen.anim_time(300);
  multiscreen.indic().align(ALIGN_BOTTOM,0,22);
  multiscreen.indic().padding_inner(3);  
  //multiscreen_btn_ctrl();

  gauge.parent(multiscreen[0]);
  gauge.size(120,120);
  gauge.title("อุณหภูมิ");
  gauge.color(TFT_RED);
  gauge.decimal(1);
  gauge = 24.5;

  for(int i=0; i < 3; i++){
    sw3d[i].parent(multiscreen[i+1]);         // สร้าง sw3d[i] ให้มี parent คือ MultiScreen[i] (คือ Screen หน้าที่ i )
    sw3d[i].size(60,75);
    sw3d[i].color(sw3d_color[i]);
    sw3d[i].align_center(0,-10);
    sw3d[i].gpio(relay_pin[i], ACTIVE_LOW); // กำหนดขา gpio ที่ต่อกับ relay และ เป็น relay แบบ ACIIVE_LOW / ACTIVE_HIGH
    
    label[i].parent(multiscreen[i+1]);        // สร้าง label[i] ให้มี parent คือ MultiScreen[i] (คือ Screen หน้าที่ i )
    label[i].font(prasanmit_25,TFT_BLACK);
    label[i].align(sw3d[i], ALIGN_BOTTOM);
  } 
}


void app_keylock_design(){
  app_keylock.size(GScreen);
  app_keylock.color(TFT_BLACK);

  app_keylock_random_key();

  cont_keylock.fit(FIT_TIGHT, FIT_TIGHT);
  cont_keylock.layout(LAYOUT_ROW_M);
  cont_keylock.opa(0);
  cont_keylock.padding_inner(35);
  cont_keylock.align(ALIGN_CENTER,0, -15);
  
  for(int i=0; i < KEYLOCK_NUM; i++){
    lb_key[i].parent(cont_keylock);
    lb_key[i].font(BebasNeueBold_num_40, TFT_WHITE);
    lb_key[i] = String("*");
    lb_key[i].body_draw( true);
    lb_key[i].body_opa(0);
    lb_key[i].border(3  , TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));
    lb_key[i].border_part(BORDER_FULL);
  }

  lb_ok = " ตกลง ";
  lb_ok.font(prasanmit_20,TFT_WHITE);
  lb_ok.align( lb_key[KEYLOCK_NUM-1], ALIGN_BOTTOM_RIGHT,0, 20);
  lb_ok.body_draw(true);
  lb_ok.body_opa(0);
  lb_ok.corner_radius(15);
  lb_ok.border(3  , TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE,220));

  msg_box.size(GScreen.width()*.9 ,GScreen.height()*.9);
  msg_box.color(TFT_WHITE);
  msg_box.border(5, TFT_DARKGRAY);
  msg_box.corner_radius(20);
  msg_box.font(prasanmit_30, TFT_BLACK);
  msg_box.hidden(true);
  lb_msg = "Wrong Password!!"; 
}
