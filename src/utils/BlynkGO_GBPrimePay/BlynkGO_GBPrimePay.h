/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ BlynkGO_GBPrimePay.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
 *   สร้างโดย TridentTD
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
 * ---------------------------
 *  [History]
 *    Version 0.9 @ 31/10/2021
 *      - GBPrimePay รองรับ สร้าง QR Code แบบ Dynamic เปลี่ยนไปเรื่อยๆ ไม่ซ้ำ
 *         และ สามารถดักรับ event เมื่อ Customer มีการชำระเงินโอนผ่านแอพธนาคาร แล้วได้
 *      - รองรับ กระจายไปยังหลายๆตัวได้
 *
 *    Version 0.9.1 @ 14/11/2021
 *      - มี API ให้เปลี่ยน topic ของ tunnel และ paid ได้
 * 
 *********************************************************************
 */


#ifndef __BLYNKGO_GBPRIMEPAY__
#define __BLYNKGO_GBPRIMEPAY__

#define BLYNKGO_GBPRIMEPAY_VERSION     0.9.1

#include "../../config/blynkgo_config.h"

#if BLYNKGO_USE_WIFI
#if BLYNKGO_USE_NTP
#if BLYNKGO_USE_MQTT
#if BLYNKGO_USE_QRPAYMENT

#include <BlynkGOv3.h>
#include <WiFi.h>
#include <ArduinoJson.h> // version 6.x

#define GBPRIMEPAY_OK    0
#define MQTT_SUBSCRIBE_TOPIC_TUNNELS  "blynkgo/tunnels"
#define MQTT_SUBSCRIBE_TOPIC_PAID     "blynkgo/paid"

typedef struct _gbprime_qrcash {
  char bill_no[15+1];
  char bill_detail[250+1];
  float amount;
  
  char qrcode[1024];
  int16_t  resultCode;
  char gbpReferenceNo[250+1];
} gbprimepay_qrcash_t;

typedef struct _gbprimepay_paid {
  uint16_t currencyCode;      // 764
  double   amount;             // 0.01
  double   totalAmount;        // 0.01
  double   fee;                // 0.00008
  double   vat;                // 0.00001
  double   thbAmount;          // 0.01
  
  char     bill_no[15+1];         // referenceNo 
  char     bill_detail[250+1];
  char     destination_ip[20]; // esp32 ที่ request qr cash นี้ไปและมีการชำระ  (merchantDefined1)
  char     gbpReferenceNo[250+1]; // gbpReferenceNo 
  int16_t  resultCode;        // "00" OK
  char     paymentType[10];   // "Q"

  char     date[15];          // "27102021"  --> "27/10/2021"
  char     time[10];          // "005838"  --> "00:58:38"
} gbprimepay_paid_t;

enum{
  GBP_ERR_OK,
  GBP_ERR_CONNECTION,     // เกิดเมื่อ กำลังเชื่อมไปยัง gbp server แล้วเชื่อมไม่ได้
  GBP_ERR_REQ_QRCASH,     // 
  GBP_ERR_RESULT_QRCASH,    // เกิดเมื่อ มีการส่งคำขอ qrcash ไปแล้ว แต่ข้อมูลจัดส่งอาจมีการผิดพลาด
  GBP_EER_PAID_BILL_NO,
  GBP_ERR_PAID,           // เกิดเมื่อ customer ชำระแล้ว ไม่ได้มีการรับเงินเสร็จสมบูรณ์
  GBP_ERR_PAYMENT_TIMEOUT,
};
typedef uint8_t gbprimepay_error_t;

typedef void(* gbprimepay_qrcode_cb)(gbprimepay_qrcash_t response);
typedef void(* gbprimepay_paid_cb)(gbprimepay_paid_t paid);
typedef void(* gbprimepay_inited_cb)();
typedef void(* gbprimepay_unpaid_cb)();
typedef void(* gbprimepay_timeout_cb)();
typedef void(* gbprimepay_error_cb)(gbprimepay_error_t error);
enum { GBP_STATE_DISCONNECTED, GBP_STATE_CONNECTING, GBP_STATE_CONNECTED, GBP_STATE_REQ_QRCASH, GBP_STATE_RECV_QRCASH, GBP_STATE_WAIT4PAID, GBP_STATE_PAID, };
typedef uint8_t gbp_state_t;

