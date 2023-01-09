namespace ScreenTurnOFF {
  GRect  layer_backlight_off;
  GTimer timer_backlight_off;
  uint32_t turnoff_timeout = 30;

  void start(){
    timer_backlight_off.setInterval(15000,[](){
      if( millis() - last_touch() > ScreenTurnOFF::turnoff_timeout*1000){
        layer_backlight_off.hidden(false);
        layer_backlight_off.toForeground();
        BlynkGO.backlight_OFF();
        Serial.println("[BackLight] turn off");
        timer_backlight_off.del();
      }
    });
  }
  
  void set(uint32_t timeout_sec){
    turnoff_timeout = timeout_sec;
    layer_backlight_off.hidden(true);
    layer_backlight_off.opa(0);
    layer_backlight_off.onClicked([](GWidget*widget){
      layer_backlight_off.hidden(true);
      BlynkGO.backlight_ON();
      Serial.println("[BackLight] turn on");
      ScreenTurnOFF::start();
    });
    ScreenTurnOFF::start();
    Serial.printf("[BackLight] screen turn off : %d secs\n",  ScreenTurnOFF::turnoff_timeout );
  }
}

