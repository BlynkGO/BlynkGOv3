/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ BlynkGO_AsyncMQTT32.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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
 *      - _topic มีการเก็บ topic ย้ายออกมา
 *      - _data มีการเก็บ data ย้ายออกมา
 *      - _sub_msg_ids, _unsub_msq_ids, _pub_msq_ids มีการจำไว้ 
 *      - รองรับ กำหนด client_id เองได้  client_id(...) เพื่อใช้กับ mqtt broker บางแห่ง เช่นที่ NETPIE ได้
 *
 *   [V1.0.14] @24/03/25
 *      - ปรับ destroy() ที่มาจาก _stop + _destroy() ให้เช็คว่าเคย connected อยู่ ถึงค่อยมี event disconnected() แค่ครั้งเดียวออกไป
 *      - client_id เปลี่ยนทุกครั้งที่ _init ใหม่  (ยกเว้น หากมีการ กำหนด client_id เองเช่นใน netpie)
 *      - reconnect 5 รอบแล้วไม่ได้ ให้ _stop + _destroy แล้ว 10วิ ค่อย _init ใหม่ + _start
 *      - reconnect 5 รอบ ใช้ของภายใน แล้ว บางทีกลับเชื่อมไม่ได้อีก อาจเป็นเพราะ client_id เดิมบน server ค้างอยู่
 *        ทำการเปลี่ยนใหม่ ให้ _stop + _destroy ทิ้ง แล้ว  1 วินาทีค่อย _init client_id ใหม่ + _start แทน reconnect ภายใน
 *              E (259013) MQTT_CLIENT: mqtt_message_receive: transport_read() error: errno=128
 *              E (259014) MQTT_CLIENT: mqtt_process_receive: mqtt_message_receive() returned -1
 *        ค่อยกลับมาเชื่อมต่อใหม่ได้!!
 *      - WiFi ไม่หลุด Net หลุด Net มาสักพัก
 *        หรือ WiFi หลุด แล้ว WiFi มา
 *              E (998499) TRANSPORT_BASE: poll_read select error 104, errno = Connection reset by peer, fd = 48
 *              E (998500) MQTT_CLIENT: Poll read error: 119, aborting connection
 *        แล้วกลับมาเชื่อมต่อใหม่ได้!!
 *      - WiFi & Net หลุด แล้ว WiFi มา แต่ Net ไม่มา
 *              E (1144206) esp-tls: [sock=48] select() timeout
 *              E (1144207) TRANSPORT_BASE: Failed to open a new connection: 32774
 *              E (1144207) MQTT_CLIENT: Error transport connect
 *        จะเข้าสู่ reconnect 5รอบ แล้ว หลุด 10 วิ ไปเรื่อยๆ
 *        เมื่อ Net มา ก็กลับมาเชื่อมต่อได้!!
 *      - subscribe มี ปิด ไม่ให้ save ลง subscribe topic list แต่เป็น direct subscribe ได้ (สำหรับใช้ สั่ง subscribe เองใน MQTT_CONNECTED)
 *
 *********************************************************************
 */

#ifndef __BLYNKGO_ASYNCMQTT32_H__
#define __BLYNKGO_ASYNCMQTT32_H__

/** Major version number (X.x.x) */
#define BLYNKGO_ASYNC_MQTT32_VERSION_MAJOR   1
/** Minor version number (x.X.x) */
#define BLYNKGO_ASYNC_MQTT32_VERSION_MINOR   0
/** Patch version number (x.x.X) */
#define BLYNKGO_ASYNC_MQTT32_VERSION_PATCH   14

#define BLYNKGO_ASYNC_MQTT32_VERSION_TEXT    (String(BLYNKGO_ASYNC_MQTT32_VERSION_MAJOR)+"."+String(BLYNKGO_ASYNC_MQTT32_VERSION_MINOR)+"."+String(BLYNKGO_ASYNC_MQTT32_VERSION_PATCH))


#include "../../config/blynkgo_config.h"

// #define BLYNKGO_USE_ASYNC_MQTT32   1
#if BLYNKGO_USE_ASYNC_MQTT32

