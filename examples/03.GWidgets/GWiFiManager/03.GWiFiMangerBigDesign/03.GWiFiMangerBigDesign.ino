/*******************************************************
 * ตัวอย่างพื้นฐาน GWiFiManager โดยปรับแต่ง customize ให้เป็นปุ่มใหญ่
 * เชื่อมต่อ WiFi แบบอัตโนมัติ
 * จากค่าที่ User ได้กำหนด SSID, PASSWORD ( Fixed IP, Gateway, Subnet )
 * ผ่านทาง Interface สัมผัสทางหน้าจอ
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

FONT_DECLARE(font_blynkgo_80);

GWiFiManager WifiManager;                         // วิตเจ็ต GWiFiManager
GLabel       lb_wifimanager("WiFi",WifiManager);  // วิตเจ็ตข้อความ กำหนดให้เป็นเลเยอร์ลูกของ WifiManager

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  WifiManager.font(font_blynkgo_80);            // กำหนดฟอนต์
  WifiManager.color(TFT_GREEN, TFT_RED, TFT_SILVER); // กำหนดสีของสัญญาณ wifi ขณะ connected, disconnected, สีสัญญาณรองพื้น
  WifiManager.bg_color(TFT_BLUE, TFT_CYAN);     // กำหนดสีพื้นหลังแบบเล่น 2 เฉดสี
  WifiManager.opa(255);                         // ให้สีพื้นหลังไม่โปร่งใส (opa=255)
  WifiManager.border(2, TFT_NAVY);              // กำหนดขอบและสีขอบ
  WifiManager.corner_radius(15);                // กำหนดรัศมีมุมโค้ง
  WifiManager.padding(15);                      // กำหนดระยะช่องไฟ
  WifiManager.fit(FIT_NONE);                    // ให้ระยะกว้างยาวแบบ fit อัตโนมัติ ไม่ต้องมี
  WifiManager.height(WifiManager.width());      // กำหนดความสูง เท่ากับความกว้าง
  WifiManager.align(ALIGN_CENTER);              // วางไว้กลางหน้าจอ
  
  lb_wifimanager.font(prasanmit_30,TFT_BLACK);  // กำหนดฟอนต์และสี
  lb_wifimanager.align(ALIGN_BOTTOM);           // วางไว้ด้านล่างภายในของ WifiManager
}

void loop() {
  BlynkGO.update();
}


