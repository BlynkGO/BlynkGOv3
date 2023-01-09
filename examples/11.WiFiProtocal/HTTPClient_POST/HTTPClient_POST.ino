
#include <BlynkGOv3.h>
#include <HTTPClient.h>

#define URL         "https://httpbin.org/post"

GWiFiManager wifi_manager;
GLabel       lb_wificonnection;

GTimer       timer_http;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);
  
  timer_http.setInterval(5000,[](){
    if(!WiFi.isConnected()) return;

    std::unique_ptr<WiFiClientSecure>client (new WiFiClientSecure);
//    std::unique_ptr<WiFiClient>client (new WiFiClient);
    if(client) {
      client->setInsecure();  // สำหรับ WiFiClientSecure  443 แบบไม่ต้องเช็ค secure
      {
        HTTPClient http;
        http.begin(*client, URL);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int http_code = http.POST("key1=value1&key2=value2");
        Serial.println(http_code);
        if( http_code == 200) {
          String payload = http.getString();
          Serial.println(payload);
          Serial.println("-----------------------------");
        }
      }
    }
  });
    
}

void loop() {
  BlynkGO.update();  
}

WIFI_CONNECTED(){
  lb_wificonnection = WiFi.localIP().toString();
}
