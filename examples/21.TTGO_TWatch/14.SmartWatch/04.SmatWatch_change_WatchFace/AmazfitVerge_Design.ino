
IMG_DECLARE(img_amafit_verge_bg);
IMG_DECLARE(img_amafit_verge_hr);
IMG_DECLARE(img_amafit_verge_min);
IMG_DECLARE(img_amafit_verge_sec);

namespace AmazfitVerge {

void design(){
  watch_bg  = img_amafit_verge_bg;
  watch_hr  = img_amafit_verge_hr;
  watch_min = img_amafit_verge_min;
  watch_sec = img_amafit_verge_sec;

  watch_hidden_all_label();
}
  
void update() { 
  watch_drawPin(hour(),minute(),second());
}

} // namespace AmazfitVerge

