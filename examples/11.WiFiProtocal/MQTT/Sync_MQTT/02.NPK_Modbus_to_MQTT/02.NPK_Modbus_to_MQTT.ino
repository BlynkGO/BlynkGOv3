/***********************************************
 * ตัวอย่าง การอ่านค่า NPK sensor MODBUS rs485 
 * และทำการส่งและรับข้อมูล MQTT แบบ Syncronized (การเชื่อมต่อจะสะดุด)
 * ด้วย ไลบรารี่ PubSubClient  ดาวโหลดได้ที่ https://github.com/knolleary/pubsubclient 
 * มาแสดงผลบนหน้าจอ TFT
 *
 * ก่อน compile ให้เปิดใช้งาน WIFI ที่ blynkgo_config.h
 * 
 *     #define BLYNKGO_USE_WIFI     1
 *     
 *  และหาก MQTT แบบ ไม่สะดุด หากเปิดใช้อยู่ ให้ปิดการใช้งานเสียก่อน
 *  จะได้ไม่ชนกัน
 *  
 *     #define BLYNKGO_USE_MQTT     0
 * 
 * หมายเหตุ
 * -------
 *   การเชื่อมต่อแบบนี้ หาก การเชื่อมต่อ MQTT มีปัญหา
 *   ยังเชื่อมต่อไม่สำเร็จ อุปกรณ์รอบข้างอื่นๆ รวมทั้ง ระบบสัมผัสหน้าจอ
 *   จะไม่สามารถทำงานได้
 */

#include <BlynkGOv3.h>

#define MQTT_HOST         "broker.hivemq.com" //"test.mosquitto.org" //"192.168.0.180"
#define MQTT_PORT         1883
#define MQTT_USER         ""
#define MQTT_PASSWORD     ""

#if defined (BLYNKGO_BOARD_V1_1)
  #define MODBUS_RX       22    // MODBUS_R0
  #define MODBUS_TX       25    // MODBUS_DI
  #define MODBUS_DE        5
  #define MODBUS_RE       21
#elif defined (BLYNKGO_EMMA_II)
  #define MODBUS_RX       25    // MODBUS_R0
  #define MODBUS_TX       26    // MODBUS_DI
  #define MODBUS_DE        2
  #define MODBUS_RE        4
#endif


IMG_DECLARE(img_background_npk);

namespace SYNC_MQTT {
  void begin(String mqtt_host, uint16_t mqtt_port, String mqtt_user, String mqtt_pass );
  void onData(void (*fn)(String topic, String message));
  void publish(String topic, String message);
  void subscribe(String topic);
  bool connected();
  void handle();
}

namespace NPK { 
  void begin(uint8_t rx, uint8_t tx, uint8_t de, uint8_t re); 
  byte nitrogen(); 
  byte phosphorous(); 
  byte potassium();
}

GImage        background(img_background_npk);
GWiFiManager  wifi_manager;

GLabel        lb_nitrogen;
GLabel        lb_phosphorous;
GLabel        lb_potassium;

GTimer        timer_npk;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  //----------------------------------------------------
  // Graphic Design
  background.align_center();
  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);
  wifi_manager.color(TFT_BLUE, TFT_RED, TFT_DARKGRAY);
  
  lb_nitrogen   .font(prasanmit_40, TFT_BLACK); lb_nitrogen   .position(225,  15);
  lb_phosphorous.font(prasanmit_40, TFT_BLACK); lb_phosphorous.position(225, 105);
  lb_potassium  .font(prasanmit_40, TFT_WHITE); lb_potassium  .position(225, 195);

  //----------------------------------------------------
  // SYNCRONIZED MQTT
  SYNC_MQTT::begin(MQTT_HOST, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);
  SYNC_MQTT::onData([](String topic, String message){
    Serial.printf("[MQTT] recieved : %s --> %s\n", topic.c_str(), message.c_str());

    // เทียบ topic เพื่อนำ message ไปกำหนดให้ Widgt ตามต้องการ
    if(topic == "BlynkGO/nitrogen" ){
      lb_nitrogen     = message;
    }else
    if(topic == "BlynkGO/phosphorous" ){
      lb_phosphorous  = message;      
    }else
    if(topic == "BlynkGO/potassium" ){
      lb_potassium    = message;      
    }
  });

  SYNC_MQTT::subscribe("BlynkGO/nitrogen");
  SYNC_MQTT::subscribe("BlynkGO/phosphorous");
  SYNC_MQTT::subscribe("BlynkGO/potassium");


  //----------------------------------------------------
  // NPK Modbus Sensor
  NPK::begin(MODBUS_RX, MODBUS_TX, MODBUS_DE, MODBUS_RE);
  timer_npk.setInterval(5000,[](){
    byte n = NPK::nitrogen();
    byte p = NPK::phosphorous();
    byte k = NPK::potassium();

    if(SYNC_MQTT::connected() ) {
      Serial.printf("[MQTT] publish NPK : %d : %d : %d\n", n,p,k);
      SYNC_MQTT::publish("BlynkGO/nitrogen"    , String(n) );
      SYNC_MQTT::publish("BlynkGO/phosphorous" , String(p) );
      SYNC_MQTT::publish("BlynkGO/potassium"   , String(k) );
    }
  });
}

void loop() {
  BlynkGO.update();
  SYNC_MQTT::handle();
}
