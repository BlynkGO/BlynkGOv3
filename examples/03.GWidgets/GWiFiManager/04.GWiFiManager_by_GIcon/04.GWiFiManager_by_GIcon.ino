/*******************************************************
 * ตัวอย่างการใช้ GIcon มาทำเป็นปุ่มกดตั้ง WiFi อัตโนมัติ
 *
 * ---------------------------------------
 * ก่อนใช้งาน WiFi และ NTP 
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_NTP           1
 *
 ******************************************************/

#include <BlynkGOv3.h>

IMG_DECLARE(ico_wifi3d);      // ประกาศก่อนใช้ รูปภาพแบบ c file

GIcon         btn_wifisetting(ico_wifi3d, "WiFi");  // วิตเจ็ต Icon แต่ประยุกต์นำมาใช้สร้างเป็นปุ่มกด
GWiFiSetting  wifisetting;      // วิตเจ็ต GWiFiSetting

void setup() {
  Serial.begin(115200);Serial.println();
  BlynkGO.begin();

  btn_wifisetting.size(125,125);                    // กำหนดขนาด
  btn_wifisetting.body_draw(true);                  // กำหนดให้แสดงพื้นที่ส่วน body ด้วย (ทำเป็นปุ่มกด) ปกติจะโปร่งใส
  btn_wifisetting.corner_radius(20);                // รัศมีโค้งตามมุมของ body
  btn_wifisetting.color(TFT_WHITE, TFT_SILVER);     // กำหนดสีพื้นของ GIcon ที่นำมาประยุกตืเป็นปุ่มกด
  btn_wifisetting.border(3, TFT_GRAY);              // กำหนดระยะขอบและสี
  btn_wifisetting.font_color(TFT_BLACK);            // กำหนดสีฟอนต์ข้อความ
  
  btn_wifisetting.onClicked([](GWidget* w){
    wifisetting.onBack([](GWidget*){wifisetting.hidden(true);});  // กำหนดปุ่ม back ให้ซ่อน wifisetting 
    wifisetting.hidden(false);                                    // เริ่มทำงานให้ ไม่ซ่อน wifisetting
  });
  wifisetting.autoconnection();      // ให้ระบบ wifiseting เริ่มทำการโหลดและเชื่อมต่ออัตโนมัติ
}  

void loop() {
  BlynkGO.update();
}

// เมื่อ WiFi เชื่อมต่อสำเร็จ ให้ ปุ่มตั้ง WiFi สลับมาสีปกติ
WIFI_CONNECTED(){
  Serial.println("[WiFi] connected");
  btn_wifisetting.getImage()->reset();             // ล้าง filter คืนสู่ภาพดั้งเดิม
  btn_wifisetting.getLabel()->color(TFT_BLACK);    // สีปุ่มให้เป็นสีเขียว เวลาเชื่อมต่อได้
}


// เมื่อ WiFi เชื่อมต่อสำเร็จ ให้ ปุ่มตั้ง WiFi เปลี่ยนเป็นสีแดง
WIFI_DISCONNECTED(){
  Serial.println("[WiFi] disconnected");
  btn_wifisetting.getImage()->filter(TFT_RED);     // ใส่ filter ให้รูปภาพ ของ btn 
  btn_wifisetting.getLabel()->color(TFT_RED);      // สีปุ่มให้เป็นสีแดง เวลาเชื่อมต่อได้
}

