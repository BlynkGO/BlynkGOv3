/****************************************************
 * ก่อนใช้งาน GDatabase
 * ให้กำหนด เปิด การใช้งาน GDatabase ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_GDATABASE    1
 *
 ***************************************************/

#include <BlynkGOv3.h>

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // SD.remove("/DHT22_DATABASE.db");     // คำสั่งลบ file ฐานข้อมูล บน SD
  // SPIFFS.remove("/DHT22_DATABASE.db"); // คำสั่งลบ file ฐานข้อมูล บน SPIFFS

  try {
    // เปิดแฟ้ม database  หากวางไว้บน SPIFFS ให้เรียกใช้ db("SPIFFS://DHT22_DATABASE.db");
    GDatabase  db("SD://DHT22_DATABASE.db"); 

    // db.exec( "DROP TABLE IF EXISTS dht22_sensor;"); 

    if(!db.tableExists("dht22_sensor")) {
      // คำสั่ง API exec(...) ของ อ๊อบเจ็ค db 
      // ใช้สำหรับยิงคำสั่ง sql แบบ sql เดี่ยวๆ หรือ แบบหลายๆ sql ในคราวเดียวก็ได้
      db.exec( "CREATE TABLE dht22_sensor (date date, time time, name varchar(50), temperature float(4), humid float(4));" );

      // ตัวอย่างยิงคำสั่ง sql แบบ หลายๆ sql ในคราวเดียว จาก SQL_insert_data ที่เครียมไว้
      const char* SQL_insert_data = R"(
        INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:00', 'dht22_01', 23.12, 30.11 );
        INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:05', 'dht22_01', 23.55, 31.52 );
        INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:05', 'dht22_02', 25.47, 25.75 );
        INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:10', 'dht22_03', 21.34, 20.35 );
        INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:10', 'dht22_01', 23.60, 30.67 );
        INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:20', 'dht22_01', 24.15, 31.43 );
      )";
      db.exec( SQL_insert_data );     // ยิงคำสั่ง sql แบบ หลายๆ sql ในคราวเดียว
    }

    // ตัวอย่าง คำสั่งหาจำนวนแถว ของ ตาราง
    int row_count = db.execAndGet("SELECT COUNT(*) FROM dht22_sensor").toInt();
    Serial.println();
    Serial.print("Total row in table 'dht22_sensor' : ");
    Serial.println(row_count);

    // อ๊อบเจ็ค GQuery จะใช้สำหรับ ยิงคำสั่ง sql แบบ ต้องการรอรับผลข้อมูลที่ดึงคืนกลับมาจาก table บน GDatabase
    Serial.println("Query GDatabase ...");
    GQuery query(db, "SELECT time, name, temperature, humid FROM dht22_sensor");
    int r=0;
    while ( query.available()) {
      if(r==0) {
        Serial.println("------------------------------------------------------------------");
        for(int c=0; c < query.getColumnCount(); c++){
          Serial.printf("%-15s\t", query.getColumnName(c));
        }
        Serial.println();
        Serial.println("------------------------------------------------------------------");
      }
      
      r++;
      for(int c=0; c <query.getColumnCount(); c++){
        Serial.printf("%-15s\t", query.getColumn(c).c_str());
      }
      Serial.println();
    }
  }
  catch (String exception){     // ส่วนของ catch(...) ใส่เพื่อ ดักป้องกันเมื่อการเรียกใช้ GDatabase หากมีความผิดพลาดจะมาแสดงตรงส่วนนี้
    Serial.printf("[Exception] %s\n", exception.c_str());
    while(1) delay(1);          // สั่งให้ ESP32 หยุดการทำงานลง หากไม่ต้องการให้เอาบรรทัดนี้ออก
  }
  catch (std::exception& e){
    Serial.printf("[Exception] %s\n", e.what());
    while(1) delay(1);          // สั่งให้ ESP32 หยุดการทำงานลง หากไม่ต้องการให้เอาบรรทัดนี้ออก
  }
}

void loop() {
  BlynkGO.update();
}

