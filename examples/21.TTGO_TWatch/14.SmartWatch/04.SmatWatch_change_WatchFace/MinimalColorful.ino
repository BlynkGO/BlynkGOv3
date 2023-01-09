
IMG_DECLARE(img_minimalcolorful_bg);
IMG_DECLARE(img_minimalcolorful_hr);
IMG_DECLARE(img_minimalcolorful_min);
IMG_DECLARE(img_minimalcolorful_sec);

namespace MinimalColorful {

void design(){
  watch_bg  = img_minimalcolorful_bg;
  watch_hr  = img_minimalcolorful_hr;
  watch_min = img_minimalcolorful_min;
  watch_sec = img_minimalcolorful_sec;

  watch_hidden_all_label();
  
  watch_date  .hidden(false);   watch_date.font(prasanmit_20); watch_date.center(50,60);  watch_date.toBackground();
  watch_label[0].hidden(false); watch_label[0].font(prasanmit_20); watch_label[0].center(150, 60); watch_label[0].toBackground();
}
  
void update() { 
  watch_drawPin(hour(),minute(),second());

  watch_date = String(day())+ "-" + String(month());

  const char* th_wday[] = { "อาทิตย์", "จันทร์", "อังคาร", "พุธ", "พฤหัส", "ศุกร์", "เสาร์" };  
  watch_label[0] = th_wday[wday()];
}

} // namespace MinimalColorful

