/*
 *   ตัวอย่าง Open-Meteo API:
 *     https://api.open-meteo.com/v1/forecast?latitude=13.7563&longitude=100.5018&current=temperature_2m,relative_humidity_2m,apparent_temperature,rain,cloud_cover,surface_pressure,wind_speed_10m,wind_direction_10m&hourly=temperature_2m,relative_humidity_2m,apparent_temperature,rain,wind_speed_10m,wind_direction_10m&timezone=Asia%2FBangkok&forecast_days=1
 *
 *   หมายเหตุ:
 *     1. ปรับแต่ง API ได้ที่: https://open-meteo.com/en/docs 
 *        เพื่อเพิ่มข้อมูลพยากรณ์อากาศที่ต้องการ
 *
 *     2. เปลี่ยน:
 *           latitude=13.7563&longitude=100.5018 
 *        เป็น:
 *           latitude=%f&longitude=%f
 *
 *     3. เปลี่ยน:
 *           timezone=Asia%2FBangkok 
 *        เป็น:
 *           timezone=Asia%%2FBangkok
 */


#include <BlynkGOv3.h>    // version 3.1.1
#include <ArduinoJson.h>  // version 7.2.1
#include <HTTPClient.h>

#define GPS_LAT             13.7563
#define GPS_LONG            100.5018
#define OPEN_METEO_API      "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,relative_humidity_2m,apparent_temperature,rain,cloud_cover,surface_pressure,wind_speed_10m,wind_direction_10m&hourly=temperature_2m,relative_humidity_2m,apparent_temperature,rain,wind_speed_10m,wind_direction_10m&timezone=Asia%%2FBangkok&forecast_days=1"


GWiFiManager wifi_manager;

GContainer  container_gps;
  GTextArea    ta_gps_lat (container_gps);
  GTextArea    ta_gps_long(container_gps);

GContainer  container_weather;
  GLabel  lb_temp(container_weather);
  GLabel  lb_humid(container_weather);
  GLabel  lb_cur_time(container_weather);

SoftTimer timer_weather;


void Weather_Request();

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-10);

  container_gps.radius(10);
  container_gps.width(200);
  container_gps.layout(LAYOUT_COL_M, 15,15,15,15,10);
  container_gps.align(ALIGN_TOP,0,10);
    ta_gps_lat.width(container_gps.content_width());  ta_gps_lat.cursor_type(CURSOR_NONE);  ta_gps_lat  = String(GPS_LAT,4);
    ta_gps_long.width(container_gps.content_width()); ta_gps_long.cursor_type(CURSOR_NONE); ta_gps_long = String(GPS_LONG,4);

    ta_gps_lat.onValueChanged([](GWIDGET){
      Weather_Request();
    });
    ta_gps_long.onValueChanged([](GWIDGET){
      Weather_Request();
    });

  container_weather.opa(0);
  container_weather.layout(LAYOUT_COL_L, 0,0,0,0,0);
  container_weather.align(container_gps, ALIGN_BOTTOM_LEFT, 0,30);
    lb_temp  = "อุณหภูมิ : ";
    lb_humid = "ความชื้น : ";
    lb_cur_time = "";
    lb_cur_time.font(prasanmit_20);
  
}

void loop(){
  BlynkGO.update();
}

WIFI_CONNECTED(){
  // ที่ website API จะมีอัพเดตค่าทุกๆ 15นาที  หากเข้าไป request เร็วกว่า 15นาที ไม่มีประโยชน์
  timer_weather.setInterval(15*60*1000, [](){ 
    Weather_Request();
  }, true); // true : ให้ ทำงานแต่แรกเลย แล้วเว้นไปทุกๆช่วงที่กำหนด
}

WIFI_DISCONNECTED(){
  timer_weather.del();
}


