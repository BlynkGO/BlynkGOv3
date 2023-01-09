#include <BlynkGOv3.h>
#include "NixieClock.h"


#define SSID                  "----------------"
#define PASSWORD              "----------------"

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);

  WiFi.begin(SSID, PASSWORD);
  NixieClock::create();
}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED(){
  Serial.print("WiFi Connected. IP : ");
  Serial.println(WiFi.localIP());
}

NTP_SYNCED(){
  static GTimer clock_timer;
  clock_timer.setInterval(1000L,[](){
    static time_t now;
    static struct tm tm;
    time(&now); localtime_r(&now, &tm);
    NixieClock::setTime(tm);
  });
}
