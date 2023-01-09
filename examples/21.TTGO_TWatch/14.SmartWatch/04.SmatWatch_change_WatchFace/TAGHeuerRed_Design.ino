
IMG_DECLARE(img_tagheuer_red_bg);
IMG_DECLARE(img_tagheuer_hr);
IMG_DECLARE(img_tagheuer_min);
IMG_DECLARE(img_tagheuer_sec);

namespace TAGHeuerRed {
  
void design(){
  watch_bg  = img_tagheuer_red_bg;
  watch_hr  = img_tagheuer_hr;
  watch_min = img_tagheuer_min;
  watch_sec = img_tagheuer_sec;

  watch_hidden_all_label();

}
  
void update() { 
  watch_drawPin(hour(),minute(),second());
}
  
} //namespace TAGHeuerRed

