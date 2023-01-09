/***********************************************
 * ตัวอย่างการเชื่อมต่อ MQTT แบบ Syncronized (การเชื่อมต่อจะสะดุด)
 * ด้วย ไลบรารี่ PubSubClient  ดาวโหลดได้ที่ https://github.com/knolleary/pubsubclient 
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
#include <PubSubClient.h>

#define MQTT_HOST       "broker.hivemq.com" //"test.mosquitto.org" //"192.168.0.180"
#define MQTT_PORT       1883
#define MQTT_USER       ""
#define MQTT_PASSWORD   ""

WiFiClient    wifi_client;
PubSubClient  mqtt(wifi_client);

GWiFiManager wifi_manager;
GLabel       lb_mqtt;

uint32_t timer_mqtt_connection;
GTimer timer_mqtt_publish;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-10,5);
  lb_mqtt.color(TFT_WHITE);


  // กำหนด MQTT SERVER & PORT
  mqtt.setServer(MQTT_HOST, MQTT_PORT);

  // จุดรับข้อมูลจาก MQTT
  mqtt.setCallback([](char* tp, byte* data, unsigned int data_len){
    char buf[data_len]; memcpy(buf, data, data_len); buf[data_len] = '\0';
    String topic   = tp;
    String message = buf;
    // if( topic == "xxxx" ) {  // เช็คว่า topic ตรงที่ต้องการไหม
      Serial.printf("[MQTT] recieved : %s --> %s\n", topic.c_str(), message.c_str());
      lb_mqtt = message;
    // }
  });

  // ตั้งเวลา ในการส่งข้อมูลไปยัง MQTT
  timer_mqtt_publish.setInterval(2000,[](){
    static int c = 1;
    if(mqtt.connected()){
      String topic   = "BlynkGO/topic1";
      String message = "Hello #" + String(c++);
      Serial.printf("[MQTT] publish  : %s --> %s\n", topic.c_str(), message.c_str());

      // publish แบบปกติ
      mqtt.publish( topic.c_str(), message.c_str());

      // publish แบบ retain ค่าบน MQTT Broker
      // mqtt.publish( topic.c_str(), message.c_str(), true);
      
    }
  });
}

void loop() {
  BlynkGO.update();
  mqtt.loop();

  if( millis() > timer_mqtt_connection ) {
    timer_mqtt_connection = millis() + 5000;
    if(WiFi.isConnected() && !mqtt.connected()){
      BlynkGO.update();
      Serial.printf("[MQTT] connecting to '%s:%d' ...\n", MQTT_HOST, MQTT_PORT);
      String mqtt_client_id = "MQTT_CID_" + String(random(0xFFFF),HEX);

      bool ret = false;
      if(String(MQTT_USER) == "") {
        ret = mqtt.connect(mqtt_client_id.c_str());
      }else{
        ret = mqtt.connect(mqtt_client_id.c_str(), MQTT_USER, MQTT_PASSWORD);
      }
      if(ret) {
        Serial.println("[MQTT] connected & subscribe");
        
        // จะ subscribe อะไร ให้วางตรงนี้ ---------
        mqtt.subscribe("BlynkGO/topic1");

        // subscribe แบบ กำหนด QOS
        // uint8_t QOS = 0;   // 0, 1, 2
        // mqtt.subscribe("BlynkGO/topic1", QOS);

      }else{
        Serial.println("[MQTT] ERR :  connection fail!");
      }
    }
  }
}

