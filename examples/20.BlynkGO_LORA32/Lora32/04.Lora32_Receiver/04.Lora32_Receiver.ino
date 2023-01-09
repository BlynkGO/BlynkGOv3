/**********************************************************************
 *
 *  ก่อนใช้งาน
 *  เข้าไปที่ BlynkGOv3/src/config/blynkgo_config.h
 *    เปลี่ยนบอร์ดเป็น
 *       #define  BLYNKO_LORA32
 *
 *    และที่ ArduinoIDE เลือกบอร์ดเป็น BlynkGO Lora32
 *
 **********************************************************************/

/**********************************************************************
 * ตัวอย่าง เป็นการรับข้อมูลที่ ส่งมาจากอีก LoRa อื่นๆ
 *  ด้วยการตั้งเวลาเป็นระยะๆ ดัก packet ว่ามีเข้ามาหรือไม่
 *
 * หมายเหตุ 
 *  แนะนำให้ใช้ตัวอย่าง 05.LoRa32_ReceiveCallback แทน
 *  
 **********************************************************************/

#include <BlynkGOv3.h>

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define LORA_FREQUENCY 915E6

GLabel label;
GTimer lora_timer;
int    counter =0;

void setup(){
  Serial.begin(115200); Serial.println();

  BlynkGO.begin();

  label.font(prasanmit_20);
  label.text_line_space(-5);

  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("[LORA] failed!");
    label = "[LORA] failed!";
  } else {
    Serial.println("[LORA] inited.");
    // ตั้งเวลาทุกๆ 10ms ทำการเช็คข้อมูลที่เข้ามา
    lora_timer.setInterval(10,[](){
      int packet_sz = LoRa.parsePacket();
      if( packet_sz ){
          // received a packet
          Serial.print("Received packet '");
      
          // read packet
          String data="";
          while(LoRa.available()) {
            data += (char) LoRa.read();             // รับข้อมูลที่เข้ามาทาง Lara
          }
          label = String("[Receiver]\n") + data;    // แสดงข้อมูล ไปที่ วิตเจ็ตข้อความ
      	
      	  Serial.print(data);
          // print RSSI of packet
          Serial.print("' with RSSI ");
          Serial.println(LoRa.packetRssi());
      }
    });
  }  
}

void loop(){
  BlynkGO.update();  
}


