#ifndef __BLYNKGO_BMA423_H__
#define __BLYNKGO_BMA423_H__

#include <Arduino.h>
#include <Wire.h>
extern "C" {
  #include "driver/bma423.h"
}

#include "../../config/blynkgo_config.h"

typedef enum {
  DIRECTION_NONE            = 0,
  DIRECTION_TOP_EDGE        = 1,
  DIRECTION_BOTTOM_EDGE     = 2,
  DIRECTION_LEFT_EDGE       = 3,
  DIRECTION_RIGHT_EDGE      = 4,
  DIRECTION_DISP_UP         = 5,
  DIRECTION_DISP_DOWN       = 6,
} direction_t;

 // *        0x00    | BMA423_USER_STATIONARY
 // *        0x01    | BMA423_USER_WALKING
 // *        0x02    | BMA423_USER_RUNNING
 // *        0x03    | BMA423_STATE_INVALID

typedef enum {
  ACTIVITY_STATIONARY,
  ACTIVITY_WALKING,
  ACTIVITY_RUNNING,
  ACTIVITY_INVALID,
} activity_t; 

// typedef struct bma4_accel bma423_accel_t;
typedef struct {
  int16_t x;          // lsb
  int16_t y;          // lsb
  int16_t z;          // lsb
  float x_ms2;        // m/s2
  float y_ms2;        // m/s2
  float z_ms2;        // m/s2
  float roll_z;       // z เป็นแกนแนวดิ่งชี้ขึ้น
  float pitch_z;
  float roll_y;       // y เป็นแกนแนวดิ่ง
  float pitch_y;
  float roll_x;
  float pitch_x;
} acceleration_t;

typedef void (*BMA423_StepCounterCB_t)(int step_counter);
typedef void (*BMA423_DirectionCB_t)(direction_t direction);
typedef void (*BMA423_AccelerationCB_t)(acceleration_t accel);
typedef void (*BMA423_DoubleClickCB_t)(void);
typedef void (*BMA423_TiltCB_t)(void);
typedef void (*BMA423_Activity_t)(activity_t activity);
typedef void (*BMA423_AnyNoMotionCB_t)(void);


class BlynkGO_BMA423 {
  public:
    BlynkGO_BMA423() {}

    bool init(TwoWire &wire, uint8_t bma423_interrupt_pin);
    inline bool    isInited()          { return this->_is_inited; }


    bool          enableStepCounter(bool enable);
    bool          enableDirection(bool enable);
    bool          enableAcceleration(bool enable);


    acceleration_t getAcceleration();


    void          resetStepCounter();
    

    void          onStepCounter(BMA423_StepCounterCB_t fn_step_cb);
    void          onDirection(BMA423_DirectionCB_t fn_direction_cb);
    void          onAcceleration(BMA423_AccelerationCB_t fn_accel_cb);
    void          onDoubleClick(BMA423_DoubleClickCB_t fn_doubleclick_cb);
    void          onTilt(BMA423_TiltCB_t fn_tilt_cb);
    void          onActivity(BMA423_Activity_t fn_activity_cb);
    void          onAnyNoMotion(BMA423_AnyNoMotionCB_t  fn_anynomotion_cb);


    //-------------------------------------------------

    BMA423_StepCounterCB_t  fn_step_cb=NULL;
    BMA423_DirectionCB_t    fn_direction_cb=NULL;
    BMA423_AccelerationCB_t fn_accel_cb = NULL;
    BMA423_DoubleClickCB_t  fn_doubleclick_cb = NULL;
    BMA423_TiltCB_t         fn_tilt_cb = NULL;
    BMA423_Activity_t       fn_activity_cb = NULL;
    BMA423_AnyNoMotionCB_t  fn_anynomotion_cb = NULL;

    static uint16_t bma423_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *write_data, uint16_t len);
    static uint16_t bma423_read (uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_data , uint16_t len);

    struct bma4_dev bma423_device;
    struct bma4_accel_config accel_conf;
    
  private:
    bool _is_inited  = false;
    uint8_t bma423_int_pin;
  
};

extern BlynkGO_BMA423 BMA423;

#endif // __BLYNKGO_BMA423_H__
