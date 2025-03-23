/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ BlynkGO_AsyncMQTT32.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
 *   สร้างโดย BlynkGO
 *   
 *   2.ไม่อนุญาต ให้แจกจ่าย สำเนา หรือแก้ไข โดยไม่ได้รับอนุญาต 
 *   
 *   3.สำหรับผู้ได้รับ ลิขสิทธิ์ สามารถนำไปใช้สร้าง firmware/rom 
 *   สำหรับ บอร์ด ESP32 ที่ระบุเท่านั้น เพื่อใช้ทางการค้าต่างๆได้
 *   โดยห้ามแจกจ่าย จำหน่าย ดัดแปลง แก้ไขใดๆ ในตัว source ไลบรารี่ออกไป
 *
 *   4.หากมีการนำไปใช้คอมไพล์ถือว่าได้ยอมรับ ตามเงื่อนไขลิขสิทธิ์ดังกล่าว
 *   เป็นที่เรียบร้อยแล้ว
 * 
 * [History]
 *   [V1.0.3] @13/07/24
 *      - ปรับปรุง BlynkGO_AsyncMQTT32 connection 
 *        แก้ไขปัญหา BlynkGO_AsyncMQTT32 เมื่อเชื่อมต่อได้แล้ว แต่พอเข้าไป GWiFiManager เพื่อปิด SSID 
 *        มีอาการตาย reset ตัวเอง  และ เมื่อปิดแล้วแก้ได้แล้วแต่พอจะเปิดใหม่มีอาการหน่วงๆ  ได้แก้ทั้ง 2 issues นี้แล้ว
 * 
 *   [V1.0.4] @16/07/24
 *      - ปรับปรุง subscribe(...)
 *        หากยังไม่เชื่อมต่อ จะจำไว้ใน list พอเชื่อมต่อแล้ว subscribe จริงไล่ทั้งหมดให้อัตโนมัติ
 *        แต่หากเชื่อมต่อแล้ว เกิดมีเพิ่ม subscribe ให้ เพิ่มใน list และ ให้ subscribe จริงทันทีเลย
 * 
 *   [V1.0.5] @14/08/24
 *      - แก้ไข ส่วนเมื่อ mqtt มีสถานะ disconnect ใหม่
 *        หากโหลด auto_reconnect ทำงาน จะใช้ MQTT.reconnect อย่างเดียว
 *        โดยให้มีการลอง 5 ครั้งแล้ว เว้น 10วินาที ค่อยลองใหม่ 5 ครั้ง แทนของเดิมที่ใช้ ตัดไปเลยแล้ว MQTT.connect()
 * 
 *   [V1.0.6] @12/02/25
 *      - fix  ::disconnected()  เช็ค ว่า client != NULL เพิ่ม
 * 
 *   [V1.0.7] @01/03/25
 *      - fix :: onMessage ที่ใช้ std::unique_ptr ทำ topic, message แล้ว เมื่อไปใส่ GLabel ข้อมูลหายแล้วค้างได้ ใช้ std::string จัดการแทน
 * 
 *   [V1.0.8] @06/03/25
 *      - ปรับปรุง การเชื่อมต่อ กรณีที่ MQTT Broker ไม่มีอยู่จริง / เชื่อมต่อไม่ได้ สักพัก มีอาการ reset ตัวเอง
 *        มีการปิด mqtt config ให้มี disable_auto_reconnect เป็น true จะได้ทำลาย _stop/_destroy ได้
 *      - สามารถให้ โหมด auto_reconnect ได้ ซึ่งถ้าเชื่อมไม่ได้ หรือ mqtt broker ไม่มีอยู่จริง ก็จะไม่ตาย
 *        จะมีการ try รองเชื่อม 5 รอบ แล้วหยุดไป 10 วินาที ค่อย try ใหม่อีก 5 รอบ ก็ไม่ตาย
 *      - และหากโหมดปิด auto_reconnect ก็จะรองเชื่อม 1 รอบ (หากมีวางที่ WIFI_CONNECTED ให้ mqtt.connect() )
 *        หากเชื่อมไม่ได้ ก็จะ mqtt ก็จะไม่เชื่อมต่ออีก (ไว้สำหรับตอน ควบคุม manual mqtt connection เอง)
 * 
 *   [V1.0.9] @09/03/25
 *      - แก้ ตอน WiFi หลุดแล้ว พอต่อใหม่ MQTT ภายในทำลาย น่าจะผิดสักอย่าง เลยเกิด watchdog eror
 *        ได้ ปิด ไม่ต้องไปเรียก MQTT.disconnect() หลัง WiFi DISCONNECTED
 *
 *   [V1.0.10] @14/03/25
 *      - ยกเลิก stop() เดิม ที่เป็น _stop() + _destroy() 
 *      - ยกเลิก disconnect() เดิม  เปลี่ยนเป็น _disconnect()
 *      - disconnect() ใหม่ เป็น _stop() + _destroy()
 *      - _stop() คือ esp mqtt stop จาก IDF  (ภายในเขาถ้ามีการเชื่อมต่ออยู่ จะมีการ _disconnect ภายใน ใช้หลังจาก esp mqtt start)
 *      - _destroy() คือ esp mqtt destroy จาก IDF (ภายในเขา ไว้ทำลอย esp mqtt init คืนเม็ม)
 *      - user บนๆ ใช้ disconnect() ใหม่ ที่เป็นการ ตัดการเชื่อมต่อ และ ทำลาย ลง เรียกว่า ล้างหมด
 *      - ยกเลิก connect(bool auto_reconnect) เดิมออกไป
 *        ยกเลิก connect() เดิมออกไป
 *        เปลี่ยน เป็น connect(bool clean_session=true) แทน     หากใช้ QoS1 หรือ QoS2 อาจไม่ต้อง clean_session เกิดมีการเริ่มใหม่ได้
 * 
 *   [V1.0.11] @14/03/25
 *      - onMessage(...) ย้ายไปทำงานใน loop() แทน
 *
 *   [V1.0.12] @16/03/25
 *      - กลับใช้ autoreconnect ของ mqtt idf คืนมา แล้ว ให้ _init คงไว้แต่แรก ไม่ต้อง _destroy ไปทำลาย
 *        ควบคุมแค่ _start _stop พอ
 *
 *   [V1.0.13] @22/03/25
 *      - ออกแบบใหม่ให้ event มา เก็บไว้ แล้วย้ายไปทำงานใน loop แทน ผ่าน SoftTimer
 *        เพื่อแก้ปัญหา บางที eventถูกทำลายไปก่อนที่ graphic จะวาดจริงเสร็จ (ที่เกิดใน ตอน lv_timer_handle(..)) ทำให้กราฟิกไปค้าง
 *      - buf_data, buf_data_len มีการเก็บ dataย้ายออกมา
 *
 *********************************************************************
 */

