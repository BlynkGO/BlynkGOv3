#include <BlynkGOv3.h>

IMG_DECLARE(ico_spiderman);               // ประกาศ resource รูปแบบ c file ก่อนใช้ในวิตเจ็ตรูป
IMG_DECLARE(ico_dr_strange);

GContainer btn_spiderman;                                 // วิตเจ็ต GContainer
  GImage img_spiderman(ico_spiderman  , btn_spiderman);   // วิตเจ็ตรูป พร้อมกำหนด รูป และ parent
  GLabel lb_spiderman ("SpiderMan"    , btn_spiderman);   // วิตเจ็ตข้อความ พร้อมกำหนด ข้อความ และ parent
GContainer btn_dr_strange;
  GImage img_dr_strange(ico_dr_strange, btn_dr_strange);
  GLabel lb_dr_strange ("Dr. Strange" , btn_dr_strange);

void spiderman_event_cb(GWidget*widget, event_t event);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  btn_spiderman.layout(LAYOUT_ROW_M);     // layout สำหรับวิตเจ็ตลูกๆ ให้เรียงแบบแถว แนวตรงกลางตรงกัน
  btn_spiderman.padding(10,10,30,30,10);  // ระยะช่องไฟ บน, ล่าง, ซ้าย,ขวา ,ภายใน
  btn_spiderman.corner_radius(10);        // มุมโค้ง 10 pixel
  btn_spiderman.border(5, TFT_RED);       // ขอบ 5 สีแดง
  btn_spiderman.color(TFT_BLUE);          // สีพื้นหลัง สีฟ้า
  btn_spiderman.event_cb( my_event_cb);   // กำหนด ฟังก์ชั่น my_event_cb สำหรับจัดการ event
  btn_spiderman.align(ALIGN_TOP_RIGHT, -20, 20);  // จัดตำแหน่ง ขวาบน ขยับมาซ้าย 20 ลงล่าง 20 pixel
    img_spiderman.create();               // วิตเจ็ตรูปภาพ ให้สร้าง (โดยมีการโหลดรูปและมี parent ตามกำหนด)
    lb_spiderman.create();                // วิตเจ็ตข้อความ ให้สร้าง (โดยมีการโหลดรูปและมี parent ตามกำหนด)

  btn_spiderman.onClicked([](GWidget*widget){ // เมื่อ user กด clicked เข้ามา
    Serial.println("SpiderMan Clicked");
  });

  btn_dr_strange.layout(LAYOUT_ROW_M);     // layout สำหรับวิตเจ็ตลูกๆ ให้เรียงแบบแถว แนวตรงกลางตรงกัน
  btn_dr_strange.padding(10,10,30,30,10);  // ระยะช่องไฟ บน, ล่าง, ซ้าย,ขวา ,ภายใน
  btn_dr_strange.width(btn_spiderman);
  btn_dr_strange.corner_radius(10);
  btn_dr_strange.border(5, TFT_BLUE);
  btn_dr_strange.color(TFT_RED);
  btn_dr_strange.event_cb( my_event_cb);  // กำหนด ฟังก์ชั่น my_event_cb สำหรับจัดการ event
  btn_dr_strange.align(btn_spiderman, ALIGN_BOTTOM, 0, 20);
    img_dr_strange.create();
    lb_dr_strange.create();

  btn_dr_strange.onClicked([](GWidget*widget){
    Serial.println("Dr.Strange Clicked");
  });
}

void loop() {
  BlynkGO.update();
}

// ฟังกชั่นจัดการ event สำหรับวิตเจ็ต ให้วิตเจ็ตนั้นๆ เมื่อมีการกด จะมีสีที่ดำเข้มขึ้น  เมื่อปล่อยจะกลับสู่สีเดิม
void my_event_cb(GWidget*widget, event_t event){
  static color_t color[3];
  switch(event){
    case EVENT_PRESSED:         // เมื่อวิตเจ็ตนั้นๆ มีสัญญาณ "เริ่มกด" เข้ามา
      color[0] = widget->color();             // เก็บค่าสีพื้นหลัง
      color[1] = widget->border_color();      // เก็บค่าสีขอบ
      color[2] = widget->font_color();        // เก็บค่าสีฟอนต์

      widget->image_color(TFT_BLACK);
      widget->image_intense(128);
      widget->color( TFT_COLOR_MIX(TFT_BLACK, color[0], 128));          // ผสมสีดำจากสีดเดิม ให้เข้มขึ้น
      widget->border_color(  TFT_COLOR_MIX(TFT_BLACK, color[1], 128));
      widget->font_color( TFT_COLOR_MIX(TFT_BLACK, color[2], 128));
            
      break;
    case EVENT_RELEASED:      // เมื่อวิตเจ็ตนั้นๆ มีสัญญาณ "เริ่มปล่อย" จากการกด
    case EVENT_PRESS_LOST:    // หรือเมื่อวิตเจ็ตนั้นๆ หลุดออกนอกพื้นที่จากการกดวิตเจ็ตนั้นๆ
      widget->image_intense(0);             // คืนค่า สีเดิม ก่อนมีการกด
      widget->color(color[0]);
      widget->border_color(color[1]);
      widget->font_color(color[2]);
      break;
  }
}


