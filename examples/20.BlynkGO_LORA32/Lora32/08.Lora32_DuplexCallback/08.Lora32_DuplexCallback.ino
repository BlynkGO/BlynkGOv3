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
 * ตัวอย่าง ใช้ LoRa ทำงานทั้งส่งและรับในตัวเดียวกัน 
 * โดยใช้ callback function ในการรับข้อมูล
 * 
 **********************************************************************/


#include <BlynkGOv3.h>

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define LORA_FREQUENCY 915E6

GLabel label;

String outgoing;              // outgoing message
byte msgCount     = 0;        // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination  = 0xFF;     // destination to send to
long lastSendTime = 0;        // last send time
int interval      = 2000;     // interval between sends

void setup() {
  Serial.begin(115200); Serial.println();

  BlynkGO.begin();

  label.font(prasanmit_20);
  label.text_line_space(-5);

  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("[LORA] failed!");
    label = "[LORA] failed!";
  } else {
    Serial.println("[LORA] inited.");

    LoRa.receive();
    LoRa.onReceived([](int packet_sz) {     // เมื่อได้รับข้อมูลเข้ามา
      if ( packet_sz == 0) return;

      // read packet header bytes:
      int recipient       = LoRa.read();    // recipient address
      byte sender         = LoRa.read();    // sender address
      byte incomingMsgId  = LoRa.read();    // incoming msg ID
      byte incomingLength = LoRa.read();    // incoming msg length

      String incoming = "";                 // payload of packet

      while (LoRa.available()) {            // can't use readString() in callback, so
        incoming += (char)LoRa.read();      // add bytes one by one
      }

      if (incomingLength != incoming.length()) {   // check length for error
        Serial.println("error: message length does not match length");
        return;                             // skip rest of function
      }

      // if the recipient isn't this device or broadcast,
      if (recipient != localAddress && recipient != 0xFF) {
        Serial.println("This message is not for me.");
        return;                             // skip rest of function
      }

      // if message is for this device, or broadcast, print details:
      Serial.println("Received from  : 0x" + String(sender, HEX));
      Serial.println("Sent to        : 0x" + String(recipient, HEX));
      Serial.println("Message ID     : " + String(incomingMsgId));
      Serial.println("Message length : " + String(incomingLength));
      Serial.println("Message        : " + incoming);
      Serial.println("RSSI           : " + String(LoRa.packetRssi()));
      Serial.println();

      label = StringX::printf("[msg id %d]\n%s", incomingMsgId, incoming.c_str());

    });

    LoRa.onSent([]() {                      // เมื่อข้อมูลส่งสำเร็จจะมาตรงนี้
      Serial.println("[LORA] message sent.");
      LoRa.receive_mode();                  // ให้สลับเป็นโหมด รับข้อมูล
    });

  }
}

void loop() {
  BlynkGO.update();
  if (millis() - lastSendTime > interval) {
    String message = "HeLoRa! " + String(millis());   // send a message
    sendMessage(message);
    Serial.println("Sending : " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(2000) + 1000;     // 2-3 seconds
    LoRa.receive_mode();                // go back into receive mode
  }
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}


