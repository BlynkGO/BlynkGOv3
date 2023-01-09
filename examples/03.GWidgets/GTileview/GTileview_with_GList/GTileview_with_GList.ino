/*
 * ตัวอย่าง สร้าง GTileview 
 * โดย tile จะมีด้วยกัน 3 tile
 * การเลื่อนจะสามารถเลื่อนไปตาม ที่กำหนด valid_pos จะให้เลื่อนได้ 
 * และ tile ที่ 2 จะ เป็น GList สามารถ scroll เลื่อนภายใน list ได้ 
 * และยัง เลื่อนไปตาม tileview  ได้
 */

#include <BlynkGOv3.h>

GTileview tileview;

/* พื้นที่ ที่ valid เพื่อให้สามารถ scroll ไปได้ พื้นที่ไหนไม่กำหนดจะเลื่อนไปไม่ได้ */
/* พิกัด valid {0,0} คือ ซ้าย บนสุด */
static point_t valid_pos[] = {{0, 0}, 
                              {0, 1},{1,1}};

GObject   tile1(tileview);  // tile1 เป็น GObject มี parent คือ tileview
GList     list(tileview);   // list  เป็น GList   มี parent คือ tileview
GObject   tile3(tileview);  // tile3 เป็น GObject มี parent คือ tileview

GLabel    label(tile1);     // label  เป็น GLabel  มี parent คือ tile1
GButton   button(tile3);    // button เป็น GButton มี parent คือ tile3

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  // ต้องมีใช้กำหนดว่า tile ตำแหน่งไหน ที่ valid บ้าง
  tileview.valid_positions(valid_pos, 3);
  
  tileview.addElement(tile1, valid_pos[0]);  // tile1 อยู่ตำแหน่ง {0,0} ของ tileview
  tileview.addElement(list , valid_pos[1]);  // list  อยู่ตำแหน่ง {1,0} ของ tileview
  tileview.addElement(tile3, valid_pos[2]);  // tile3 อยู่ตำแหน่ง {1,1} ของ tileview

  // tile1
  tile1.color(TFT_RED);
  label = "Tile 1";

  // list
  list.addItem("One");
  list.addItem("Two");
  list.addItem("Three");
  list.addItem("Four");
  list.addItem("Five");
  list.addItem("Six");
  list.addItem("Seven");
  list.addItem("Eight");

  // เมื่อ list มีการถูกกดเลือก ให้แสดง item ที่ถูกเลือก
  list.onClicked([](GWidget* widget){
    Serial.println(list.item_selected());
  });

  // tile3
  tile3.color(TFT_BLUE);
  button = "Button";
}

void loop() {
  BlynkGO.update();
}

