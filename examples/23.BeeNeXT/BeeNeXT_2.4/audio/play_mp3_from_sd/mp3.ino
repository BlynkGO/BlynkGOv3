/* ติดตั้งไลบรารี่ ESP8266Audio */
#include <AudioGeneratorMP3.h>
#include <AudioFileSourceSD.h>
#include <AudioFileSourceID3.h>
#include <AudioOutputI2S.h>

AudioGeneratorMP3 mp3;
AudioOutputI2S    sound_out(0 /*port*/, AudioOutputI2S::INTERNAL_DAC ,64 /*dma_buf_count*/);
std::unique_ptr<AudioFileSourceSD> file;
std::unique_ptr<AudioFileSourceID3> id3;

extern uint8_t music_state;

namespace MP3 {
  void init();
  void play(String filename);
  void stop();
  void run();
  void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);
  void StatusCallback(void *cbData, int code, const char *string);
}

void MP3::init(){
  audioLogger = &Serial;
  mp3.RegisterStatusCB(MP3::StatusCallback, (void*)"mp3");
  static SoftTimer timer_audio_play;
  timer_audio_play.setInterval(1,[](){
    if( music_state == MUSIC_PLAY) {
      MP3::run();
    }
  });
}

void MP3::play(String filename){
  MP3::stop();
  if(!BlynkGO.sd_mount()) return;
  
  file = std::unique_ptr<AudioFileSourceSD> (new AudioFileSourceSD(filename.c_str()));
  id3  = std::unique_ptr<AudioFileSourceID3>(new AudioFileSourceID3(file.get()));
  id3->RegisterMetadataCB(MP3::MDCallback, (void*)"ID3TAG");
  mp3.begin(id3.get(), &sound_out);  

}

void MP3::stop(){
  if (mp3.isRunning()) {
    mp3.stop();
  }
}

void MP3::run(){
  if (mp3.isRunning()) {
    if (!mp3.loop())  mp3.stop();
  }
}

void MP3::MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("[MP3] ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void MP3::StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1)-1]=0;
  Serial.printf("[MP3] STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}