class BlynkGO_GBPrimePay {
  public:
      BlynkGO_GBPrimePay();
      
             void     init(String mqtt_server="192.168.0.180", uint16_t mqtt_port=1883, String mqtt_user="", String mqtt_pass="");

      inline void     setToken(String token)                    { _token = token; }
      inline String   getToken()                                { return _token; }

      inline void     setTunnelTopic(String topic)		          { _tunnel_topic = topic; }
      inline void     setPaidTopic(String topic)				        { _paid_topic   = topic; }
      inline String   getTunnelTopic()                          { return _tunnel_topic;  }
      inline String   getPaidTopic()                            { return _paid_topic;    }

      inline void     setTopic(String tunnel_topic, String paid_topic )
                      {
                        _tunnel_topic = tunnel_topic;
                        _paid_topic   = paid_topic;
                      }
       
             void     setForwardPort(uint16_t port);
      inline uint16_t getForwarPort()                        { return _forward_port; }

      void   update();


      bool   requestQRCash(String bill_no, float bill_amount, String bill_detail="", String background_url="");
      bool   requestQRCash();
      inline void   onInitialized(gbprimepay_inited_cb fc)      { if(!_is_inited) this->init(); _inited_fc = fc; }
      inline void   onQRCash(gbprimepay_qrcode_cb fc)           { if(!_is_inited) this->init(); _qrcash_fc = fc; }
      inline void   onPaid(gbprimepay_paid_cb fc)               { if(!_is_inited) this->init(); _paid_fc   = fc; }
      inline void   onPaymentTimeout(gbprimepay_timeout_cb fc ) { if(!_is_inited) this->init(); _pay_timeout_fc  = fc; }
      inline void   onError(gbprimepay_error_cb fc)             { if(!_is_inited) this->init(); _error_fc  = fc; }
      inline void   onWiFiConnected(gbprimepay_inited_cb fc)    { if(!_is_inited) this->init(); _wifi_connected_fc  = fc; }
      inline void   onWiFiDisconnected(gbprimepay_inited_cb fc) { if(!_is_inited) this->init(); _wifi_disconnected_fc  = fc; }

      static String gen_bill_no(int16_t private_bill_no=-1 /* 0-9999*/);
      // inner use
      String _bill_no;
      String _bill_detail;
      float  _bill_amount;
      String _background_url;
      gbprimepay_inited_cb        _wifi_connected_fc = NULL;
      gbprimepay_inited_cb        _wifi_disconnected_fc=NULL;
      gbprimepay_inited_cb        _inited_fc = NULL;
      gbprimepay_qrcode_cb        _qrcash_fc = NULL; 
      gbprimepay_paid_cb          _paid_fc   = NULL;
      gbprimepay_unpaid_cb        _unpaid_fc = NULL;
      gbprimepay_timeout_cb       _pay_timeout_fc= NULL;
      gbprimepay_error_cb         _error_fc  = NULL;
      gbprimepay_qrcash_t _qrcash_data;
      gbprimepay_paid_t _paid_data;
      gbp_state_t _state = GBP_STATE_DISCONNECTED;
      uint32_t _timer;
      uint32_t _timeout = 30000;  // 30 วิ ถ้ายังไม่จ่ายก็เลิก
      bool _is_payment_master = false;
      String _tunnel_url;
      String _forward_ip;
      uint16_t _forward_port=12345;
      String  _tunnel_topic = MQTT_SUBSCRIBE_TOPIC_TUNNELS;
      String  _paid_topic   = MQTT_SUBSCRIBE_TOPIC_PAID;
      
  private:
      String _mqtt_server = "192.168.0.99";
      uint16_t _mqtt_port = 1883;
      
      String _token;
      bool   _is_inited = false; 
      int8_t _try_connecting = 1;
      
};

extern BlynkGO_GBPrimePay  GBPrimePay;



#endif  //#if(BLYNKGO_USE_WIFI && BLYNKGO_USE_NTP && BLYNKGO_USE_MQTT  && BLYNKGO_USE_QRPAYMENT )
#endif
#endif
#endif

#endif // __BLYNKGO_GBPRIMEPAY__