#include "BlynkGO_AsyncMQTT32.h"
#include <algorithm>
#include <memory>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"  // mutex

// #include "esp_task_wdt.h"

#if BLYNKGO_USE_ASYNC_MQTT32

void MQTTNoOpCbk() {}
MQTT_CONNECTED()    __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_DISCONNECTED() __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_SUBSCRIBED()   __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_UNSUBSCRIBED() __attribute__((weak, alias("MQTTNoOpCbk")));
MQTT_PUBLISHED()    __attribute__((weak, alias("MQTTNoOpCbk")));

static const char *TAG = "ASYNC_MQTT32";

BlynkGO_AsyncMQTT32      MQTT;
BlynkGO_AsyncMQTT32*     _pMQTTClient32 = NULL;
static SoftTimer         _timer_reconnect;
static SemaphoreHandle_t _event_mutex;            // ประกาศ mutex

BlynkGO_AsyncMQTT32::BlynkGO_AsyncMQTT32(){

  this->_mqtt_conf.user_context = (void*) this;
  _pMQTTClient32 = this;

  _event_mutex = xSemaphoreCreateMutex();  // สร้าง mutex
  if (_event_mutex == NULL) {
    Serial.println("[MQTT32] Failed to create mutex");
  }

  this->_subscribe_topics.clear();
  this->_event_list.clear();
  this->_topic= "";
  this->_data.clear();
  this->_sub_msg_ids.clear();
  this->_unsub_msg_ids.clear();
  this->_pub_msg_ids.clear();
}

