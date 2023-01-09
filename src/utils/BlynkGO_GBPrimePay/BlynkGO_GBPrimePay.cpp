/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ BlynkGO_GBPrimePay.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "BlynkGO_GBPrimePay.h"

#if BLYNKGO_USE_WIFI
#if BLYNKGO_USE_NTP
#if BLYNKGO_USE_MQTT
#if BLYNKGO_USE_QRPAYMENT

//QRCASH_URL  "https://api.gbprimepay.com/gbp/gateway/qrcode/text"
#define GBPRIME_HOST                  "api.gbprimepay.com"
#define GBPRIME_PORT                  443
#define MAX_RETRY_CONNECTION          10



static String urlencode(String url);
static AsyncClientSecure _client;
static BlynkGO_GBPrimePay* _p_gbp = NULL;
static WiFiServer _server;

BlynkGO_GBPrimePay  GBPrimePay;

static uint8_t  gbp_day(String date)    { return date.substring(0,2).toInt(); }
static uint8_t  gbp_month(String date)  { return date.substring(2,4).toInt(); }
static uint16_t gbp_year(String date)   { return date.substring(4).toInt();   }
static uint8_t  gbp_hour(String time)   { return time.substring(0,2).toInt(); }
static uint8_t  gbp_minute(String time) { return time.substring(2,4).toInt(); }
static uint8_t  gbp_second(String time) { return time.substring(4).toInt();   }


BlynkGO_GBPrimePay::BlynkGO_GBPrimePay() { _p_gbp = this; }

