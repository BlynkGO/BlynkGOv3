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
 *  ด้วยการตั้งเวลาเป็นระยะๆ ส่งแบบรอจนจบขั้นตอนส่ง (non-async)
 *
 * หมายเหตุ 
 *  แนะนำให้ใช้ตัวอย่าง 03.LoRa32_Sender_NonBlockingCallback 
 *  ซึ่งเป็นการส่งแบบ async แทน
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
    // ตั้งเวลาทุกๆ 5000ms ทำการส่งข้อมูลออกไป
    lora_timer.setInterval(5000,[](){
      String data = String("Hello ") + String(counter++);
      LoRa.beginPacket();
      LoRa.print(data);
      LoRa.endPacket();

      label = String("[Sender]\n" + data);
    });
  }  
}

void loop(){
  BlynkGO.update();  
}

