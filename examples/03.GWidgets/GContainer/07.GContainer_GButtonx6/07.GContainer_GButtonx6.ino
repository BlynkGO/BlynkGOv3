/* ตัวอย่าง จัดเรียง ปุ่มกด 6 ปุ่ม อัตโนมัติ แบบ 3 แถว แถวละ 2 ปุ่ม 
   ด้วย GContainer แบบแถว 3 แถว แถวละ 2 ปุ่ม */

#include <BlynkGOv3.h>

GContainer cont_btn;
  GContainer cont_row[3];
    GButton btn[6];
    String btn_str[6] = { 
      "ปุ่ม 1", "ปุ่ม 2",
      "ปุ่ม 3", "ปุ่ม 4",
      "ปุ่ม 5", "ปุ่ม 6",
    };
  
void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  
  cont_btn.layout(LAYOUT_COL_L,0,0,0,0,10);         // จัด LAYOUT ไปพร้อมกับกำหนด padding บน,ล่าง,ซ้าย,ขวา,ภายใน
  cont_btn.opa(0);
  for(int row=0; row<3; row++){
    cont_row[row].layout(LAYOUT_ROW_M,0,0,0,0,10);  // จัด LAYOUT ไปพร้อมกับกำหนด padding บน,ล่าง,ซ้าย,ขวา,ภายใน
    cont_row[row].opa(0);
    cont_row[row].parent(cont_btn);
    for(int col=0; col<2; col++){
      int i = row*2 +col;
      btn[i] = btn_str[i];
      btn[i].size(150,50);
      btn[i].parent(cont_row[row]);
      btn[i].onClicked([](GWidget*widget){
        int ii = (GButton*) widget - &btn[0];
        Serial.printf("[GButtun] %d : clicked\n", ii);
      });
    }
  }
}

void loop() {
  BlynkGO.update();
}

