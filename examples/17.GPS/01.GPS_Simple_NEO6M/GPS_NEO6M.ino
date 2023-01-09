/****************************************************
 * ก่อน compile ให้เข้าไปตั้งค่าใน blynkgo_config.h 
 * ให้ เปิดใช้ GPS เป็นดังนี้
 *  
 *       #define BLYNKGO_USE_GPS           1 
 * 
 ****************************************************/

namespace GPS_NEO6M {
  GTimer _timer;
  TinyGPSPlus _gps;
  uint32_t _interval  = 5000;
  void (*_fn)(void)   = NULL;
  void begin(uint8_t rx, uint8_t tx){
    Serial2.begin(9600, SERIAL_8N1, rx, tx, false);
    _timer.setOnce(5000,[](){
      if(GPS_NEO6M::_gps.charsProcessed() < 10){
        Serial.println(F("[GSP] error : no GPS detected."));
      }else{
        GPS_NEO6M::_timer.setInterval(GPS_NEO6M::_interval,[](){
          while (Serial2.available() > 0){
            if (GPS_NEO6M::_gps.encode(Serial2.read())){
              if( GPS_NEO6M::_gps.location.isValid() &&GPS_NEO6M::_fn != NULL){
                GPS_NEO6M::_fn();
              }
            }
          }
        });
      }
    });
  }
  void onGPS( uint32_t interval, void (*fn)(void))  { _interval = interval; _fn = fn; }
  double lat()            { return GPS_NEO6M::_gps.location.lat(); }
  double lon()            { return GPS_NEO6M::_gps.location.lng(); } 

  int8_t day()            { return (!GPS_NEO6M::_gps.date.isValid())? -1: GPS_NEO6M::_gps.date.day();         }
  int8_t month()          { return (!GPS_NEO6M::_gps.date.isValid())? -1: GPS_NEO6M::_gps.date.month();       }
  int16_t year()          { return (!GPS_NEO6M::_gps.date.isValid())? -1: GPS_NEO6M::_gps.date.year();        } 

  int8_t hour()           { return (!GPS_NEO6M::_gps.time.isValid())? -1: GPS_NEO6M::_gps.time.hour();        }
  int8_t minute()         { return (!GPS_NEO6M::_gps.time.isValid())? -1: GPS_NEO6M::_gps.time.minute();      }
  int8_t second()         { return (!GPS_NEO6M::_gps.time.isValid())? -1: GPS_NEO6M::_gps.time.second();      }
  int8_t centisecond()    { return (!GPS_NEO6M::_gps.time.isValid())? -1: GPS_NEO6M::_gps.time.centisecond(); }
}

