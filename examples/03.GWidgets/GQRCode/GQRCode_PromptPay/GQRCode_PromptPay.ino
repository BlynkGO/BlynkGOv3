#include <BlynkGOv3.h>

GQRCode qrcode;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  qrcode.size(180, 180);
  /* PrompPay QRCode by citizen_id*/
  String citizen_id = "3210123456789";  // Thai Citizen ID 13 digits
  float  amount     = 1000.25;
  qrcode.encodePromptPay(citizen_id , amount);

  /* PrompPay QRCode by mobile-phone number */
  // String mobile_phone = "0891234567"    // Thai Mobile Phone Number 10 digits
  // qrcode.encodePromptPay(mobile_phone , amount);
  
}

void loop() {
  BlynkGO.update();
}
