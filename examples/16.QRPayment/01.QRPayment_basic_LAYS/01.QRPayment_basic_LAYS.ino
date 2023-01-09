/*******************************************************
 * ตัวอย่างพื้นฐาน สร้าง Thai QR Payment ชำระเงิน ผ่านสื่อกลาง GBPrimePay
 *  
 *  Customer สามารถกด สร้าง Thai QR Payment แบบไม่ซ้ำ
 *  และใช้ แอพธนาคารบนมือถือ ทำการ แสกน ชำระเงิน
 *  เข้าบัญชีธนาคาร ได้  
 *  
 *  หลังเงินได้รับจาก customer แล้ว
 *  จะสามารถ สั่งให้ทำงานระบบตางๆต่อไปได้
 *
 * ---------------------------------------
 * ก่อนใช้งาน 
 * 
 *  1. ให้กำหนดที่ไฟล์ config/blynkgo_config.h
 *     โดยกำหนดเป็น
 *        #define BLYNKGO_USE_WIFI          1
 *        #define BLYNKGO_USE_NTP           1 
 *        #define BLYNKGO_USE_MQTT          1
 *        #define BLYNKGO_USE_QRPAYMENT     1
 *
 *  2. ติดตั้งไลบรารี่ ArduinoJson version 6.x
 *  
 *  3. ติดตั้ง BlynkGO_QRPayment บน PC 
 *
 *  4. เปิดบัญชี GBPrimePay ที่ https://www.gbprimepay.com 
 *     พร้อมผูกบัญชีธนาคารให้เรียบร้อย และนำค่า Customer Key ( GBPrimePay Token )
 *     มากำหนดใน code
 *
 ******************************************************/

#include <BlynkGOv3.h>


#define GBPRIMEPAY_TOKEN    "--------------------------"

#define MQTT_HOST           "192.168.0.180"   //"test.mosquitto.org"
#define MQTT_PORT           1883
#define MQTT_TUNNEL_TOPIC   "blynkgo/customer_01/tunnel"
#define MQTT_PAID_TOPIC     "blynkgo/customer_01/paid"

#define PRODUCT_DETAIL      "มันฝรั่ง Lays"
#define PRODUCT_AMOUNT      0.01


IMG_DECLARE(img_lays_logo);
IMG_DECLARE(img_thai_qrpayment_w200);

GWiFiManager wifi_manager;
GButton  btn_qrcash("Gen QRCash");

GRect   rect_qr_payment;
  GImage   image_promptpay(img_thai_qrpayment_w200 , rect_qr_payment);
  GQRCode  qrcode(rect_qr_payment);
  GImage   image_logo(img_lays_logo, rect_qr_payment);
  GLabel   lb_bill_no(rect_qr_payment);

GLabel   lb_qrcash_status("Please wait a moment..");  // เอาไว้บอกสถานะ
GLabel   lb_detail;

