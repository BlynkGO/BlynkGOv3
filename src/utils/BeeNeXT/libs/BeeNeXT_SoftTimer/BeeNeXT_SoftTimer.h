#ifndef __BEENEXT_SOFTTIMER_H__
#define __BEENEXT_SOFTTIMER_H__

#include <Arduino.h>
#include "linklist.h"

enum { SWTIMER_TYPE_INTERVAL, SWTIMER_TYPE_DELAY };
typedef uint8_t swtimer_type_t;
typedef void(*swtimer_cb_t)(void);
typedef void(*swtimer_param_cb_t)(void* param);
typedef struct _swtimer {
  uint16_t        id;
  bool            pause;
  swtimer_type_t  type;
  unsigned long   timeout;
  unsigned long   timer;
  swtimer_cb_t    fn_cb;
  swtimer_param_cb_t fn_param_cb;
  void*           param;
} swtimer_t;

class SoftTimer {
  public:
    SoftTimer();

    void setInterval(unsigned long period_ms, swtimer_cb_t fn, bool start_first=false);
    void setInterval(unsigned long period_ms, swtimer_param_cb_t fn, void* param, bool start_first=false);

    void pause();     // สำหรับ timer ที่ setInterval แล้วต้องการให้หยุดทำงานชั่วคราว
    void replay();    // สำหรับ timer ที่ setInterval หลังมีการ pause() ต้องการให้กลับมาทำงานต่อ

    void delay(unsigned long delay_ms, swtimer_cb_t fn, bool start_first=false);
    void delay(unsigned long delay_ms, swtimer_param_cb_t fn, void* param, bool start_first=false);
    void del();

    inline void setOnce(unsigned long delay_ms, swtimer_cb_t fn, bool start_first=false){
      this->delay(delay_ms, fn, start_first);
    }
    inline void setOnce(unsigned long delay_ms, swtimer_param_cb_t fn, void* param, bool start_first=false){
      this->delay(delay_ms, fn, param, start_first);
    }
    static void       run();

    static bool       _ll_inited;
    static ll_t       swtimer_ll;
    static uint16_t   add_swtimer(swtimer_type_t type, unsigned long timeout, swtimer_cb_t fn_cb);
    static uint16_t   add_swtimer(swtimer_type_t type, unsigned long timeout, swtimer_param_cb_t fn_cb, void* param);
    static swtimer_t* find_swtimer(uint16_t id);
    static bool       del_swtimer(uint16_t id);
    static uint32_t   count_swtimer();
    static void       listall_swtimer();
    static void       clear_swtimer();
  private:
    uint16_t _swtimer_id = 0;
};


#endif // __BEENEXT_SOFTTIMER_H__
