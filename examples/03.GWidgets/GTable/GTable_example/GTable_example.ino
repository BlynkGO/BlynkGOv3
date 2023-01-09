#include <BlynkGOv3.h>

GTable table;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // สร้างตาราง
  table.create();
  // table.column(0).width(150);    // กำหนดความกว้าง column 0
  // table.column(1).width(200);    // กำหนดความกว้าง column 1

  table.cell(0,0) = "ชื่อ";       table.cell(0,1) = "ราคา";
  table.cell(1,0) = "แอปเปิ้ล";   table.cell(1,1) = 15;
  table.cell(2,0) = "กล้วย";     table.cell(2,1) = FLOAT(16.1 , 1); 
  table.cell(3,0) = "ส้ม";       table.cell(3,1) = FLOAT(17.25, 2);


  // ตัวอย่างดึงข้อมูลออกจาก cell
  Serial.print( table.cell(1,0).toString() + " : ");
  Serial.println( table.cell(1,1).toInt() );

  Serial.print( table.cell(2,0).toString() + " : " );
  Serial.println( table.cell(2,1).toFloat() );
  

  // style กำหนดได้ตั้งแต่ TYPE1 - TYPE4 สำหรับใช้เป็น style ให้ cell แบบต่างๆ
  // TYPE 1 สำหรับ cell ทั่วไป
  // TYPE 2 สำหรับ cell ที่เป็น table header
  // TYPE 3-4 สำหรับ customize
  table.style[TYPE3].color(TFT_RED, TFT_ORANGE);
  table.style[TYPE3].text_color(TFT_WHITE);  
  table.style[TYPE4].color(TFT_BLUE);
  table.style[TYPE4].text_color(TFT_WHITE);

  // กำหนดตารางแบบไม่มีเส้นตี
  // table.style[TYPE1].border(0);
  // table.style[TYPE2].border(0);
  // table.style[TYPE3].border(0);
  // table.style[TYPE4].border(0);

  // จัดเรียงชิด และ กำหนด style ให้ cell
  table.column(0).align(LABEL_ALIGN_CENTER);  // column 0 จัดเรียงแบบ ชิดตรงกลาง
  table.row(3).style_type(TYPE3);             // row 3    ใช้ style แบบ TYPE3
  table.cell(2,1).style_type(TYPE4);          // เฉพาะ cell(2,1) ใช้ style แบบ TYPE4


}

void loop() {
  BlynkGO.update();
}

