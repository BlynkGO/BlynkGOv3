/*****************************************************
 *
 * วิตเจ็ต GButton ปุ่มกด จะมี 2 แบบ คือ 
 *   1. BUTTON_PUSH   : กดติดปล่อยดับ
 *   2. BUTTON_SWITCH : กดติดกดดับ
 *
 * โดยเป็นวิตเจ็ตที่ใช้การดักรับสัญญาณการกดจากผู้ใช้เข้ามา
 * สามารถจัดการได้ด้วย API ต่างๆ ดังนี้
 *
 *   onClicked(...)   : เมื่อการ clicked เกิดขึ้น (ตั้งแต่ Pressed --> Released ปล่อยมือ)
 *   onPressed(...)   : เมื่อเริ่มแรก กด เข้ามา
 *   onPressing(...)  : เมื่อระหว่างกดค้าง
 *   onReleased(...)  : เมื่อปล่อยการกด
 *
 ****************************************************/

#include <BlynkGOv3.h>

GButton button1(BUTTON_PUSH  , SYMBOL_VIDEO " ปุ่มกด 1");   // ปุ่มกดแบบ PUSH (กดติดปล่อยดับ)
GButton button2(BUTTON_SWITCH, "ปุ่ม Toggle 2");            // ปุ่มกดแบบ SWITCH (กดติดกดดับ)

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  button1.center(120,70);
  button1.font(prasanmit_40);                   // เปลี่ยน font สำหรับข้อความบนปุ่ม
  button1.color(TFT_GREEN);                     // กำหนดสีแบบง่าย  

  /* จัดการเมื่อมีสัญญาณกดแบบต่างๆ */
  button1.onClicked([](GWidget* widget){        // เมื่อปุ่มมีการ click 
    Serial.println("Button1 : clicked");
  });
  
  button1.onPressed([](GWidget* widget){       // เมื่อปุ่มมีการเริ่มกด
    Serial.println("Button1 : pressed");
  });

  button1.onPressing([](GWidget* widget){      // เมื่อปุ่มมีการกดค้าง
    Serial.println("Button1 : pressing");
  });

  button1.onReleased([](GWidget* widget){      // เมื่อปุ่มการการปล่อยการกด
    Serial.println("Button1 : released");
  });

  button2.center(120,160);              // จัดตำแหน่งศูนย์กลางปุ่มไปยัง pixel 160,70
  button2.rounded();                    // ทำให้ปุ่มโค้ง
  button2.color(TFT_BLUE);
  button2.onClicked([](GWidget* widget){
    Serial.printf("Button2 : Toggle %s\n", (button2.isON())? "ON" : "OFF");
    button2 = (button2.isON())? "ปุ่ม Toggle 2 ON" : "ปุ่ม Toggle 2 OFF"; // เปลี่ยนข้อความของปุ่ม
  });

  // button2.ON();                      // สั่งให้มีสถานะ ON
  // button2.OFF();                     // สั่งให้มีสถานะ OFF
  // button2.toggle();                  // สั่งให้มีสถานะกลับจากเดิม ON--> OFF, OFF-->ON
  // button2.inactive(true);            // สั่งให้ปุ่มมีสถานะ กดไม่ได้
  // button2.inactive(false);           // สั่งให้ปุ่มมีสถานะ กดได้  
}

void loop() {
  BlynkGO.update();
}

