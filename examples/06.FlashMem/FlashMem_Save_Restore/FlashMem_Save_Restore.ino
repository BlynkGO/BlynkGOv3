/********************************************************
 * 
 * BlynkGO สนับสนุน การจำค่าของตัวแปร ลงบน Flash Memory 
 * โดยผ่านการกำหนด key และ ค่าที่ต้องการบันทึก
 * เพื่อสามารถเรียกคืนขึ้นมาใช้ภายหลังได้ โดยข้อมูลไม่หายไป
 * 
 * ข้อดี : การบันทึกค่าลงบน Flash Memory นี้จะใช้สะดวกกว่า EEPROM 
 * 
 ********************************************************/

#include <BlynkGOv3.h>

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();

  //---------------------------------------
  // จำค่า และ ดึงค่า ลงบน flash memory ด้วยตัวแปร String
  String ssid     = "MySSID";
  String password = "MyPassword";
  
  BlynkGO.flashMem("SSID"    , ssid);                     // จำค่า ssid     ด้วย key "SSID"     ลงบน flash memory
  BlynkGO.flashMem("PASSWORD", password);                 // จำค่า password ด้วย key "PASSWORD" ลงบน flash memory

  String ssid2    = BlynkGO.flashMem_String("SSID");      // ดึงค่า key "SSID"      ออกมาเป็น String
  String password2= BlynkGO.flashMem_String("PASSWORD");  // ดึงค่า key "PASSWORD"  ออกมาเป็น String
  
  Serial.println(ssid2);
  Serial.println(password2);

  //---------------------------------------
  // จำค่า และ ดึงค่า ลงบน flash memory ด้วยตัวแปร const char*
  char * note     = "This is a note."; 
  BlynkGO.flashMem("NOTE", note);                         // จำค่า note  ด้วย key "NOTE" ลงบน flash memory

  char* note2;
  note2  = BlynkGO.flashMem_CharArray("NOTE", note2);     // ดึงค่า key "NOTE"  ออกมาเป็น char array ให้ตัวแปร note2
  Serial.println(note2);
  
  //---------------------------------------
  // จำค่า และ ดึงค่า ลงบน flash memory ด้วยตัวแปร int16_t
  int16_t distance = 245;
  BlynkGO.flashMem("DISTANCE", distance);                           // จำค่า distance  ด้วย key "DISTANCE" ลงบน flash memory

  int16_t distance2 =  (int16_t) BlynkGO.flashMem_Int("DISTANCE");  // ดึงค่า key "PASSWORD"  ออกมาเป็น int16_t
  Serial.println(distance2);

  //---------------------------------------
  // จำค่า และ ดึงค่า ลงบน flash memory ด้วยตัวแปร float
  float temperature = 89.13582;
  BlynkGO.flashMem("TEMPERATURE", temperature);                     // จำค่า temperature  ด้วย key "TEMPERATURE" ลงบน flash memory

  float temperature2 = BlynkGO.flashMem_Float("TEMPERATURE");       // ดึงค่า key "TEMPERATURE"  ออกมาเป็น float
  Serial.println(temperature2, 5 ); 

  //---------------------------------------
  // จำค่า และ ดึงค่า ลงบน flash memory ด้วยตัวแปร Object
  uint8_t mac[6] = {0xDF, 0xEE, 0x10, 0x49, 0xA1, 0x42};
  BlynkGO.flashMem( "MAC", &mac, sizeof(mac) );                     // จำค่า mac[6]   ด้วย key "MAC"  ลงบน flash memory

  uint8_t mac2[6];
  BlynkGO.flashMem_Object( "MAC", &mac2, sizeof(mac2) );            // ดึงค่า key "MAC"  ออกมาเป็น object ให้ตัวแปร mac2[6]
  
  Serial.printf("mac : %02X:%02X:%02X:%02X:%02X:%02X\n", 
                        mac2[0],mac2[1],mac2[2],mac2[3],mac2[4],mac2[5]);
                        
  //---------------------------------------
  // จำค่า และ ดึงค่า timestamp แบบ array ลงบน flash memory 
#define TIMESTAMP(h,m)          ((h%24)*3600 + (m%60)*60)       // ตัวแปลง hh:mm --> timestamp (จำนวนเต็มบนเส้นเวลา)
#define TIMESTAMP2Hour(ts)      ((ts%86400)/3600)               // ตัวแปลง timestamp --> hh
#define TIMESTAMP2Minute(ts)    (((int)(ts%86400)/60) %60)      // ตัวแปลง timestamp --> mm
#define TIMESTAMP2String(ts)    StringX::printf("%02d:%02d", TIMESTAMP2Hour(ts), TIMESTAMP2Minute(ts) ) // ตัวแปลง timestamp --> hh:mm

  // เก็บ array ของ timestamp ตามเวลา hh:mm ที่กำหนด
  time_t  timestamp[4]  = { TIMESTAMP(2,30), TIMESTAMP(4,0), TIMESTAMP(5,30), TIMESTAMP(8,0)} ;
  BlynkGO.flashMem("TIMESTAMP", &timestamp, sizeof(timestamp));             // จำค่า timestamp[4]  ด้วย key "TIMESTAMP" ลงบน flash memory

  time_t timestamp2[4];
  BlynkGO.flashMem_Array("TIMESTAMP", &timestamp2, sizeof(timestamp2));    // ดึงค่า ออกจาก flashMem มาใส่ตัวแปร timestamp2[6]

  for(int i=0; i< 4; i++){
    Serial.printf("[%d] %s\n",  i, TIMESTAMP2String(timestamp2[i]).c_str());
  }

  //---------------------------------------
  // สำหรับเช็คว่า มี key "SSID" อยู่บน flashMem แล้วหรือไม่
  bool isExist = BlynkGO.flashMem_exists("SSID");
  Serial.printf(" Is exists 'SSID' key ? : %s\n", (isExist)? "True" : "False");
}

void loop() {
  BlynkGO.update();
}