void BlynkGO_GBPrimePay::init(String mqtt_server, uint16_t mqtt_port, String mqtt_user, String mqtt_pass){
  if(_is_inited) return;

  this->_mqtt_server = mqtt_server;
  this->_mqtt_port   = mqtt_port;
  
  
  if(mqtt_user == "") {
    MQTT.setServer(this->_mqtt_server.c_str(), this->_mqtt_port);
  }else{
    MQTT.setServer(this->_mqtt_server.c_str(), this->_mqtt_port, mqtt_user.c_str(), mqtt_pass.c_str());
  }

  MQTT.onMessage([](char* topic, char* data, MessageProperties properties, size_t len, size_t index, size_t total){
    static String  payload;
    if(index == 0) payload = "";
    char buf[len]; memcpy(buf, data, len); buf[len]=0; payload += buf;
    
    if( index + len >= total) {

      if( String(topic) == _p_gbp->_tunnel_topic) { //MQTT_SUBSCRIBE_TOPIC_TUNNELS){
        DynamicJsonDocument doc(5000);  // เผื่อไว้เลย กันไม่พอ
        auto error = deserializeJson(doc, payload);
        if(!error){
          
          for (JsonObject tunnels_item : doc["tunnels"].as<JsonArray>()) {
            String tunnels_item_public_url  = tunnels_item["public_url"].as<String>();
            String tunnels_item_config_addr = tunnels_item["config"]["addr"].as<String>();
  
            if( tunnels_item_public_url.startsWith("https://") ) {
              int16_t index_start = tunnels_item_config_addr.indexOf("://")+3;
              int16_t index_end   = tunnels_item_config_addr.indexOf(":", index_start);

              String   tunnel_url   = tunnels_item_public_url;
              String   forward_ip   = tunnels_item_config_addr.substring( index_start, index_end);
              uint16_t forward_port = (uint16_t) tunnels_item_config_addr.substring(index_end+1).toInt();

              bool is_tunnel_update = false;
              if( _p_gbp->_forward_port != forward_port ){
                _p_gbp->setForwardPort(_p_gbp->_forward_port);
                is_tunnel_update = true;
              }
              if( _p_gbp->_forward_ip != forward_ip ){
                _p_gbp->_forward_ip = forward_ip;
                _p_gbp->_is_payment_master = (_p_gbp->_forward_ip == WiFi.localIP().toString());
                is_tunnel_update = true;
              }
              if( _p_gbp->_tunnel_url != tunnel_url ){
                _p_gbp->_tunnel_url = tunnel_url;
                is_tunnel_update = true;
              }
              
              if(is_tunnel_update) {
//                Serial.printf("[MQTT] <message recv>  topic : %s -> payload : %s\n", topic, payload.c_str() );
                Serial.println("---------------------------------------------------------");
                Serial.printf("[GBPrimePay] Forward Mapping   : %s -> %s:%d (master : %s)\n",
                                    tunnel_url.c_str(), forward_ip.c_str(), forward_port,
                                    _p_gbp->_is_payment_master? "true" : "false");
                Serial.printf("[GBPrimePay] MQTT tunnel topic : %s\n"
                              "             MQTT paid topic   : %s\n", 
                                    _p_gbp->_tunnel_topic.c_str(),
                                    _p_gbp->_paid_topic.c_str());
                Serial.println("---------------------------------------------------------");
                Serial.println("[GBPrimePay] initialized!");


                if( _p_gbp->_inited_fc !=NULL){
                  _p_gbp->_inited_fc();
                }
              }
            }
          }
        }
      }
      else if( String(topic) == _p_gbp->_paid_topic ){ //MQTT_SUBSCRIBE_TOPIC_PAID ){
        DynamicJsonDocument doc(2048);  // เผื่อไว้เลย กันไม่พอ
        auto error = deserializeJson(doc, payload);
        if(!error){
          _p_gbp->_paid_data.resultCode  = doc["resultCode"].as<String>().toInt();
          snprintf(_p_gbp->_paid_data.bill_no, sizeof(_p_gbp->_paid_data.bill_no), "%s", doc["referenceNo"].as<const char*>());
          snprintf(_p_gbp->_paid_data.bill_detail, sizeof(_p_gbp->_paid_data.bill_detail), "%s", doc["detail"].as<const char*>());  
          snprintf(_p_gbp->_paid_data.destination_ip, sizeof(_p_gbp->_paid_data.destination_ip), "%s", doc["merchantDefined1"].as<const char*>());  
          snprintf(_p_gbp->_paid_data.gbpReferenceNo, sizeof(_p_gbp->_paid_data.gbpReferenceNo), "%s", doc["gbpReferenceNo"].as<const char*>());
          snprintf(_p_gbp->_paid_data.date, sizeof(_p_gbp->_paid_data.date), "%02d/%02d/%d", 
                    gbp_day(doc["date"].as<String>()),gbp_month(doc["date"].as<String>()),gbp_year(doc["date"].as<String>()) );
          snprintf(_p_gbp->_paid_data.time, sizeof(_p_gbp->_paid_data.time), "%02d:%02d:%02d", 
                    gbp_hour(doc["time"].as<String>()),gbp_minute(doc["time"].as<String>()),gbp_second(doc["time"].as<String>()) );
          snprintf(_p_gbp->_paid_data.paymentType, sizeof(_p_gbp->_paid_data.paymentType), "%s", doc["paymentType"].as<const char*>());
          _p_gbp->_paid_data.currencyCode  = doc["currencyCode"].as<int>();
    
          _p_gbp->_paid_data.amount        = doc["amount"].as<double>();
          _p_gbp->_paid_data.totalAmount   = doc["totalAmount"].as<double>();
          _p_gbp->_paid_data.fee           = doc["fee"].as<double>();
          _p_gbp->_paid_data.vat           = doc["vat"].as<double>();
          _p_gbp->_paid_data.thbAmount     = doc["thbAmount"].as<double>();
    
          Serial.printf("[QR PAID] result code   : %d\n"    , _p_gbp->_paid_data.resultCode);
          Serial.printf("[QR PAID] dest ip       : %s\n"    , _p_gbp->_paid_data.destination_ip);
          Serial.printf("[QR PAID] bill no       : %s\n"    , _p_gbp->_paid_data.bill_no);
          Serial.printf("[QR PAID] bill detail   : %s\n"    , _p_gbp->_paid_data.bill_detail);
          Serial.printf("[QR PAID] GBP ref.code  : %s\n"    , _p_gbp->_paid_data.gbpReferenceNo);
          Serial.printf("[QR PAID] Date&Time     : %s @%s\n", _p_gbp->_paid_data.date,_p_gbp->_paid_data.time);
          Serial.printf("[QR PAID] payment Type  : %s\n"    , _p_gbp->_paid_data.paymentType);
          Serial.printf("[QR PAID] currencyCode  : %d\n"    , _p_gbp->_paid_data.currencyCode);
    
          Serial.printf("[QR PAID] amount        : %.2f\n"  , _p_gbp->_paid_data.amount);
          Serial.printf("[QR PAID] totalAmount   : %.2f\n"  , _p_gbp->_paid_data.totalAmount);
          Serial.printf("[QR PAID] fee           : %.5f\n"  , _p_gbp->_paid_data.fee);
          Serial.printf("[QR PAID] vat           : %.5f\n"  , _p_gbp->_paid_data.vat);
          Serial.printf("[QR PAID] ThBaht Amount : %.2f\n"  , _p_gbp->_paid_data.thbAmount);

          if( _p_gbp->_paid_data.resultCode == GBPRIMEPAY_OK ) {
            if( WiFi.localIP().toString() == String(_p_gbp->_paid_data.destination_ip) ){
              if( String(_p_gbp->_qrcash_data.bill_no) == String(_p_gbp->_paid_data.bill_no) ){
                if( _p_gbp->_paid_fc != NULL){
                  _p_gbp->_paid_fc(_p_gbp->_paid_data);
                }
              }else{
                if( _p_gbp->_error_fc !=NULL){
                  _p_gbp->_error_fc(GBP_EER_PAID_BILL_NO);
                }
              }
            }
          }else{
             if( _p_gbp->_error_fc !=NULL){
                _p_gbp->_error_fc(GBP_ERR_PAID);
             }
          }
        }
      } // if( String(topic) == MQTT_SUBSCRIBE_TOPIC_PAID )
    } // รับ payload มาครบ
  });
  
  _client.onConnect( [](void* arg, AsyncClient* client) {
    AsyncClientSecure * _p_client = (AsyncClientSecure*) client;
    BlynkGO_GBPrimePay *blynkgo_pay = (BlynkGO_GBPrimePay*) arg;
    Serial.println("[GBPrimePay] connected & request QRCash");

      blynkgo_pay->_state = GBP_STATE_CONNECTED;


    String payload = StringX::printf("token=%s&amount=%.2f&referenceNo=%s&backgroundUrl=%s&merchantDefined1=%s",
                             urlencode(blynkgo_pay->getToken()).c_str(), 
                             blynkgo_pay->_bill_amount, blynkgo_pay->_bill_no.c_str(),
                             urlencode(blynkgo_pay->_background_url).c_str(),
                             urlencode(WiFi.localIP().toString()).c_str());

    if( blynkgo_pay->_bill_detail != ""){
      payload += String("&detail=")+ urlencode(blynkgo_pay->_bill_detail);
    }

    String request =  "POST /gbp/gateway/qrcode/text HTTP/1.1\n"
                      "Host: api.gbprimepay.com\n"
                      "content-type: application/x-www-form-urlencoded\n"
                      "Content-Length: " + String( payload.length()) + "\n\n" + payload;
//    Serial.println("------------------");
//    Serial.println(request);
//    Serial.println("------------------");
    blynkgo_pay->_state = GBP_STATE_REQ_QRCASH;
    _p_client->write(request);
  }, this);

  
  _client.onDisconnect([](void* arg, AsyncClient* client) {
    AsyncClientSecure * _p_client = (AsyncClientSecure*) client;
    BlynkGO_GBPrimePay *blynkgo_pay = (BlynkGO_GBPrimePay*) arg;

    if( blynkgo_pay->_state == GBP_STATE_CONNECTING ){
      blynkgo_pay->requestQRCash();
      return;
    }else
    if( blynkgo_pay->_state == GBP_STATE_REQ_QRCASH ){
      if( blynkgo_pay->_error_fc ){
        blynkgo_pay->_error_fc(GBP_ERR_REQ_QRCASH);
        blynkgo_pay->_state = GBP_STATE_DISCONNECTED;
      }
    }else
    if( blynkgo_pay->_state == GBP_STATE_WAIT4PAID ){
      // ไม่ต้องเปลีย่นสถานะอะไร
    }
    
    Serial.println("[GBPrimePay] disconnected");
  }, this);

  _client.onData([](void* arg, AsyncClient* client, void *data, size_t len) {
    AsyncClientSecure * _p_client = (AsyncClientSecure*) client;
    BlynkGO_GBPrimePay *blynkgo_pay = (BlynkGO_GBPrimePay*) arg;
    blynkgo_pay->_state = GBP_STATE_RECV_QRCASH;
//    Serial.println("[GBPrimePay] onData");
    char buf[len+1];
    memcpy((void*)buf, (void*)data, len); 
    buf[len+1] = '\0';
    String response = buf;
    String payload = response.substring(response.indexOf("\r\n\r\n")+4);
    String   json_hex_len = payload.substring(0, payload.indexOf("\r\n") ); // บรรทัดแรก payload นี้เป็นความยาว json
    uint16_t json_len     = stoul(json_hex_len.c_str(), nullptr, 16);
//    Serial.println(json_hex_len_str);
//    Serial.println(json_len);

    memset(&blynkgo_pay->_qrcash_data, 0, sizeof(blynkgo_pay->_qrcash_data) );
    blynkgo_pay->_qrcash_data.resultCode = -1;
    if( json_len ){
      String  json_str  = payload.substring(payload.indexOf("\r\n")+2, payload.indexOf("\r\n")+2 + json_len);
//      Serial.printf("[GBPrimePay] json : %s\n", json_str.c_str());

      //ตย -> {"referenceNo":"211022130044890","qrcode":"00020101021230830016A0000006770101120115010556006812755021800000021102302017703180002110221300448905303764540515.205802TH5910GBPrimePay630488D4","resultCode":"00","gbpReferenceNo":"gbp5710122427368"}

      String keyword[] = { "resultCode", "referenceNo", "detail", "qrcode", "gbpReferenceNo" };
      enum {RES_RESULT_CODE, RES_BILL_REFERENCE_NO /* BILL_NO */, RES_BILL_DETAIL, RES_BILL_QRCODE, RES_GBP_REFERENCE_NO,};
      uint8_t keyword_cnt = sizeof(keyword)/sizeof(keyword[0]);
//      Serial.println(keyword_cnt);
      for(int i=0; i < keyword_cnt; i++){
        int index_found = json_str.indexOf(keyword[i]);
        if(index_found != -1 ){
          int index_start = index_found + keyword[i].length()+3;
          String buf_str = json_str.substring( index_start, json_str.indexOf( "\"", index_start + 1));
//          Serial.println(buf_str);
          switch( i){
            case RES_RESULT_CODE:
              blynkgo_pay->_qrcash_data.resultCode = buf_str.toInt();
//              Serial.println( blynkgo_pay->_qrcash_data.resultCode );
              break;
            case RES_BILL_REFERENCE_NO: /* = BILL_NO */
              snprintf(blynkgo_pay->_qrcash_data.bill_no, sizeof(blynkgo_pay->_qrcash_data.bill_no), "%s", buf_str.c_str());
//              Serial.println( blynkgo_pay->_qrcash_data.bill_no );
              break;
            case RES_BILL_DETAIL:
              snprintf(blynkgo_pay->_qrcash_data.bill_detail, sizeof(blynkgo_pay->_qrcash_data.bill_detail), "%s", buf_str.c_str());
//              Serial.println( blynkgo_pay->_qrcash_data.bill_detail );
              break;
            case RES_BILL_QRCODE:
              snprintf(blynkgo_pay->_qrcash_data.qrcode, sizeof(blynkgo_pay->_qrcash_data.qrcode), "%s", buf_str.c_str());
//              Serial.println( blynkgo_pay->_qrcash_data.qrcode );
              break;
            case RES_GBP_REFERENCE_NO:
              snprintf(blynkgo_pay->_qrcash_data.gbpReferenceNo, sizeof(blynkgo_pay->_qrcash_data.gbpReferenceNo), "%s", buf_str.c_str());
//              Serial.println( blynkgo_pay->_qrcash_data.gbpReferenceNo );
              break;
          }
        }
      }
    }
//    Serial.println( blynkgo_pay->_bill_no );
//    Serial.println( blynkgo_pay->_qrcash_data.bill_no );
    if( blynkgo_pay->_bill_no == blynkgo_pay->_qrcash_data.bill_no ){
      if( String(blynkgo_pay->_qrcash_data.bill_detail) == ""){
//        Serial.println(blynkgo_pay->_bill_detail);
        snprintf(blynkgo_pay->_qrcash_data.bill_detail, sizeof(blynkgo_pay->_qrcash_data.bill_detail), "%s", blynkgo_pay->_bill_detail.c_str());
//        Serial.println(blynkgo_pay->_qrcash_data.bill_detail);
      }

      if( blynkgo_pay->_qrcash_data.amount == 0 ){
        blynkgo_pay->_qrcash_data.amount = blynkgo_pay->_bill_amount;
      }
    }

    if( blynkgo_pay->_qrcash_data.resultCode == GBPRIMEPAY_OK ) {
      if( blynkgo_pay->_qrcash_fc ){
        blynkgo_pay->_qrcash_fc(blynkgo_pay->_qrcash_data);
      }
      blynkgo_pay->_state = GBP_STATE_WAIT4PAID;
      blynkgo_pay->_timer = millis() +  blynkgo_pay->_timeout;
    }else{
      if( blynkgo_pay->_error_fc ){
        blynkgo_pay->_error_fc(GBP_ERR_RESULT_QRCASH);
      }
    }
    _p_client->close();
  }, this);

  static GTimer timer;
  // ป้องกันสำหรับบอร์ด BlynkGO Board1.2 ให้หน่วงกัน WiFiServer ไปชนภายใน tcp 
  // แต่สำหรับ BlynkGO EMMA-II ไม่เป็น
  // timer.setOnce(2000,[](){_server.begin(_p_gbp->_forward_port);});
  _is_inited = true;  
}

