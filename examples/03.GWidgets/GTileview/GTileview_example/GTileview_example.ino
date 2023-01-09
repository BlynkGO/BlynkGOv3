/*
 * ตัวอย่าง สร้าง GTileview 
 * โดย tile จะมีด้วยกัน 7 tile
 * การเลื่อนจะสามารถเลื่อนไปตาม ที่กำหนด valid_pos จะให้เลื่อนได้ 
 * และเมื่อกดช่องสีจะขึ้นค่าสีที่เลือกให้
 */

#include <BlynkGOv3.h>

GTileview tileview;

#define COUNT 7

/* พื้นที่ ที่ valid เพื่อให้สามารถ scroll ไปได้ พื้นที่ไหนไม่กำหนดจะเลื่อนไปไม่ได้ */
/* พิกัด valid {0,0} คือ ซ้าย บนสุด */
static point_t valid_pos[] = {{0, 0}, 
                              {0, 1},
                              {0, 2},{1, 2}, {2, 2}, {3, 2},
                              {0, 3} };

GObject   tile[COUNT];
GLabel    label[COUNT];
uint16_t  colors[] = { TFT_RED, TFT_GREENBLYNK, TFT_BLUE, TFT_ORANGE, TFT_VIOLET, TFT_PINK, TFT_SILVER };

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();


  // ต้องมีใช้กำหนดว่า tile ตำแหน่งไหน ที่ valid บ้าง
  tileview.valid_positions(valid_pos, COUNT);

  for(int i = 0; i < COUNT; i++){
    tileview.addElement(tile[i]); // เพิ่ม เข้าไปใน tileview
    tile[i].size(320,240);
    tile[i].position( valid_pos[i].x * RES_WIDTH, valid_pos[i].y * RES_HEIGHT);  // ตำแหน่งที่วาง ต้องอยู่ในพื้นที่ ที่ valid ที่กำหนดไว้ข้างต้น
    tile[i].color(colors[i]);
    tile[i].onClicked([](GWidget* widget){
      Serial.println(widget->color(), HEX);
    });

    label[i].parent(tile[i]);
    label[i] = StringX::printf("(%d,%d)", valid_pos[i].x, valid_pos[i].y);
  }
}

void loop() {
  BlynkGO.update();
}

