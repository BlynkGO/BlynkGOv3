/**********************************************************************
 * ตัวอย่าง การใช้ BlynkGO ร่วมกับ WiFiManager
 * 
 *   - Double Click ปุ่มกด ปุ่มบน จะเข้าสู่โหมด WiFiManager ตั้งค่า WiFi/ Blynk 
 *     ผ่านทาง http://192.168.4.1  โดยให้เชื่อม WiFi ไปที่ AP ชื่อ OnDemandAP
 * 
 *   - Click ปุ่มกด ปุ่มบน/ล่าง เพื่อเลื่อนหน้า GMultiScreen 
 *     โดยตัวอย่างนี้ได้เพิ่มไว้ 3 screen
 *     
 *   - หากไวไฟเชื่อมต่อไม่ได้ ครบ 5 รอบ จะเข้าสู่โหมด WiFiManager ตั้งค่า WiFi/ Blynk 
 *   
 * หมายเหตุ
 *  1. ก่อน compile โปรเจคนี้ ให้เข้าไปตั้งค่าที่ blynkgo_config.h
 *     โดยกำหนดให้ compile option เป็นดังนี้ 
 *       #define  BLYNKGO_USE_WIFI       1
 *       #define  BLYNKGO_USE_NTP        1
 *       #define  BLYNKGO_USE_BLYNK      1      <--- ถ้าไม่ใช้ให้ปิดเป็น 0
 *     
 *  2. ติดตั้งไลบรารี่ WiFiManager.h
 *     ดาวโหลด จาก https://github.com/tzapu/WiFiManager
 *   
 **********************************************************************/

#include <BlynkGOv3.h>
#include <WiFiManager.h>  // ดาวโหลด จาก https://github.com/tzapu/WiFiManager

GMultiScreen multiscreen;
void multiscreen_btn_control();

//-- Screen 0 ---
GWiFiManager wifi_manager;

#if BLYNKGO_USE_BLYNK
GBlynkManager blynk_manager;
#endif //BLYNKGO_USE_BLYNK


//-- Screen 1 ---
GLabel lb_clock("[NTP Clock]");

//-- Screen 2 ---
GGaugeRainbow gauge;

//-- Message Box for WiFiManager ConfigPortal Mode ---
GRect rect_msgbox;
  GLabel lb_msgbox1("โหมด WiFiManager", rect_msgbox);  
  GLabel lb_msgbox2(" - เชื่อมไวไฟ onDemandAP\n - ตั้งค่าจาก http://192.168.4.1", rect_msgbox);  


//-- WiFiManager -----
WiFiManager wm; 
bool wifimanager_mode=false;
int8_t wifi_disconnect_cnt = 0;

#if BLYNKGO_USE_BLYNK
// Parameter เสริม สำหรับ WiFiManager ให้สามารถ config ค่าอื่นๆ เพิ่มเติมได้
WiFiManagerParameter wm_param_blynk_auth("WM_ID_BLYNK_AUTH", "Blynk Auth");
WiFiManagerParameter wm_param_blynk_host("WM_ID_BLYNK_HOST", "Blynk Host");
WiFiManagerParameter wm_param_blynk_port("WM_ID_BLYNK_PORT", "Blynk Port");
#endif //BLYNKGO_USE_BLYNK


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  multiscreen.addScreen(3);     // เพิ่ม 3 screen
  multiscreen.anim_time(500);
  multiscreen.indic().align(ALIGN_BOTTOM,0,18);
  multiscreen.indic().padding_inner(5);
  multiscreen_btn_control();

  //-- Screen 0 ---
  wifi_manager.parent(multiscreen[0]);

#if BLYNKGO_USE_BLYNK
  blynk_manager.parent(multiscreen[0]);
  wifi_manager.align(ALIGN_CENTER, -40);
  blynk_manager.align(ALIGN_CENTER, 40);

  WiFiManager_init();
