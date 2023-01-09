IMG_DECLARE(img_doraemon_00);
IMG_DECLARE(img_doraemon_01);
IMG_DECLARE(img_doraemon_02);
IMG_DECLARE(img_doraemon_03);
IMG_DECLARE(img_doraemon_04);
IMG_DECLARE(img_doraemon_05);
IMG_DECLARE(img_doraemon_scene);

GImage doraemon_scene(img_doraemon_scene);
GImage doraemon[6] = { {img_doraemon_00}, {img_doraemon_01}, {img_doraemon_02}, {img_doraemon_03}, {img_doraemon_04}, {img_doraemon_05} }; 
GAnimation anim_doraemon;
int8_t cur_doraemon_id = 0;

void DoraemonAnime_create(GWidget&parent){
  doraemon_scene.parent(parent);
  for(int i=0; i < 6; i++){
    doraemon[i].parent(parent);
    if(i) doraemon[i].hidden(true);
  }
  
  anim_doraemon.init(doraemon[0], [](GWidget*widget, int16_t value){
    static uint32_t timer;
    BTN1.loop(); BTN2.loop();  // ใส่เพื่อให้ ปุ่มกด สามารถรับสัญญาณ กดเลื่อนได้ด้วย
    if(timer <= millis()){     // หากถึง 40ms ให้เปลี่ยนเป็น doraemon ภาพถัดไป
      timer = millis()+40;
      doraemon[cur_doraemon_id].hidden(true);
      cur_doraemon_id = (cur_doraemon_id+1) %6;
      doraemon[cur_doraemon_id].hidden(false);
    }
    doraemon[cur_doraemon_id].posX(value);  // ขยับตำแหน่ง
  }, -doraemon[0].width()-10, GScreen.width()+10, 8000);
  
  anim_doraemon.repeat(true);   // ให้ anim มีการเล่นวนซ้ำ
  anim_doraemon.create();       // เริ่ม anim สร้างขึ้นมาเพื่อให้ทำงานตามกำหนด

}
