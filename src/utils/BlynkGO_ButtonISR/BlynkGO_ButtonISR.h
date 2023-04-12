#ifndef __BLYNKGO_BUTTONISR_H__
#define __BLYNKGO_BUTTONISR_H__

#include "../../config/blynkgo_config.h"
// #define BLYNKGO_USE_BUTTON_ISR    1

#if defined (BLYNKGO_MINI) || defined (TTGO_TWATCH_2019) || defined (BLYNKGO_LORA32) || defined (BLYNKGO_M5STACK) || defined (BEENEXT_1_9) || BLYNKGO_USE_BUTTON_ISR

#include <Arduino.h>

/** Major version number (X.x.x) */
#define BUTTONISR_VERSION_MAJOR   2
/** Minor version number (x.X.x) */
#define BUTTONISR_VERSION_MINOR   0
/** Patch version number (x.x.X) */
#define BUTTONISR_VERSION_PATCH   0


#define BUTTONISR_VERSION_TEXT    (String(BUTTONISR_VERSION_MAJOR)+"."+String(BUTTONISR_VERSION_MINOR)+"."+String(BUTTONISR_VERSION_PATCH))

/**
 * Macro to convert version number into an integer
 *
 * To be used in comparisons, such as BUTTONISR_VERSION >= BUTTONISR_VERSION_VAL(3, 0, 0)
 */
#define BUTTONISR_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current  version, as an integer
 *
 * To be used in comparisons, such as BUTTONISR_VERSION >= BUTTONISR_VERSION_VAL(3, 0, 0)
 */
#define BUTTONISR_VERSION  BUTTONISR_VERSION_VAL( BUTTONISR_VERSION_MAJOR, \
                                                  BUTTONISR_VERSION_MINOR, \
                                                  BUTTONISR_VERSION_PATCH)



typedef enum {BTN_ISR_STANDBY, BTN_ISR_PRESSED, BTN_ISR_PRESSING, 
              BTN_ISR_LONGPRESSED, BTN_ISR_LONGPRESSED_PRESSING, BTN_ISR_LONGPRESSED_REPEAT,
              BTN_ISR_RELEASED } btn_isr_state_t;

class ButtonISR { 
public:
  ButtonISR() {}
  ButtonISR(uint8_t pin);

  uint32_t pressing_cnt=0; btn_isr_state_t state = BTN_ISR_STANDBY;

  inline void separate_Clicked_LongPressed(bool enable) {  _mode_separate_Clicked_LongPressed = enable; }

  void gpio(uint8_t);

  static void IRAM_ATTR isr(void* arg);
  void attach();
  void detach();
  void onPressed(void(*fn)(ButtonISR*));
  void onPressing(void(*fn)(ButtonISR*));
  void onReleased(void(*fn)(ButtonISR*));
  void onClicked(void(*fn)(ButtonISR*));
  void onDoubleClicked(void(*fn)(ButtonISR*));
  void onLongPressed(void(*fn)(ButtonISR*));
  void onLongPressedRepeat(void(*fn)(ButtonISR*));
  void onLongClicked(void(*fn)(ButtonISR*));

  inline void onDoubleClicked(uint32_t timeout, void(*fn)(ButtonISR*))      {  _timeout_doubleclick = timeout; this->onDoubleClicked(fn);     }
  inline void onLongPressed(uint32_t timeout, void(*fn)(ButtonISR*))        {  _timeout_longpressed = timeout; this->onLongPressed(fn);       }
  inline void onLongPressedRepeat(uint32_t timeout, void(*fn)(ButtonISR*))  {  _timeout_longpressed = timeout; this->onLongPressedRepeat(fn); }
  
  inline void reset(){
    _fn_pressed = NULL; _fn_pressing    = NULL; _fn_released = NULL;
    _fn_clicked = NULL; _fn_longpressed = NULL; _fn_longpressed_repeat = NULL;
    _fn_doubleclicked = NULL;
  }

  bool isPressing();
  bool hasLongPressed();
  void event_pressed();
  void event_pressing();
  void event_released();
  void event_clicked();
//  void event_double_clicked();
  void event_longpressed();
  void event_longpressed_repeat();
  void event_longclicked();
  void loop();
  inline void run()     { this->loop();   }
  inline void update()  { this->update(); }

private:
  void (*_fn_pressed)(ButtonISR*)= NULL;
  void (*_fn_pressing)(ButtonISR*)= NULL;
  void (*_fn_released)(ButtonISR*)= NULL;
  void (*_fn_clicked)(ButtonISR*)= NULL;
  void (*_fn_doubleclicked)(ButtonISR*)= NULL;
  void (*_fn_longpressed)(ButtonISR*)=NULL;
  void (*_fn_longpressed_repeat)(ButtonISR*)=NULL;
  void (*_fn_longclicked)(ButtonISR*)=NULL;

  uint8_t PIN; 
  bool _attached = false;
  bool _longpressed = false;
  uint32_t _timer, _timeout_longpressed = 1500;
  
  int _click_count = 0;
  bool _double_clicked = false;
  bool _double_clicked_called = false;
  unsigned long _click_time = 0;
  uint32_t _timeout_doubleclick = 400;
  bool _mode_separate_Clicked_LongPressed = false;
  uint8_t _cnt_long_pressed_repeate = 0;
};

#endif 

#endif // __BLYNKGO_BUTTONISR_H__