#include <Arduino.h>
#include <WiFi.h>
#include "mqtt_client.h"
#include <vector>
#include <BeeNeXT.h>


enum {
  QOS0=0,
  QOS1=1,
  QOS2=2,
};
typedef uint8_t qos_t;

typedef struct _subscribe_topic_t {
  char    topic[512];
  uint8_t qos;
  int     msg_id;
} subscribe_topic_t;

typedef void(*mqtt_onany_cb)(void);
typedef void(*mqtt_onmessage_cb)( String topic, String message);
typedef void(*mqtt_ondata_cb)   ( String topic, uint8_t* data, size_t len);
typedef void(*mqtt_onsubscribe_cb)(int msg_id);

#define AUTO_RECONNECT      true
#define NO_AUTO_RECONNECT   false

#if BLYNKGO_USE_ASYNC_MQTT32
  
  #define MQTT_CONNECTED()    void MqttOnConnected()
  #define MQTT_DISCONNECTED() void MqttOnDisconnected()
  #define MQTT_SUBSCRIBED()   void MqttOnSubscribed()
  #define MQTT_UNSUBSCRIBED() void MqttOnUnsubscribed()
  #define MQTT_PUBLISHED()    void MqttOnPublished()

  #ifdef __cplusplus
  extern "C" {
  #endif
    void MQTTNoOpCbk();     // Weak Prototype Function
    MQTT_CONNECTED();
    MQTT_DISCONNECTED();
    MQTT_SUBSCRIBED();
    MQTT_UNSUBSCRIBED();
    MQTT_PUBLISHED();
  #ifdef __cplusplus
  }
  #endif
#endif // BLYNKGO_USE_ASYNC_MQTT32


class BlynkGO_AsyncMQTT32 {
  public:
    BlynkGO_AsyncMQTT32();
    ~BlynkGO_AsyncMQTT32();

    void setServer(String host, uint16_t port=1883, String user="", String password="");
    void setServerSecure( String host, uint16_t port=8883, String user="", String password="");
    void setWebSocket( String host, uint16_t port=80, String user="", String password="");
    void setWebSocketSecure( String host, uint16_t port=443, String user="", String password="");
    void setCertPEM(const char * mqtt_cert_pem);
    inline void client_id(String client_id)       { this->_client_id = client_id.c_str(); }
    inline String client_id()                     { return this->_client_id.c_str();      }
    inline void clean_session(bool enable)        { this->_clean_session = enable;        }
    inline bool clean_session()                   { return this->_clean_session;          }

    inline void setNetPie(String netpie_client_id, String netpie_token, String netpie_secret) {
      this->setServer("mqtt.netpie.io", 1883, netpie_token, netpie_secret);
      this->client_id(netpie_client_id);
      this->unsubscribe_all();
      this->subscribe("@msg/#");
    }
    
    void use_global_ca_store(bool enable);
    
    void connect(bool clean_session = true);      // _start() ที่ _init() ไว้
    void disconnect();                            // _stop() + _destroy() + เรียก disconnected cb
    void reconnect();                             // reconnect ที่ _init() ไว้
    
    inline bool connected()                       { return this->_connected;              }

    inline void auto_reconnect(bool enable)       { this->_auto_reconnect = enable;       }
    inline bool auto_reconnect()                  { return this->_auto_reconnect;         }


    int  publish(String topic, String message, uint8_t qos=QOS0, bool retain=false);
    int  publish(String topic, uint8_t* data, size_t data_len, uint8_t qos=QOS0, bool retain=false);
    int  subscribe(String topic, uint8_t qos=QOS0, bool auto_subscribe=true); 
    bool unsubscribe(String topic);
    void unsubscribe_all();

    inline int clearRetain(String topic, uint8_t qos = QOS0)            { return publish(topic, NULL, 0, qos, true); }
    inline int clearTopic(String topic, uint8_t qos = QOS0)             { return clearRetain(topic, qos); }

