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
 *  ด้วยการตั้งเวลาเป็นระยะๆ ส่งแบบไม่ต้องรอจนจบขั้นตอนส่ง  (async)
 *  โดยมีการกำหนด callback function เมื่อกระบวนการส่งได้ส่งเสร็จ
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
      Serial.print("Sending packet non-blocking: ");
      String message = String("Hello ") + String(counter++);
      LoRa.send(message);

      label = String("[Sender]\n" + message);
    });

    LoRa.onSent([](){   // เมื่อข้อมูลส่งสำเร็จจะมาตรงนี้
      Serial.println(" message sent");
    });
  }
}

void loop(){
  BlynkGO.update();  
}


