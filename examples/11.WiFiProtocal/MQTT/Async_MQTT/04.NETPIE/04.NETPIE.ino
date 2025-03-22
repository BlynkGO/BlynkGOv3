#include <BlynkGOv3.h>

#define NETPIE_CLIENT_ID    "---------------------------------"
#define NETPIE_TOKEN        "---------------------------------"
#define NETPIE_SECRET       "---------------------------------"

GWiFiManager wifi_manager;
GLabel lb_clock;

/* สำหรับแสดง mqtt connection */
GRect rect_mqtt_connection;
  GLabel lb_mqtt_connection(rect_mqtt_connection);

/* สำหรับแสดงค่า mqttsub ที่เข้ามา */
GCircle cir_mqttsub_status;
  GLabel lb_mqttsub_status(cir_mqttsub_status);

GLed led;

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  // GWiFiSetting::clean_WiFiConfig();      // สำหรับ clean WiFiConfig ที่เคยตั้งไว้ พร้อมตัด WiFi และ ล้าง catchบน ESP32
  wifi_manager.align(ALIGN_TOP_RIGHT);
  lb_clock.align(wifi_manager, ALIGN_LEFT,-10);

  rect_mqtt_connection.size(80,60);
  rect_mqtt_connection.radius(RADIUS_CIRCLE);
  rect_mqtt_connection.color(TFT_RED);
  rect_mqtt_connection.align(wifi_manager, ALIGN_BOTTOM_RIGHT, -10, 20);
  rect_mqtt_connection.onClicked([](GWIDGET){
    MQTT.connected()? MQTT.disconnect() : MQTT.connect();
  });
    lb_mqtt_connection = "NETPIE";
  
  cir_mqttsub_status.radius(50);
  cir_mqttsub_status.border(4, TFT_GREEN);
  cir_mqttsub_status.color(TFT_VIOLET);
    lb_mqttsub_status.font(prasanmit_40, TFT_WHITE);

  led.size(40,40);
  led.color(TFT_RED);
  led.align(ALIGN_TOP_LEFT, 20, 20);
  led.clickable(true);
  led.onClicked([](GWIDGET){
    led.toggle();
  });


  MQTT.setNetPie(NETPIE_CLIENT_ID, NETPIE_TOKEN, NETPIE_SECRET);
  MQTT.onMessage([](String topic, String message){
    Serial.printf("[MQTT] %s --> %s\n", topic.c_str(), message.c_str());

  });

  static SoftTimer timer_mqtt_publish;
  timer_mqtt_publish.setInterval(1000,[](){
    if(MQTT.connected()){
      float temp  = random(10000)/100.00;
      float humid = random(10000)/100.00;
      bool  led_state= led.isON();

      // จัด json สำหรับ shadow data ที่จะส่งไปให้ netpie
      String topic   = "@shadow/data/update";
      String message = StringX::printf( R"(
      {"data": { "temperature": %.2f, "humidity": %.2f, "led": %d, "timestamp":%d}}
      )", temp, humid, led_state, timestamp());
      message.trim();  // ลบ whitespaces หน้าหลังออก

      MQTT.publish(topic, message);
      Serial.printf("[NETPIE] pub : %s\n", message.c_str());
    }
  });
}

void loop(){
  BlynkGO.update();
}

NTP_SYNCED(){
  static SoftTimer timer_clock;
  timer_clock.setInterval(1000,[](){
    lb_clock = StringX::printf("%02d:%02d:%02d", hour(),minute(),second());
  });
}

WIFI_CONNECTED(){
}

WIFI_DISCONNECTED(){
}

MQTT_CONNECTED(){
  Serial.println("[MQTT] connected!");
  rect_mqtt_connection.color(TFT_BLUE);
}

MQTT_DISCONNECTED(){
  Serial.println("[MQTT] disconnected!");
  rect_mqtt_connection.color(TFT_RED);
}

MQTT_SUBSCRIBED(){
  Serial.println("[MQTT] subscribed!");
}