    inline BlynkGO_AsyncMQTT32& onConnected(mqtt_onany_cb fn)           { _fn_onconnected = fn; return *this;   }
    inline BlynkGO_AsyncMQTT32& onDisconnected(mqtt_onany_cb fn)        { _fn_ondisconnected = fn; return *this;}
    inline BlynkGO_AsyncMQTT32& onError(mqtt_onany_cb fn)               { _fn_onerror = fn; return *this;       }
    inline BlynkGO_AsyncMQTT32& onMessage(mqtt_onmessage_cb fn)         { _fn_onmessage = fn; return *this;     }
    inline BlynkGO_AsyncMQTT32& onData(mqtt_ondata_cb fn)               { _fn_ondata  = fn; return *this;       }
    inline BlynkGO_AsyncMQTT32& onPublished(mqtt_onsubscribe_cb fn)     { _fn_onpublish  = fn; return *this;    }
    inline BlynkGO_AsyncMQTT32& onSubscribed(mqtt_onsubscribe_cb fn)    { _fn_onsubscribe  = fn; return *this;  }
    inline BlynkGO_AsyncMQTT32& onUnsubscribed(mqtt_onsubscribe_cb fn)  { _fn_onunsubscribe = fn; return *this; }
    
    mqtt_onany_cb       _fn_onconnected     = NULL;
    mqtt_onany_cb       _fn_ondisconnected  = NULL;
    mqtt_onany_cb       _fn_onerror         = NULL;
    mqtt_onmessage_cb   _fn_onmessage       = NULL;
    mqtt_ondata_cb      _fn_ondata          = NULL;
    mqtt_onsubscribe_cb _fn_onsubscribe     = NULL;
    mqtt_onsubscribe_cb _fn_onunsubscribe   = NULL;
    mqtt_onsubscribe_cb _fn_onpublish       = NULL;
    
    static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    bool _connected = false;
    bool _auto_reconnect = true;
    uint8_t _auto_reconnect_try = 5;
    bool _client_inited = false;
    bool _client_started = false;
    bool _clean_session = true;
    std::vector<subscribe_topic_t>  _subscribe_topics;
    std::vector<esp_mqtt_event_id_t> _event_list; // เป็น buffer เก็บ mqtt event id
    std::string          _topic;  // เป็น buffer เก็บตอน event MQTT_EVENT_DATA เข้ามา
    std::vector<uint8_t> _data;   // เป็น buffer เก็บตอน event MQTT_EVENT_DATA เข้ามา
    std::vector<int>     _sub_msg_ids;   // เป็น buffer เก็บ msg_id ของ event MQTT_EVENT_SUBSCRIBED
    std::vector<int>     _unsub_msg_ids; // เป็น buffer เก็บ msg_id ของ event MQTT_EVENT_UNSUBSCRIBED
    std::vector<int>     _pub_msg_ids;   // เป็น buffer เก็บ msg_id ของ event MQTT_EVENT_PUBLISHED
    esp_mqtt_client_handle_t _client = NULL;

    /* เริ่มต้น จองเม็ม _client มีกำหนด host, port, uri, clean_session ในนี้ แบบ idf */
    void _init(int8_t clean_session=-1);  // 0 : ไม่ต้อง clean session ; 1 คือ clean sessoin; -1 ใช้ ที่กำหนดจาก this->_clean_session

    /* เริ่มต้น เชื่อมต่อ ไปยัง host,port ที่กำหนด แบบ idf */
    void _start();

    /* ตัดการเชื่อมต่อ */
    void _disconnect();

    /* มีการตัดการเชื่อมต่อ และ หยุด mqtt task จากภายใน idf */
    void _stop();

    /* ทำลายที่จอง mem ตอน init แบบ idf*/
    void _destroy();


  private:
    esp_mqtt_client_config_t _mqtt_conf;
    char _host[128];
    uint16_t _port=1883;
    char _uri[512];
    std::string _client_id;
};

extern BlynkGO_AsyncMQTT32 MQTT;


#endif // #if BLYNKGO_USE_ASYNC_MQTT32

#endif //__BLYNKGO_ASYNCMQTT32_H__
