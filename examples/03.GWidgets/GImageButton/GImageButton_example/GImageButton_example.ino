#include <BlynkGOv3.h>

IMG_DECLARE(img_fish_feed_pressed);
IMG_DECLARE(img_fish_feed_released);

GImageButton imgbtn( BUTTON_PUSH, img_fish_feed_released, img_fish_feed_pressed);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  imgbtn.onClicked([](GWidget* widget){
    Serial.print("Button Clicked! : ");
    Serial.println(imgbtn.isON());
  });

}


void loop() {
  BlynkGO.update();
}

