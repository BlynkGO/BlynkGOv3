IMG_DECLARE(img_watertank120);
IMG_DECLARE(img_watertank120_mask);

GContainer cont_watertank;
  GMask  watertank_mask(img_watertank120_mask,cont_watertank);   // ประกาศวิตเจ็ต GMask โดยโหลด mask ด้วย img_watertank120_mask
    GRect  watertank_water(watertank_mask);                 // ประกาศวิตเจ็ต GRect ให้มี parent คือ watertank_mask
  GImage watertank(img_watertank120, cont_watertank);            // ประกาศวิตเจ็ตรูปภาพ GImage โดยโหลดรูปด้วย img_watertank120

GLabel lb_water_info("น้ำประปา");
GLabel lb_water_volume;
GLabel lb_water_unit("liters");

namespace WaterTank {
  void design(GWidget&parent);
  void setWaterHeight(float h);  // 0-100  
  void anim_show();
}

void WaterTank::design(GWidget*parent){
  cont_watertank.parent(parent);
  cont_watertank.padding(0);
  cont_watertank.layout(LAYOUT_NONE);
  cont_watertank.opa(0);

    watertank.align(ALIGN_LEFT);      // วาง watertank ชิดขอบซ้ายของจอ TFT
    watertank_mask.create();
      watertank_water.height(100);            // กำหนดความสูงของ สี่เหลี่ยม
      watertank_water.color(TFT_BLUE);
      watertank_water.opa(180);
      watertank_water.align(ALIGN_BOTTOM);    // วาง water ชิดขอบล่างของ watertank_mask (เนื่องจาก watertank_mask เป็น parent)    
    watertank_mask.align(watertank,ALIGN_IN_TOP);  // วาง watertank_mask ชิดขอบบนกลางของ watertank (เนื่องจาก watertank เป็น parent)
  
    watertank.clickable(false);
    watertank_mask.clickable(false);
    watertank_water.clickable(false);
  
  cont_watertank.align(ALIGN_LEFT,50);

  lb_water_info.parent(parent);
  lb_water_info.font(prasanmit_25, TFT_WHITE);
  lb_water_info.align(cont_watertank, ALIGN_RIGHT_TOP,25,10);

  lb_water_volume.parent(parent);
  lb_water_volume.font(prasanmit_40, TFT_WHITE);
  lb_water_volume.align(ALIGN_TOP_RIGHT, -70, lb_water_info.bottomY());

  lb_water_unit.parent(parent);
  lb_water_unit.font(prasanmit_20, TFT_WHITE);
  lb_water_unit.align(lb_water_volume, ALIGN_RIGHT_BOTTOM, 5,-6);
}

void WaterTank::setWaterHeight(float h ){
  h = constrain(h, 0, 100);
  watertank_water.height(map_f(h,0,100, 2, 115));
  watertank_water.align(ALIGN_BOTTOM);
  lb_water_volume = h*100;
}

void WaterTank::anim_show(){
  static GAnimation anim;
  anim.init(watertank_water,[](GWidget*widget, int16_t v){
    float h = v/10.0;
    WaterTank::setWaterHeight(h);
  }, 0, 1000, 5000,true, true);
  anim.create();
}
