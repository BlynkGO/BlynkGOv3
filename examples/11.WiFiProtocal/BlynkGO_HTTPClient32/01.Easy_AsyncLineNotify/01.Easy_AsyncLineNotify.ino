
#include <BlynkGOv3.h>

#define LINE_TOKEN      "----------------------"

GWiFiManager wifi_manager;

GLabel  label;
GButton btn_linenotify;
GButton btn_other;

void EASY_LINE_NOTIFY(String message);

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT);

  btn_linenotify = "LINE Notify";
  btn_linenotify.onClicked([](GWIDGET){
    EASY_LINE_NOTIFY("hello : " + String(random(100)));
    btn_linenotify.inactive(true);
  });

  btn_other = "ปุ่มอื่นๆ";
  btn_other.align(btn_linenotify, ALIGN_BOTTOM,0,30);
  btn_other.onClicked([](GWIDGET){
    static int c;
    label = String("ยังกดได้ ") + String(c++);
  });

  label.align(btn_linenotify, ALIGN_TOP,0,-30);

}

void loop() {
  BlynkGO.update();
}

BlynkGO_HTTPClient32 http;
static const size_t BUFFER_SIZE = 1024;
static char buf[BUFFER_SIZE];
static size_t bufferIndex = 0;

void EASY_LINE_NOTIFY(String message){
  String url = "https://notify-api.line.me/api/notify";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Bearer " LINE_TOKEN );
  String postData = "message=" + message;
  http.POST(postData);
  bufferIndex = 0; memset(buf, 0, BUFFER_SIZE);
  http.onData([](uint8_t* data, size_t data_len, bool is_finished){
    if (bufferIndex + data_len < BUFFER_SIZE) {
      // คัดลอกข้อมูลจาก data ไปยัง buf
      memcpy(buf + bufferIndex, data, data_len);
      bufferIndex += data_len;
      if (is_finished) {  // ตรวจสอบว่าได้ข้อมูลทั้งหมดแล้ว
        buf[bufferIndex] = '\0'; // สิ้นสุด buffer ด้วย null character
        Serial.println("Received data:");
        Serial.println(buf);
        bufferIndex = 0; // รีเซ็ต index สำหรับการรับข้อมูลครั้งถัดไป
      }
    } else {
      Serial.println("Buffer overflow");
    }

    btn_linenotify.inactive(false);
  });
}