void Weather_Request(){
  String open_meteo_api = StringX::printf(OPEN_METEO_API, ta_gps_lat.toFloat(), ta_gps_long.toFloat());
  Serial.println("[OPEN_METRO] request... : " + open_meteo_api);
  
  std::unique_ptr<WiFiClientSecure>client (new WiFiClientSecure);

  if(client) {
    client->setInsecure();  // สำหรับ WiFiClientSecure  443 แบบไม่ต้องเช็ค secure
    {
      HTTPClient http;
      http.begin(*client, open_meteo_api);
      int http_code = http.GET();
      Serial.println(http_code);
      if( http_code == 200) {
        String payload = http.getString();
        // Serial.println(payload);
        // Serial.println("-----------------------------");

        // ตย. payload ที่ได้รับ response กลับ
        //
        //     {"latitude":13.75,"longitude":100.5,"generationtime_ms":0.07402896881103516,"utc_offset_seconds":25200,"timezone":"Asia/Bangkok","timezone_abbreviation":"+07","elevation":4.0,"current_units":{"time":"iso8601","interval":"seconds","temperature_2m":"°C","relative_humidity_2m":"%","apparent_temperature":"°C","rain":"mm","cloud_cover":"%","surface_pressure":"hPa","wind_speed_10m":"km/h","wind_direction_10m":"°"},"current":{"time":"2024-12-13T23:00","interval":900,"temperature_2m":26.5,"relative_humidity_2m":77,"apparent_temperature":31.0,"rain":0.00,"cloud_cover":100,"surface_pressure":1010.9,"wind_speed_10m":4.0,"wind_direction_10m":350},"hourly_units":{"time":"iso8601","temperature_2m":"°C","relative_humidity_2m":"%","apparent_temperature":"°C","rain":"mm","wind_speed_10m":"km/h","wind_direction_10m":"°"},"hourly":{"time":["2024-12-13T00:00","2024-12-13T01:00","2024-12-13T02:00","2024-12-13T03:00","2024-12-13T04:00","2024-12-13T05:00","2024-12-13T06:00","2024-12-13T07:00","2024-12-13T08:00","2024-12-13T09:00","2024-12-13T10:00","2024-12-13T11:00","2024-12-13T12:00","2024-12-13T13:00","2024-12-13T14:00","2024-12-13T15:00","2024-12-13T16:00","2024-12-13T17:00","2024-12-13T18:00","2024-12-13T19:00","2024-12-13T20:00","2024-12-13T21:00","2024-12-13T22:00","2024-12-13T23:00"],"temperature_2m":[28.4,28.6,28.2,27.9,27.7,27.5,27.3,26.8,27.1,27.7,28.0,26.3,25.5,28.3,29.3,29.5,28.8,28.5,28.0,28.3,27.6,27.2,26.9,26.5],"relative_humidity_2m":[73,73,75,77,78,78,76,76,75,72,69,79,85,74,69,69,71,69,69,69,72,75,76,77],"apparent_temperature":[33.5,33.9,33.6,33.4,33.0,32.6,31.9,31.1,31.3,31.5,31.1,29.8,29.8,33.0,33.7,34.0,33.2,32.6,32.0,32.5,31.8,31.7,31.3,31.0],"rain":[0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.20,0.60,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00],"wind_speed_10m":[3.0,2.2,2.2,2.6,3.3,4.2,4.6,5.8,6.5,8.3,11.9,11.3,8.0,6.2,7.2,8.3,7.9,7.2,5.8,5.4,5.5,4.5,4.5,4.0],"wind_direction_10m":[14,9,9,34,41,59,72,90,84,72,55,31,352,339,6,2,357,357,353,4,349,346,346,350]}}
        //

        // ทำการถอดค่าจาก json ที่ได้รับ
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }

        float latitude                      = doc["latitude"];              // ค่า latitude
        float longitude                     = doc["longitude"];             // ค่า longitude
        double generationtime_ms            = doc["generationtime_ms"];     // ค่า generation time (ms)
        int utc_offset_seconds              = doc["utc_offset_seconds"];    // ค่า UTC offset
        const char* timezone                = doc["timezone"];              // ค่า timezone
        const char* timezone_abbreviation   = doc["timezone_abbreviation"]; // ค่า timezone abbreviation
        int elevation                       = doc["elevation"];             // ค่า elevation

        //--------------------------------------------------------------------------
        // ข้อมูลพยากรณ์อากาศ ปัจจุบัน (Current Data)  โดยที่เวป open-meteo จะ update ทุก 15 นาที
        //--------------------------------------------------------------------------
        // Current Units
        JsonObject current_units = doc["current_units"];
        const char* current_units_time                  = current_units["time"];                          // ค่า unit ของ time
        const char* current_units_interval              = current_units["interval"];                      // ค่า unit ของ interval
        const char* current_units_temperature_2m        = current_units["temperature_2m"];                // ค่า unit ของ temperature_2m
        const char* current_units_relative_humidity_2m  = current_units["relative_humidity_2m"];          // ค่า unit ของ relative_humidity_2m
        const char* current_units_apparent_temperature  = current_units["apparent_temperature"];          // ค่า unit ของ apparent_temperature
        const char* current_units_rain                  = current_units["rain"];                          // ค่า unit ของ rain
        const char* current_units_cloud_cover           = current_units["cloud_cover"];                   // ค่า unit ของ cloud_cover
        const char* current_units_surface_pressure      = current_units["surface_pressure"];              // ค่า unit ของ surface_pressure
        const char* current_units_wind_speed_10m        = current_units["wind_speed_10m"];                // ค่า unit ของ wind_speed_10m
        const char* current_units_wind_direction_10m    = current_units["wind_direction_10m"];            // ค่า unit ของ wind_direction_10m

        // Current Data 
        JsonObject current = doc["current"];
        const char* current_time                        = current["time"].as<const char*>();              // ค่า time ปัจจุบัน
        int current_interval                            = current["interval"].as<int>();                  // ค่า interval
        float current_temperature_2m                    = current["temperature_2m"].as<float>();          // ค่า temperature_2m
        float current_relative_humidity_2m              = current["relative_humidity_2m"].as<float>();    // ค่า relative_humidity_2m
        float current_apparent_temperature              = current["apparent_temperature"].as<float>();    // ค่า apparent_temperature
        int current_rain                                = current["rain"];                                // ค่า rain
        int current_cloud_cover                         = current["cloud_cover"];                         // ค่า cloud_cover
        float current_surface_pressure                  = current["surface_pressure"];                    // ค่า surface_pressure
        int current_wind_speed_10m                      = current["wind_speed_10m"];                      // ค่า wind_speed_10m
        int current_wind_direction_10m                  = current["wind_direction_10m"];                  // ค่า wind_direction_10m

        // อยากดึงค่าพยากรณ์อากาศไหน ไปใช้ ให้นำค่าจากด้านบน ไปใช้
        Serial.printf("[current] [%s] temp %.2f; humid %.2f, rain %d\n", current_time, current_temperature_2m, current_relative_humidity_2m, current_rain);
        lb_temp   = String("อุณหภูมิ : ") + String(current_temperature_2m, 2);
        lb_humid  = String("ความชื้น : ") + String(current_relative_humidity_2m, 2);
        lb_cur_time = String("(update : ") + current_time + String(")");

        //--------------------------------------------------------------------------
        // ข้อมูลพยากรณ์อากาศ แบบรายชั่วโมง (Hourly Data)
        //--------------------------------------------------------------------------
        JsonObject hourly_units = doc["hourly_units"];
        const char* hourly_units_time                   = hourly_units["time"];                           // ค่า unit ของ time ใน hourly
        const char* hourly_units_temperature_2m         = hourly_units["temperature_2m"];                 // ค่า unit ของ temperature_2m ใน hourly
        const char* hourly_units_relative_humidity_2m   = hourly_units["relative_humidity_2m"];           // ค่า unit ของ relative_humidity_2m ใน hourly
        const char* hourly_units_apparent_temperature   = hourly_units["apparent_temperature"];           // ค่า unit ของ apparent_temperature ใน hourly
        const char* hourly_units_rain                   = hourly_units["rain"];                           // ค่า unit ของ rain ใน hourly
        const char* hourly_units_wind_speed_10m         = hourly_units["wind_speed_10m"];                 // ค่า unit ของ wind_speed_10m ใน hourly
        const char* hourly_units_wind_direction_10m     = hourly_units["wind_direction_10m"];             // ค่า unit ของ wind_direction_10m ใน hourly

        // Hourly Data Arrays
        JsonArray hourly_time = doc["hourly"]["time"];                                                    // ข้อมูลเวลา รายชั่วโมง
        JsonArray hourly_temperature_2m = doc["hourly"]["temperature_2m"];                                // ข้อมูลอุณหภูมิใน รายชั่วโมง
        JsonArray hourly_relative_humidity_2m = doc["hourly"]["relative_humidity_2m"];                    // ข้อมูลความชื้นใน รายชั่วโมง
        JsonArray hourly_apparent_temperature = doc["hourly"]["apparent_temperature"];                    // ข้อมูล apparent_temperature ใน รายชั่วโมง

        const int hourly_count = 24; // จำนวนชั่วโมงที่ต้องการเก็บข้อมูล
        const char* hourly_time_array[hourly_count];                                                      // array สำหรับ เก็บเวลาใน hourly
        float hourly_temperature_2m_array[hourly_count];                                                  // array สำหรับ เก็บอุณหภูมิใน hourly
        int hourly_relative_humidity_2m_array[hourly_count];                                              // array สำหรับ เก็บความชื้นใน hourly
        float hourly_apparent_temperature_array[hourly_count];                                            // array สำหรับ เก็บ apparent_temperature ใน hourly

        // Populate arrays
        for (int i = 0; i < hourly_count; i++) { // ลูปเก็บข้อมูลใน array
          hourly_time_array[i]                  = hourly_time[i];                                         // เก็บเวลา ลงในตัวแปร array
          hourly_temperature_2m_array[i]        = hourly_temperature_2m[i];                               // เก็บอุณหภูมิ ลงในตัวแปร array
          hourly_relative_humidity_2m_array[i]  = hourly_relative_humidity_2m[i];                         // เก็บความชื้น ลงในตัวแปร array
          hourly_apparent_temperature_array[i]  = hourly_apparent_temperature[i];                         // เก็บ apparent_temperature ลงในตัวแปร array

          // ตย.นำมาแสดงออก Serial Monitor
          Serial.printf("[%s] temp %.2f; humid %.2f; apparent temp : %.2f\n", 
                              hourly_time_array[i], 
                              hourly_temperature_2m_array[i],
                              hourly_relative_humidity_2m_array[i],
                              hourly_apparent_temperature_array[i] );
        }
      }
    }
  }
}

