/***************************************************************
 * ก่อนใช้งาน ร่วมกับแอพ Blynk
 *  1. ให้กำหนด เปิด การใช้งาน Blynk ให้ BlynkGO ได้ที่
 *     ไฟล์ config/blynkgo_config.h
 *     โดยกำหนดเป็น
 *       #define BLYNKGO_USE_WIFI           1
 *       #define BLYNKGO_USE_NTP            1
 *       #define BLYNKGO_USE_BLYNK          1
 *
 **************************************************************/


#include <BlynkGOv3.h>

FONT_DECLARE(LedDotMatrix_70);
IMG_DECLARE(blynkgo_logo);


#define SSID          "-----------------"
#define PASSWORD      "-----------------"
#define BLYNK_AUTH    "-----------------"
#define BLYNK_HOST    "blynk.jpnet.co.th"   // Blynk Server แบบกำหนดเอง  ( ที่ blynk.jpnet.co.th ให้ energy 10ล้าน )
#define BLYNK_PORT    8080                  // Blynk Port   แบบกำหนดเอง


typedef struct _smartfarm {
  GLabel label;
  GGauge gauge_temp;
  GGauge gauge_humid;
} smartfarm_t;

GMultiScreen multiscreen;
smartfarm_t smartfarm[2];//[3];//[4];
String name[] = { "ผักกาดแก้ว", "เรดโอ๊ค"}; //,  "กรีนโอ๊ค"};//, "ร็อคเก็ต"};

extern GRect watertank_water;
void WaterTank_create(GWidget&parent=GScreen);
void WaterTank_WaterHeight(float h );

void PowerMeter_create(GWidget&parent=GScreen);
void SmartWatch_create(GWidget& parent=GScreen);
void SmartWatch_setTime(uint8_t h, uint8_t m, uint8_t s) ;
void DoraemonAnime_create(GWidget&parent=GScreen);

GWiFiManager wifi_manager;
GBlynkManager blynk_manager;
GLabel       lb_wifi_info("Disconnected");
GLabel       lb_blynk_info("Disconnected");

GLabel  lb_blynkgo_show("WELCOME TO BLYNKGO");
GAnimation anim_blynkgo_show;
GImage Blynkgo_LOGO(blynkgo_logo);

