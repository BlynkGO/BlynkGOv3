#ifndef __BLYNKGO_ASYNCMQTT32_H__
#define __BLYNKGO_ASYNCMQTT32_H__

/** Major version number (X.x.x) */
#define BLYNKGO_ASYNC_MQTT32_VERSION_MAJOR   1
/** Minor version number (x.X.x) */
#define BLYNKGO_ASYNC_MQTT32_VERSION_MINOR   0
/** Patch version number (x.x.X) */
#define BLYNKGO_ASYNC_MQTT32_VERSION_PATCH   2

#define BLYNKGO_ASYNC_MQTT32_VERSION_TEXT    (String(BLYNKGO_ASYNC_MQTT32_VERSION_MAJOR)+"."+String(BLYNKGO_ASYNC_MQTT32_VERSION_MINOR)+"."+String(BLYNKGO_ASYNC_MQTT32_VERSION_PATCH))


#include "../../config/blynkgo_config.h"

// #define BLYNKO_USE_ASYNC_MQTT32   1
#if BLYNKO_USE_ASYNC_MQTT32

#include <Arduino.h>
#include <WiFi.h>
#include "mqtt_client.h"
#include <vector>


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

#if BLYNKO_USE_ASYNC_MQTT32
  
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
#endif // BLYNKO_USE_ASYNC_MQTT32


class BlynkGO_AsyncMQTT32 {
  public:
    BlynkGO_AsyncMQTT32();
    ~BlynkGO_AsyncMQTT32();

    void setServer(String host, uint16_t port=1883, String user="", String password="");
    void setServerSecure( String host, uint16_t port=8883, String user="", String password="");
    void setWebSocket( String host, uint16_t port=80, String user="", String password="");
    void setWebSocketSecure( String host, uint16_t port=443, String user="", String password="");
    void setCertPEM(const char * mqtt_cert_pem);

   void use_global_ca_store(bool enable);
    
    void connect(bool auto_reconnect=true);
    void disconnect();
    void reconnect();
    void stop();

    
    inline bool connected()                     { return this->_connected; }
    inline void auto_reconnect(bool enable)     { this->_auto_reconnect = enable;  }
    inline bool auto_reconnect()                { return this->_auto_reconnect;    }

    int  publish(String topic, String message, uint8_t qos=QOS0, bool retain=false);
    int  publish(String topic, uint8_t* data, size_t data_len, uint8_t qos=QOS0, bool retain=false);
    int  subscribe(String topic, uint8_t qos=QOS0); 
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
    std::vector<subscribe_topic_t>  subscribe_topics;
    bool _connected = false;
    bool _auto_reconnect = true;
    bool _client_inited = false;
    
  private:
    void _stop();
    void _destroy();
    esp_mqtt_client_handle_t _client = NULL;
    esp_mqtt_client_config_t _mqtt_conf;
    char _host[128];
    uint16_t _port=1883;
    char _uri[512];
};

extern BlynkGO_AsyncMQTT32 MQTT;


#endif // #if BLYNKO_USE_ASYNC_MQTT32

#endif //__BLYNKGO_ASYNCMQTT32_H__

