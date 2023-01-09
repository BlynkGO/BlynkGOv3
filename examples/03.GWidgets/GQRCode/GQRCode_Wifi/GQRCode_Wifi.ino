#include <BlynkGOv3.h>

GQRCode qrcode;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
//  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);
  
  qrcode.encodeWiFi("MyWiFi","11112222");
  
}

void loop() {
  BlynkGO.update();
}