void   BlynkGO_GBPrimePay::setForwardPort(uint16_t port){
  if( port != this->_forward_port) {
    this->_forward_port = port;
    _server.end();
    _server.begin(this->_forward_port );
  }
}

void BlynkGO_GBPrimePay::update(){
  if( this->_timer <= millis() && this->_state == GBP_STATE_WAIT4PAID){
    this->_state = GBP_STATE_DISCONNECTED;
    if( this->_pay_timeout_fc){
      this->_pay_timeout_fc();
    }
    if( this->_error_fc ){
      this->_error_fc(GBP_ERR_PAYMENT_TIMEOUT);
    }
    this->_state = GBP_STATE_DISCONNECTED;
    return;
  }

  if(!this->_is_payment_master) return;
  
  WiFiClient client = _server.available();
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        String payload = client.readString();             // read a byte, then
        payload.replace("\r\n", "\n");
        //Serial.println(payload);
        
        if( payload.indexOf("\n\n") > -1 ) {
          String json_str = payload.substring(payload.indexOf("\n\n")+2);
          Serial.println(json_str);

          MQTT.publish(_p_gbp->_paid_topic, json_str);
        }        
        break;
      }
    }
    
    client.stop();
    this->_state = GBP_STATE_DISCONNECTED;
    Serial.println("Client Disconnected.");
  }
}


