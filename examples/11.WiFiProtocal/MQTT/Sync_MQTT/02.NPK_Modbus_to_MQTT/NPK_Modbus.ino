#define NPK_Modbus_Serial    Serial2 

namespace NPK {
  const byte nitro_cmd[]  = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
  const byte phos_cmd[]   = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
  const byte pota_cmd[]   = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

  uint8_t _rx, _tx, _de, _re;
  byte _values[8];
  
  void begin(uint8_t rx, uint8_t tx, uint8_t de, uint8_t re) {
    _rx = rx; _tx = tx; _de = de; _re = re;
    NPK_Modbus_Serial.begin(9600, SERIAL_8N1, _rx, _tx);
    pinMode(_de, OUTPUT); digitalWrite(_de, LOW);
    pinMode(_re, OUTPUT); digitalWrite(_re, LOW)
  }
  
  byte _get_sensor(const byte *cmd, size_t len){
    memset(_values,0, sizeof(_values));
    digitalWrite(_de, HIGH);
    digitalWrite(_re, HIGH);
    delay(10);
    if(NPK_Modbus_Serial.write(cmd, len)== len){
      digitalWrite(_de, LOW);
      digitalWrite(_re, LOW);
      if( NPK_Modbus_Serial.available()){
        NPK_Modbus_Serial.readBytes((uint8_t *)_values, sizeof(_values) );
      }
    }
    return _values[4];
  }
  byte nitrogen()       { return _get_sensor( nitro_cmd , sizeof(nitro_cmd)); }
  byte phosphorous()    { return _get_sensor( phos_cmd  , sizeof(phos_cmd));  }
  byte potassium()      { return _get_sensor( pota_cmd  , sizeof(pota_cmd));  }
}
