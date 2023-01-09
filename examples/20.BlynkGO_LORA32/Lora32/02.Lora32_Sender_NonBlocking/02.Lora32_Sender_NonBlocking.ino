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
 * ตัวอย่าง เป็นการส่งข้อมูลออกไปแบบไม่เจาะจง LoRa อื่นๆ ใดๆ สามารถรับข้อมูลได้
 *  ด้วยการตั้งเวลาเป็นระยะๆ ส่งแบบไม่ต้องรอจนจบขั้นตอนส่ง (async)
 *  
 **********************************************************************/


#include <BlynkGOv3.h>

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define LORA_FREQUENCY 915E6

GLabel label;
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
  }
}

void loop(){
  BlynkGO.update();  

    // wait until the radio is ready to send a packet
  if (LoRa.beginPacket() == 0) {
    Serial.println("waiting for radio ... ");
    return;
  }
  
  Serial.print("Sending packet non-blocking: ");
  String message = String("Hello ") + String(counter++);

  // send in async / non-blocking mode
  LoRa.send(message);
  label = String("[Sender]\n" + message);
}