bool BlynkGO_GBPrimePay::requestQRCash(String bill_no, float bill_amount,  String bill_detail,String background_url)
{
  if(!_is_inited) this->init();
  if( _token == "") return false;

  this->_bill_no      = bill_no;
  this->_bill_amount  = bill_amount;
  this->_bill_detail  = bill_detail;
  this->_background_url = (background_url=="")? this->_tunnel_url : background_url;

  _state = GBP_STATE_CONNECTING;
  _try_connecting = 1;
  Serial.println("[GBPrimePay] connecting ...");
  return _client.connect(GBPRIME_HOST, GBPRIME_PORT);
}

bool BlynkGO_GBPrimePay::requestQRCash(){
  if(!_is_inited) this->init();
  
  if( _token == "")             { Serial.println("[GBPrimePay] Error : no token."); return false; }
  if( this->_bill_no == "")     { Serial.println("[GBPrimePay] Error : no bill no."); return false; }
  if( this->_bill_amount == 0)  { Serial.println("[GBPrimePay] Error : no bill amount."); return false; }

  _try_connecting++;
  if(  _try_connecting > MAX_RETRY_CONNECTION ) { 
    Serial.println("[GBPrimePay] error : try reconnection."); 
    if( this->_error_fc ){
      this->_error_fc(GBP_ERR_CONNECTION);
    }
    return false;
  }

  _state = GBP_STATE_CONNECTING;
  Serial.printf("[GBPrimePay] retry connecting (%d) ...\n", _try_connecting);
  return _client.connect(GBPRIME_HOST, GBPRIME_PORT);
}

