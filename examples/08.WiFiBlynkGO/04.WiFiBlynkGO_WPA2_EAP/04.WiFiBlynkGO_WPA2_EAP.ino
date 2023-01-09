#include <BlynkGOv3.h>
#include "esp_wpa2.h"

#define SSID          "---------------"
#define EAP_IDENTITY  "---------------" //username
#define EAP_PASSWORD  "---------------" //password

GWiFiManager wifi_manager;
GLabel       lb_clock;

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();


  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA);
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config to default (fixed for 2018 and Arduino 1.8.5+)
  esp_wifi_sta_wpa2_ent_enable(&config); //set config to enable function (fixed for 2018 and Arduino 1.8.5+)
  WiFi.begin(SSID); //connect to Wifi

  wifi_manager.align(ALIGN_TOP_RIGHT, -5, 5);  
}

void loop(){
  BlynkGO.update();
}


WIFI_CONNECTED(){
  Serial.print("[WiFi] Connected. IP : "); Serial.println(WiFi.localIP());
}

WIFI_DISCONNECTED(){
  Serial.println("[WiFi] Disconnected");
}


NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000L, [](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

