/********************************************************************
 * ตัวอย่าง การสร้าง Gauge 
 * ที่สามารถแสดงผลได้พร้อมกันทั้ง TFT และ แอพ Blynk
 * โดยจะเป็นการส่งข้อมูลไปจากฝั่ง BlynkGO ไปยังแอพ Blynk
 * ด้วย BlynkTimer
 * --------------------------------------------
 * ที่แอพ Blynk ให้สร้าง Widget Gauge สีฟ้า ให้มีช่วง 0-100
 * โดยกำหนด virtual pin V6
 * ------------------------------------------------------------------
 * หมายเหตุ 
 * ก่อนใช้งาน ร่วมกับแอพ Blynk
 *  1. ให้ copy ไฟล์ ในโฟลเดอร์ BlynkGOv3/BlynkBlynkGO ให้นำโฟลเดอร์ src ไปวางไว้ที่ 
 *     โฟลเดอร์ไลบรารี่ Blynk หากมีไฟล์ซ้ำให้ copy ทับ
 *
 *  2. ให้กำหนด เปิด การใช้งาน Blynk ให้ BlynkGO ได้ที่
 *     ไฟล์ config/blynkgo_config.h
 *     โดยกำหนดเป็น
 *       #define BLYNKGO_USE_WIFI           1
 *       #define BLYNKGO_USE_NTP            1
 *       #define BLYNKGO_USE_BLYNK          1
 *
 **************************************************************/
#include <BlynkGOv3.h>

// #define BLYNKGO_KEY   "----------------"
// #define SSID          "----------------"
// #define PASSWORD      "----------------"

// #define BLYNK_AUTH    "----------------"
// #define BLYNK_HOST    "blynk.jpnet.co.th"   // Blynk Server แบบกำหนดเอง  ( ที่ blynk.jpnet.co.th ให้ energy 10ล้าน )
// #define BLYNK_PORT    8080                  // Blynk Port   แบบกำหนดเอง

GWiFiManager wifi_manager;
GBlynkManager blynk_manager;


GGauge gauge("อุณหภูมิ",TFT_BLUE);
BlynkTimer timer;

int8_t val = 0;
int8_t step = 1;
void Blynk_Gauge_Update()
{
  gauge = val;                    // กำหนดค่าให้ gauge ของ BlynkGO  (ที่จอ TFT)
  BlynkGO.virtualWrite(V6, val);  // ส่งค่าเดียวกันนี้ไปยังแอพ Blynk ด้วย

  // ตัวอย่างจำลองค่า สำหรับกำหนดให้ gauge 
  // ตามปกติให้อ่านค่าจาก sensor เป็นต้น
  val += step; 
  if(val >100) {val = 100; step = -step; }
  else if(val <0) { val = 0; step = -step; }
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD);  // แบบเชื่อมต่อ Blynk Server มาตรฐาน
  // BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD, BLYNK_HOST, BLYNK_PORT); // แบบเชื่อมต่อ Blynk Server กำหนดเอง
  
  wifi_manager.align(ALIGN_TOP_RIGHT, -10,5);
  blynk_manager.align(wifi_manager, ALIGN_LEFT, -10);

  gauge.align(ALIGN_CENTER);
  
  timer.setInterval(200L, Blynk_Gauge_Update); // ตั้งเวลาทุกๆ 200ms เพื่อทำการกำหนดค่าให้ gauge ที่แอพ Blynk
}

void loop() {
  BlynkGO.update();
  timer.run();
}

BLYNK_CONNECTED() {
  Serial.println("[Blynk] Connected");
  BlynkGO.syncAll();
}

BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] DisConnected");
}


