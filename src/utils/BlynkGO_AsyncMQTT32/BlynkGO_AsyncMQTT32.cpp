#include "BlynkGO_AsyncMQTT32.h"
#include <Ticker.h>
#include <algorithm>
#include <memory>

#if BLYNKO_USE_ASYNC_MQTT32

void MQTTNoOpCbk() {}
MQTT_CONNECTED()    __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_DISCONNECTED() __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_SUBSCRIBED()   __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_UNSUBSCRIBED() __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_PUBLISHED()    __attribute__((weak, alias("MQTTNoOpCbk")));

static const char *TAG = "ASYNC_MQTT32";

BlynkGO_AsyncMQTT32 MQTT;
BlynkGO_AsyncMQTT32* _pMQTTClient32 = NULL;
static Ticker ticker_reconnect;

BlynkGO_AsyncMQTT32::BlynkGO_AsyncMQTT32(){
  this->subscribe_topics.clear();

  this->_mqtt_conf.user_context = (void*) this;
  _pMQTTClient32 = this;
}

BlynkGO_AsyncMQTT32::~BlynkGO_AsyncMQTT32(){
  this->stop();
}

//  mqtt://mqtt.eclipseprojects.io:1884
//  mqtt://username:password@mqtt.eclipseprojects.io:1884
void BlynkGO_AsyncMQTT32::setServer(String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;
  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;

  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "mqtt://%s:%u", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "mqtt://%s:%s@%s:%u", user.c_str(), password.c_str(), this->_host, this->_port);
  }
  Serial.printf("[MQTT] connect by TCP >  %s\n",this->_uri);

  this->_mqtt_conf.uri = _uri;
}

void BlynkGO_AsyncMQTT32::setServerSecure( String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;
  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;

  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "mqtts://%s:%u", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "mqtts://%s:%s@%s:%u", user.c_str(), password.c_str(), this->_host, this->_port);
  }
  
  Serial.printf("[MQTT] connect by SSL/TLS >  %s\n",this->_uri);

  this->_mqtt_conf.uri = _uri;
}

//  ws://mqtt.eclipseprojects.io:80/mqtt
void BlynkGO_AsyncMQTT32::setWebSocket( String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;

  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;
  
  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "ws://%s:%u/mqtt", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "ws://%s:%s@%s:%u/mqtt", user.c_str(), password.c_str(), this->_host, this->_port);
  }

  Serial.printf("[MQTT] connect by WebSocket >  %s\n",this->_uri);
  
  this->_mqtt_conf.uri = _uri;
}

//wss://mqtt.eclipseprojects.io:443/mqtt
void BlynkGO_AsyncMQTT32::setWebSocketSecure( String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;
  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;
  
  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "wss://%s:%u/mqtt", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "wss://%s:%s@%s:%u/mqtt", user.c_str(), password.c_str(), this->_host, this->_port);
  }
  
  Serial.printf("[MQTT] connect by WebSocket Secure >  %s\n",this->_uri);
  
  this->_mqtt_conf.uri = _uri;
}


void BlynkGO_AsyncMQTT32::setCertPEM(const char * mqtt_cert_pem){
  this->_mqtt_conf.cert_pem = (const char *)mqtt_cert_pem;
}

void BlynkGO_AsyncMQTT32::use_global_ca_store(bool enable){
  this->_mqtt_conf.use_global_ca_store = enable;
}

