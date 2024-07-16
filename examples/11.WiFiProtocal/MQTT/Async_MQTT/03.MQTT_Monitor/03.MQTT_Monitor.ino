#include <BlynkGOv3.h>
#include <BlynkGO_CircularBuffer.h>


GWiFiManager  wifi_manager;
GTextArea     ta_topic;
GButton       btn_subscribe;
GPage         page_monitor;
GLabel        label_monitor(page_monitor);
GLabel        lb_info;

String  mqtt_topic = "/BeeNeXT/mqtt/monitor";

BlynkGO_CircularBuffer<String> cbuf(30);      // ให้มีการจำเพียง 30 message

// ทดสอบส่ง ด้วย mosquitto_pub
// > mosquitto_pub -h broker.hivemq.com -p 1883 -t "/BeeNeXT/mqtt/monitor" -m "Hello"

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  if(BlynkGO.flashMem_exists("MQTT_TOPIC")){
    mqtt_topic = BlynkGO.flashMem_String("MQTT_TOPIC");
  }

  wifi_manager.align(ALIGN_TOP_RIGHT,-10);
  btn_subscribe= "Subscribe";
  btn_subscribe.font(prasanmit_25);
  btn_subscribe.height(ta_topic);
  btn_subscribe.radius(100);
  btn_subscribe.align(wifi_manager, ALIGN_LEFT,-10);
  btn_subscribe.onClicked([](GWIDGET){
    if(ta_topic.toString() == "") return;

    MQTT.unsubscribe(mqtt_topic);
    mqtt_topic = ta_topic.toString();
    MQTT.subscribe(mqtt_topic);
    BlynkGO.flashMem("MQTT_TOPIC", mqtt_topic);
  });

  ta_topic.width(btn_subscribe.top_left().x-20 );
  ta_topic.align(ALIGN_TOP_LEFT,10,10);
  ta_topic.cursor_type(CURSOR_NONE);
  ta_topic.placeholder("MQTT Topic");
  ta_topic = mqtt_topic;

  page_monitor.size(BlynkGO.width()-20, BlynkGO.height() - ta_topic.bottom_left().y- 20);
  page_monitor.radius(0);
  page_monitor.scrollbar(SCROLLBAR_AUTO, true, true);
  page_monitor.opa(0);
  page_monitor.align(ALIGN_BOTTOM,0,-10);
  page_monitor.border(1,TFT_SILVER);
  page_monitor.layout(LAYOUT_COL_L,10,10,10,10,0);
    label_monitor.color(TFT_WHITE);

  lb_info.font(prasanmit_20, TFT_WHITE);
  lb_info.body_draw(true);
  lb_info.body_color(TFT_BLUE);
  lb_info.align(page_monitor, ALIGN_IN_TOP_RIGHT,-10,10);
  lb_info.hidden(true);

  MQTT.setServer("broker.hivemq.com");
  MQTT.subscribe(mqtt_topic);
  MQTT.onMessage([](String topic, String message){
    cbuf.insert(message);
    String data="";
    for(int i=0; i< cbuf.count(); i++){
      int id = (cbuf.cur_id()-i) % cbuf.max_size();
      data += cbuf[id];
      if(i < cbuf.count()- 1 ) data += "\n";
    }
    Serial.println(data);
    label_monitor = data;
    page_monitor.scrollTop();
  });

  static SoftTimer timer;
  timer.setInterval(1000,[](){
    if(MQTT.connected()){
      MQTT.publish(mqtt_topic, String("My Data : ")+ String(random(1000)));
    }
  });
}

void loop() {
  BlynkGO.update();
}

MQTT_SUBSCRIBED(){
  Serial.println("[MQTT] subscribed");
  lb_info = " Subscribed ";
  lb_info.show(true);
  static SoftTimer timer;
  timer.delay(3000,[](){
    lb_info.show(false);
  });
}

MQTT_UNSUBSCRIBED(){
  Serial.println("[MQTT] unsubscribed");
}
