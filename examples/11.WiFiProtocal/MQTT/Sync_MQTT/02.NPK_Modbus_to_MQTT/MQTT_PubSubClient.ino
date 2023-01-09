#include <PubSubClient.h>
#include <vector>
#include <string>

namespace SYNC_MQTT {
  WiFiClient    _wifi_client;
  PubSubClient  _mqtt(_wifi_client);
  String    _mqtt_host;
  uint16_t  _mqtt_port=1883;
  String    _mqtt_user="";
  String    _mqtt_pass="";
  void (*_fn)(String topic, String message)=NULL;
  uint32_t  _timer_mqtt_connection;
  std::vector<std::string> topic_list;

  void begin(String mqtt_host, uint16_t mqtt_port, String mqtt_user, String mqtt_pass ){
    _mqtt_host = mqtt_host;
    _mqtt_port = mqtt_port;
    _mqtt_user = mqtt_user;
    _mqtt_pass = mqtt_pass;
    topic_list.clear();
    _mqtt.setServer(_mqtt_host.c_str(), _mqtt_port);
    _mqtt.setCallback([](char* tp, byte* data, unsigned int data_len){
      char buf[data_len]; memcpy(buf, data, data_len); buf[data_len] = '\0';
      String topic   = tp;
      String message = buf;
      if( SYNC_MQTT::_fn !=NULL){
        SYNC_MQTT::_fn(topic, message);
      }
    });
  }
  bool connected(){
    return _mqtt.connected();
  }
  void onData(void (*fn)(String topic, String message)){ _fn = fn;}

  void subscribe(String topic){
    std::string _topic = topic.c_str();
    topic_list.push_back(_topic);
  }
  
  void publish(String topic, String message){
    _mqtt.publish(topic.c_str(), message.c_str());
  }

  void handle(){
    _mqtt.loop();
    if( millis() > _timer_mqtt_connection ) {
      _timer_mqtt_connection = millis() + 5000;
      if(WiFi.isConnected() && !_mqtt.connected()){
        BlynkGO.update();
        Serial.printf("[SYNC_MQTT] connecting to '%s:%d' ...\n", MQTT_HOST, MQTT_PORT);
        randomSeed(millis());
        String mqtt_client_id = "MQTT_CID_" + String(random(0xFFFF),HEX);
  
        bool ret = false;
        if( _mqtt_user != "") {
          ret = _mqtt.connect(mqtt_client_id.c_str(), _mqtt_user.c_str(), _mqtt_pass.c_str());
        }else{
          ret = _mqtt.connect(mqtt_client_id.c_str());
        }
        
        if (ret){
          Serial.println("[SYNC_MQTT] connected & subscribe");        
          for(int i=0; i < topic_list.size(); i++){
            _mqtt.subscribe(topic_list[i].c_str());
          }
  
        }else{
          Serial.println("[SYNC_MQTT] error : connection fail!");
        }
      }
    }
  }
}