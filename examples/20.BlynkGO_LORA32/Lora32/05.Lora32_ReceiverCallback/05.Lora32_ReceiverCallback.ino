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
 *  ด้วยการทำเป็น callback function เมื่อมีข้อมูลเข้ามา
 *  ระบบจะเรียก callback function ให้
 *
 *  แบบนี้จะเหมาะสมในการใช้งานกว่า ตัวอย่าง 04.LoRa32_Receive 
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

    LoRa.receive_mode();
    LoRa.onReceived([](int packet_sz){
      // received a packet
      Serial.print("Received packet '");
  
      // read packet
      String data="";
      for (int i = 0; i < packet_sz; i++) {
        data += (char) LoRa.read();             // รับข้อมูลที่เข้ามาทาง Lara
      }
      label = String("[Receiver]\n") + data;    // แสดงข้อมูล ไปที่ วิตเจ็ตข้อความ

      Serial.print(data);
      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    });
  }  
}

void loop(){
  BlynkGO.update();  
}


