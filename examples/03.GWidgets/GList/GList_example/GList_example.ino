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
  list.add( SYMBOL_FILE       , "ใหม่");
  list.add( SYMBOL_DIRECTORY  , "เปิด");
  list.add( SYMBOL_CLOSE      , "ลบ");
  list.add( SYMBOL_EDIT       , "แก้ไข");
  list.add( SYMBOL_SAVE       , "บันทึก");

  list.color(TFT_RED);
  list.onClicked([](GWidget* widget){
    label = list.selected();
    Serial.println(label);
  });

  list.selected_id(2);
}

void loop() {
  BlynkGO.update();
}

