#ifndef __GANIMATION_H__
#define __GANIMATION_H__

#include "GObject.h"
#include "GWidgets.h"

enum {
  ANIM_PATH_LINEAR,         // ปกติ ค่าเพิ่มแบบ liner, 
  ANIM_PATH_EASE_IN,        // ค่า จากค่อยๆช้า->เร็ว, 
  ANIM_PATH_EASE_OUT,       // ค่า จากเร็ว->ช้าลง, 
  ANIM_PATH_EASE_IN_OUT,    // ค่า จากค่อยๆช้า->เร็ว->ช้า, 
  ANIM_PATH_OVERSHOOT,      
  ANIM_PATH_BOUNCE,         // ค่าเพิ่มแบบเด้งสะท้อน คือ เด้งสะท้อนแรงจนสุดท้ายค่าเด้งสะท้อนค่อยๆลดลงๆจนถึงค่าเป้าหมาย, 
  ANIM_PATH_STEP,
};
typedef uint8_t anim_path_t;

class GAnimation;

typedef void (*anim_exec_cb_t)(GWidget*widget, int16_t value);
// typedef void (*anim_ready_cb_t)(GWidget*widget);
typedef void (*anim_ready_cb_t)(GAnimation *a);

/* standard anim_exec_cb_t */
void ANIM_X(GWidget* widget, int16_t value);
void ANIM_Y(GWidget* widget, int16_t value);
void ANIM_WIDTH(GWidget* widget, int16_t value);
void ANIM_HEIGHT(GWidget* widget, int16_t value);
void ANIM_ANGLE(GWidget* widget, int16_t value);  // for GImage
void ANIM_ZOOM(GWidget* widget, int16_t value);   // for GImage
void ANIM_OPA(GWidget* widget, int16_t value);

#define REPEAT_ON         1
#define REPEAT_OFF        0
#define PLAYBACK_ON       1
#define PLAYBACK_OFF      0


class GAnimation {
  public:
    GAnimation();
    GAnimation(GWidget &widget);
    GAnimation(GWidget *widget);
    GAnimation(GWidget &widget, anim_exec_cb_t fn, int32_t start, int32_t end, uint16_t time, bool repeat = false, bool playback = false);
    GAnimation(GWidget *widget, anim_exec_cb_t fn, int32_t start, int32_t end, uint16_t time, bool repeat = false, bool playback = false);
    ~GAnimation();

    void widget(GWidget &widget);
    void widget(GWidget *widget);
    GWidget* widget();

    void exec_cb(anim_exec_cb_t fn, int32_t start, int32_t end);
    void ready_cb(anim_ready_cb_t fn);

    void value(int32_t start, int32_t end);
    void time(uint16_t time);
    void delay(uint16_t time);

    void repeat(bool enable);
    void repeat_pause(uint16_t time);
    void playback(bool enable);
    void playback_pause(uint16_t time);

    inline void repeat(bool enable, uint16_t repeat_pause_time)       { this->repeat(enable); this->repeat_pause(repeat_pause_time); }
    inline void playback(bool enable, uint16_t playback_pause_time)   { this->playback(enable);  this->playback_pause(playback_pause_time); }

    /*  anim_path_t : 
          ANIM_PATH_LINEAR      ปกติ ค่าเพิ่มแบบ liner, 
          ANIM_PATH_EASE_IN     ค่า จากค่อยๆช้า->เร็ว, 
          ANIM_PATH_EASE_OUT    ค่า จากเร็ว->ช้าลง, 
          ANIM_PATH_EASE_IN_OUT ค่า จากค่อยๆช้า->เร็ว->ช้า, 
          ANIM_PATH_OVERSHOOT, 
          ANIM_PATH_BOUNCE      ค่าเพิ่มแบบเด้งสะท้อน คือ เด้งสะท้อนแรงจนสุดท้ายค่าเด้งสะท้อนค่อยๆลดลงๆจนถึงค่าเป้าหมาย, 
          ANIM_PATH_STEP */
    void path_type(anim_path_t path_type);
    void path_cb(lv_anim_path_cb_t anim_path_cb);   // สำหรับกำหนด path_cb เอง  โดยฟังกชั่น anim_path_cb มี prototype ดังนี้ lv_anim_value_t (*anim_path_cb)(const lv_anim_t *a);

    void  user_data(void* data);
    void* user_data();


    void init(GWidget &widget, anim_exec_cb_t fn, int32_t start, int32_t end, uint16_t time, bool repeat = false, bool playback = false);
    void init(GWidget *widget, anim_exec_cb_t fn, int32_t start, int32_t end, uint16_t time, bool repeat = false, bool playback = false);

    bool create();
    void del();
    inline bool isCreated()        { return _created; }

    anim_ready_cb_t _ready_cb= NULL;

    static uint16_t count_running();
    static void     refresh_now();

  private:
    GWidget*  _widget     = NULL;
    void*     _user_data  = NULL;
    bool      _created    = false;
    lv_anim_t a;

    static void ganim_ready_cb(lv_anim_t *a);
};

GAnimation * new_GAnimation();

#endif //__GANIMATION_H__

