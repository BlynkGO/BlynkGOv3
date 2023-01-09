#include <BlynkGOv3.h>

IMG_DECLARE(img_bg_01);
IMG_DECLARE(img_bg_02);
IMG_DECLARE(img_bg_03);
IMG_DECLARE(img_bg_04);

GImage image[4] = {{img_bg_01}, {img_bg_02}, {img_bg_03}, {img_bg_04} };
GContainer cont_btns;
  GButton btn[4];

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  cont_btns.layout(LAYOUT_ROW_M);
  cont_btns.padding(10,10,10,10,5);
  cont_btns.opa(0);
  cont_btns.align(ALIGN_TOP_RIGHT);
  
  for(int i=0;i < 4; i++){
    image[i].create();    
    btn[i] = StringX::printf("หน้า %d", i+1);
    btn[i].size(80,50);
    btn[i].color(TFT_ORANGE);
    btn[i].parent(cont_btns);
    btn[i].onClicked([](GWidget*widget){
      uint8_t id= (GButton*)widget - &btn[0];
      for(int i=0;i <4;i++)  btn[i].color((i==id)? TFT_RED : TFT_ORANGE);
      image[id].toForeground();
      cont_btns.toForeground();
    });
  }
  btn[0].event_send(EVENT_CLICKED);
}

void loop() {
  BlynkGO.update();
}

