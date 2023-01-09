#include <BlynkGOv3.h>

IMG_DECLARE(ico_setting);
IMG_DECLARE(ico_camera);
IMG_DECLARE(ico_filemanager);
IMG_DECLARE(ico_smarthome);
IMG_DECLARE(ico_alarm);
IMG_DECLARE(ico_weather);
IMG_DECLARE(ico_battery);

GList list;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  list.size(GScreen);
  
  list.padding_top(7);                   // ระยะช่องไฟด้านบนของ item
  list.padding_bottom(7);                // ระยะช่องไฟด้านล่างของ item
  list.padding_left(20);                 // ระยะช่องไฟด้านซ้ายของ item
  list.font(prasanmit_40, TFT_WHITE);

  list.style[RELEASED].color(TFT_BLACK);
  list.style[PRESSED].color(TFT_DARKGRAY);

  list.addItem("SD://TridentTD/icons/png60x60/calculator.png", "เครื่องคิดเลข"); 
  list.addItem(ico_setting,     "ตั้งค่า");
  list.addItem(ico_camera,      "กล้อง");
  list.addItem(ico_filemanager, "ไฟล์");
  list.addItem(ico_smarthome,   "บ้านอัจฉริยะ");
  list.addItem(ico_alarm,       "นาฬิกาปลุก");
  list.addItem(ico_weather,     "พยากรณ์อากาศ");
  list.addItem(ico_battery,     "แบตเตอรี่");

  list.onClicked([](GWidget* widget) {
    uint8_t id = list.item_id(list.item_selected());
    Serial.printf("List Clicked : [%d] %s\n", id, list.item_selected().c_str());
  });
}

void loop() {
  BlynkGO.update();
}