String BlynkGO_GBPrimePay::gen_bill_no(int16_t private_bill_no){
  String bill_no; // 15หลัก
  String private_bill_no_str = StringX::printf("%04d", (private_bill_no>-1)? constrain(private_bill_no,0,9999): random(10000));
  String random_str          = String(random(100000));
  if(BlynkGO.isNTPSynced()){
    bill_no = StringX::printf("%02d%02d%02d", year()%100, month(), day()) + private_bill_no_str + random_str;
  }else{
    bill_no = String(random(1000000))  + private_bill_no_str + random_str;
  }
  return bill_no;
}


static String urlencode(String url) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
//  char code2;
  for (int i = 0; i < url.length(); i++) {
    c = url.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
//      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
      //encodedString+=code2;
    }
  }
  encodedString.replace("%2E", ".");
  return encodedString;
}

WIFI_CONNECTED(){
  if( _p_gbp->_wifi_connected_fc) {
    _p_gbp->_wifi_connected_fc();
  }
  _server.begin(_p_gbp->_forward_port);
  MQTT.connect();
}

WIFI_DISCONNECTED(){
  if( _p_gbp->_wifi_disconnected_fc) {
    _p_gbp->_wifi_disconnected_fc();
  }  
}

MQTT_CONNECTED(){
  Serial.println("[MQTT] connected & subscribe topic");
  MQTT.subscribe(_p_gbp->_tunnel_topic.c_str());                 // กำหนด subscribe topic
  MQTT.subscribe(_p_gbp->_paid_topic.c_str());
}


MQTT_DISCONNECTED(){
  Serial.println("[MQTT] disconnected");  
}


#endif  //#if(BLYNKGO_USE_WIFI && BLYNKGO_USE_NTP && BLYNKGO_USE_MQTT  && BLYNKGO_USE_QRPAYMENT )
#endif
#endif
#endif
