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
 * LoRa32 Simple Gateway/Node Exemple
 * ตัวอย่าง ทำ Node อย่างง่าย  เพื่อส่งไปให้ Gateway
 * ใช้คู่กันกับ ตัวอย่าง LoRa32 Simple Gateway
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
    Serial.println("LoRa Simple Node");
    Serial.println("Only receive messages from gateways");
    Serial.println();

    LoRa.type(LORA_NODE);
    LoRa.onReceived([](int packet_sz) {     // เมื่อได้รับข้อมูลเข้ามา
      String message = "";
      while (LoRa.available()) {
        message += (char)LoRa.read();
      }
      Serial.print("[LORA] Node Receive: ");
      Serial.println(message);
      
      label = String("[Node]\n") + message;
    });
    
    LoRa.onSent([]() {                      // เมื่อข้อมูลส่งสำเร็จจะมาตรงนี้
      Serial.println("[LORA] message sent.");
      LoRa.receive_mode();                  // ให้สลับเป็นโหมด รับข้อมูล
    });

    // ตั้งเวลาทุกๆ 1000ms ทำการส่งข้อมูลออกไป
    lora_timer.setInterval(1000,[](){
      String message = "I'm a Node!\n";
      message += millis();
      LoRa.send(message);            // คำการส่ง message ไปยัง node
    });
  }
}

void loop(){
  BlynkGO.update();  
}