void setup() {
  Serial.begin(115200); Serial.println();     // เริ่มใช้งาน Serial Monitor
  BlynkGO.begin();                            // เริ่มใช้งาน BlynkGO
  BlynkGO.fillScreen(TFT_BLACK);              // เติมสีหน้าจอเป็นสี ดำ
  // BlynkGO.configBlynk(BLYNK_AUTH);                       // กำหนด config ให้แอพ Blynk แบบใช้ Server มาตรฐาน
  BlynkGO.configBlynk(BLYNK_AUTH, BLYNK_HOST, BLYNK_PORT);  // กำหนด config ให้แอพ Blynk แบบใช้ Server อื่นๆ

  WiFi.begin(SSID, PASSWORD);

  multiscreen.addScreen(8);
  multiscreen.anim_time(500);
  multiscreen.indic().align(ALIGN_BOTTOM_RIGHT,-2,15);
  multiscreen.indic().padding_inner(2);

  // multiscreen[0]
  Blynkgo_LOGO    .parent(multiscreen[0]);
  lb_blynkgo_show .parent(multiscreen[0]);
  lb_blynkgo_show .font(LedDotMatrix_70);
  lb_blynkgo_show .align(Blynkgo_LOGO, ALIGN_RIGHT,20);
  lb_blynkgo_show .auto_realign(true);

  // สร้าง Animation ภาพเคลื่อนไหวพร้อมข้อความเคลื่อนไหว
  anim_blynkgo_show.init(Blynkgo_LOGO, [](GWidget*w, int16_t v) {
    BTN1.loop(); BTN2.loop();
    Blynkgo_LOGO.posX(v);
    lb_blynkgo_show.align(Blynkgo_LOGO, ALIGN_RIGHT,20);
  }, GScreen.width(), -lb_blynkgo_show.width() -Blynkgo_LOGO.width()-20 , 25000);
  anim_blynkgo_show.repeat(true, 500);
  anim_blynkgo_show.create();

  // multiscreen[1]  
  wifi_manager  .parent(multiscreen[1]);  wifi_manager.c_align(ALIGN_LEFT, GScreen.width()/4, -20);    wifi_manager.font(prasanmit_40);
  blynk_manager .parent(multiscreen[1]);  blynk_manager.c_align(ALIGN_RIGHT, -GScreen.width()/4, -20);
  lb_wifi_info  .parent(multiscreen[1]);  lb_wifi_info.font(prasanmit_20);        lb_wifi_info.align(wifi_manager,ALIGN_BOTTOM,0,-5);
  lb_blynk_info .parent(multiscreen[1]);  lb_blynk_info.linkStyle(lb_wifi_info);  lb_blynk_info.align( ALIGN_TOP, GScreen.width()/4, lb_wifi_info.topY());

  
  // multiscreen[2]
  SmartWatch_create(*multiscreen[2]);

  // multiscreen[3] - multiscreen[4]  
  for(int i=0; i < 2; i++){
    smartfarm[i].label      .parent(multiscreen[i+3]);
    smartfarm[i].gauge_humid.parent(multiscreen[i+3]);
    smartfarm[i].gauge_temp .parent(multiscreen[i+3]);

    smartfarm[i].label = name[i];
    smartfarm[i].label.font(prasanmit_20, TFT_WHITE);
        
    smartfarm[i].gauge_temp.size(120,120); 
    smartfarm[i].gauge_temp.title("อุณหภูมิ (°C)");
    smartfarm[i].gauge_temp.color( TFT_COLOR_HEX(0xF45757) );
    smartfarm[i].gauge_temp.decimal(1);
    smartfarm[i].gauge_temp.align(ALIGN_LEFT);
    smartfarm[i].gauge_temp = random(1000)/10.0;

    smartfarm[i].gauge_humid.size(120,120); 

    smartfarm[i].gauge_humid.title("ความชื้น (%)");
    smartfarm[i].gauge_humid.color( TFT_COLOR_HEX(0x0455BF) );
    smartfarm[i].gauge_humid.decimal(1);
    smartfarm[i].gauge_humid.align(ALIGN_RIGHT);
    smartfarm[i].gauge_humid = random(1000)/10.0;

    smartfarm[i].label.align(smartfarm[i].gauge_temp, ALIGN_BOTTOM,0,-20);
  }

  DoraemonAnime_create(*multiscreen[5]);

  // multiscreen[6]
  WaterTank_create(*multiscreen[6]);

  // multiscreen[7]
  PowerMeter_create(*multiscreen[7]);
  
  // ควบคุมปุ่มกด เมื่อมีการกด Pressed เข้ามาให้เลื่อนหน้า multiscreen ถอยกลับ
  // เมื่อ BTN1 มีการ pressed ให้ทำการเลื่อนหน้าจอ multiscreen ถอยย้อนหน้าก่อน
  BTN1.onPressed([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() > 0) {
      multiscreen.current_screen( multiscreen.current_screen_id()-1);
    }else{
      multiscreen.current_screen(multiscreen.num()-1, false);
    }
  });

  // ควบคุมปุ่มกด เมื่อมีการกด Pressed เข้ามาให้เลื่อนหน้า multiscreen ถัดไป
  // เมื่อ BTN2 มีการ pressed ให้ทำการเลื่อนหน้าจอ multiscreen ไปหน้าถัดไป
  BTN2.onPressed([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() < multiscreen.num()-1) {
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
    }else{
      multiscreen.current_screen( 0, false);
    }
  });
}

int16_t h;
int8_t step=1;
bool ntp_synced = false;
void loop() {
  BlynkGO.update();       // ให้ BlynkGO ทำการ update สถานะต่างๆ
 
  // กำหนดค่า ให้ความสูงถังน้ำ
  WaterTank_WaterHeight(h);

  // จำลองค่าให้ แทงค์น้ำ ขึ้นลง 
  h += step;
  if( h > 100){ h = 100; step = -step; }
  if( h < 0 ) { h = 0;  step = -step; }

  // หากเวลายัง sync กับ NTP ไม่ได้ ให้ จำลองนาฬิกา เริ่มตั้งแต่เวลาที่กำหนด 
  if(!ntp_synced){
    static int8_t h = 10;
    static int8_t m = 12;
    static int8_t s = 24;

    static uint32_t sec_of_day =  ((h*60 + m)*60)+s;
    static uint32_t smartwatch_simulation_timer;
    if(smartwatch_simulation_timer <= millis()){
      smartwatch_simulation_timer = millis() + 1000;      

      sec_of_day = (sec_of_day+1)% 86400;
      Serial.println(sec_of_day);

      s = sec_of_day % 60;
      m = int((sec_of_day - s)/60) % 60;
      h = int((sec_of_day - s - 60*m)/3600);
      
      SmartWatch_setTime( h, m, s);      
    }    
  }
}

WIFI_CONNECTED(){
  lb_wifi_info = WiFi.localIP().toString();
}

WIFI_DISCONNECTED(){
  lb_wifi_info = "Disconnected";
}

BLYNK_CONNECTED(){
  lb_blynk_info = "Connected";
}

BLYNK_DISCONNECTED(){
  lb_blynk_info = "Disconnected";
}

NTP_SYNCED(){
  ntp_synced = true;
  static GTimer timer;
  timer.setInterval(1000L, [](){
    static struct tm tm;
    if( !BlynkGO.getLocalTime(tm)) return;
    SmartWatch_setTime(tm.tm_hour, tm.tm_min, tm.tm_sec);
  });
}

