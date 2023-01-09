#include <BlynkGOv3.h>

FONT_DECLARE(BebasNeueBold_num_45);

//----------------------------------------------------
GLabel  lb_counter;     // วิตเจ็ตข้อความ
GContainer cont_btns;   // วิตเจ็ตสำหรับรวมกลุ่ม วิตเจ็ตปุ่มทั้งหลายเข้าด้วยกัน
  GButton btn_15_min("15"             , cont_btns);
  GButton btn_30_min("30"             , cont_btns);
  GButton btn_stop(SYMBOL_STOP        , cont_btns);
  GButton btn_start_pause(SYMBOL_PLAY , cont_btns);

//----------------------------------------------------
GTimer  timer;

enum { SELECT_15, SELECT_30 };
uint8_t select_type = SELECT_15;
int32_t counter=15;

enum { STATE_STOP, STATE_START, STATE_PAUSE };
uint8_t state = STATE_STOP;
//----------------------------------------------------

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  //-----------------------------------------------------------
  // ออกแบบ กราฟิก (เทียบได้กับ ออกแบบต่างๆด้วย HTML)
  lb_counter = StringX::printf(" %d ", counter);
  lb_counter.font(BebasNeueBold_num_45, TFT_WHITE);
  lb_counter.align(ALIGN_TOP,0,60);

  cont_btns.layout(LAYOUT_ROW_M);     // จัด layout ให้ปุ่มวางเรียงกันแนวนอน
  cont_btns.padding(40,40,40,40,30);  // ช่องไฟ บนล่างซ้ายขวา 40 px; ช่องภายใน ระหว่างปุ่มห่างกัน 30px
  cont_btns.opa(0);                   // ให้พื้นหลังโปร่งใส
  cont_btns.align_center(0, 30);      // วางไว้กลางจอ 

    btn_15_min.size(80,80);
    btn_15_min.color(TFT_BLUE);
    btn_15_min.border(5,TFT_SKYBLUE);
    btn_15_min.corner_radius(20);
    btn_15_min.shadow(40);
    btn_15_min.shadow_offset(10,10);
  
    btn_30_min.size(80,80);
    btn_30_min.color(TFT_BLUE);
    btn_30_min.border(5,TFT_SKYBLUE);
    btn_30_min.corner_radius(20);
    btn_30_min.shadow(40);
    btn_30_min.shadow_offset(10,10);
    
    btn_stop.size(80,80);
    btn_stop.color(TFT_RED);
    btn_stop.border(5,TFT_ORANGE);
    btn_stop.corner_radius(20);
    btn_stop.shadow(40);
    btn_stop.shadow_offset(10,10);
  
    btn_start_pause.size(80,80);
    btn_start_pause.color(TFT_GREEN);
    btn_start_pause.border(5,TFT_DARKGREEN);
    btn_start_pause.corner_radius(20);
    btn_start_pause.shadow(40);
    btn_start_pause.shadow_offset(10,10);

  //-----------------------------------------------------------
  // ควบคุมการทำงาน (เทียบได้กับ Javascript บน HTML)
  btn_15_min.onClicked([](GWidget*widget){
    lb_counter = StringX::printf(" %d ", counter=15);
    select_type = SELECT_15;
  });
  
  btn_30_min.onClicked([](GWidget*widget){
    lb_counter = StringX::printf(" %d ", counter=30);
    select_type = SELECT_30;
  });
  
  btn_stop.onClicked([](GWidget*widget){
    state = STATE_STOP;
    btn_start_pause = SYMBOL_PLAY;
    timer.del();
    switch(select_type) { 
      case SELECT_15: lb_counter = StringX::printf(" %d ", counter=15); break;
      case SELECT_30: lb_counter = StringX::printf(" %d ", counter=30); break;
    }
  });
  
  btn_start_pause.onClicked ([](GWidget*widget){
    switch(state){
      case STATE_STOP: 
      case STATE_PAUSE:
        state = STATE_START; 
        btn_start_pause = SYMBOL_PAUSE;
        timer.setInterval(1000,[](){ 
          lb_counter = StringX::printf(" %d ", --counter);
          if(counter <= 0) {
            // ให้ อีก 3 วินาทีถัดไป ปุ่ม btn_stop ถูกสั่งเหมือนถูก clicked โดยผู้ใช้
            timer.setOnce(3000,[](){ 
              btn_stop.event_send(EVENT_CLICKED);
            });
          }
        });
        break;
      case STATE_START:
        state = STATE_PAUSE; 
        btn_start_pause = SYMBOL_PLAY;
        timer.del();
        break;
    }
  }); 
}

void loop() {
  BlynkGO.update();
}
