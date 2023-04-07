#ifndef __BLYNKGO_BUTTONISR_H__
#define __BLYNKGO_BUTTONISR_H__

#include "../../config/blynkgo_config.h"

#if defined (BLYNKGO_MINI) || defined (TTGO_TWATCH_2019) || defined (BLYNKGO_LORA32) || defined(BEENEXT_1_9) || defined (BLYNKGO_M5STACK) || BLYNKGO_USE_BUTTON_ISR

#include <Arduino.h>

typedef enum {BTN_ISR_STANDBY, BTN_ISR_PRESSED, BTN_ISR_PRESSING, 
              BTN_ISR_LONGPRESSED, BTN_ISR_LONGPRESSED_PRESSING, BTN_ISR_LONGPRESSED_REPEAT,
              BTN_ISR_RELEASED } btn_isr_state_t;

class ButtonISR { 
public:
  ButtonISR() {}
  ButtonISR(uint8_t pin);

  uint32_t pressing_cnt=0; btn_isr_state_t state = BTN_ISR_STANDBY;

  void gpio(uint8_t);

  static void IRAM_ATTR isr(void* arg);
  void attach();
  void detach();
  void onPressed(void(*fn)(ButtonISR*));
  void onPressing(void(*fn)(ButtonISR*));
  void onReleased(void(*fn)(ButtonISR*));
  void onClicked(void(*fn)(ButtonISR*));
  void onLongPressed(void(*fn)(ButtonISR*));
  void onLongPressedRepeat(void(*fn)(ButtonISR*));
  void onLongClicked(void(*fn)(ButtonISR*));

  inline void reset(){
    _fn_pressed = NULL; _fn_pressing    = NULL; _fn_released = NULL;
    _fn_clicked = NULL; _fn_longpressed = NULL; _fn_longpressed_repeat = NULL;
  }

  bool isPressing();
  bool hasLongPressed();
  void event_pressed();
  void event_pressing();
  void event_released();
  void event_clicked();
  void event_longpressed();
  void event_longpressed_repeat();
  void event_longclicked();
  void loop();

private:
  void (*_fn_pressed)(ButtonISR*)= NULL;
  void (*_fn_pressing)(ButtonISR*)= NULL;
  void (*_fn_released)(ButtonISR*)= NULL;
  void (*_fn_clicked)(ButtonISR*)= NULL;
  void (*_fn_longpressed)(ButtonISR*)=NULL;
  void (*_fn_longpressed_repeat)(ButtonISR*)=NULL;
  void (*_fn_longclicked)(ButtonISR*)=NULL;

  uint8_t PIN; 
  bool _attached = false;
  bool _longpressed = false;
  uint32_t _timer, _timeout_longpressed = 1500;
};

#endif 

#endif // __BLYNKGO_BUTTONISR_H__
