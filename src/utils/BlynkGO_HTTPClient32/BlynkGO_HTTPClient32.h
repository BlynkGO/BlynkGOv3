#ifndef __BLYNKGO_HTTPCLIENT_H__
#define __BLYNKGO_HTTPCLIENT_H__

#include <Arduino.h>

#include "../../config/blynkgo_config.h"

#if BLYNKGO_USE_WIFI

#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_tls.h"
#include "esp_http_client.h"


/** Major version number (X.x.x) */
#define BLYNKGO_HTTPCLIENT_VERSION_MAJOR   0
/** Minor version number (x.X.x) */
#define BLYNKGO_HTTPCLIENT_VERSION_MINOR   5
/** Patch version number (x.x.X) */
#define BLYNKGO_HTTPCLIENT_VERSION_PATCH   0

#define BLYNKGO_HTTPCLIENT_VERSION_TEXT    (String(BLYNKGO_HTTPCLIENT_VERSION_MAJOR)+"."+String(BLYNKGO_HTTPCLIENT_VERSION_MINOR)+"."+String(BLYNKGO_HTTPCLIENT_VERSION_PATCH))

/**
 * Macro to convert IDF version number into an integer
 *
 * To be used in comparisons, such as BLYNKGO_HTTPCLIENT_VERSION >= BLYNKGO_HTTPCLIENT_VERSION_VAL(3, 0, 0)
 */
#define BLYNKGO_HTTPCLIENT_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current IDF version, as an integer
 *
 * To be used in comparisons, such as BLYNKGO_VERSION >= BLYNKGO_VERSION_VAL(3, 0, 0)
 */
#define BLYNKGO_HTTPCLIENT_VERSION  BLYNKGO_HTTPCLIENT_VERSION_VAL( BLYNKGO_VERSION_MAJOR, \
                                                                    BLYNKGO_VERSION_MINOR, \
                                                                    BLYNKGO_VERSION_PATCH)



#define MAX_HTTP_RECV_BUFFER      1024  // 512
#define MAX_HTTP_OUTPUT_BUFFER    2048
#define INSECURE                  NULL

typedef void(*http_on_response_cb_t)(int httpCode, uint8_t* data, size_t data_len);
typedef void(*http_on_header_cb_t)(String header, String value);
typedef void(*http_on_event_cb_t)();
typedef void(*http_on_data_cb_t)(uint8_t* data, size_t data_len, bool is_finished);

class BlynkGO_HTTPClient32 {
  public:
    BlynkGO_HTTPClient32()  {}

    BlynkGO_HTTPClient32& begin(String url, const char* cert_pem = NULL);
    void end();


    inline BlynkGO_HTTPClient32& onHeader( http_on_header_cb_t fn)          { _fn_on_header_cb = fn;    return *this; }
    inline BlynkGO_HTTPClient32& onResponse( http_on_response_cb_t fn)      { _fn_on_response_cb = fn;  _fn_on_data = NULL; return *this; }
    inline BlynkGO_HTTPClient32& onDisconnected( http_on_event_cb_t fn)     { _fn_on_disconnected = fn; return *this; }
    inline BlynkGO_HTTPClient32& onConnected( http_on_event_cb_t fn)        { _fn_on_connected = fn;    return *this; }
    inline BlynkGO_HTTPClient32& onData( http_on_data_cb_t fn)              { _fn_on_data = fn;         return *this; }

    inline BlynkGO_HTTPClient32& header(String key, String value) { 
      esp_http_client_set_header(_client, key.c_str(), value.c_str()); 
      return *this;
    }
    inline String header(String key){
      char *value; 
      esp_http_client_get_header(_client, key.c_str(), &value);
      return value;
    }
    
    inline BlynkGO_HTTPClient32& addHeader(String key, String value)        { return header(key, value); }
    inline BlynkGO_HTTPClient32& setHeader(String key, String value)        { return header(key, value); }
    inline String              getHeader(String key)                      { return header(key);        }
    inline BlynkGO_HTTPClient32& delHeader(String header){
      esp_http_client_delete_header(_client,header.c_str());
      return *this;
    }

    inline BlynkGO_HTTPClient32& user_password(String user, String password="") { 
      esp_http_client_set_username(_client, user.c_str());
      esp_http_client_set_password(_client, password.c_str());
      return *this;
    }

    inline BlynkGO_HTTPClient32& authtype(esp_http_client_auth_type_t auth_type) {
      esp_http_client_set_authtype(_client, auth_type);
      return *this;
    }

    inline BlynkGO_HTTPClient32& timeout(int timeout_ms){  // default : 5000ms
      esp_http_client_set_timeout_ms(_client, timeout_ms);
      return *this;
    }
    inline BlynkGO_HTTPClient32& setTimeout(int timeout_ms)               {  return timeout(timeout_ms); }

    // BlynkGO_HTTPClient32& GET(String url, http_on_response_cb_t fn=NULL);
    BlynkGO_HTTPClient32& GET();
    BlynkGO_HTTPClient32& POST(String post_data = "");

    inline bool connected()                                             { return this->_is_connected;  }



    int httpCode();
    int contentLength();

    http_on_response_cb_t   _fn_on_response_cb=NULL;
    http_on_header_cb_t     _fn_on_header_cb=NULL;
    http_on_event_cb_t      _fn_on_disconnected=NULL;
    http_on_event_cb_t      _fn_on_connected=NULL;
    http_on_data_cb_t       _fn_on_data=NULL;
    bool _is_connected = false;

    esp_http_client_handle_t  _client;
    bool                      _http_checking = false;
    TaskHandle_t              _task_handler;
  private:
    String _url;
    String _post_data;
};

#endif // #if BLYNKGO_USE_WIFI

#endif // __BLYNKGO_HTTPCLIENT_H__
