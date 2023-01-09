/****************************************************
 * ก่อนใช้งาน GDatabase
 * ให้กำหนด เปิด การใช้งาน GDatabase ให้ BlynkGO ได้ที่
 * ไฟล์ config/blynkgo_config.h
 * โดยกำหนดเป็น
 *     #define BLYNKGO_USE_GDATABASE    1
 *
 ***************************************************/

 
#include <BlynkGOv3.h>

GTable table;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  GDatabase  db("SD://DHT22_DATABASE.db"); // เปิดแฟ้ม database

  db.exec("DROP TABLE IF EXISTS dht22_sensor;");
  db.exec("CREATE TABLE dht22_sensor (date date, time time, name varchar(50), temperature float(4), humid float(4));");
  db.exec("INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:00', 'dht22_01', 23.12, 30.11 );");
  db.exec("INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:05', 'dht22_01', 23.55, 31.52 );");
  db.exec("INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:05', 'dht22_02', 25.47, 25.75 );");
  db.exec("INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:10', 'dht22_03', 21.34, 20.35 );");
  db.exec("INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:10', 'dht22_01', 23.60, 30.67 );");
  db.exec("INSERT INTO dht22_sensor VALUES ('2018-11-28', '10:00:20', 'dht22_01', 24.15, 31.43 );");

  table.create();

  // กำหนดความกว้างของ column ต่างๆ
  table.column(0).width(120);               
  table.column(1).width(120);
  table.column(2).width(120);  
  table.column(3).width(100);  

  table.style[TYPE1].font(prasanmit_25);    // TYPE1 สำหรับ ทั่วไป
  table.style[TYPE2].font(prasanmit_25);    // TYPE2 สำหรับ header


  Serial.println("Query SQLite ...");
  GQuery query(db, "SELECT time, name, temperature, humid FROM dht22_sensor");
  int r=0;
  while ( query.available()) {
    if(r==0) {
      Serial.println("----------------------------------------------------");
      for(int c=0; c < query.getColumnCount(); c++){
        Serial.printf("%-15s\t", query.getColumnName(c));
        table.cell(0,c) = query.getColumnName(c);
      }
      Serial.println();
      Serial.println("----------------------------------------------------");
    }
    r++;
    
    for(int c=0; c <query.getColumnCount(); c++){
      Serial.printf("%-15s\t", query.getColumn(c).c_str());
      table.cell(r,c) = String(query.getColumn(c).c_str());
    }
    Serial.println();
  }

 	// จัดชิด ให้จัดตอนท้ายของทั้งหมด
  table.column(0).align(LABEL_ALIGN_CENTER);  // column 0 จัดเรียงแบบ ชิดกลาง
  table.column(1).align(LABEL_ALIGN_CENTER);  // column 1 จัดเรียงแบบ ชิดกลาง
  table.column(2).align(LABEL_ALIGN_RIGHT);   // column 2 จัดเรียงแบบ ชิดขวา
  table.column(3).align(LABEL_ALIGN_RIGHT);   // column 3 จัดเรียงแบบ ชิดขวา
  table.row(0).align(LABEL_ALIGN_CENTER);     // row 0 จัดเรียงแบบ ชิดกลาง

}

void loop() {
  BlynkGO.update();
}

