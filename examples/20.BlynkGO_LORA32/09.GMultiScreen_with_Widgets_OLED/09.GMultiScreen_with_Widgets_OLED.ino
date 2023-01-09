/**********************************************************************
 *
 *  ก่อนใช้งาน
 *  เข้าไปที่ BlynkGOv3/src/config/blynkgo_config.h
 *    เปลี่ยนบอร์ดเป็น
 *       #define  BLYNKO_LORA32
 *
 *    และที่ ArduinoIDE เลือกบอร์ดเป็น BlynkGO Lora32
 *
 **********************************************************************/

/**********************************************************************
 * ตัวอย่างนี้ มีการใช้ WiFi และ NTP ในการ compile
 * ให้กำหนด เปิด การ compile ด้วย WiFi และ NTP ได้ที่
 * ไฟล์ BlynkGOv3/src/config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 *  จากนั้น ให้ upload ไฟล์ GIF ซึ่งอยู่ในโฟลเดอร์ data ของโปรเจค (กด Ctrl+K จะเห็นโฟลเดอร์ data )
 *  ขึืนสู่พื้นที่ SPIFFS ของ ESP32 ผ่านทางเมนู Tools > ESP32 Sketch Data Upload
 *
 * วิธีใช้งาน ให้กดปุ่มล่าง เพื่อเลื่อนหน้า GMultiScreen ไปหน้าถัดๆไป
 * ช่วยให้เพิ่มพื้นที่ได้หลาย screen ในการแสดงผลได้ตามต้องการ
 *
 **********************************************************************/

#include <BlynkGOv3.h>

#define SSID            "-------------"
#define PASSWORD        "-------------"
#define SCREEN_NUM      7


FONT_DECLARE(prasanmit_15);
FONT_DECLARE(LedDotMatrix_25);
IMG_DECLARE(img_powermeter);

GMultiScreen multiscreen;
GLabel label[SCREEN_NUM];

GLabel lb_clock;
GLabel  lb_wifi_connected(SYMBOL_WIFI);

GCircle cir_clock;
  GLabel lb_dial_tick[12];
  GLine  line_hour_pin(cir_clock);
  GLine  line_minute_pin(cir_clock);
  GLine  line_second_pin(cir_clock);

GBar  bar;

GCircle gauge_cir1;
  GCircle gauge_cir2(gauge_cir1);
  GArc    gauge_arc(gauge_cir1);
  GLabel  lb_gauge_val(gauge_cir1);

GQRCode qrcode;

GRect tank;
  GRect tank_water(tank);
GLabel lb_water;

