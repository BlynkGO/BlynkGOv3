/********************************************************
 * 
 * ตัวอย่างการประยุกต์ GContainer ทำการกระจาย GIcon จำนวน 6 ชุด
 * ด้วย API คำสั่งจัดเรียง layout(..) 
 * ซึ่งจะใช้ควบคู่กับ API คำสั่งจัดช่องไฟ padding_XXXX(...) ต่างๆ
 * ทำให้ไม่ต้องมาเล็งแต่ละ GIcon เอง
 * 
 * ---------------------------------------
 * ตัวอย่างนี้ใช้วิตเจ็ต GWiFiSetting ด้วย
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 ********************************************************/

#include <BlynkGOv3.h>

IMG_DECLARE(ico_wifi3d);      // ประกาศก่อนใช้ รูปภาพแบบ c file
IMG_DECLARE(ico_switch_on);   // ประกาศก่อนใช้ รูปภาพแบบ c file
IMG_DECLARE(ico_switch_off);  // ประกาศก่อนใช้ รูปภาพแบบ c file

//--------------------------------------------------------------
// TitleBar
GRect   rect_titlebar;        // วิตเจ็ตสี่เหลี่ยม กำหนดให้พื้นที่ทำ titlebar
GLabel  lb_title  ("หน้าหลัก"                 , rect_titlebar);   // วิตเจ็ตข้อความ โดยให้เป็นเลเยอร์ลูกของ rect_titlebar
GLabel  lb_setting("  " SYMBOL_SETTINGS "  "  ,rect_titlebar);   // วิตเจ็ตข้อความ โดยให้เป็นเลเยอร์ลูกของ rect_titlebar
//--------------------------------------------------------------
// Container สำหรับ รวบรวม GIcon ที่ทำเป็น ปุ่มกด
GContainer cont_btn;
// กำหนดให้ปุ่มกดที่สร้างด้วย GIcon มี parent คือ cont_btn หรือกำหนดด้วย API คำสั่ง parent(..) ก็ได้
GIcon btn[6] = { 
  {ico_switch_off, "ห้องครัว",    cont_btn},  
  {ico_switch_off, "ห้องนอน",   cont_btn},
  {ico_switch_off, "ประตูหน้า",   cont_btn},
  {ico_switch_off, "ห้องน้ำ",     cont_btn},
  {ico_switch_off, "ห้องนั่งเล่น",  cont_btn},
  {ico_wifi3d,     "WiFi",      cont_btn }
};

GWiFiSetting wifisetting;
// state สถานะปุ่มกด
bool state[5];
//--------------------------------------------------------------

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.rotation(3);
  BlynkGO.fillScreen(TFT_WHITE);

  rect_titlebar .size(GScreen.width(), 100);
  rect_titlebar .color(TFT_NAVY);
  rect_titlebar .align(ALIGN_TOP);
  lb_title      .align(ALIGN_TOP);
  lb_setting    .align(ALIGN_TOP_RIGHT);
  lb_setting    .font(prasanmit_40);
  lb_setting    .clickable(true);
  lb_setting    .onClicked([](GWidget*w){
    Serial.println("ปุ่ม Setting Clicked");
    // codeส่วนปุ่ม setting ...
  });

  // Containter สำหรับรวบรวม Icon เข้าเป็นกลุ่ม
  cont_btn      .fit(FIT_FLOOD, FIT_NONE);    // ขนาด ให้มีการปรับความกว้างขยายไปเท่า prasent, ส่วนสูงไม่ต้อง
  cont_btn      .height(300);                 // กำหนดส่วนสูงเอง
  cont_btn      .align(ALIGN_BOTTOM);         // ให้วางชิดด้านล่างของ parent
  cont_btn      .opa(0);                      // ให้โปร่งใสมองไม่เห็น โดย layer นี้ยังซ้อนทำงานอยู่
  cont_btn      .layout(LAYOUT_GRID);         // จัด layout แบบ GRID 
  cont_btn      .padding_top(30);             // ระยะขอบด้านบน
  cont_btn      .padding_left(30);            // ระยะขอบด้านซ้าย
  cont_btn      .padding_right(30);           // ระยะขอบด้านขวา
  cont_btn      .padding_inner(10);           // ระยะห่างภายใน
  cont_btn      .clickable(false);            // ไม่ให้มีการรับสัญญาณกดทั้งหลาย

  for(int i=0; i < 6;i++){
    btn[i].size(125,125);
    btn[i].body_draw(true);                   // กำหนดให้แสดงพื้นที่ส่วน body ด้วย (ทำเป็นปุ่มกด) ปกติจะโปร่งใส
    btn[i].corner_radius(20);                 // รัศมีโค้งตามมุมของ body
    btn[i].color(TFT_WHITE, TFT_SILVER);
    btn[i].border(3, TFT_GRAY);
    btn[i].font_color(TFT_BLACK);

    if( i<5 ){
      // btn[0]..btn[4] เมื่อมีการกดเข้ามา
      btn[i].onClicked([](GWidget* w){
        for(int i=0; i < 5; i++) {
          if(w->sameWith(btn[i])) {           // หาก pointer วิตเจ็ต w เป็นวิตเจ็ตเดียวกับ btn[i]
            // ให้สลับสถานะปุ่มกด พร้อมสลับรูป on/off ให้ตรง
            btn[i].setImage( (state[i] = !state[i])? ico_switch_on : ico_switch_off);
            Serial.print(btn[i].text()); Serial.println( (state[i])? " ON": " OFF");
          }
        }
      });
    }else
    if( i==5){
      // btn[5] เมื่อมีการกดเข้ามา ให้ wifiseting เรียกทำงาน
      btn[5].onClicked([](GWidget* w){
        wifisetting.onBack([](GWidget*){wifisetting.hidden(true);});  // กำหนดปุ่ม back ให้ซ่อน wifisetting 
        wifisetting.hidden(false);                                    // เริ่มทำงานให้ ไม่ซ่อน wifisetting
      });
    }
  }
  wifisetting.autoconnection();      // ให้ระบบ wifiseting เริ่มทำการโหลดและเชื่อมต่ออัตโนมัติ
}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED() {
  Serial.println("[WiFi] connected");
  btn[5].getImage()->reset();
  btn[5].getLabel()->color(TFT_BLACK);    // สีปุ่มให้เป็นสีเขียว เวลาเชื่อมต่อได้
}

WIFI_DISCONNECTED(){
  Serial.println("[WiFi] disconnected");
  btn[5].getImage()->filter(TFT_RED);
  btn[5].getLabel()->color(TFT_RED);      // สีปุ่มให้เป็นสีแดง เวลาเชื่อมต่อได้
}