void BlynkGO_AsyncMQTT32::connect(bool auto_reconnect){
  if(this->_mqtt_conf.uri == NULL) return;

  this->_auto_reconnect = auto_reconnect;
  ticker_reconnect.detach();

  this->stop();   // =  _stop() + _destroy()

  if( !_client_inited ) {
    this->_client = esp_mqtt_client_init(&this->_mqtt_conf); // ภายในมี malloc ---> ใช้คู่กับ _detroy() เพื่อคืน mem
    esp_mqtt_client_register_event(this->_client, (esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, BlynkGO_AsyncMQTT32::mqtt_event_handler, this->_client);
    esp_mqtt_client_start(this->_client);
    _client_inited = true;
  }
}

void BlynkGO_AsyncMQTT32::disconnect(){
  esp_mqtt_client_disconnect(this->_client);
}

void BlynkGO_AsyncMQTT32::reconnect(){
  esp_mqtt_client_reconnect(this->_client);
}

void BlynkGO_AsyncMQTT32::stop(){
  if( this->_client_inited ) {
    this->_stop();
    this->_destroy();
    this->_client_inited = false;
  }
}

void BlynkGO_AsyncMQTT32::_stop(){
  esp_mqtt_client_stop(this->_client);
}

void BlynkGO_AsyncMQTT32::_destroy(){
  esp_mqtt_client_destroy(this->_client);
}

int BlynkGO_AsyncMQTT32::subscribe(String topic, uint8_t qos){
  subscribe_topic_t new_subscribe_topic;
  snprintf(new_subscribe_topic.topic, sizeof(new_subscribe_topic.topic), "%s", (char*)topic.c_str());
  new_subscribe_topic.qos = qos;
  this->subscribe_topics.push_back( new_subscribe_topic );
  return subscribe_topics.size();
//  return esp_mqtt_client_subscribe(this->_client, (const char *)topic.c_str(), qos);
}

bool BlynkGO_AsyncMQTT32::unsubscribe(String topic){
  int ret = esp_mqtt_client_unsubscribe( this->_client, (const char *) topic.c_str() );
  if(ret !=-1){
    ESP_LOGI(TAG, "unsubscribe successful, topic=%s",  topic.c_str() );
    for( int i=0; i< this->subscribe_topics.size();i++){
      if( String(this->subscribe_topics[i].topic) == topic){
        this->subscribe_topics.erase(this->subscribe_topics.begin() + i);
        break;
      }
    }
    return true;
  }else{
    ESP_LOGI(TAG, "unsubscribe failed, topic=%s",  topic.c_str() );
    return false;
  }
}

void BlynkGO_AsyncMQTT32::unsubscribe_all(){
  for( int i=0; i< this->subscribe_topics.size();i++){
    esp_mqtt_client_unsubscribe( this->_client, (const char *) this->subscribe_topics[i].topic );
  }
  this->subscribe_topics.clear();
}

int BlynkGO_AsyncMQTT32::publish(String topic, String message, uint8_t qos, bool retain){
  return esp_mqtt_client_publish(this->_client, (const char *) topic.c_str(), (const char *)message.c_str(), message.length(), qos, retain);
}

int BlynkGO_AsyncMQTT32::publish(String topic, uint8_t* data, size_t data_len, uint8_t qos, bool retain){
  return esp_mqtt_client_publish(this->_client, (const char *) topic.c_str(), (const char *)data, data_len, qos, retain);  
}

void BlynkGO_AsyncMQTT32::mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
  ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
  esp_mqtt_event_handle_t   event   = (esp_mqtt_event_handle_t) event_data;
  esp_mqtt_client_handle_t  client  = (esp_mqtt_client_handle_t) event->client;

  BlynkGO_AsyncMQTT32* pMQTTClient32 = (BlynkGO_AsyncMQTT32*) (event->user_context);
  //Serial.println(pMQTTClient32->name);


//  int msg_id;
  switch ((esp_mqtt_event_id_t)event_id)
  {
    case MQTT_EVENT_CONNECTED:
    {
      ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
      Serial.println("[MQTT] connected");
      ticker_reconnect.detach();

      pMQTTClient32->_connected = true;

      MqttOnConnected();
      if( pMQTTClient32->_fn_onconnected != NULL) {
        pMQTTClient32->_fn_onconnected();
      }

      for(int i=0; i< pMQTTClient32->subscribe_topics.size(); i++){
        pMQTTClient32->subscribe_topics[i].msg_id = esp_mqtt_client_subscribe(client, pMQTTClient32->subscribe_topics[i].topic, pMQTTClient32->subscribe_topics[i].qos);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", pMQTTClient32->subscribe_topics[i].msg_id);
      }
      delay(1);
      break;
    }
    case MQTT_EVENT_DISCONNECTED:
      ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
      Serial.println("[MQTT] disconnected");
      // pMQTTClient32->disconnect();
      pMQTTClient32->_connected = false;

      MqttOnDisconnected();
      if( pMQTTClient32->_fn_ondisconnected != NULL) {
        pMQTTClient32->_fn_ondisconnected();
      }
      if( pMQTTClient32->auto_reconnect() ){
        if(WiFi.isConnected()) {
          ticker_reconnect.once_ms(5000,[]{
            if(WiFi.isConnected()) {
              Serial.println("[MQTT] reconnect...");
              // _pMQTTClient32->reconnect();
              _pMQTTClient32->connect();
            }
          });
        }else{
          ticker_reconnect.detach();
          ticker_reconnect.once_ms(500,[]{  // ถ่วงเวลาเพื่อรอให้ event_cb นี้ทำงานเรียบร้อยไปก่อนค่อย ทำลายด้วย stop()
            _pMQTTClient32->stop();
          });
        }
      }
      // pMQTTClient32->stop();
      delay(1);
      break;

    case MQTT_EVENT_SUBSCRIBED:
      ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

      MqttOnSubscribed();
      if( pMQTTClient32->_fn_onsubscribe != NULL) {
        pMQTTClient32->_fn_onsubscribe(event->msg_id);
      }
      delay(1);
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
      MqttOnUnsubscribed();
      if( pMQTTClient32->_fn_onunsubscribe != NULL) {
        pMQTTClient32->_fn_onunsubscribe(event->msg_id);
      }
      delay(1);
      break;
    case MQTT_EVENT_PUBLISHED:
      ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
      MqttOnPublished();
      if( pMQTTClient32->_fn_onpublish != NULL) {
        pMQTTClient32->_fn_onpublish(event->msg_id);
      }
      delay(1);
      break;
    case MQTT_EVENT_DATA:
    {
      ESP_LOGI(TAG, "MQTT_EVENT_DATA");
      std::unique_ptr<char[]> topic_buf(new char[event->topic_len + 1]);
      std::copy(event->topic, event->topic + event->topic_len, topic_buf.get());
      topic_buf[event->topic_len] = '\0';
      String topic(topic_buf.get());
      if( pMQTTClient32->_fn_onmessage != NULL) {        
        std::unique_ptr<char[]> message_buf(new char[event->data_len + 1]);
        std::copy(event->data, event->data + event->data_len, message_buf.get());
        message_buf[event->data_len] = '\0';
        String message(message_buf.get());
        pMQTTClient32->_fn_onmessage(topic, message);
      }
      if( pMQTTClient32->_fn_ondata != NULL) {
        pMQTTClient32->_fn_ondata(topic, (uint8_t*)event->data,  event->data_len);
      }
      delay(1);
      break;
    }
    case MQTT_EVENT_ERROR:
      ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
      if( pMQTTClient32->_fn_onerror != NULL) {
        pMQTTClient32->_fn_onerror();
      }
      delay(1);
      break;
    default:
      ESP_LOGI(TAG, "Other event id:%d", event->event_id);
      delay(1);
      break;
  }
}


#endif // #if BLYNKO_USE_ASYNC_MQTT32
