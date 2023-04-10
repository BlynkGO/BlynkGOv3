#include <BlynkGOv3.h>

typedef struct _smartfarm {
  GLabel label;
  GGauge gauge_temp;
  GGauge gauge_humid;
  
  void design(GWidget *parent, String name){
    label = name;
    label.parent(parent);
    label.font(prasanmit_20, TFT_WHITE);
    
    gauge_temp.size(120,120); 
    gauge_temp.parent(parent);
    gauge_temp.title("อุณหภูมิ (°C)");
    gauge_temp.color( TFT_COLOR_HEX(0xF45757) );
    gauge_temp.thickness(10);
    gauge_temp.decimal(1);
    gauge_temp.align(ALIGN_CENTER,-80,-5);
    gauge_temp = random(1000)/10.0;

    gauge_humid.size(120,120); 
    gauge_humid.parent(parent);
    gauge_humid.title("ความชื้น (%)");
    gauge_humid.color( TFT_COLOR_HEX(0x0455BF) );
    gauge_humid.thickness(10);
    gauge_humid.decimal(1);
    gauge_humid.align(ALIGN_CENTER, 80,-5);
    gauge_humid = random(1000)/10.0;

    label.font(prasanmit_35);
    label.align( gauge_temp, ALIGN_BOTTOM,0, -10);
  }
} smartfarm_t;

GMultiScreen multiscreen;
  //----- multiscreen 0-3 ---------
  String name[] = { "ผักกาดแก้ว", "เรดโอ๊ค",  "กรีนโอ๊ค", "ร็อคเก็ต"};
  smartfarm_t smartfarm[4];

  //----- multiscreen 4 ---------
  namespace WaterTank {
    void design(GWidget*parent=&GScreen);
    void setWaterHeight(float h);  // 0-100  
    void anim_show();
  }
  //----- multiscreen 5 ---------
  namespace PowerMeter {
    void design(GWidget*parent=&GScreen);
    void value(float val);
  }
  //----- multiscreen 6 ---------
  GQRCode qrcode;
  GLabel  lb_qrcode;

  //----- multiscreen 7 ---------
  GGaugeLine gauge_line_temp;
  GGaugeLine gauge_line_humid;
  GLabel     lb_gauge_temp;
  GLabel     lb_gauge_humid;
  
  //----- multiscreen 8 ---------
  namespace DoraemonAnime {
    void design(GWidget*parent);
  }
  
