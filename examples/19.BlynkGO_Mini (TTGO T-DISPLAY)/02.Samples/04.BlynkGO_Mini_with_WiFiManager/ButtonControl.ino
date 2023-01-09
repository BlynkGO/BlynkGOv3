void multiscreen_btn_control(){
  
  // ควบคุมปุ่มกด เมื่อมีการกด Pressed เข้ามาให้เลื่อนหน้า multiscreen ถอยกลับ
  // เมื่อ btn1 มีการ click ให้ทำการเลื่อนหน้าจอ multiscreen ถอยย้อนหน้าก่อน
  BTN1.onClicked([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() > 0) {
      multiscreen.current_screen( multiscreen.current_screen_id()-1);
    }else{
      multiscreen.current_screen(multiscreen.num()-1, false);
    }
  });

  BTN1.onLongPressed([](ButtonISR*btn){
    rect_msgbox.hidden(false);  
    static GTimer timer;
    timer.setOnce(1000,[](){ wifimanager_mode = true; } );
  });

  // ควบคุมปุ่มกด เมื่อมีการกด Pressed เข้ามาให้เลื่อนหน้า multiscreen ถัดไป
  // เมื่อ btn2 มีการ click ให้ทำการเลื่อนหน้าจอ multiscreen ไปหน้าถัดไป
  BTN2.onClicked([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() < multiscreen.num()-1) {
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
    }else{
      multiscreen.current_screen( 0, false);
    }
  });
}
