// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef LORA_H
#define LORA_H

#include "config/blynkgo_config.h"

#if BLYNKGO_USE_LORA

#include <Arduino.h>
#include <SPI.h>

#if defined(ARDUINO_SAMD_MKRWAN1300)
#define LORA_DEFAULT_SPI           SPI1
#define LORA_DEFAULT_SPI_FREQUENCY 200000
#define LORA_DEFAULT_SS_PIN        LORA_IRQ_DUMB
#define LORA_DEFAULT_RESET_PIN     -1
#define LORA_DEFAULT_DIO0_PIN      -1
#elif defined(ARDUINO_SAMD_MKRWAN1310)
#define LORA_DEFAULT_SPI           SPI1
#define LORA_DEFAULT_SPI_FREQUENCY 200000
#define LORA_DEFAULT_SS_PIN        LORA_IRQ_DUMB
#define LORA_DEFAULT_RESET_PIN     -1
#define LORA_DEFAULT_DIO0_PIN      LORA_IRQ
#else
#define LORA_DEFAULT_SPI           SPI
#define LORA_DEFAULT_SPI_FREQUENCY 8E6 
#define LORA_DEFAULT_SS_PIN        10
#define LORA_DEFAULT_RESET_PIN     9
#define LORA_DEFAULT_DIO0_PIN      2
#endif

#define PA_OUTPUT_RFO_PIN          0
#define PA_OUTPUT_PA_BOOST_PIN     1

//BlynKGO
enum {
  LORA_NORMAL,
  LORA_GATEWAY,
  LORA_NODE,
};
typedef uint8_t  lora_type_t;


class LoRaClass : public Stream {
public:
  LoRaClass();

  int begin(long frequency);
  void end();

  int beginPacket(int implicitHeader = false);
  int endPacket(bool async = false);

  int parsePacket(int size = 0);
  int packetRssi();
  float packetSnr();
  long packetFrequencyError();

  int rssi();

  // from Print
  virtual size_t write(uint8_t byte);
  virtual size_t write(const uint8_t *buffer, size_t size);

  // from Stream
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();

#ifndef ARDUINO_SAMD_MKRWAN1300
  void onReceive(void(*callback)(int));
  void onTxDone(void(*callback)());
  
  // BlynkGO
  inline void onReceived(void(*callback)(int))  { onReceive(callback); }
  inline void onSent(void(*callback)())         { onTxDone(callback);  }

  void receive(int size = 0);
#endif
  void idle();
  void sleep();


  // BlynkGO
  void type(lora_type_t type)             { _lora_type = type; }
  lora_type_t type()                      { return _lora_type; }

  // BlynkGO
  void send_mode() {
    if(_lora_type==LORA_NORMAL) return;
    this->idle();                          // set standby mode
    switch(_lora_type){
      case LORA_GATEWAY: this->enableInvertIQ();  break;          // active invert I and Q signals
      case LORA_NODE   : this->disableInvertIQ(); break;          // normal mode
    }
  }
  
  // BlynkGO
  void receive_mode() {
    switch(_lora_type){
      case LORA_NORMAL : break;
      case LORA_GATEWAY: this->disableInvertIQ(); break;        // normal mode
      case LORA_NODE   : this->enableInvertIQ();  break;          // active invert I and Q signals
    }
    this->receive();                       // set receive mode
  }

  // BlynkGO
  void send(String message) {
    send_mode();                          // set send mode
    this->beginPacket();                   // start packet
    this->print(message);                  // add payload
    this->endPacket(true);                 // finish packet and send it
  }

  // BlynkGO
  void send(uint8_t *data, size_t data_sz) {
    send_mode();
    this->beginPacket();                   // start packet
    this->write(data, data_sz);            // add payload
    this->endPacket(true);                 // finish packet and send it
  }


  void setTxPower(int level, int outputPin = PA_OUTPUT_PA_BOOST_PIN);
  void setFrequency(long frequency);
  void setSpreadingFactor(int sf);
  void setSignalBandwidth(long sbw);
  void setCodingRate4(int denominator);
  void setPreambleLength(long length);
  void setSyncWord(int sw);
  void enableCrc();
  void disableCrc();
  void enableInvertIQ();
  void disableInvertIQ();
  
  void setOCP(uint8_t mA); // Over Current Protection control
  
  void setGain(uint8_t gain); // Set LNA gain

  // deprecated
  void crc() { enableCrc(); }
  void noCrc() { disableCrc(); }

  byte random();

  void setPins(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN, int dio0 = LORA_DEFAULT_DIO0_PIN);
  void setSPI(SPIClass& spi);
  void setSPIFrequency(uint32_t frequency);

  void dumpRegisters(Stream& out);

private:
  void explicitHeaderMode();
  void implicitHeaderMode();

  void handleDio0Rise();
  bool isTransmitting();

  int getSpreadingFactor();
  long getSignalBandwidth();

  void setLdoFlag();

  uint8_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint8_t value);
  uint8_t singleTransfer(uint8_t address, uint8_t value);

  static void onDio0Rise();

private:
  SPISettings _spiSettings;
  SPIClass* _spi;
  int _ss;
  int _reset;
  int _dio0;
  long _frequency;
  int _packetIndex;
  int _implicitHeaderMode;
  void (*_onReceive)(int);
  void (*_onTxDone)();

  lora_type_t  _lora_type = LORA_NORMAL;
};

extern LoRaClass LoRa;


#endif //#if BLYNKGO_USE_LORA

#endif
