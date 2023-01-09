#include <BlynkGOv3.h>

GButton button1(SYMBOL_VIDEO " ปุ่มกด 1");
GButton button2("ปุ่ม Toggle 2");

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  //-------------------------------------------------
  // กำหนด mode ของปุ่ม มีได้ 3 แบบ BUTTON_MODE_NORMAL, BUTTON_MODE_TOGGLE, BUTTON_MODE_INACTIVE
  
  button1.mode(BUTTON_MODE_NORMAL);   // ปุ่มกด แบบปกติ (กดติด ปล่อยดับ)
  button1.center(120,70);
  button1.width(170);
  button1.color(TFT_GREEN);           // กำหนดสีแบบง่าย  


  /* จัดการเมื่อมีสัญญาณกดแบบต่างๆ */
  button1.onClicked([](GWidget* widget){       // เมื่อปุ่มมีการ click 
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

  //-------------------------------------------------
  
  button2.mode(BUTTON_MODE_TOGGLE);   // ปุ่มกด แบบ Toggle (กดติด กดดับ)
  button2.center(120,160);             // จัดตำแหน่งศูนย์กลางปุ่มไปยัง pixel 160,70
  button2.size(170,70);               // กำหนด กว้างxสูง
  button2.radius(button1.height()/2); // กำหนด รัศมีความโค้ง ให้ปุ่ม

  /* กำหนดสีให้ button2 แบบ customize ด้วยสถานะต่างๆ */
  button2.style[RELEASED]       .color(TFT_ORANGE  , TFT_RED);        // กำหนดสี เมื่อปุ่มมีสถานะ released (ปล่อย)
  button2.style[PRESSED]        .color(TFT_DARKRED , TFT_DARKRED);    // กำหนดสี เมื่อปุ่มมีสถานะ pressed  (กด)
  button2.style[TOGGLE_RELEASED].color(TFT_BLACK   , TFT_RED);        // กำหนดสี เมื่อปุ่มมีสถานะ toggle released
  button2.style[TOGGLE_PRESSED] .color(TFT_DARKRED , TFT_DARKRED);    // กำหนดสี เมื่อปุ่มมีสถานะ toggle pressed
  button2.style[INACTIVE]       .color(TFT_GRAY    , TFT_GRAY);       // กำหนดสี เมื่อปุ่มมีสถานะ inactive

  button2.font(prasanmit_25);

  button2.onClicked([](GWidget* widget){
    Serial.printf("Button2 : Toggle %s\n", (button2.isON())? "ON" : "OFF");
    button2 = (button2.isON())? "ปุ่ม Toggle 2 ON" : "ปุ่ม Toggle 2 OFF"; // เปลี่ยนข้อความของปุ่ม
  });

}

void loop() {
  BlynkGO.update();
}

