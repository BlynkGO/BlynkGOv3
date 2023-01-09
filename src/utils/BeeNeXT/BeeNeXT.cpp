#include "BeeNeXT.h"

#if defined(BEENEXT)

void BeeNeXT_NoOpCbk() {}
BEENEXT_DATA()    __attribute__((weak, alias("BeeNeXT_NoOpCbk")));
SERIAL_DATA()    	__attribute__((weak, alias("BeeNeXT_NoOpCbk")));

BeeNeXT_class BeeNeXT;

BeeNeXT_class::~BeeNeXT_class(){
  this->end();
}

void BeeNeXT_class::begin(HardwareSerial *serial){
  this->end();
#if CONFIG_IDF_TARGET_ESP32S3
#if defined(BEENEXT_4_3) ||  defined(BEENEXT_4_3C) ||  defined(BEENEXT_4_3IPS)
  pinMode(18,OUTPUT);
  digitalWrite(18, LOW); // ทำขา 18 เป็น GND
#endif
  Serial2.begin(115200);  // RX19 ; TX20
  _hw_serial = &Serial2;
#else
  _hw_serial = (serial == NULL)? &Serial : serial; 
#endif
  _hw_serial->setTimeout(50);
}

void BeeNeXT_class::begin(HardwareSerial &serial ){
  this->begin(&serial);
}

#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
void BeeNeXT_class::begin(unsigned long baud, uint8_t rx, uint8_t tx){
  this->end();
  _sw_serial = new SoftwareSerial();
  if(_sw_serial != NULL) {
    _is_swserial_alloced = true;
    _sw_serial->begin(baud, rx, tx);
    _sw_serial->setTimeout(50);
  }
}
void BeeNeXT_class::begin(uint8_t rx, uint8_t tx){
  this->begin(115200, rx,tx);
}
#endif //#if BEENEXT_USE_SOFTWARESERIAL

// #if defined(CONFIG_IDF_TARGET_ESP32S3)
// void BeeNeXT_class::begin(unsigned long baud, uint8_t rx, uint8_t tx){
//   Serial.println("[BeeNeXT] use Serial2");
//   Serial2.begin(baud, SERIAL_8N1, rx, tx);
//   this->begin(&Serial2);
// }

// void BeeNeXT_class::begin(uint8_t rx, uint8_t tx){
//   this->begin(115200, rx, tx);
// }
// #endif // #if defined(ESP32)

void BeeNeXT_class::end(){
  if( _hw_serial != NULL) { 
    _hw_serial->end(); 
    _hw_serial = NULL;
  }
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  if( _sw_serial != NULL ){
    _sw_serial->end();
    if(_is_swserial_alloced){
      free(_sw_serial); _sw_serial = NULL;
      _is_swserial_alloced = false;
    }
  }
#endif
}

void BeeNeXT_class::update(){
  if(_hw_serial != NULL) {
    if(_hw_serial->available()){
      String data = _hw_serial->readStringUntil('\n'); _data.trim();
      if(data.startsWith("[BN]")){
        data.replace("[BN]", "");
        _data = data;
        BeeNeXT_onData();
      }else{
        BeeNeXT_onSerialData();
      }
    }
  }
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  else if(_sw_serial !=NULL ){
    if(_sw_serial->available()){
      String data = _sw_serial->readStringUntil('\n'); _data.trim();
      if(data.startsWith("[BN]")){
        data.replace("[BN]", "");
        _data = data;
        BeeNeXT_onData();
      }else{
        BeeNeXT_onSerialData();
      }
    }
  }
#endif

#if BEENEXT_USE_SOFTTIMER
  SoftTimer::run();
#endif

}

void BeeNeXT_class::command(uint16_t cmd) { 
  if(cmd>=0 && cmd < CMD_MAX) {
    String _cmd = String("cmd:")+String(cmd);
    Serial.println(_cmd);
    this->println(_cmd); 
  }
}

//------------------------------------------------------------
//virtual function
size_t BeeNeXT_class::write(uint8_t data){
  if(_hw_serial){
    _hw_serial->write('[');_hw_serial->write('B');_hw_serial->write('N');_hw_serial->write(']');
    size_t sz = _hw_serial->write(data);
    return sz+4;
  }
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  else if( _sw_serial){
    _sw_serial->write('[');_sw_serial->write('B');_sw_serial->write('N');_sw_serial->write(']');
    size_t sz = _sw_serial->write(data);
    return sz+4;
  }
#endif
}

size_t BeeNeXT_class::write(const uint8_t *buffer, size_t size){
  if(_hw_serial){
    _hw_serial->write('[');_hw_serial->write('B');_hw_serial->write('N');_hw_serial->write(']');
    size_t sz = _hw_serial->write(buffer, size);
    return sz + 4;
  }
#if BEENEXT_USE_SOFTWARESERIAL && (CONFIG_IDF_TARGET_ESP32S3==0)
  else if(_sw_serial){
    _sw_serial->write('[');_sw_serial->write('B');_sw_serial->write('N');_sw_serial->write(']');
    size_t sz = _sw_serial->write(buffer, size);
    return sz + 4;
  }
#endif
}


#endif //#if defined(BEENEXT)
