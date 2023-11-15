#include <BlynkGOv3.h>

// วางไฟล์ mp3 ใน SD card ที่ folder "/Music"
// สามารถเปิดไฟล์ชื่อภาษาไทย บน SD ได้   (ปกติ ESP32 อ่านไฟล์ชื่อไทยไม่ได้)

#define FILE_NAME   "/Music/Memories-Maroon5.mp3"
// #define FILE_NAME   "/Music/สนิทใจ - PALMY.mp3"

namespace SD_CARD {
  void listDir(const char * dirname, uint8_t levels=0);
}

namespace MP3 {
  void init();
  void play(String filename);
  void stop();
}

enum { MUSIC_STANDBY, MUSIC_PLAY, MUSIC_PAUSE };
uint8_t music_state = MUSIC_STANDBY;

GButton btn_play;


void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
  BlynkGO.rotation(ROTATION_DEFAULT);

  SD_CARD::listDir("/Music");
  MP3::init();

  btn_play = SYMBOL_PLAY;
  btn_play.round_design();
  btn_play.size(55,55);
  btn_play.border(1, TFT_SILVER);
  btn_play.border_opa(200);

  static bool long_pressed;
  btn_play.onPressed([long_pressed](GWidget*widget){      // เมื่อปุ่ม ขณะเริ่มกด
    long_pressed    = false;
  });
  btn_play.onLongPressed([long_pressed](GWidget*widget){  // เมื่อปุ่มกดค้างยาว ให้หยุดเล่น mp3
    long_pressed    = true;
    music_state     = MUSIC_STANDBY;
    btn_play        = SYMBOL_STOP;
    Serial.println("[MP3] stop");
    MP3::stop();
  });
  btn_play.onClicked([long_pressed](GWidget*widget){    // เมื่อปุ่ม มีการ clicked ให้เล่น mp3
    if(long_pressed) return;
    switch(music_state){
      case MUSIC_STANDBY:
        music_state = MUSIC_PLAY;
        btn_play    = SYMBOL_PAUSE;
        MP3::play(FILE_NAME);
        break;
      case MUSIC_PLAY:
        music_state = MUSIC_PAUSE;
        btn_play    = SYMBOL_PLAY;
        break;
      case MUSIC_PAUSE:
        music_state = MUSIC_PLAY;
        btn_play    = SYMBOL_PAUSE;
        break;
    }
  });
}

void loop() {
  BlynkGO.update();
}

