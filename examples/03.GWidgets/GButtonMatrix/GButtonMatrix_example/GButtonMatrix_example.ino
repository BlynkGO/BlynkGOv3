#include <BlynkGOv3.h>

GButtonMatrix btnm;
GTextArea textarea;

const char * btnm_map[] = { "1", "2", "3", "\n",
                            "4", "5", "6", "\n",
                            "7", "8", "9", "\n",
                            "Bksp", "0", ".", "\n",
                            "Cancel", "Save", ""};

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  textarea.align(ALIGN_IN_TOP_MID);   // กำหนดตำแหน่งชิดขอบบน ตรงกลาง
  textarea.width(BlynkGO.width()-10);
  textarea.default_keyboard(false);   // ไม่ใช้ default keyboard 

  btnm.map(btnm_map);     // กำหนด map ของปุ่มให้ ButtonMatrix
  btnm.size(BlynkGO.width(),180);     // กำหนดขนาด
  btnm.padding(2);        // กำหนดช่องไฟระหว่างปุ่ม
  btnm.border(1);         // กำหนดขอบให้ปุ่ม
  btnm.radius(5);         // กำหนดรัศมีมุมของปุ่ม
  btnm.button_width(12,2); // ปุ่ม id 12 (คือ ปุ่ม Cancel ใน map) กำหนดความกว้าง 2 เท่าปกติ
  btnm.style[RELEASED].color(TFT_BLUE, TFT_CYAN);    // กำหนดสี เมื่อสถานะ ปลอ่ยจากการกดปุ่ม
  btnm.style[PRESSED].color(TFT_GRAY, TFT_GRAY);     // กำหนดสี เมื่อสถานะ มีการกดปุ่ม
  
  btnm.onValueChanged([](GWidget* widget) {        // เมื่อมีการกดปุ่มเปลี่ยนค่า เข้ามา
    String val = btnm.active_button();
    if(val == "") return;
    
    Serial.printf("%s was pressed\n", val.c_str() );
    
    if( val == "Bksp")        { textarea.backspace(); }     // ให้ทำการลบ 1 ตัวอักขระ ด้านซ้ายของ cursor ของ textarea
    else if (val == "Cancel") { /* Your code*/ }
    else if (val == "Save")   { /* Your code*/ }
    else                      { textarea.add(val); }        // เพิ่ม ต่อท้ายของ cursor ของ textarea
  });
}

void loop() {
  BlynkGO.update();
}

