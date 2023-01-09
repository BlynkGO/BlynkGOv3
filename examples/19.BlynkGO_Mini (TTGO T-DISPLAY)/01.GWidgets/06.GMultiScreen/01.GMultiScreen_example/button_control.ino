void multiscreen_btn_ctrl(){
  BTN1.reset();
  BTN2.reset();

  BTN1.onClicked([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() < multiscreen.num()-1) {
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
    }else{
      multiscreen.current_screen( 0, false);
    }
  });  

  BTN1.onLongPressed([](ButtonISR* btn){ 
    multiscreen.current_screen(0);
  });

  //--------------------------------------------
  BTN2.onPressed([](ButtonISR* btn) {
    sw3d[multiscreen.current_screen_id()].toggle();
  });
}
