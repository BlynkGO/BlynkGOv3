/*******************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ MQTT
 * การส่งข้อมูลด้วย publish และ รับข้อมูล subscibe
 *
 * -------------------------------------------------
 * ก่อนใช้งาน WiFi และ MQTT
 * ให้กำหนด เปิด การใช้งาน WiFi และ MQTT ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI             1
 *     #define BLYNKGO_USE_ASYNC_MQTT32     1
 *
 * -------------------------------------------------
 * หมายเหตุ
 *   - สามารถเชื่อมต่อ MQTT ได้ตั้งแบบ TCP, SSL/TLS, WebSocket, WebSocket Secure
 *     และมี user/password ได้
 *   - หลัง Smart WiFi เชื่อมต่อ MQTT จะเชื่อมต่อให้อัตโนมัติ
 *   - MQTT ที่กำหนด subcribe เพิ่มไว้ จะเพิ่มให้อัตโนมัติ หลัง MQTT เชื่อมต่อ
 *
 * วิธีการหา Cert_PEM ของ MQTT HOST
 *   - ติดตั้ง openssl สำหรับ Windows  
 *   - ใช้คำสั่ง openssl s_client -connect broker.hivemq.com:8883 -showcerts  
 *     โดยเปลี่ยน HOST และ PORT ตามที่ใช้
 *   - แล้วนำค่า Cert_PEM ตั้งแต่ 
 *
 *         -----BEGIN CERTIFICATE-----
 *           ....
 *           ....
 *         -----END CERTIFICATE-----
 *
 *     มาสร้างเป็นตัวแปร const char* 
 *
 ******************************************************/

#include <BlynkGOv3.h>

/*********************************************************
 * MQTT : เชื่อมต่อแบบ TCP ธรรมดา
 *********************************************************/
#define MQTT_HOST       "broker.hivemq.com"
#define MQTT_PORT       1883
#define MQTT_USER       ""
#define MQTT_PASSWORD   ""

/*********************************************************
 * MQTT : เชื่อมต่อแบบ SSL/TSL  ( Server Secure )
 *********************************************************/
//#define MQTT_HOST       "broker.hivemq.com"
//#define MQTT_PORT       8883   // TLS/SSL
//#define MQTT_USER       ""
//#define MQTT_PASSWORD   ""
//
//// openssl s_client -connect broker.hivemq.com:8883 -showcerts
//extern const char* broker_hivemq_com_8883_cert_pem;     // MQTT Server Secure's Cert-PEM

/*********************************************************
 * MQTT : เชื่อมต่อแบบ WebSocket
 *********************************************************/
//#define MQTT_WEBSOCKET_HOST     "broker.hivemq.com"
//#define MQTT_WEBSOCKET_PORT     8000
//#define MQTT_WEBSOCKET_USER     ""
//#define MQTT_WEBSOCKET_PASS     ""

/*********************************************************
 * MQTT : เชื่อมต่อแบบ WebSocket Secure
 *********************************************************/
//#define MQTT_WEBSOCKET_HOST     "broker.hivemq.com"
//#define MQTT_WEBSOCKET_PORT     8884
//#define MQTT_WEBSOCKET_USER     ""
//#define MQTT_WEBSOCKET_PASS     ""
//
//// openssl s_client -connect broker.hivemq.com:8884 -showcerts
// extern const char* broker_hivemq_com_8884_cert_pem;     // MQTT WebSocket Secure's Cert-PEM