#endif
  
  //-- Screen 1 ---
  lb_clock.parent(multiscreen[1]);
  lb_clock.font(prasanmit_40);  

  //-- Screen 2 ---
  gauge.parent(multiscreen[2]);
  gauge.size(110,110);
  gauge.range(0,100);
  gauge.decimal(1);
  gauge = 95.7;

  //-- MessageBox เพื่อ บอกว่า เข้าสู่โหมด WiFiManager ---
  rect_msgbox.size(GScreen.width()*95/100, GScreen.height()*80/100);
  rect_msgbox.corner_radius(15);
  rect_msgbox.border(5, TFT_BLUE);
  rect_msgbox.font(prasanmit_20, TFT_BLACK);
  rect_msgbox.line_space(-5);
  rect_msgbox.hidden(true);                       // ให้ซ่อนลงไปก่อน
    lb_msgbox1.font(prasanmit_30, TFT_BLACK);
    lb_msgbox1.position(20,5);
    lb_msgbox2.align(lb_msgbox1, ALIGN_BOTTOM_LEFT,0,-5);
}


void loop() {
  BlynkGO.update();
  if( wifimanager_mode ) {
    wifimanager_mode = false;

    wm.setConfigPortalTimeout(120);      // secs

#if BLYNKGO_USE_BLYNK
    wm_param_blynk_auth.setValue( blynk_manager.auth().c_str(), 64);
    wm_param_blynk_host.setValue( blynk_manager.host().c_str(), 255);
    wm_param_blynk_port.setValue( blynk_manager.port_str().c_str(), 8);
#endif
        
    if (!wm.startConfigPortal("OnDemandAP")) {
      Serial.println("-------------------------------------------------");
      Serial.println("[WiFiManager] failed to connect and hit timeout");
      rect_msgbox.hidden(true);
    }else{
      Serial.println("-------------------------------------------------");
      Serial.println("[WiFiManager] ConfigPortal Done.");
      rect_msgbox.hidden(true);
    }
  }
}

WIFI_CONNECTED(){
  Serial.println("[WiFi] connected");
  wifi_disconnect_cnt = 0;
}

WIFI_DISCONNECTED(){
  Serial.println("[WiFi] disconnected");
  wifi_disconnect_cnt++;
  if(wifi_disconnect_cnt >5){
    wifi_disconnect_cnt = 0;
    rect_msgbox.hidden(false);  
    static GTimer timer;
    timer.setOnce(1000,[](){ wifimanager_mode = true; } );  // ให้เข้าสู่โหมดตั้งค่า WiFi ผ่านทาง WiFiManager ConfigPortal
  }
}

BLYNK_CONNECTED(){
  Serial.println("[Blynk] connected");
}

BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] disconnected");
}

NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000,[](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

void WiFiManager_init(){

  wm.addParameter(&wm_param_blynk_auth);
  wm.addParameter(&wm_param_blynk_host);
  wm.addParameter(&wm_param_blynk_port);

  wm.setSaveConfigCallback([](){        // เมื่อ WiFiConfig ผู้ใช้มีการ save จากที่กำหนดใน http://192.168.4.1
    String ssid = wm.server->arg(F("s"));
    String pass = wm.server->arg(F("p"));

    if( ssid != "") {
      Serial.printf("[GWiFiManager] save WiFi : ssid -> %s ; password -> %s\n", ssid.c_str(), pass.c_str());
      wifi_manager.save(ssid, pass);    // ให้นำมา save ให้ wifi_manager ด้วย (ปกติ ให้ไวไฟเชื่อมต่อผ่านทาง GWiFiManager เป็นหลัก)
    }
    
#if BLYNKGO_USE_BLYNK
    String blynk_auth   = wm_param_blynk_auth.getValue();
    String blynk_host   = wm_param_blynk_host.getValue();
    uint16_t blynk_port = String(wm_param_blynk_port.getValue()).toInt();

    if( blynk_auth != "" && blynk_host != "" && blynk_port != 0 ) {
      Serial.printf("[GBlynkManager] save Blynk : auth -> %s ; host -> %s ; port -> %d\n", blynk_auth.c_str(), blynk_host.c_str(), blynk_port);
      blynk_manager.save( blynk_auth, blynk_host, blynk_port);  // ให้นำมา save ให้ blynk_manager หลัง WiFi เชื่อมต่อ blynk_manager จะทำการเชื่อมต่อกับ blynk host ให้อัตโนมัติ
    }
#endif
  });
}


