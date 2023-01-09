#include <BlynkGOv3.h>

GQRCode qrcode;

#define LINE_ID   "blynkgo32"

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  /* ทำการ generate สร้าง qr code สำหรับแชร์ LINE ID*/  
  qrcode.encodeLine(LINE_ID);
  
}

void loop() {
  BlynkGO.update();
}

