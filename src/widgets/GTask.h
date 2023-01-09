#ifndef __GTASK_H__
#define __GTASK_H__

#include <Arduino.h>
#include "../extra/GLVGL.h"

typedef void(*task_cb_t)(void*);

class GTask {
  public:
    GTask();
    ~GTask();

    // ใช้ API นี้ เป็นหลัก    
    // task_prio_t : TASK_PRIO_OFF , TASK_PRIO_LOWEST, TASK_PRIO_LOW, TASK_PRIO_MID, TASK_PRIO_HIGH, TASK_PRIO_HIGHEST
    inline void setInterval(uint32_t period, task_cb_t fn, void* param=NULL, task_prio_t prior=TASK_PRIO_LOWEST)   { this->create(period, fn, param, prior); }
    inline void setOnce(uint32_t period, task_cb_t fn, void* param=NULL, task_prio_t prior=1)       { this->create(period, fn, param, prior); this->once(); }
           // bool isCreated()      { return this->_created; }

    // API แบบ Original
    void create(uint32_t period, task_cb_t fn, void* param=NULL, task_prio_t prior=1);
    void create(uint32_t period, task_cb_t fn, task_prio_t prior);
    void del();


    void task_cb(task_cb_t fn, void*param=NULL);
    void period(uint32_t period_ms);
    void priority(task_prio_t prior); /* 0-5 */
    void reset();
    void once();
    void ready();

    static void enable(bool en);

    task_cb_t fn;
    void *param;

  private:
    task_t *_task=NULL;
    bool  _created = false;
};

#endif //__GTASK_H__
