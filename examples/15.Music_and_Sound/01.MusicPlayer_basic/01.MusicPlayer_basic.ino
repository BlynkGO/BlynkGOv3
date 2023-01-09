#include <BlynkGOv3.h>

enum { PY_STOP, PY_PLAY, PY_PAUSE} player_state= PY_STOP;
GButton btn;
GLabel  label;
bool longpressed = false;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  btn.size(100,100);
  btn.rounded(true);
  label.font(prasanmit_40);
  label.align(btn, ALIGN_TOP,0, -20);

  BlynkGO.AUDIO.stop();
  
  btn.onClicked([](GWidget*w){
    // หากมีสถานะกดปุ่มมีสถานะ longpressed มาก่อน
    // ให้ reset ค่าเป็น false แล้วข้าม onClicked รอบนี้ไป
    if(longpressed== true) { longpressed = false; return; }    
    
    switch(player_state){
      case PY_STOP:                     // หากสถานะเป็น PY_STOP
        BlynkGO.AUDIO.play(SD, "/Gail_OnlyYou.mp3");  // คำสั่งเล่น mp3 จาก SD
        break;
      case PY_PLAY:                     // หากสถานะเป็น PY_PLAY
      case PY_PAUSE:                    // หากสถานะเป็น PY_PAUSE
        BlynkGO.AUDIO.pauseResume();    // คำสั่ง หยุดเล่นชั่วคราว/กลับมาเล่นต่อ
        break;
    }
  });

  btn.onLongPressed([](GWidget*widget){
    longpressed  = true;
    BlynkGO.AUDIO.stop();               // คำสั่งหยุดเล่น
  });

  BlynkGO.AUDIO.onStop([](){
    player_state = PY_STOP;
    btn          = SYMBOL_PLAY;
    label        = "STOP";
    btn.border(5, TFT_RED);
    btn.color(TFT_NAVY);
  });
  
  BlynkGO.AUDIO.onPlay([](){
    player_state = PY_PLAY;
    btn          = SYMBOL_PAUSE;
    label        = "Playing...";

    btn.border(1, TFT_NAVY);
    btn.color(TFT_NAVY);
  });
  
  BlynkGO.AUDIO.onPause([](){
    player_state = PY_PAUSE;
    btn          = SYMBOL_PLAY;
    label        = "Pause";
  });
}

void loop() {
  BlynkGO.update();
}