GImage powermeter(img_powermeter);
GLabel lb_power;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  WiFi.begin(SSID, PASSWORD);

  multiscreen.addScreen(SCREEN_NUM);
  multiscreen.anim_time(300);
  multiscreen.indic().align(ALIGN_BOTTOM,0,15);
  multiscreen.indic().padding_inner(6);
  multiscreen.indic().hidden(true);

  /* เปลี่ยน font ให้ จุด indic เพื่อให้มีขนาดพอดีจับ จอ oled*/
  gmultiscr_ext_t* ext = (gmultiscr_ext_t*) multiscreen.ext_attr();
  for(int i=0; i <  SCREEN_NUM; i++)  ext->lb_indic[i]->font(prasanmit_20);

  
  for(int i=0; i < SCREEN_NUM; i++){
    label[i].parent(multiscreen[i]);
    label[i] = String("S") + String(i+1);

    label[i].font(prasanmit_15);
    label[i].align(ALIGN_TOP_RIGHT,-2,-4);
  }

  lb_clock.parent( multiscreen[0]);
  lb_clock.font(LedDotMatrix_25);

  cir_clock.parent( multiscreen[1]);
  cir_clock.radius(32);
  cir_clock.opa(0);
  cir_clock.border(1,TFT_WHITE);

    Point c = cir_clock.centerpoint();   // จุดศูนย์กลางวงกลม เทียบภายใน cir_clock
    for(int i=0; i < 12; i++){
      //จุด p สร้างจาก แขน 23pixels หมุนไปมุม i*30 โดยเริ่มต้นที่มุม 270 รอบจุด c นับมุมแบบตามเข็ม
      Point p = Point(24, i*30, 270, c, CLOCKWISE);  
      lb_dial_tick[i].parent(cir_clock);
      lb_dial_tick[i].center(p);               // กำหนดตำแหน่ง
      lb_dial_tick[i].font(prasanmit_15);
      lb_dial_tick[i] = String((i>0)? i : 12); // ใส่ข้อความให้ วิตเจ็ตข้อความ
    }

    line_hour_pin  .thickness(5); line_hour_pin  .color(TFT_WHITE); line_hour_pin  .rounded(true);
    line_minute_pin.thickness(3); line_minute_pin.color(TFT_WHITE); line_minute_pin.rounded(true);
    line_second_pin.thickness(1); line_second_pin.color(TFT_WHITE);


  bar.parent( multiscreen[2]);
  bar.size(80,12);
  bar.range(0,200);
  bar = 150;

  bar.style[BG].color(TFT_BLACK);
  bar.style[BG].border(1, TFT_WHITE);
  bar.style[BG].border_opa(255);
  bar.style[BG].corner_radius(6);
  bar.style[INDIC].corner_radius(6);
  bar.style[INDIC].padding(0);
  bar.style[INDIC].color(TFT_WHITE);


  gauge_cir1.parent(multiscreen[3]);
  gauge_cir1.radius(30);
  gauge_cir1.opa(0);
  gauge_cir1.border(1, TFT_WHITE);

    uint8_t thickness = 10;
    
    gauge_cir2.radius(gauge_cir1.radius()-thickness);
    gauge_cir2.opa(0);
    gauge_cir2.border(1, TFT_WHITE);
  
    gauge_arc.radius(gauge_cir1.radius());
    gauge_arc.thickness(thickness);
    gauge_arc.color(TFT_WHITE);
    gauge_arc.direction(ARC_CLOCKWISE);
    gauge_arc.angle_offset(270);              // กำหนดมุม offset จากแกน x + 

    uint32_t val = random(100);
    gauge_arc.angle( 0, map(val,0,100,0,360));

    lb_gauge_val = String( val);

    static GTimer gauge_timer;
    gauge_timer.setInterval(80,[](){
      static int8_t val=0;
      static int8_t step=1;
      gauge_arc.angle( 0, map(val,0,100,0,360));  
      lb_gauge_val = String( val);

      val += step;
      if(val >100) { val = 100; step = -1; }
      else if(val <0) { val = 0; step = 1; }
    });
    
  qrcode.parent(multiscreen[4]);
  qrcode.size(64,64);
  qrcode.version(6);
  qrcode.encode("สวัสดี BlynkGO");
  
  tank.parent(multiscreen[5]);
  tank.size(40,55);
  tank.opa(0);
  tank.border(1, TFT_WHITE);
  tank.corner_radius(8);
  tank.corner_mask(true);
  tank.align(ALIGN_LEFT,24);
    tank_water.size(tank.width()-2, tank.height()-2);
    tank_water.color(TFT_WHITE);
    tank_water.align(ALIGN_BOTTOM,0,-1);

    lb_water.parent(tank.parent());
    lb_water.font(prasanmit_20);
    lb_water.text_line_space(-10);
    lb_water.text_align(TEXT_ALIGN_CENTER);
    lb_water.align(tank, ALIGN_RIGHT,4);

    int8_t water_level = 80;
    tank_water.height( map(water_level,0,100, 0, tank.height()-2 ));

    static GTimer tank_timer;
    tank_timer.setInterval(100,[](){
      static int8_t water_level;
      static int8_t step = 1;
      
      tank_water.height( map(water_level,0,100, 0, tank.height()-2 ));

      lb_water = StringX::printf("น้ำประปา\n %d L",  map(water_level,0,100, 0,2000));

      water_level += step;
      if( water_level > 100) { water_level = 100; step = -1; }
      else if( water_level <0) {water_level=0; step = 1; }
   }); 

  powermeter.parent(multiscreen[6]);
  powermeter.align(ALIGN_LEFT,10);
    lb_power.parent(powermeter.parent());
    lb_power.align(powermeter, ALIGN_RIGHT,-5);
    lb_power.text_align(TEXT_ALIGN_CENTER);
    lb_power.linkStyle(lb_water);
    lb_power = "Power\n232kWh";
    
  lb_wifi_connected.font(prasanmit_20);
  lb_wifi_connected.align(ALIGN_TOP_LEFT,0,-8);
  lb_wifi_connected.hidden(true);

  /* ปุ่มควบคุมเพื่อทำการเลื่อนหน้า multiscreen ไปหน้าถัดๆไป */
  BTN.onPressed([](ButtonISR* btn) {  
    if( multiscreen.current_screen_id() < multiscreen.num()-1)
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
    else
      multiscreen.current_screen( 0 , false);
  });
}

void loop() {
  BlynkGO.update();  
}


WIFI_CONNECTED(){
  lb_wifi_connected.hidden(false);
}

WIFI_DISCONNECTED(){
  lb_wifi_connected.hidden(true);
}

NTP_SYNCED(){
  static GTimer timer;
  // ตั้งเวลาทุกๆ 1 วินาที ให้ ดึงค่า hour(), minute(), second() มาแสดงบน lb_clock และ ลากเส้นทำเข็มนาฬิกา
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());

    Point c = cir_clock.centerpoint();    // จุดศูนย์กลางวงกลม เทียบภายใน cir_clock
    line_hour_pin   .setPoints(c, Point(12, (hour()  %12)*30, 270, c, CLOCKWISE)); 
    line_minute_pin .setPoints(c, Point(22, (minute()%60)*6 , 270, c, CLOCKWISE));
    line_second_pin .setPoints(c, Point(28, (second()%60)*6 , 270, c, CLOCKWISE)); 
  });
}

