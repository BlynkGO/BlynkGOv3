/* ใช้ GMask ทำ mask แบบ arc ให้ logo */
#include <BlynkGOv3.h>

IMG_DECLARE(img_honda_adv350);
IMG_DECLARE(img_dlog);

GRect rect_logo_show;
  GMask mask_logo(320,320, rect_logo_show);
    GImage logo(img_honda_adv350, mask_logo);
  GAnimation anim_logo;

GRect rect_gauge;
  GArc arc_dial(rect_gauge);
  GArc gauge_arc [17];
  GLabel lb_gauge[17];
  GTriangle tri_arm(rect_gauge);
  GCircle cir_arm(rect_gauge);
  Point pivot = Point(240,160);
  
void anim1(), anim2(), anim3(), anim4(), anim5();


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  rect_logo_show.color(TFT_BLACK);
    mask_logo.fill(0);  // ให้มองยังไม่เห็น
      logo.align_center();

  rect_gauge.hidden(true);
  rect_gauge.color(TFT_BLACK);
    arc_dial.radius(135);
    arc_dial.thickness(3);
    arc_dial.color(TFT_WHITE);
    arc_dial.angle_offset(90);
    arc_dial.angle(0,270);
    for(int i=0;i < 17;i++){
      gauge_arc[i].angle_offset(90);
      gauge_arc[i].radius(130);
      gauge_arc[i].thickness(20);
      gauge_arc[i].angle(i*16, i*16+12);
      gauge_arc[i].color( i< 8 ? TFT_WHITE : i < 12 ? TFT_YELLOW : TFT_RED);
      gauge_arc[i].parent(rect_gauge);

      lb_gauge[i] = String(i);
      lb_gauge[i].parent(rect_gauge);
      lb_gauge[i].font(prasanmit_25);
      lb_gauge[i].center( Point(100, i*16+6, 90, Point(240,160), CLOCKWISE) );
    }

    
    Point p[3];
      p[0] = pivot + Point( 0,-95);  /*         /\ p[0]     */
      p[1] = p[0]  + Point( 8, 95);  /*        /  \         */
      p[2] = p[0]  + Point(-8, 95);  /*  p[2] /____\ p[1]   */

    tri_arm.setPoints(p);
    tri_arm.color(TFT_RED);
    tri_arm.direction(CLOCKWISE);
    tri_arm.angle_offset(180);  // จากจุด 12 นาฬิกา หมุนไป 180 องศา ให้เป็นองศาเริ่มต้น
    tri_arm.angle( 0, pivot);
    cir_arm.radius(12);
    cir_arm.color(TFT_RED);


  anim1();
  
}

void loop() {
  BlynkGO.update();
}


void anim1(){
  logo = img_honda_adv350;
  mask_logo.fill(0);  // ให้มองยังไม่เห็น
  anim_logo.init(mask_logo, [](GWidget*widget, int16_t v){
    static int16_t prev_v=1;
    if(v > 0) {
      mask_logo.arc(255, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                      0, prev_v-1, v);
      mask_logo.arc(255, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                    180, prev_v-1, v);
      prev_v = v;
    }
  }, 0, 180, 1500);
  anim_logo.delay(1000);
  anim_logo.ready_cb([](GAnimation*a){  mask_logo.fill(255); anim2(); });
  anim_logo.create();
}

void anim2(){  
  anim_logo.init(mask_logo, [](GWidget*widget, int16_t v){
    static int16_t prev_v=1;
    if(v > 0) {
      mask_logo.arc(  0, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                      0, prev_v-1, v);
      mask_logo.arc(  0, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                    180, prev_v-1, v);
      prev_v = v;
    }
  }, 0, 180, 1500);
  anim_logo.delay(1000);
  anim_logo.ready_cb([](GAnimation*a){  mask_logo.fill(0); anim3(); });
  anim_logo.create();
}

void anim3(){
  logo = img_dlog;
  anim_logo.init(mask_logo, [](GWidget*widget, int16_t v){
    static int16_t prev_v=1;
    if(v > 0) {
      mask_logo.arc(255, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                      0, prev_v-1, v);
      mask_logo.arc(255, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                    180, prev_v-1, v);
      prev_v = v;
    }
  }, 0, 180, 1500);
  anim_logo.delay(1000);
  anim_logo.ready_cb([](GAnimation*a){ mask_logo.fill(255); anim4(); });
  anim_logo.create();
}

void anim4(){
  anim_logo.init(mask_logo, [](GWidget*widget, int16_t v){
    static int16_t prev_v=1;
    if(v > 0) {
      mask_logo.arc(  0, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                      0, prev_v-1, v);
      mask_logo.arc(  0, mask_logo.centerpoint(), mask_logo.width()/2, mask_logo.width()/2,
                    180, prev_v-1, v);
      prev_v = v;
    }
  }, 0, 180, 1500);
  anim_logo.delay(1000);
  anim_logo.ready_cb([](GAnimation*a){ mask_logo.fill(0); anim5(); });
  anim_logo.create();
}

void anim5(){
  static GTimer timer;
  timer.delay(500,[](){
    rect_gauge.show(true);
    anim_logo.init(tri_arm, [](GWidget*widget, int16_t v){
      tri_arm.angle( v, pivot);
    }, 0, 270, 800);
    anim_logo.playback(true);
    anim_logo.ready_cb([](GAnimation*a){ });
    anim_logo.create();
  });
}
