#include <BlynkGOv3.h>

GQRCode qrcode;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  qrcode.size(180, 180);
  
  String biller_id  = "010553609264101";      // รหัสร้านค้า 15 หลัก ( National ID/Tax ID + Suffix )
  String reference1 = "000002201649894";      // รหัสบิล เป็นต้น ไม่เกิน 20 หลัก ใส่หรือไม่ก็ได้
  String reference2 = "46018940516572001219"; // ไม่เกิน 20 หลัก ใส่หรือไม่ก็ได้
  float  amount     = 823.25;
  
  qrcode.encodePromptPay(biller_id, reference1, reference2, amount );
  
}

void loop() {
  BlynkGO.update();
}
