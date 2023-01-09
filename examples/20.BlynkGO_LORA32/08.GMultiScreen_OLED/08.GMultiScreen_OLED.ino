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
 * วิธีใช้งาน ให้กดปุ่มล่าง เพื่อเลื่อนหน้า GMultiScreen ไปหน้าถัดๆไป
 * ช่วยให้เพิ่มพื้นที่ได้หลาย screen ในการแสดงผลได้ตามต้องการ
 *
 **********************************************************************/


#include <BlynkGOv3.h>

#define SSID            "-------------"
#define PASSWORD        "-------------"

#include <BlynkGOv3.h>

FONT_DECLARE(prasanmit_15);
FONT_DECLARE(LedDotMatrix_25);

GMultiScreen multiscreen;
GLabel label[5];
GLabel lb_clock;

GLabel  lb_wifi_connected(SYMBOL_WIFI);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  WiFi.begin(SSID, PASSWORD);

  multiscreen.addScreen(5);
  multiscreen.anim_time(300);
  multiscreen.indic().align(ALIGN_BOTTOM,0,15);
  multiscreen.indic().padding_inner(6);

  /* เปลี่ยน font ให้ จุด indic เพื่อให้มีขนาดพอดีจับ จอ oled*/
  gmultiscr_ext_t* ext = (gmultiscr_ext_t*) multiscreen.ext_attr();
  for(int i=0; i <  multiscreen.num(); i++)  ext->lb_indic[i]->font(prasanmit_20);

  
  for(int i=0; i < 5; i++){
    label[i].parent(multiscreen[i]);
    label[i] = String("S") + String(i+1);

    label[i].font(prasanmit_15);
    label[i].align(ALIGN_TOP_RIGHT,-2,-4);
  }

  lb_clock.parent( multiscreen[0]);
  lb_clock.font(LedDotMatrix_25);

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
  // ตั้งเวลาทุกๆ 1 วินาที ให้ ดึงค่า hour(), minute(), second() มาแสดงบน labelClock
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}
