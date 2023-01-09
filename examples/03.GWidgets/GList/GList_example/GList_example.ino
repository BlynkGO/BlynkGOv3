#include <BlynkGOv3.h>

GList   list;
GLabel  label;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  label = "Click to select";
  label.align(ALIGN_TOP,0,20);

  list.size(160, 120);
  list.align(label, ALIGN_BOTTOM, 0, 20);
  list.addItem( SYMBOL_FILE       , "ใหม่");
  list.addItem( SYMBOL_DIRECTORY  , "เปิด");
  list.addItem( SYMBOL_CLOSE      , "ลบ");
  list.addItem( SYMBOL_EDIT       , "แก้ไข");
  list.addItem( SYMBOL_SAVE       , "บันทึก");

  list.color(TFT_RED);
  list.onClicked([](GWidget* widget){
    label = list.item_selected();
    Serial.println(label);
  });
}

void loop() {
  BlynkGO.update();
}