GRect       rect_thankyou;
  GRect     rect_thankyou_box(rect_thankyou);
    GLabel  lb_thankyou("ขอบคุณที่ใช้บริการ", rect_thankyou_box);


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_CYAN, TFT_COLOR_HSV(240,100,30));

  //-------------------------------------------------
  // Graphic Design
  wifi_manager.align(ALIGN_TOP_RIGHT,-5);

  btn_qrcash.center( 360, 80);
  btn_qrcash.inactive(true);
  btn_qrcash.onClicked([](GWidget*widget){  
    rect_qr_payment.hidden(true);
    lb_qrcash_status.hidden(false);
    lb_detail.hidden(true);
    
    lb_qrcash_status.color(TFT_WHITE);
    lb_qrcash_status = "Please wait a moment..";
    

    static GTimer timer;
    timer.setOnce(100,[](){ // หน่วงเวลา 100ms รอให้กราฟิกเปลี่ยนค่าข้างต้นค่อยส่ง
      String bill_no = GBPrimePay.gen_bill_no();
      GBPrimePay.requestQRCash(bill_no, PRODUCT_AMOUNT, PRODUCT_DETAIL);
    });
  });
  lb_qrcash_status.hidden(true);
  lb_qrcash_status.font(prasanmit_25,TFT_WHITE);
  lb_qrcash_status.align( ALIGN_BOTTOM_RIGHT, -30);

  
  lb_detail.hidden(true);
  lb_detail.font(prasanmit_40,TFT_WHITE);
  lb_detail.align(btn_qrcash, ALIGN_BOTTOM_LEFT,0, 40);
  lb_detail.text_align(TEXT_ALIGN_RIGHT);
  lb_detail.line_space(-10);
  

  // พื้นที่แสดง qrcode สร้างเตรียมไว้ แต่ให้เป็น layer ที่ซ่อนไว้
  rect_qr_payment.hidden(true);
  rect_qr_payment.size(210,320);
  rect_qr_payment.opa(0);
  rect_qr_payment.align(ALIGN_LEFT,25);
    image_promptpay.align(ALIGN_TOP,0,20);
    qrcode.size(200,200);
    qrcode.version( 13 );
    qrcode.align(image_promptpay,ALIGN_BOTTOM,0,5);
    image_logo.align(qrcode, ALIGN_CENTER);
    lb_bill_no.font(prasanmit_25, TFT_SILVER);
    lb_bill_no.align(qrcode, ALIGN_BOTTOM);

  // หน้า Thank You สร้างไว้ก่อนแต่ให้เป็น layer ที่ซ่อนไว้
  rect_thankyou.hidden(true);
  rect_thankyou.size(GScreen);  // ให้ขนาดใหญ่เต็มจอ
  rect_thankyou.color(TFT_BLUE, TFT_CYAN);
    rect_thankyou_box.size(350,200);
    rect_thankyou_box.border(5,TFT_COLOR_HSV(120,100,60));
    rect_thankyou_box.corner_radius(20);
    rect_thankyou_box.shadow(40,TFT_COLOR_HSV(120,100,20));
    rect_thankyou_box.shadow_offset(5,10);
    rect_thankyou_box.color(TFT_WHITE);
      lb_thankyou.font(prasanmit_40,TFT_BLACK);

  //-------------------------------------------------
  // GBPrimePay  
  GBPrimePay.init(MQTT_HOST, MQTT_PORT);
  GBPrimePay.setTopic(MQTT_TUNNEL_TOPIC,  MQTT_PAID_TOPIC);
  GBPrimePay.setToken(GBPRIMEPAY_TOKEN);

  // หาก WiFi เชื่อมต่อสำเร็จ ใช้แทน WIFI_CONNECTED() ที่จะใช้ไม่ได้หากใช้ GBPrimePay
  GBPrimePay.onWiFiConnected([](){
  });

  // หาก WiFi เชื่อมต่อสำเร็จ ใช้แทน WIFI_DISCONNECTED() ที่จะใช้ไม่ได้หากใช้ GBPrimePay
  GBPrimePay.onWiFiDisconnected([](){
  });

  // เมื่อ GBPrimePay พร้อมใช้งานแล้ว
  GBPrimePay.onInitialized([](){
    btn_qrcash.inactive(false);
  });

  // เมื่อ QRCash ส่ง ค่า qrcode มาให้
  GBPrimePay.onQRCash([](gbprimepay_qrcash_t gbp_response){
    Serial.printf("[GBPrimePay] QR Cash : %s\n", gbp_response.qrcode);
    lb_qrcash_status.hidden(true);
    lb_detail.hidden(false);
    rect_qr_payment.hidden(false);
    
    qrcode.encode( gbp_response.qrcode );
    lb_bill_no = String("NO.") + gbp_response.bill_no;   // นำรหัส bill มาใส่ค่าให้วิตเจ็ตข้อความ
    lb_detail = StringX::printf("%s\n%.2f บาท", gbp_response.bill_detail, gbp_response.amount);
  });

  // เมื่อ customer มีการชำระ
  GBPrimePay.onPaid([](gbprimepay_paid_t paid){
    Serial.println("[GBPrimePay] customer already paid");
    rect_thankyou.hidden(false); // หน้าขอบคุณแสดงขึ้นมา
    static GTimer timer;
    timer.setOnce(3000,[](){  // อีก 3วินาทีค่อยซ่อนหน้าต่างๆลงไปกลับสู่หน้าแรก
      rect_thankyou.hidden(true); 
      rect_qr_payment.hidden(true);
      lb_detail.hidden(true);
      lb_qrcash_status.hidden(true);
    });
  });
  
  // หากมี error เกิดขึ้น
  GBPrimePay.onError([](gbprimepay_error_t error){
    lb_qrcash_status.hidden(true);
    switch(error){
      case GBP_ERR_CONNECTION:
        Serial.println("[GBPrimePay] error : Connection Error");
        lb_qrcash_status.hidden(false);
        lb_qrcash_status.color(TFT_RED);
        lb_qrcash_status = "Connection Error";
        break;
      case GBP_ERR_REQ_QRCASH:
        Serial.println("[GBPrimePay] Error : qrcash request error");
        // อาจจะมีการส่งคำขอไปให้ หรือแจ้งเตือนให้ติดต่อผู้ให้บริการ
        break;
      case GBP_ERR_RESULT_QRCASH:
        Serial.println("[GBPrimePay] error : qrcash result error");
        // อาจจะมีการส่งคำขอไปให้ หรือแจ้งเตือนให้ติดต่อผู้ให้บริการ
        lb_qrcash_status.hidden(false);
        lb_qrcash_status.color(TFT_RED);
        lb_qrcash_status = "QRCash Result. Error";
        break;
      case GBP_EER_PAID_BILL_NO:
        Serial.println("[GBPrimePay] error : paid bill no not match");
        break;
      case GBP_ERR_PAID:
        Serial.println("[GBPrimePay] error : paid response error");
        // ให้แจ้งเตือน ....
        break;
    }
  });
}

void loop() {
  BlynkGO.update();
}
