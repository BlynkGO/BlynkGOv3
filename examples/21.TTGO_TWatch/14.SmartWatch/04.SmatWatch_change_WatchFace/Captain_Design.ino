
IMG_DECLARE(img_captainamerica_bg);
IMG_DECLARE(img_captainamerica_hr);
IMG_DECLARE(img_captainamerica_min);
IMG_DECLARE(img_captainamerica_sec);

namespace CaptainWatch {
  
void design(){
  watch_bg  = img_captainamerica_bg;
  watch_hr  = img_captainamerica_hr;
  watch_min = img_captainamerica_min;
  watch_sec = img_captainamerica_sec;
  
  watch_hidden_all_label();

}
  
void update() { 
  watch_drawPin(hour(),minute(),second());
}
  
} //namespace CaptainWatch