/*
 *   คำอธิบายโค้ด:
 *   -----------
 *
 *   โค้ดนี้ใช้สำหรับเรียกข้อมูลพยากรณ์อากาศผ่าน Open-Meteo API โดยระบุพิกัด (latitude, longitude) ของสถานที่ที่ต้องการ เช่น กรุงเทพฯ
 *   สามารถปรับแต่ง API เพื่อกำหนดข้อมูลที่ต้องการ เช่น อุณหภูมิ ความชื้น ลม ฝน ฯลฯ
 * 
 *   ฟังก์ชันหลักในโค้ด:
 *   ---------------
 *         Weather_Request()
 *
 *     ใช้เรียกข้อมูลจาก API และประมวลผลผลลัพธ์ที่เป็น JSON
 *     ทำการเชื่อมต่อแบบปลอดภัยผ่าน WiFiClientSecure
 *     ใช้ ArduinoJson เพื่อดึงค่าที่สำคัญ เช่น อุณหภูมิ ความชื้น ความเร็วลม ฯลฯ
 *
 *     หาก WiFi เชื่อมต่อสำเร็จ ระบบจะตั้งเวลาขอข้อมูลจาก API ทุก 15 นาที (Open-Meteo อัปเดตข้อมูลทุก 15 นาที)
 *     และจะได้รับข้อมูล JSON ที่ได้จาก API
 *     และในโค้ดนี้จะดึงค่าต่างๆ จาก JSON ออกมา เช่น:
 *
 *         temperature_2m: อุณหภูมิ
 *         relative_humidity_2m: ความชื้นสัมพัทธ์
 *         wind_speed_10m: ความเร็วลม
 *
 *   
 *   หน้าที่ของ GUI (หน้าจอแสดงผล):
 *   ---------------------------
 *     ใช้ BlynkGOv3 สำหรับจัดวางวิดเจ็ตต่างๆ เช่น ข้อความ เพื่อแสดงค่าพยากรณ์อากาศ
 *     สามารถปรับค่าพิกัด (latitude, longitude) ได้จาก UI เพื่อขอข้อมูลพยากรณ์อากาศสำหรับพื้นที่อื่น
 *
 *   
 *   การใช้งานเพิ่มเติม:
 *   --------------
 *   สามารถแก้ไขค่าที่ส่งไปยัง API ได้ เพื่อเพิ่มหรือลดข้อมูลที่ต้องการ เช่น การเพิ่มการแสดงผลความกดอากาศ หรือจำนวนวันที่พยากรณ์อากาศ
 *   เพิ่มฟังก์ชันการแจ้งเตือนหรือบันทึกข้อมูลได้ตามต้องการ
 *
 */