GWiFiManager wifi_manager;
GLabel       lb_clock;
GLabel       lb_mqtt_info;
GLabel       lb_mqtt_message;
GRect        rect_mqtt_status;
  GLabel     lb_mqtt_status("M",rect_mqtt_status);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-10);
  rect_mqtt_status.size(40,40);
  rect_mqtt_status.corner_radius(5);
  rect_mqtt_status.color(TFT_RED);
  rect_mqtt_status.border(2, TFT_RED);
  rect_mqtt_status.align(wifi_manager, ALIGN_LEFT, -10);
    lb_mqtt_status.font(prasanmit_40, TFT_WHITE);
  lb_clock.align(rect_mqtt_status, ALIGN_LEFT, -10);
  lb_mqtt_message.font(prasanmit_40);
  lb_mqtt_info.font(prasanmit_20, TFT_SILVER);

  /*********************************************************
   * MQTT : เชื่อมต่อแบบ TCP ธรรมดา
   *********************************************************/
  MQTT.setServer( MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASSWORD );

  /*********************************************************
   * MQTT : เชื่อมต่อแบบ SSL/TSL  ( Server Secure )
   *********************************************************/
//  MQTT.setServerSecure( MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASSWORD );
//  MQTT.setCertPEM( broker_hivemq_com_8883_cert_pem);

  /*********************************************************
   * MQTT : เชื่อมต่อแบบ WebSocket
   *********************************************************/
//  MQTT.setWebSocket( MQTT_WEBSOCKET_HOST, MQTT_WEBSOCKET_PORT, MQTT_WEBSOCKET_USER, MQTT_WEBSOCKET_PASS );

  /*********************************************************
   * MQTT : เชื่อมต่อแบบ WebSocket Secure
   *********************************************************/
//   MQTT.setWebSocketSecure( MQTT_WEBSOCKET_HOST, MQTT_WEBSOCKET_PORT, MQTT_WEBSOCKET_USER, MQTT_WEBSOCKET_PASS );
//   MQTT.setCertPEM( broker_hivemq_com_8884_cert_pem);

  //---------------------------------------


  MQTT.subscribe("/topic/test1");       // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ
  MQTT.onMessage([]( String topic, String message){
    Serial.printf("[MQTT] onMessage : %s --> %s\n",  topic.c_str(), message.c_str() );
    lb_mqtt_message = message;

    lb_mqtt_info = StringX::printf("mqtt topic : %s", topic.c_str());
    lb_mqtt_info.align(lb_mqtt_message, ALIGN_TOP_LEFT, 5);

    rect_mqtt_status.border(2, TFT_GREEN);
    static GTimer timer;
    timer.delay(500,[](){ rect_mqtt_status.border(2, TFT_VIOLET);  });
  });

//  /* หากต้องการรับข้อมูลเป็น byte เข้ามา */
//  MQTT.onData([]( String topic,  uint8_t* data, size_t len){
//    Serial.printf("[MQTT] onData    : %s --> %.*s\n", topic.c_str(), len, (char*) data );
//  });

  static GTimer timer_mqtt;
  timer_mqtt.setInterval(2000,[](){
    if(!MQTT.connected() ) return;
    
    static int c;
    MQTT.publish( "/topic/test1", String("Hello World : ")+ String(c++));
  });
}

void loop() {
  BlynkGO.update();
}

NTP_SYNCED(){
  static GTimer timer;
  timer.setInterval(1000,[](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(), minute(), second());
  });
}

// /* ใส่หรือไม่ก็ได้ */
MQTT_CONNECTED(){
  //Serial.println("[MQTT][4User] connected");
  rect_mqtt_status.border(2, TFT_VIOLET);
  rect_mqtt_status.color(TFT_VIOLET);
}

// /* ใส่หรือไม่ก็ได้ */
MQTT_DISCONNECTED(){
  //Serial.println("[MQTT][4User] disconnected");  
  rect_mqtt_status.border(2, TFT_RED);
  rect_mqtt_status.color(TFT_RED);
}

// /* ใส่หรือไม่ก็ได้ */
// MQTT_SUBSCRIBED(){
//   Serial.println("[MQTT][4User] subscribed");  
// }

// /* ใส่หรือไม่ก็ได้ */
// MQTT_UNSUBSCRIBED(){
//   Serial.println("[MQTT][4User] unsubscribed");  
// }

// /* ใส่หรือไม่ก็ได้ */
// MQTT_PUBLISHED(){
//   Serial.println("[MQTT][4User] published");  
// }

