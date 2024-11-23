/** เล่นเสียง mp3 จาก sd หรือจากบน web ใช้ได้ทั้ง BeeNeXT5.0, BeeNeXT7.0 */

#include <BlynkGOv3.h>
#include "Audio.h"

#define I2S_DOUT      17
#define I2S_BCLK      0
#define I2S_LRC       18

GWiFiManager wifi_manager;
GLabel label;

Audio audio;

void setup() {
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  wifi_manager.align(ALIGN_TOP_RIGHT,-10);
  label = "ทดสอบ mp3";

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(21); // 0...21
  audio.connecttoFS(SD, "/nandemo.mp3");       // หากเล่น mp3 จาก SD  โดยชื่อไฟล์ต้องมี / นำ
}

void loop()
{
  BlynkGO.update();
  audio.loop();
}

WIFI_CONNECTED(){
  // audio.connecttohost("https://music.163.com/song/media/outer/url?id=450795499.mp3");  // หากเล่น mp3 จาก streaming บน internet
}

//-------------------------------------------------------
// มีหรือไม่มีก็ได้
//---------------------
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
