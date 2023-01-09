#include <BlynkGOv3.h>

Points pts = {  Point(286,266), Point(125,189), Point(153,64),
                Point(275,81), Point(363,131), Point(355,222), Point(286,266) };
GCircle cir[6];
GLine   lines;

GLabel label;
GCircle dot;

void check_dot_in_area(){
  bool is_in = dot.center().is_in(pts);   // เช็คว่าจุดศูนย์กลางของ dot อยู่ในพื้นที่ จากจุดใน set ของ pts ไหม
  label = is_in? "IN" : "OUT";
  label.color(is_in? TFT_GREEN : TFT_RED);
  dot  .color(is_in? TFT_GREEN : TFT_RED);
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  for(int i=0; i < 6; i++){
    cir[i].clickable(true);
    cir[i].dragable(true);
    cir[i].radius(20);
    cir[i].color(TFT_WHITE);
    cir[i].center(pts[i]);
    cir[i].onPressing([](GWidget*widget){
      uint8_t ii = (GCircle*)widget - &cir[0];      // cir[] ที่มีการกดค้างเข้ามาคือลำดับที่เท่าไหร่ใน array
      pts[ii] = cir[ii].center();
      if(ii==0) { pts[6] = pts[0]; }
      lines.setPoints(pts);
    });

    cir[i].onReleased([](GWidget*widget){
      uint8_t ii = (GCircle*)widget - &cir[0];
      pts[ii] = cir[ii].center();
      if(ii==0) { pts[6] = pts[0]; }
      lines.setPoints(pts);
      check_dot_in_area();
    });
  }
  lines.clickable(false);
  lines.toBackground();
  lines.setPoints(pts);

  dot.radius(20);
  dot.clickable(true);
  dot.dragable(true);
  dot.onReleased([](GWidget*widget){
    check_dot_in_area();
  });

  label.font(prasanmit_40);
  label.align(ALIGN_TOP_RIGHT,-10);
  check_dot_in_area();
}

void loop() {
  BlynkGO.update();
}


