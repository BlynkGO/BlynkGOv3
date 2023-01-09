#include "BlynkGO_ButtonISR.h"

#if defined (BLYNKGO_MINI) || defined (TTGO_TWATCH_2019) || defined (BLYNKGO_LORA32) || defined (BLYNKGO_M5STACK) || BLYNKGO_USE_BUTTON_ISR

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
void ButtonISR::onLongPressed(void(*fn)(ButtonISR*))        { if(!_attached) attach(); _fn_longpressed = fn;          }
void ButtonISR::onLongPressedRepeat(void(*fn)(ButtonISR*))  { if(!_attached) attach(); _fn_longpressed_repeat = fn;   }
void ButtonISR::onLongClicked(void(*fn)(ButtonISR*))  			{ if(!_attached) attach(); _fn_longclicked = fn;   				}

bool ButtonISR::isPressing()               { return (state==BTN_ISR_PRESSING || state == BTN_ISR_LONGPRESSED_PRESSING); }
bool ButtonISR::hasLongPressed()           { return (_longpressed && _fn_longpressed !=NULL );        }
void ButtonISR::event_pressed()            { if(_fn_pressed) _fn_pressed(this);                       }
void ButtonISR::event_pressing()           { if(_fn_pressing) _fn_pressing(this);                     }
void ButtonISR::event_released()           { if(_fn_released) _fn_released(this);                     }
void ButtonISR::event_clicked()            { if(_fn_clicked) _fn_clicked(this);                       }
void ButtonISR::event_longpressed()        { if(_fn_longpressed) _fn_longpressed(this);               }
void ButtonISR::event_longpressed_repeat() { if(_fn_longpressed_repeat) _fn_longpressed_repeat(this); }
void ButtonISR::event_longclicked()        { if(_fn_longclicked) _fn_longclicked(this);               }

void ButtonISR::loop() { 
  switch(state){
    case BTN_ISR_PRESSED:
      _timer = millis();
      event_pressed();
      _longpressed = false;
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
      event_longpressed_repeat();
      state = BTN_ISR_LONGPRESSED_PRESSING;
      break;
    case BTN_ISR_LONGPRESSED_PRESSING:
      event_pressing();
      if( millis() >= _timer + _timeout_longpressed){
        state = BTN_ISR_LONGPRESSED_REPEAT;
      }
      break;
    case BTN_ISR_RELEASED:
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
}


#endif
