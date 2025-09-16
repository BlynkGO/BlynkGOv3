/*
 * เมนู กดแล้ว ขยาย ปุ่มย่อยๆ 4 ปุ่มออกมา 
 * ออกแบบสำหรับจอ 320x240px
 */

#include <BlynkGOv3.h>

// รูป ของ expand menu ไว้กด
IMG_DECLARE(img_lettuce_s);

// icon สำหรับปุ่ม menu 4 ปุ่ม
IMG_DECLARE(img_app);
IMG_DECLARE(img_gps);
IMG_DECLARE(img_monitor);
IMG_DECLARE(img_sensors);

GLabel        lb_selected;
GImage        expand_menu(img_lettuce_s);
GImageButton  menu_btns[4] = {
  { img_app     },
  { img_gps     },
  { img_monitor },
  { img_sensors },
  };
  enum { MENU_APP, MENU_GPS, MENU_MONITOR, MENU_SENSORS};


void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  lb_selected.font(prasanmit_40, TFT_RED);
  lb_selected.align(ALIGN_TOP, 0, 5);

  expand_menu.align(ALIGN_BOTTOM_LEFT, 5, -10);
    for(int i=3; i>=0 ; i--) {
      menu_btns[i].align(expand_menu, ALIGN_CENTER);
      menu_btns[i].toForeground();       // ย้ายไป layer ที่ บนสุด
      menu_btns[i].onClicked(GWIDGET_CB{
        int menu_id = (GImageButton*)widget -&menu_btns[0];
        Serial.printf("[Menu] %d  selected\n", menu_id);
        lb_selected.show(true); 
        lb_selected = menu_id;
        static SoftTimer timer;
        timer.delay(2000,[](){ lb_selected.show(false); }); // อีก 2 วินาทีให้ซ่อน

        expand_menu.event_send(EVENT_CLICKED);  // ส่ง EVENT ให้มีการเสมือนว่า circle_menu ถูกกด (จำลองแทนเสมือนมีคนกด)

        // menu_id ที่กดได้จะให้ไปทำอะไร
        switch (menu_id) {
          case MENU_APP:
            //...
            break;
          case MENU_GPS:
            break;
          case MENU_MONITOR:
            break;
          case MENU_SENSORS:
            break;
        }
      });
    }

  expand_menu.toForeground();           // ย้ายไปบนสุด
  expand_menu.clickable(true);          // ปกติ GImage กดไม่ได้ เปิดให้สามารถกดสัมผัสได้
  expand_menu.onClicked(GWIDGET_CB{
    enum {MENU_STATE_HIDE, MENU_STATE_ANIM, MENU_STATE_SHOW };
    static int8_t menu_state = MENU_STATE_HIDE;
    static GAnimation anim_menu;
    switch(menu_state){
      case MENU_STATE_HIDE:
        menu_state = MENU_STATE_ANIM;

        // สร้าง animation ปุ่มกดทั้ง 4 เคลื่อนที่ออกมา ด้วยระยะห่างที่ละ 64px
        // ในเวลา 800ms ให้เคลื่อนที่แบบ ช้าไปเร็ว
        anim_menu.init(expand_menu,[](GWIDGET, int16_t v){
          for(int i=0; i<4; i++){
            menu_btns[i].center(expand_menu.center() + Point( (v* 64*(i+1))/1024.0,0));
          }
        },0, 1024, 800);
        anim_menu.path_type(ANIM_PATH_EASE_OUT);
        anim_menu.ready_cb([](GAnimation*a){
          menu_state = MENU_STATE_SHOW;
          static SoftTimer timer;
          timer.delay(3000,[](){  // อีก 3 วินาที
            if(menu_state == MENU_STATE_SHOW) {       // ถ้า เมนูยังแสดงอยู่
              expand_menu.event_send(EVENT_CLICKED);  // ส่ง EVENT ให้มีการเสมือนว่า circle_menu ถูกกด (จำลองแทนเสมือนมีคนกด)
            }
          });
        });
        anim_menu.create();
        break;
      case MENU_STATE_ANIM:
        break;
      case MENU_STATE_SHOW:
        menu_state = MENU_STATE_ANIM;

        // สร้าง animation ปุ่มกดทั้ง 4 เคลื่อนที่ หดกลับเข้าไป
        // ในเวลา 800ms ให้เคลื่อนที่แบบ ช้าไปเร็ว
        anim_menu.init(expand_menu,[](GWIDGET, int16_t v){
          for(int i=0; i<4; i++){
            menu_btns[i].center(expand_menu.center() + Point( (v*64*(i+1))/1024.0,0));
          }
        }, 1024, 0, 800);
        anim_menu.path_type(ANIM_PATH_EASE_OUT);
        anim_menu.ready_cb([](GAnimation*a){
          menu_state = MENU_STATE_HIDE;
        });
        anim_menu.create();
        break;
    }
  });
}

void loop(){
  BlynkGO.update();
}
