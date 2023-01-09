/*******************************************************
 * ตัวอย่างพื้นฐาน การเชื่อมต่อ MQTT
 * การส่งข้อมูลด้วย publish และ รับข้อมูล subscibe
 *
 * ---------------------------------------
 * ก่อนใช้งาน WiFi และ MQTT
 * ให้กำหนด เปิด การใช้งาน WiFi และ NTP ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_WIFI          1
 *     #define BLYNKGO_USE_MQTT          1
 *
 ******************************************************/


#include <BlynkGOv3.h>

#define MQTT_SERVER     "broker.hivemq.com" // "test.mosquitto.org"
#define MQTT_PORT       1883

GWiFiManager  wifi_manager;
GTimer        mqtt_pub_timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-5,5);

  //------------------------------------
  MQTT.setServer(MQTT_SERVER, MQTT_PORT);
  MQTT.onMessage([](char* topic, char* data, MessageProperties properties, size_t len, size_t index, size_t total){
    static String  payload;
    if(index == 0) payload = "";
    char buf[len]; memcpy(buf, data, len); buf[len]=0; payload += buf;
    
    if( index + len >= total) {
      Serial.printf("[MQTT] <message recv>  topic : %s -> payload : %s\n", topic, payload.c_str() );
    }
  });

  // ตั้งเวลาในการ publish ไปยัง MQTT
  mqtt_pub_timer.setInterval(2000, [](){
    static int16_t counter=0;
    if(MQTT.connected()){
      // publish แบบปกติ ไม่มีการจำค่าบน MQTT Broker
      MQTT.publish("/MyMQTT/data", String("Hello MQTT ") + String(counter++));
      
      // publish แบบ retain จะมีการจำค่าบน MQTT Broker
      // MQTT.publish("/MyMQTT/data", String("Hello MQTT ") + String(counter++), QOS0, true);
    }
  });
}

void loop() {
  BlynkGO.update();
}

/* ใส่หรือไม่ก็ได้ */
WIFI_CONNECTED(){
  Serial.println("[WiFi] connected");
}

/* ใส่หรือไม่ก็ได้ */
WIFI_DISCONNECTED(){
  Serial.println("[WiFi] disconnected");  
}

/* ใส่หรือไม่ก็ได้ หากจะ subscribe ให้มีด้วย */
MQTT_CONNECTED(){
  Serial.println("[MQTT] connected");
  Serial.println("[MQTT] subscribe topic");
  MQTT.subscribe("/MyMQTT/data");                 // กำหนด subscribe topic
  
  // subscribe แบบกำหนด QOS
  // MQTT.subscribe("/MyMQTT/data", QOS0); 
}

/* ใส่หรือไม่ก็ได้ */
MQTT_DISCONNECTED(){
  Serial.println("[MQTT] disconnected");  
}

/* ใส่หรือไม่ก็ได้ */
MQTT_SUBSCRIBED(){
  Serial.println("[MQTT] subscribed");  
}

/* ใส่หรือไม่ก็ได้ */
MQTT_UNSUBSCRIBED(){
  Serial.println("[MQTT] unsubscribed");  
}

/* ใส่หรือไม่ก็ได้ */
MQTT_PUBLISHED(){
  Serial.println("[MQTT] published");  
}

