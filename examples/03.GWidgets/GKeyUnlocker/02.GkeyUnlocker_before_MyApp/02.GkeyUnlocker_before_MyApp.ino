#include <BlynkGOv3.h>

GButton btn_myapp;

GRect screen_lock;
  GButton btn_back_screen_lock(SYMBOL_ARROW_LEFT, screen_lock);
  GKeyUnlocker keyunlocker(screen_lock);

GRect myapp;
  GButton btn_back_myapp(SYMBOL_ARROW_LEFT, myapp);
  GLabel label("หน้า MyApp", myapp);
  GGaugeRainbow gauge( myapp);
  
void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_RED,TFT_ORANGE);

  btn_myapp = "หน้า MyApp";
  btn_myapp.color(TFT_BLUE);
  btn_myapp.onClicked([](GWidget*widget){
    screen_lock.hidden(false);
    keyunlocker.hidden(false);
    keyunlocker.mode(GKEYUNLOCKER_MODE_LOCK); 
    // หลังจากใส่รหัสผ่านแล้วจะให้ไปทำอะไร
    keyunlocker.onApply([](GWidget* w){
      Serial.println("ใส่รหัสถูกต้อง ผ่าน!!");
      static GTimer timer;
      timer.setOnce(1200,[](){
        screen_lock.hidden(true);
        keyunlocker.hidden(true);
        myapp.hidden(false);
        myapp.toForeground();
      });
    });
    
    // หลังจากใส่รหัสไม่ผ่านครบ 3 ครั้งแล้ว จะให้ไปทำอะไร
    keyunlocker.onCancel([](GWidget* w){
      Serial.println("ใส่รหัสไม่ถูกต้อง 3 ครั้ง");
      screen_lock.hidden(true);
      keyunlocker.hidden(true);
    });

  });

  // หน้า รหัสผ่าน ด้วย keyunlocker ป้องกันการเข้าไปหน้าพิเศษ
  screen_lock.hidden(true);
  screen_lock.color(TFT_BLACK);
    keyunlocker.hidden(true);
    btn_back_screen_lock.size(50,50);
    btn_back_screen_lock.round_design();
    btn_back_screen_lock.align(ALIGN_TOP_LEFT,3,3);
    btn_back_screen_lock.onClicked([](GWidget*widget){
      screen_lock.hidden(true);
      keyunlocker.hidden(true);
    });

  // ออกแบบ หน้าพิเศษ
  myapp.hidden(true);
  myapp.color(TFT_BLUE, TFT_CYAN);
    btn_back_myapp.size(50,50);
    btn_back_myapp.round_design();
    btn_back_myapp.align(ALIGN_TOP_LEFT,3,3);
    btn_back_myapp.onClicked([](GWidget*widget){
      myapp.hidden(true);
    });
    label.font(prasanmit_40);
    label.align(ALIGN_TOP);
    gauge.align_center(0,30);
    gauge.decimal(1);
    gauge = 85.3;
}

void loop() {
  BlynkGO.update();
}