BlynkGO_AsyncMQTT32::~BlynkGO_AsyncMQTT32(){
  this->_subscribe_topics.clear();
  this->_event_list.clear();
  this->_topic= "";
  this->_data.clear();
  this->_sub_msg_ids.clear();
  this->_unsub_msg_ids.clear();
  this->_pub_msg_ids.clear();
}

//  mqtt://mqtt.eclipseprojects.io:1884
//  mqtt://username:password@mqtt.eclipseprojects.io:1884
void BlynkGO_AsyncMQTT32::setServer(String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;
  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;

  char _uri_[128];
  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "mqtt://%s:%u", this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "mqtt://%s:%u", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "mqtt://%s:%s@%s:%u", user.c_str(), password.c_str(), this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "mqtt://[USER]:[PASS]@%s:%u", this->_host, this->_port);
  }
  Serial.printf("[MQTT] connect by TCP >  %s\n",_uri_);

  this->_mqtt_conf.uri = _uri;
}

void BlynkGO_AsyncMQTT32::setServerSecure( String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;
  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;

  char _uri_[128];
  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "mqtts://%s:%u", this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "mqtts://%s:%u", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "mqtts://%s:%s@%s:%u", user.c_str(), password.c_str(), this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "mqtts://[USER]:[PASS]@%s:%u", this->_host, this->_port);
  }
  Serial.printf("[MQTT] connect by SSL/TLS >  %s\n",_uri_);

  this->_mqtt_conf.uri = _uri;
}

//  ws://mqtt.eclipseprojects.io:80/mqtt
void BlynkGO_AsyncMQTT32::setWebSocket( String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;

  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;
  
  char _uri_[128];
  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "ws://%s:%u/mqtt", this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "ws://%s:%u/mqtt", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "ws://%s:%s@%s:%u/mqtt", user.c_str(), password.c_str(), this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "ws://[USER]:[PASS]@%s:%u/mqtt", this->_host, this->_port);
  }
  Serial.printf("[MQTT] connect by WebSocket >  %s\n",_uri_);
  
  this->_mqtt_conf.uri = _uri;
}

//wss://mqtt.eclipseprojects.io:443/mqtt
void BlynkGO_AsyncMQTT32::setWebSocketSecure( String host, uint16_t port, String user, String password){
  snprintf(this->_host, sizeof(this->_host), "%s", host.c_str());
  _port = port;
  this->_mqtt_conf.host = _host;
  this->_mqtt_conf.port = _port;
  
  char _uri_[128];
  if( user == "" ) {
    snprintf(this->_uri, sizeof(this->_uri), "wss://%s:%u/mqtt", this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "wss://%s:%u/mqtt", this->_host, this->_port);
  }else{
    snprintf(this->_uri, sizeof(this->_uri), "wss://%s:%s@%s:%u/mqtt", user.c_str(), password.c_str(), this->_host, this->_port);
    snprintf(_uri_     , sizeof(_uri_)     , "wss://[USER]:[PASS]@%s:%u/mqtt", this->_host, this->_port);
  }
  Serial.printf("[MQTT] connect by WebSocket Secure >  %s\n",_uri_);
  
  this->_mqtt_conf.uri = _uri;
}

void BlynkGO_AsyncMQTT32::setCertPEM(const char * mqtt_cert_pem){
  // esp_task_wdt_reset();
  this->_mqtt_conf.cert_pem = (const char *)mqtt_cert_pem;
}


void BlynkGO_AsyncMQTT32::use_global_ca_store(bool enable){
  // esp_task_wdt_reset();
  this->_mqtt_conf.use_global_ca_store = enable;
}

void BlynkGO_AsyncMQTT32::connect(bool clean_session ){
  // esp_task_wdt_reset();
  if(this->_mqtt_conf.uri == NULL) return;

  _timer_reconnect.del();
  this->_auto_reconnect_try = 5;

  if( !this->_client_inited ) this->_init(clean_session);  
  if( this->_client_inited )  this->_start();
}

