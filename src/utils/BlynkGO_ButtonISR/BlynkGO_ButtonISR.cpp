#include "BlynkGO_ButtonISR.h"

#if defined (BLYNKGO_MINI) || defined (TTGO_TWATCH_2019) || defined (BLYNKGO_LORA32) || defined (BLYNKGO_M5STACK) || defined (BEENEXT_1_9) || BLYNKGO_USE_BUTTON_ISR

ButtonISR::ButtonISR(uint8_t pin)
:PIN(pin)
{ 
	pinMode(PIN, INPUT_PULLUP);
}

void ButtonISR::gpio(uint8_t pin){
  if(_attached){  detach();  }

  PIN = pin;
  pinMode(PIN, INPUT_PULLUP);
  attach();
}
// static class function
void ButtonISR::isr(void* arg)  { 
  ButtonISR* btn = (ButtonISR*)arg;
  switch(btn->state){
    case BTN_ISR_STANDBY:
      btn->state = BTN_ISR_PRESSED;
      btn->pressing_cnt++;
      break;
    case BTN_ISR_PRESSED:
    case BTN_ISR_PRESSING:
    case BTN_ISR_LONGPRESSED:
    case BTN_ISR_LONGPRESSED_PRESSING:
    case BTN_ISR_LONGPRESSED_REPEAT:
      btn->state = BTN_ISR_RELEASED;
      btn->pressing_cnt = 0;
      break;
  }
}

void ButtonISR::attach() { 
  attachInterruptArg(PIN, ButtonISR::isr, this, CHANGE);
  _attached= true; 
}

void ButtonISR::detach() { 
  detachInterrupt(PIN); 
  _attached =false;
}

void ButtonISR::onPressed(void(*fn)(ButtonISR*))            { if(!_attached) attach(); _fn_pressed = fn;              }
void ButtonISR::onPressing(void(*fn)(ButtonISR*))           { if(!_attached) attach(); _fn_pressing = fn;             }
void ButtonISR::onReleased(void(*fn)(ButtonISR*))           { if(!_attached) attach(); _fn_released = fn;             }
void ButtonISR::onClicked(void(*fn)(ButtonISR*))            { if(!_attached) attach(); _fn_clicked = fn;              }
void ButtonISR::onDoubleClicked(void(*fn)(ButtonISR*))      { if(!_attached) attach(); _fn_doubleclicked = fn;        }
void ButtonISR::onLongPressed(void(*fn)(ButtonISR*))        { if(!_attached) attach(); _fn_longpressed = fn;          }
void ButtonISR::onLongPressedRepeat(void(*fn)(ButtonISR*))  { if(!_attached) attach(); _fn_longpressed_repeat = fn;   }
void ButtonISR::onLongClicked(void(*fn)(ButtonISR*))  			{ if(!_attached) attach(); _fn_longclicked = fn;   				}

bool ButtonISR::isPressing()               { return (state==BTN_ISR_PRESSING || state == BTN_ISR_LONGPRESSED_PRESSING); }
bool ButtonISR::hasLongPressed()           { return (_longpressed && _fn_longpressed !=NULL );        }
void ButtonISR::event_pressed()            { if(_fn_pressed) _fn_pressed(this);                       }
void ButtonISR::event_pressing()           { if(_fn_pressing) _fn_pressing(this);                     }
//void ButtonISR::event_released()           { if(_fn_released) _fn_released(this);                     }
//void ButtonISR::event_clicked()            { if(_fn_clicked) _fn_clicked(this);                       }
//void ButtonISR::event_double_clicked()     { if(_fn_doubleclicked) _fn_doubleclicked(this);           }
void ButtonISR::event_longpressed()        { if(_fn_longpressed) _fn_longpressed(this);               }

void ButtonISR::event_longpressed_repeat() { 
  if(_fn_longpressed_repeat) _fn_longpressed_repeat(this);
}

void ButtonISR::event_longclicked()        { if(_fn_longclicked) _fn_longclicked(this);               }

void ButtonISR::event_clicked() { 
  if( !_mode_separate_Clicked_LongPressed && this->hasLongPressed() ) return;
  unsigned long now = millis();
  if (_click_count == 0) {
    _click_count = 1;
    _click_time = now;
  } else {
    if (now - _click_time <= _timeout_doubleclick) {
      _click_count++;
    } else {
      _click_count = 1;
      _click_time = now;
      _double_clicked = false; // Reset double clicked flag
    }
  }
}

void ButtonISR::event_released(){ 
//  if(_fn_released) _fn_released(this);
  if(_double_clicked_called) {
    if(_fn_released) _fn_released(this);
    _double_clicked_called =false;
  }
  if (_double_clicked) {
    _double_clicked = false;
  } else {
    if (_fn_released) _fn_released(this);
  }
}

void ButtonISR::loop() { 
  static bool _must_attach=false;
  switch(state){
    case BTN_ISR_PRESSED:
      _timer = millis();
      event_pressed();
      _longpressed = false;
      _cnt_long_pressed_repeate = 0;
      state = BTN_ISR_PRESSING;
      break;
    case BTN_ISR_PRESSING:
      event_pressing();
      if( millis() >= _timer + _timeout_longpressed){
        state = BTN_ISR_LONGPRESSED;
      }
      break;
    case BTN_ISR_LONGPRESSED:
      _timer = millis();
      _longpressed = true;
      event_longpressed();
      state = BTN_ISR_LONGPRESSED_PRESSING;
      break;
    case BTN_ISR_LONGPRESSED_REPEAT:
      _timer = millis();
      _longpressed = true;
      if( _cnt_long_pressed_repeate++ < 2) {
        event_longpressed_repeat();
      }else{
        // กันสัญญาณรบกวน แล้ว change ของ ISR รวน ทำให้กลับทิศ ให้ล้างแล้ว attach ISR ใหม่
        this->detach();
        this->attach();
        pressing_cnt=1;
        _longpressed = true;
        _cnt_long_pressed_repeate = 0;
        event_longpressed_repeat();
        _must_attach = true;
        state = BTN_ISR_LONGPRESSED_REPEAT;
      }
      state = BTN_ISR_LONGPRESSED_PRESSING;
      break;
    case BTN_ISR_LONGPRESSED_PRESSING:
      event_pressing();
      if( millis() >= _timer + _timeout_longpressed){
        state = BTN_ISR_LONGPRESSED_REPEAT;
      }
      break;
    case BTN_ISR_RELEASED:
      if(_must_attach == true){
        if(digitalRead(PIN) == HIGH) {// มีสถานะปล่อยจริงๆเป็นการกันซ้ำ
          this->detach();
          this->attach();
          _must_attach = false;
          state= BTN_ISR_STANDBY;
          if (_fn_released) _fn_released(this);
        }
        return;
      }
      event_released();
      if(_longpressed && _fn_longclicked !=NULL) {
      	event_longclicked();
      	_longpressed = false;
      }else{
      	event_clicked();
      }
      state = BTN_ISR_STANDBY;
      pressing_cnt = 0;
      break;
  }

  if (_click_count == 1) {
//    if(now - _click_time < _timeout_doubleclick)
    if( !_fn_doubleclicked) { if (_fn_clicked ) _fn_clicked(this); _click_count = 0; }
    else if (_fn_clicked && (millis() - _click_time > _timeout_doubleclick) && !_double_clicked) { 
      _fn_clicked(this); _click_count = 0;
    }
  } else if (_click_count == 2) {
    if (_fn_doubleclicked) { _fn_doubleclicked(this); _double_clicked_called = true; }
    _double_clicked = true; // Set double clicked flag
    _click_count = 0;
  }
}


#endif
