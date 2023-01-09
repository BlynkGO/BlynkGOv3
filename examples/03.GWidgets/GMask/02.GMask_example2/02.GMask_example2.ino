#include <BlynkGOv3.h>

GMask   Mask(200,80);
GRect   rect(Mask);
GLabel  label(rect);

GMask   Mask2;
GRect   rect2(Mask2);
GLabel  label2(rect2);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

//  Mask.arc(255, Point(120,120), 80, 30, 210, -30, true );
//  Mask.line(255, Point(20,20), Point(20,180), 40, true );

  Point p[4] = { Point(40,0), Point(199,0), Point(160,79), Point(0,79) };
  Mask.polygon( 255, p, 4 );

  Mask.align(ALIGN_IN_TOP_MID, 0, 26);
  Mask.dragable(true);
  rect.color(TFT_BLUE);
  rect.clickable(false);
  label = "สวัสดีครับ";
  label.font(prasanmit_40);
  label.color(TFT_WHITE);

  // สร้าง Mask อีกชุดนึง แต่ buffer เดียวกับ Mask แรก
  Mask2.linkBuffer( Mask ); 
  Mask2.align(ALIGN_IN_BOTTOM_MID,0, -26);
  Mask2.dragable(true);
  rect2.color(TFT_RED);
  rect2.clickable(false);
  label2 = "ฮัลโหล";
  label2.font(prasanmit_40);
}

void loop() {
  BlynkGO.update();
}