void BlynkGO_AsyncMQTT32::disconnect(){
  // esp_task_wdt_reset();
  if( this->_client_inited ) {
    this->_stop();
    this->_destroy();
    static SoftTimer timer;
    timer.delay(10,[](){
      MqttOnDisconnected();
      if(_pMQTTClient32->_fn_ondisconnected != NULL){
        _pMQTTClient32->_fn_ondisconnected();
      }
    });
  }
}

void BlynkGO_AsyncMQTT32::_init(int8_t clean_session){
  // esp_task_wdt_reset();
  if(this->_client_inited) return;
  if(this->_client != NULL) return;

  // this->_mqtt_conf.disable_auto_reconnect  = true;
  if( clean_session == -1) clean_session = _clean_session;

  if( clean_session == false) {
    this->_mqtt_conf.disable_clean_session = true;
  }else{
    this->_mqtt_conf.disable_clean_session = false;
  }
  if(this->_client_id.size() > 0) {
    this->_mqtt_conf.client_id = this->_client_id.c_str();
  }

  this->_client = esp_mqtt_client_init(&this->_mqtt_conf);
  if( this->_client != NULL ){
    esp_mqtt_client_register_event(this->_client, (esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, BlynkGO_AsyncMQTT32::mqtt_event_handler, this->_client);
    this->_client_inited = true;
  }else {
    Serial.println("[MQTT] Failed to initialize MQTT client!");  // เพิ่มข้อความเมื่อสร้าง client ไม่สำเร็จ
  }
}

void BlynkGO_AsyncMQTT32::reconnect(){
  // esp_task_wdt_reset();
  if(!this->_client_inited ) return;
  if(!this->_client_started) return;
  if(!this->connected())  {
    Serial.println("[MQTT] reconnect");
    esp_mqtt_client_reconnect(this->_client);
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // ปรับค่า delay ตามที่ต้องการ
  }
}

void BlynkGO_AsyncMQTT32::_start(){
  // esp_task_wdt_reset();
  if(!this->_client_inited) return;
  if(this->_client_started) return;
  if (this->connected()) {
    Serial.println("[MQTT] Already connected.");
    return;
  }
  esp_mqtt_client_start(this->_client);
  this->_client_started =true;

  static SoftTimer timer_event_handler;
  timer_event_handler.setInterval(1,[](){
    if (xSemaphoreTake(_event_mutex, portMAX_DELAY)) {
      if(_pMQTTClient32->_event_list.size()){
        // ดึง event_id ตัวแรกจาก vector
        esp_mqtt_event_id_t event_id = _pMQTTClient32->_event_list.front();
        // ลบข้อมูลตัวแรกออกจาก vector
        _pMQTTClient32->_event_list.erase(_pMQTTClient32->_event_list.begin());
        switch ((esp_mqtt_event_id_t)event_id)
        {
          case MQTT_EVENT_CONNECTED:
          {
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            Serial.println("[MQTT32] connected");
            _timer_reconnect.del();
            _pMQTTClient32->_connected = true;
            _pMQTTClient32->_auto_reconnect_try = 5;
            _pMQTTClient32->_topic = "";
            _pMQTTClient32->_data.clear();
            _pMQTTClient32->_sub_msg_ids.clear();
            _pMQTTClient32->_unsub_msg_ids.clear();
            _pMQTTClient32->_pub_msg_ids.clear();

            if( _pMQTTClient32->_fn_onconnected != NULL) {
              _pMQTTClient32->_fn_onconnected();
            }

            Serial.println("[MQTT32] subscribing...");
            for(int i=0; i< _pMQTTClient32->_subscribe_topics.size(); i++){
              _pMQTTClient32->_subscribe_topics[i].msg_id = esp_mqtt_client_subscribe( _pMQTTClient32->_client, _pMQTTClient32->_subscribe_topics[i].topic, _pMQTTClient32->_subscribe_topics[i].qos);
              ESP_LOGI(TAG, "subscribe successful, msg_id=%d", _pMQTTClient32->subscribe_topics[i].msg_id);
            }
            MqttOnConnected();
            break;
          }
          case MQTT_EVENT_DISCONNECTED:
          {
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            Serial.println("[MQTT32] disconnected");
            _pMQTTClient32->_connected = false;

            MqttOnDisconnected();
            if( _pMQTTClient32->_fn_ondisconnected != NULL) {
              _pMQTTClient32->_fn_ondisconnected();
            }

            if( _pMQTTClient32->_auto_reconnect ){
              if(WiFi.isConnected()) {
                if(_pMQTTClient32->_auto_reconnect_try){
                  Serial.printf("[MQTT32 reconnecting ... (%d)\n",  _pMQTTClient32->_auto_reconnect_try);
                  _pMQTTClient32->reconnect();
                  _pMQTTClient32->_auto_reconnect_try--;
                }else{
                  _timer_reconnect.delay(10000,[](){
                    _pMQTTClient32->_auto_reconnect_try = 5;
                    _pMQTTClient32->_start();
                  });
                }
              }else{
                _timer_reconnect.delay(500,[]{  // ถ่วงเวลาเพื่อรอให้ event_cb นี้ทำงานเรียบร้อยไปก่อนค่อย ทำลายด้วย stop()            
                  _pMQTTClient32->_stop();
                });
              }
            }
            break;
          }
          case MQTT_EVENT_SUBSCRIBED:
          {
            int msg_id = _pMQTTClient32->_sub_msg_ids.front();
            _pMQTTClient32->_sub_msg_ids.erase(_pMQTTClient32->_sub_msg_ids.begin());

            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", msg_id);
            MqttOnSubscribed();
            if( _pMQTTClient32->_fn_onsubscribe != NULL) {
              _pMQTTClient32->_fn_onsubscribe(msg_id);
            }
            break;
          }
          case MQTT_EVENT_UNSUBSCRIBED:
          {
            int msg_id = _pMQTTClient32->_unsub_msg_ids.front();
            _pMQTTClient32->_unsub_msg_ids.erase(_pMQTTClient32->_unsub_msg_ids.begin());

            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", msg_id);
            MqttOnUnsubscribed();
            if( _pMQTTClient32->_fn_onunsubscribe != NULL) {
              _pMQTTClient32->_fn_onunsubscribe(msg_id);
            }
            break;
          }
          case MQTT_EVENT_PUBLISHED:
          {
            int msg_id = _pMQTTClient32->_pub_msg_ids.front();
            _pMQTTClient32->_pub_msg_ids.erase(_pMQTTClient32->_pub_msg_ids.begin());

            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", msg_id);
            MqttOnPublished();
            if( _pMQTTClient32->_fn_onpublish != NULL) {
              _pMQTTClient32->_fn_onpublish(msg_id);
            }
            break;
          }
          case MQTT_EVENT_DATA:
          {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            if( _pMQTTClient32->_fn_onmessage != NULL) {
              std::string _message(reinterpret_cast<char*>(_pMQTTClient32->_data.data()), _pMQTTClient32->_data.size());
              _pMQTTClient32->_fn_onmessage(_pMQTTClient32->_topic.c_str(), _message.c_str());
            }
            if( _pMQTTClient32->_fn_ondata != NULL) {
              _pMQTTClient32->_fn_ondata(_pMQTTClient32->_topic.c_str(), (uint8_t*)_pMQTTClient32->_data.data(),  _pMQTTClient32->_data.size());
            }
            break;
          }
          default:
            break;
        }
      }
      xSemaphoreGive(_event_mutex);  // Unlock
    }
  });
}

void BlynkGO_AsyncMQTT32::_stop(){
  // esp_task_wdt_reset();
  if(!this->_client_inited ) return;
  if(!this->_client_started) return;

  esp_mqtt_client_stop(this->_client);
  this->_client_started = false;
  this->_connected = false;
  vTaskDelay(100 / portTICK_PERIOD_MS); // ป้องกัน loop เช็คเร็วเกินไป
}

void BlynkGO_AsyncMQTT32::_disconnect(){
  // esp_task_wdt_reset();
  if(!this->_client_inited ) return;
  if(!this->_client_started) return;
  esp_mqtt_client_disconnect(this->_client);
  this->_connected = false;
  vTaskDelay(100 / portTICK_PERIOD_MS); // ป้องกัน loop เช็คเร็วเกินไป
}

void BlynkGO_AsyncMQTT32::_destroy(){
  // esp_task_wdt_reset();
  if(!this->_client_inited ) return;
  esp_mqtt_client_destroy(this->_client);
  this->_client_inited = false;
  this->_client_started =false;
  this->_connected = false;
  this->_client = NULL;
  vTaskDelay(100 / portTICK_PERIOD_MS); // ป้องกัน loop เช็คเร็วเกินไป
}

int BlynkGO_AsyncMQTT32::subscribe(String topic, uint8_t qos){
  // esp_task_wdt_reset();
  subscribe_topic_t new_subscribe_topic;
  snprintf(new_subscribe_topic.topic, sizeof(new_subscribe_topic.topic), "%s", (char*)topic.c_str());
  new_subscribe_topic.qos = qos;
  this->_subscribe_topics.push_back( new_subscribe_topic );
  if(this->connected() && this->_client != NULL) {
    uint32_t id = _subscribe_topics.size()-1;
    this->_subscribe_topics[id].msg_id = esp_mqtt_client_subscribe(_client, this->_subscribe_topics[id].topic, this->_subscribe_topics[id].qos);
    ESP_LOGI(TAG, "subscribe successful, msg_id=%d", this->subscribe_topics[id].msg_id);    
  }
  return _subscribe_topics.size();
}

bool BlynkGO_AsyncMQTT32::unsubscribe(String topic){
  // esp_task_wdt_reset();
  int ret = esp_mqtt_client_unsubscribe( this->_client, (const char *) topic.c_str() );
  if(ret !=-1){
    ESP_LOGI(TAG, "unsubscribe successful, topic=%s",  topic.c_str() );
    for( int i=0; i< this->_subscribe_topics.size();i++){
      if( String(this->_subscribe_topics[i].topic) == topic){
        this->_subscribe_topics.erase(this->_subscribe_topics.begin() + i);
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
  // esp_task_wdt_reset();
  for( int i=0; i< this->_subscribe_topics.size();i++){
    esp_mqtt_client_unsubscribe( this->_client, (const char *) this->_subscribe_topics[i].topic );
  }
  this->_subscribe_topics.clear();
}

int BlynkGO_AsyncMQTT32::publish(String topic, String message, uint8_t qos, bool retain){
  // esp_task_wdt_reset();
  return esp_mqtt_client_publish(this->_client, (const char *) topic.c_str(), (const char *)message.c_str(), message.length(), qos, retain);
}

int BlynkGO_AsyncMQTT32::publish(String topic, uint8_t* data, size_t data_len, uint8_t qos, bool retain){
  // esp_task_wdt_reset();
  return esp_mqtt_client_publish(this->_client, (const char *) topic.c_str(), (const char *)data, data_len, qos, retain);  
}

void BlynkGO_AsyncMQTT32::mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
  ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
  esp_mqtt_event_handle_t   event   = (esp_mqtt_event_handle_t) event_data;
  esp_mqtt_client_handle_t  client  = (esp_mqtt_client_handle_t) event->client;

  if (xSemaphoreTake(_event_mutex, portMAX_DELAY)) {  // Lock
    _pMQTTClient32->_event_list.push_back((esp_mqtt_event_id_t)event_id);

    switch((esp_mqtt_event_id_t)event_id ) {
      case MQTT_EVENT_DATA:
      {
        _pMQTTClient32->_topic.assign(event->topic, event->topic_len);            // กำหนดค่าด้วย assign() เก็บเป็น buffer
        _pMQTTClient32->_data.assign(event->data, event->data + event->data_len); // กำหนดค่าด้วย assign() เก็บเป็น buffer
          break;
      }
      case MQTT_EVENT_PUBLISHED:
      {
        _pMQTTClient32->_pub_msg_ids.push_back(event->msg_id);
        break;
      }
      case MQTT_EVENT_SUBSCRIBED:
      {
        _pMQTTClient32->_sub_msg_ids.push_back(event->msg_id);
        break;
      }
      case MQTT_EVENT_UNSUBSCRIBED:
      {
        _pMQTTClient32->_unsub_msg_ids.push_back(event->msg_id);
        break;
      }
    }
    xSemaphoreGive(_event_mutex);  // Unlock
  }
}

#endif // #if BLYNKGO_USE_ASYNC_MQTT32
