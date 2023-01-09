#include <BlynkGOv3.h>

GMultiScreen MultiScreen;
GSwitch3D    Switch3D;
GLabel       lb_room;

GGauge       gauge_temp;
GGauge       gauge_humid;

GTextArea    ta_wifi_ssid;
GTextArea    ta_wifi_password;

void setup() {
  BlynkGO.begin();
  MultiScreen.addScreen(3);  // เพิ่ม 3 หน้าจอ

  // กำหนดว่าวิตเจ็ตไหนให้ไปอยู่หน้าไหน
  Switch3D        .parent(MultiScreen[0]);
  lb_room         .parent(MultiScreen[0]);
  gauge_temp      .parent(MultiScreen[1]);
  gauge_humid     .parent(MultiScreen[1]);
  ta_wifi_ssid    .parent(MultiScreen[2]);
  ta_wifi_password.parent(MultiScreen[2]);


  Switch3D.sw3d_type(SW3D_TYPE_ROUND);
  Switch3D.color(TFT_BLUE);
  lb_room = "ห้องนั่งเล่น";
  lb_room.align(Switch3D ,ALIGN_BOTTOM);

  gauge_temp = 24;
  gauge_humid = 43;
  gauge_temp  .color(TFT_RED);
  gauge_humid .color(TFT_BLUE);
  gauge_temp  .align(ALIGN_CENTER, -gauge_temp.width()/2 );
  gauge_humid .align(ALIGN_CENTER, gauge_humid.width()/2);

  ta_wifi_ssid.width(BlynkGO.width()-20);
  ta_wifi_ssid.placeholder("WiFi SSID");
  ta_wifi_ssid.cursor_type(CURSOR_NONE);
  ta_wifi_ssid.align(ALIGN_CENTER,0,-ta_wifi_ssid.height());
  ta_wifi_ssid.onValueChanged([](GWidget* widget){
    Serial.printf("SSID : %s\n", ta_wifi_ssid.text().c_str());
  });

  ta_wifi_password.width(BlynkGO.width()-20);
  ta_wifi_password.password_mode(true);
  ta_wifi_password.placeholder("WiFi Password");
  ta_wifi_password.cursor_type(CURSOR_NONE);
  ta_wifi_password.align(ta_wifi_ssid, ALIGN_BOTTOM, 0, 20);
  ta_wifi_password.onValueChanged([](GWidget* widget){
    Serial.printf("Password : %s\n", ta_wifi_password.text().c_str());
  });

}

void loop() {
  BlynkGO.update();
}

