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

  list.add("SD://TridentTD/icons/png60x60/calculator.png", "เครื่องคิดเลข"); 
  list.add(ico_setting,     "ตั้งค่า");
  list.add(ico_camera,      "กล้อง");
  list.add(ico_filemanager, "ไฟล์");
  list.add(ico_smarthome,   "บ้านอัจฉริยะ");
  list.add(ico_alarm,       "นาฬิกาปลุก");
  list.add(ico_weather,     "พยากรณ์อากาศ");
  list.add(ico_battery,     "แบตเตอรี่");

  list.onClicked([](GWidget* widget) {
    Serial.printf("List Clicked : [%d] %s\n", list.selected_id(), list.selected().c_str());
  });

  list.selected_id(3);
}

void loop() {
  BlynkGO.update();
}