void setup() {
  Serial.begin(115200); Serial.println();     // เริ่มใช้งาน Serial Monitor
  BlynkGO.begin();                            // เริ่มใช้งาน BlynkGO
  BlynkGO.fillScreen(TFT_BLACK);              // เติมสีหน้าจอเป็นสี ดำ
//  BlynkGO.brightness(100);                  // ปรับความสว่าง max = 255

  //------------------------------------------------  
  multiscreen.addScreen(9);
  multiscreen.indic().align(ALIGN_BOTTOM_RIGHT,-5,20);
  multiscreen.indic().padding_inner(4);
  multiscreen.anim_time(300);
  multiscreen.current_screen( 8, false );
  //----- multiscreen 0-3 --------- 
  for(int i=0; i < 4; i++){
    smartfarm[i].design(multiscreen[i], name[i]);
  }
  
  //----- multiscreen 4 --------- 
  WaterTank::design( multiscreen[4]);
  WaterTank::anim_show();
  
  //----- multiscreen 5 --------- 
  PowerMeter::design( multiscreen[5]);
  
  //----- multiscreen 6 ---------
  qrcode.size(140,140);
  qrcode.parent(multiscreen[6]);
  qrcode.encodePromptPay("0966840014", 100.00);
  qrcode.align(ALIGN_LEFT,20,-5);
  lb_qrcode.parent(multiscreen[6]);
  lb_qrcode = "ชำระ\n PromptPay";
  lb_qrcode.text_align(TEXT_ALIGN_LEFT);
  lb_qrcode.align(qrcode, ALIGN_RIGHT_TOP,20,10);

  //----- multiscreen 7 ---------
  gauge_line_temp.size(130,130);                      // กำหนดขนาด
//  gauge_line_temp.range(0,100);                     // กำหนด ระยะค่าของ gauge
  gauge_line_temp.scale(270, 20+1, 10+1);             // กำหนดขีดบอกระดับบน gauge ให้กินพื้นที่มุม 270 องศา โดยมีขีดเส้นบอกระดับ 20+1 ค่า (รวมค่าเริ่มต้น) มีข้อความบอกระดับ 10+1 ค่า (รวมค่าเริ่มต้น)
  gauge_line_temp.scale_color(TFT_DARKGRAY, TFT_RED); // กำหนดสีของเส้นบอกระดับ
  gauge_line_temp.scale_width(3);                     // ความหนาของเส้นบอกระดับ
  gauge_line_temp.scale_length(6);                    // ความยาวของเส้นบอกระดับ
  gauge_line_temp.scale_padding(3);                   // ช่องไฟระหว่างเส้นบอกระดับ กับ ข้อความบอกระดับ
  gauge_line_temp.font(prasanmit_20,TFT_WHITE);       // กำหนดฟอนต์และสีอักษรของ ค่าบอกระดับ
  gauge_line_temp.critical_value( 60 );               // ค่าเริ่มต้นโซนที่เป็นค่าอันตราย
  gauge_line_temp.critical_color( TFT_RED);           // สีโซนค่าอันตราย
  gauge_line_temp.circle_middle_color(TFT_GREEN);     // สี วงกลมหมุดเข็ม ตรงกลาง
  gauge_line_temp.circle_middle_radius(10);           // รัศมี วงกลมหมุดเข็ม ตรงกลาง
  gauge_line_temp.needle_color( TFT_ORANGE );           // กำหนดสีเข็ม
  gauge_line_temp.parent(multiscreen[7]);
  gauge_line_temp.align(ALIGN_LEFT,14,-3);

  lb_gauge_temp.parent(multiscreen[7]);
  lb_gauge_temp.font(prasanmit_25);
  lb_gauge_temp = "อุณหภูมิ";
  lb_gauge_temp.align(gauge_line_temp, ALIGN_IN_BOTTOM,0,5);

  gauge_line_humid.size(130,130);                     // กำหนดขนาด
//  gauge_line_humid.range(0,100);                    // กำหนด ระยะค่าของ gauge
  gauge_line_humid.scale(270, 20+1, 10+1);            // กำหนดขีดบอกระดับบน gauge ให้กินพื้นที่มุม 270 องศา โดยมีขีดเส้นบอกระดับ 20+1 ค่า (รวมค่าเริ่มต้น) มีข้อความบอกระดับ 10+1 ค่า (รวมค่าเริ่มต้น)
  gauge_line_humid.scale_color(TFT_CYAN, TFT_BLUE);   // กำหนดสีของเส้นบอกระดับ
  gauge_line_humid.scale_width(3);                    // ความหนาของเส้นบอกระดับ
  gauge_line_humid.scale_length(6);                   // ความยาวของเส้นบอกระดับ
  gauge_line_humid.scale_padding(3);                  // ช่องไฟระหว่างเส้นบอกระดับ กับ ข้อความบอกระดับ
  gauge_line_humid.font(prasanmit_20,TFT_WHITE);      // กำหนดฟอนต์และสีอักษรของ ค่าบอกระดับ
  gauge_line_humid.critical_value( 60 );              // ค่าเริ่มต้นโซนที่เป็นค่าอันตราย
  gauge_line_humid.critical_color( TFT_BLUE); // สีโซนค่าอันตราย
  gauge_line_humid.circle_middle_color(TFT_GREEN);    // สี วงกลมหมุดเข็ม ตรงกลาง
  gauge_line_humid.circle_middle_radius(10);          // รัศมี วงกลมหมุดเข็ม ตรงกลาง
  gauge_line_humid.needle_color( TFT_VIOLET );        // กำหนดสีเข็ม
  gauge_line_humid.parent(multiscreen[7]);
  gauge_line_humid.align(ALIGN_RIGHT,-14,-3);

  lb_gauge_humid.parent(multiscreen[7]);
  lb_gauge_humid.font(prasanmit_25);
  lb_gauge_humid = "ความชื้น";
  lb_gauge_humid.align(gauge_line_humid, ALIGN_IN_BOTTOM,0,5);


  gauge_line_temp   = 48;
  gauge_line_humid  = 90;

  //----- multiscreen 8 ---------
  DoraemonAnime::design(multiscreen[8]);
  
  //------------------------------------------------  
  // เมื่อ BTN มีการ click ให้ทำการเลื่อนหน้าจอ multiscreen
  BTN.onClicked([](ButtonISR* btn) {
    Serial.println("btn click");
    if( multiscreen.current_screen_id() < multiscreen.num()-1) {
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
    } 
    else if(multiscreen.current_screen_id() == multiscreen.num()-1) {
      multiscreen.current_screen( 0, false );
    }
  });

//  // หากกดค้างยาว 
//  BTN.onLongPressed([](ButtonISR* btn) {
//    // ถ้าตอนนี้อยู่หน้า id =2 
//    if(  multiscreen.current_screen_id() == 2 ) {
//      // ให้ทำอะไรตามต้องการ
//    }
//  });

}

void loop() {
  BlynkGO.update();                           // ให้ BlynkGO ทำการ update สถานะต่างๆ
}
