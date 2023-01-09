#include <BlynkGOv3.h>

GTextArea textarea;
GTextArea password_ta;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  textarea.width(BlynkGO.width()-10);
  textarea.placeholder("WiFi SSID");
  textarea.cursor_type(CURSOR_NONE);
  textarea.align(ALIGN_CENTER,0,-textarea.height());
  textarea.onValueChanged([](GWidget* widget){
    Serial.printf("SSID : %s\n", textarea.c_str());
  });

  password_ta.width(BlynkGO.width()-10);
  password_ta.password_mode(true);
  password_ta.placeholder("WiFi Password");
  password_ta.cursor_type(CURSOR_NONE);
  password_ta.align(textarea, ALIGN_BOTTOM, 0, 20);
  password_ta.onValueChanged([](GWidget* widget){
    Serial.printf("Password : %s\n", password_ta.c_str());
  });


}

void loop() {
  BlynkGO.update();
}

