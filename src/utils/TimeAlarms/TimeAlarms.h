//  TimeAlarms.h - Arduino Time alarms header for use with Time library

#ifndef TimeAlarms_h
#define TimeAlarms_h

/** Major version number (X.x.x) */
#define TIMEALARM_VERSION_MAJOR   1
/** Minor version number (x.X.x) */
#define TIMEALARM_VERSION_MINOR   1
/** Patch version number (x.x.X) */
#define TIMEALARM_VERSION_PATCH   0

#define TIMEALARM_VERSION_TEXT    (String(TIMEALARM_VERSION_MAJOR)+"."+String(TIMEALARM_VERSION_MINOR)+"."+String(TIMEALARM_VERSION_PATCH))

/**
 * Macro to convert IDF version number into an integer
 *
 * To be used in comparisons, such as TIMEALARM_VERSION >= TIMEALARM_VERSION_VAL(3, 0, 0)
 */
#define TIMEALARM_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current IDF version, as an integer
 *
 * To be used in comparisons, such as TIMEALARM_VERSION >= TIMEALARM_VERSION_VAL(3, 0, 0)
 */
#define TIMEALARM_VERSION  TIMEALARM_VERSION_VAL( TIMEALARM_VERSION_MAJOR, \
                                              TIMEALARM_VERSION_MINOR, \
                                              TIMEALARM_VERSION_PATCH)


#include <Arduino.h>
#include "TimeLib.h"

#if defined(__AVR__)
#define dtNBR_ALARMS 6   // max is 255
#else
#define dtNBR_ALARMS 12  // assume non-AVR has more memory
// #define dtNBR_ALARMS 20  // assume non-AVR has more memory
// #define dtNBR_ALARMS 140  // assume non-AVR has more memory
#endif

#define USE_SPECIALIST_METHODS  // define this for testing

typedef enum {
  dtMillisecond,
  dtSecond,
  dtMinute,
  dtHour,
  dtDay
} dtUnits_t;

typedef struct {
  uint8_t alarmType      :4 ;  // enumeration of daily/weekly (in future:
                               // biweekly/semimonthly/monthly/annual)
                               // note that the current API only supports daily
                               // or weekly alarm periods
  uint8_t isEnabled      :1 ;  // the timer is only actioned if isEnabled is true
  uint8_t isOneShot      :1 ;  // the timer will be de-allocated after trigger is processed
} AlarmMode_t;

// new time based alarms should be added just before dtLastAlarmType
typedef enum {
  dtNotAllocated,   /* Alarm ยังไม่มีการจองใช้งาน */
  dtTimer,          /* Alarm แบบ timer */
  dtExplicitAlarm,  /* Alarm ณ เวลาที่เจาะจง */
  dtDailyAlarm,     /* Alarm ทุกๆวัน */
  dtWeeklyAlarm,    /* Alarm ทุกสัปดาห์ */
  dtLastAlarmType
} dtAlarmPeriod_t ; // in future: dtBiweekly, dtMonthly, dtAnnual

// macro to return true if the given type is a time based alarm, false if timer or not allocated
#define dtIsAlarm(_type_)  (_type_ >= dtExplicitAlarm && _type_ < dtLastAlarmType)
#define dtUseAbsoluteValue(_type_)  (_type_ == dtTimer || _type_ == dtExplicitAlarm)

typedef uint8_t AlarmID_t;
typedef AlarmID_t AlarmId;  // Arduino friendly name

#define dtINVALID_ALARM_ID 255
#define dtINVALID_TIME     (time_t)(-1)
#define AlarmHMS(_hr_, _min_, _sec_) (_hr_ * SECS_PER_HOUR + _min_ * SECS_PER_MIN + _sec_)
#define secs_of_day(_hr_, _min_, _sec_) (_hr_ * SECS_PER_HOUR + _min_ * SECS_PER_MIN + _sec_)%SECS_PER_DAY

typedef void (*OnTick_t)(AlarmID_t alarm_id, void* user_data);  // alarm callback function typedef

// class defining an alarm instance, only used by dtAlarmsClass
class AlarmClass
{
public:
  AlarmClass();
  OnTick_t onTickHandler;
  void updateNextTrigger();
  time_t value;
  time_t nextTrigger;
  AlarmMode_t Mode;
  //TridentTD
  void* user_data;
};

// class containing the collection of alarms
class TimeAlarmsClass
{
private:
  uint8_t isServicing;
  uint8_t servicedAlarmId; // the alarm currently being serviced
  AlarmID_t create(time_t value, OnTick_t onTickHandler, uint8_t isOneShot, dtAlarmPeriod_t alarmType, void* user_data=NULL);
public:
  AlarmClass Alarm[dtNBR_ALARMS];

  TimeAlarmsClass();

  /**
   * @brief ให้บริการการเตือนทั้งหมดที่กำหนด
   *
   * ฟังก์ชันนี้จะตรวจสอบและให้บริการการเตือนที่ถูกกำหนดทั้งหมดที่มีในระบบ
   * โดยจะทำการเรียกใช้งานการเตือนที่ถึงเวลาและดำเนินการตามที่กำหนดไว้.
   *
   * คำแนะนำ:
   * ควรวางฟังก์ชันนี้ใน loop() หรือฟังก์ชันที่ทำงานวนซ้ำ
   * เพื่อให้สามารถให้บริการการเตือนอย่างต่อเนื่อง.
   */
  void serviceAlarms();

  // TridentTD
  /**
   * @brief รีเฟรชการตั้งค่า Alarm ที่เคยตั้งไว้ทั้งหมด
   *
   * ฟังก์ชันนี้จะทำการรีเฟรชการตั้งค่า Alarm ที่เคยตั้งไว้ทั้งหมด
   * โดยจะคำนวน Alarm ครั้งถัดไปใหม่ ให้ถูกต้อง.
   *
   * คำแนะนำ:
   * ควรเรียกใช้ฟังก์ชันนี้เมื่อมีการตั้งเวลาด้วย setTime(...) ให้ระบบ
   * เพื่อให้ ระบบ Alarm มีการ refresh ตัวเองใหม่ให้ทำงานได้ถูกต้อง
   */
  void refreshAll();

  // functions to create alarms and timers

  /**
   * @brief ตั้งเตือนครั้งเดียวที่เวลาที่กำหนดในอนาคต
   *
   * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนครั้งเดียวที่จะเกิดขึ้นที่เวลาที่กำหนดในอนาคต
   *
   * @param value เวลา (ในหน่วยวินาทีตั้งแต่ epoch) ที่ต้องการให้การเตือนเกิดขึ้น
   * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
   * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
   */
  AlarmID_t triggerOnce(time_t value, OnTick_t onTickHandler, void* user_data=NULL) {
    if (value <= 0) return dtINVALID_ALARM_ID;
    return create(value, onTickHandler, true, dtExplicitAlarm, user_data);
  }

  /**
   * @brief ตั้งเตือนครั้งเดียวที่เวลาที่กำหนดในวัน
   *
   * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนครั้งเดียวที่จะเกิดขึ้นที่เวลาที่กำหนดในวัน
   *
   * @param value เวลาในหน่วยวินาทีจากจุดเริ่มต้นของวัน ที่ต้องการให้การเตือนเกิดขึ้น
   * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
   * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
   */
  AlarmID_t alarmOnce(time_t value, OnTick_t onTickHandler, void* user_data=NULL) {
    if (value <= 0 || value > SECS_PER_DAY) return dtINVALID_ALARM_ID;

    Serial.printf("[Alarm] set : %02d:%02d:%02d [alarmOnce]\n", hour(value), minute(value), second(value));
    return create(value, onTickHandler, true, dtDailyAlarm, user_data);
  }

  /**
   * @brief ตั้งเตือนครั้งเดียวที่เวลาที่กำหนดโดยชั่วโมง นาที และวินาที
   *
   * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนครั้งเดียวที่จะเกิดขึ้นที่เวลาที่กำหนดโดยชั่วโมง นาที และวินาที
   *
   * @param H ชั่วโมงในวัน (0-23) ที่ต้องการให้การเตือนเกิดขึ้น
   * @param M นาทีในชั่วโมง (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
   * @param S วินาทีในนาที (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
   * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
   * @return ID ของการเตือนที่ถูกสร้าง
   */
  AlarmID_t alarmOnce(const int H, const int M, const int S, OnTick_t onTickHandler, void* user_data=NULL) {
    return alarmOnce(AlarmHMS(H,M,S), onTickHandler, user_data);
  }

  /**
    * @brief ตั้งเตือนครั้งเดียวในวันและเวลาที่กำหนดในสัปดาห์
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนครั้งเดียวที่จะเกิดขึ้นในวันและเวลาที่กำหนดในสัปดาห์
    *
    * @param DOW วันในสัปดาห์ (1-7, โดย 1 คือวันอาทิตย์)
    * @param H ชั่วโมงในวัน (0-23) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param M นาทีในชั่วโมง (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param S วินาทีในนาที (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
    */
  AlarmID_t alarmOnce(const timeDayOfWeek_t DOW, const int H, const int M, const int S, OnTick_t onTickHandler, void* user_data=NULL) {
    time_t value = (DOW-1) * SECS_PER_DAY + AlarmHMS(H,M,S);
    if (value <= 0) return dtINVALID_ALARM_ID;
    return create(value, onTickHandler, true, dtWeeklyAlarm, user_data);
  }

  // TridentTD
  /**
    * @brief ตั้งเตือนครั้งเดียวในวันและเวลาที่กำหนดในสัปดาห์
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนครั้งเดียวที่จะเกิดขึ้นในวันและเวลาที่กำหนดในสัปดาห์
    *
    * @param DOW วันในสัปดาห์ (1-7, โดย 1 คือวันอาทิตย์)
    * @param sec_of_day วินาทีในวัน (0-86399) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
    */
  AlarmID_t alarmOnce(const timeDayOfWeek_t DOW, time_t sec_of_day, OnTick_t onTickHandler, void* user_data=NULL) {
    time_t value = (DOW-1) * SECS_PER_DAY + sec_of_day;
    if (value <= 0) return dtINVALID_ALARM_ID;
    Serial.printf("[Alarm] set : %s %02d:%02d:%02d [alarmOnce]\n", dayShortStr(DOW), hour(sec_of_day), minute(sec_of_day), second(sec_of_day));
    return create(value, onTickHandler, true, dtWeeklyAlarm, user_data);
  }

  // ตั้งเตือนทุกวันในเวลาที่กำหนด
  /**
    * @brief ตั้งเตือนทุกวันในเวลาที่กำหนดด้วย เวลาในหน่วยวินาที ภายในวัน
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นทุกวันในเวลาที่กำหนด
    *
    * @param value จำนวนวินาทีในวัน (0-86399) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
    */
  AlarmID_t alarmRepeat(time_t value, OnTick_t onTickHandler, void* user_data=NULL) {
    if (value > SECS_PER_DAY) return dtINVALID_ALARM_ID;
    Serial.printf("[Alarm] set : EveryDay %02d:%02d:%02d [alarmRepeat]\n",  hour(value), minute(value), second(value));    
    return create(value, onTickHandler, false, dtDailyAlarm, user_data);
  }

  /**
    * @brief ตั้งเตือนทุกวันในเวลาที่กำหนด ด้วย ชั่วโมง นาที และวินาที ภายในวัน
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นทุกวันในเวลาที่กำหนดด้วย ชั่วโมง นาที และวินาที ภายในวัน
    *
    * @param H ชั่วโมงในวัน (0-23) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param M นาทีในชั่วโมง (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param S วินาทีในนาที (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง โดยเรียกใช้ฟังก์ชัน alarmRepeat
    */
  AlarmID_t alarmRepeat(const int H, const int M, const int S, OnTick_t onTickHandler, void* user_data=NULL) {
    return alarmRepeat(AlarmHMS(H,M,S), onTickHandler, user_data);
  }

  // ตั้งเตือนทุกสัปดาห์ในวันในสัปดาห์และเวลาที่กำหนด
  /**
    * @brief ตั้งเตือนทุกสัปดาห์ในวันในสัปดาห์และเวลาที่กำหนดด้วย ชั่วโมง นาที และวินาที ภายในวัน
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นทุกสัปดาห์ในวันและเวลาที่กำหนดด้วย ชั่วโมง นาที และวินาที ภายในวัน
    *
    * @param DOW วันในสัปดาห์ (1-7, โดย 1 คือวันอาทิตย์)
    * @param H ชั่วโมงในวัน (0-23) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param M นาทีในชั่วโมง (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param S วินาทีในนาที (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
    */
  AlarmID_t alarmRepeat(const timeDayOfWeek_t DOW, const int H, const int M, const int S, OnTick_t onTickHandler, void* user_data=NULL) {
    time_t value = (DOW-1) * SECS_PER_DAY + AlarmHMS(H,M,S);
    if (value <= 0) return dtINVALID_ALARM_ID;
    return create(value, onTickHandler, false, dtWeeklyAlarm, user_data);
  }

  // TridentTD
  /**
    * @brief ตั้งเตือนทุกสัปดาห์ในวันในสัปดาห์และเวลาที่กำหนดในหน่วยวินาที ภายในวัน
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นทุกสัปดาห์ในวันและเวลาที่กำหนด
    *
    * @param DOW วันในสัปดาห์ (1-7, โดย 1 คือวันอาทิตย์)
    * @param sec_of_day วินาทีในวัน (0-86399) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
    */
  AlarmID_t alarmRepeat(const timeDayOfWeek_t DOW, time_t sec_of_day, OnTick_t onTickHandler, void* user_data=NULL) {
    time_t value = (DOW-1) * SECS_PER_DAY + sec_of_day;
    if (value <= 0) return dtINVALID_ALARM_ID;
    Serial.printf("[Alarm] set : %s %02d:%02d:%02d [alarmRepeat]\n", dayShortStr(DOW-1), hour(sec_of_day), minute(sec_of_day), second(sec_of_day));
    return create(value, onTickHandler, false, dtWeeklyAlarm, user_data);
  }

  /**
    * @brief ตั้งเตือนครั้งเดียวหลังจาก กี่วินาทีถัดไป
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นครั้งเดียวหลังจากเวลากี่วินาทีถัดไป
    *
    * @param value จำนวนวินาทีหลังจากการเรียกฟังก์ชันที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
    */
  AlarmID_t timerOnce(time_t value, OnTick_t onTickHandler, void* user_data=NULL) {
    if (value <= 0) return dtINVALID_ALARM_ID;
    Serial.printf("[Alarm] set : ทำงานในอีก %d วัน %02d ชม. %02d นาที %02d วินาที ถัดไป [timerOnce] [%d]\n", day(value)-1, hour(value), minute(value), second(value), value);
    return create(value, onTickHandler, true, dtTimer, user_data);
  }

  /**
    * @brief ตั้งเตือนครั้งเดียวหลังจากเวลา กี่ชั่วโมง กี่นาที กี่วินาที ถัดไป
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นครั้งเดียวหลังจากเวลา กี่ชั่วโมง กี่นาที กี่วินาที ถัดไป
    *
    * @param H ชั่วโมงในวัน (0-23) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param M นาทีในชั่วโมง (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param S วินาทีในนาที (0-59) ที่ต้องการให้การเตือนเกิดขึ้น
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง โดยเรียกใช้ฟังก์ชัน timerOnce
    */
  AlarmID_t timerOnce(const int H, const int M, const int S, OnTick_t onTickHandler, void* user_data=NULL) {
    return timerOnce(AlarmHMS(H,M,S), onTickHandler, user_data);
  }

  // ตั้งเตือนที่ระยะเวลาปกติ
  /**
    * @brief ตั้งเตือนที่ระยะเวลาปกติ
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นซ้ำที่ระยะเวลาที่กำหนด
    *
    * @param value จำนวนวินาทีระหว่างการเตือนแต่ละครั้ง
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง หรือ dtINVALID_ALARM_ID ถ้าไม่สามารถสร้างการเตือนได้
    */
  AlarmID_t timerRepeat(time_t value, OnTick_t onTickHandler, void* user_data=NULL) {
    if (value <= 0) return dtINVALID_ALARM_ID;
    Serial.printf("[Alarm] set : ทำงานซ้ำ ทุกๆ %d วินาที [timerRepeat]\n", value);
    return create(value, onTickHandler, false, dtTimer, user_data);
  }

  /**
    * @brief ตั้งเตือนที่ระยะเวลาปกติจากชั่วโมง นาที และวินาที
    *
    * ฟังก์ชันนี้ใช้เพื่อตั้งการเตือนที่จะเกิดขึ้นซ้ำที่ระยะเวลาที่กำหนดจากชั่วโมง นาที และวินาที
    *
    * @param H ชั่วโมงในวัน (0-23) ระยะเวลาการเตือน
    * @param M นาทีในชั่วโมง (0-59) ระยะเวลาการเตือน
    * @param S วินาทีในนาที (0-59) ระยะเวลาการเตือน
    * @param onTickHandler ฟังก์ชัน callback ที่จะถูกเรียกเมื่อการเตือนเกิดขึ้น
    * @return ID ของการเตือนที่ถูกสร้าง โดยเรียกใช้ฟังก์ชัน timerRepeat
    */
  AlarmID_t timerRepeat(const int H,  const int M,  const int S, OnTick_t onTickHandler, void* user_data=NULL) {
    return timerRepeat(AlarmHMS(H,M,S), onTickHandler, user_data);
  }

  void delay(unsigned long ms);

  // utility methods

  /**
   * @brief รับค่าหมายเลขปัจจุบันสำหรับหน่วยเวลาที่กำหนด
   *
   * ฟังก์ชันนี้ใช้เพื่อดึงค่าหมายเลขปัจจุบันของหน่วยเวลาที่กำหนด เช่น ชั่วโมง นาที หรือวินาที
   *
   * @param Units หน่วยเวลาที่ต้องการดึงค่าหมายเลขปัจจุบัน (เช่น ชั่วโมง นาที หรือวินาที)
   * @return ค่าหมายเลขปัจจุบันสำหรับหน่วยเวลาที่กำหนด
   */
  uint8_t getDigitsNow( dtUnits_t Units);

  /**
   * @brief รอจนกว่าจะมีการเปลี่ยนแปลงค่าหมายเลขที่กำหนด
   *
   * ฟังก์ชันนี้ใช้เพื่อรอจนกว่าค่าหมายเลขของหน่วยเวลาที่กำหนดจะตรงกับค่าที่ระบุ
   *
   * @param Digits ค่าหมายเลขที่ต้องการให้เกิดการเปลี่ยนแปลง
   * @param Units หน่วยเวลาที่ต้องการตรวจสอบ (เช่น ชั่วโมง นาที หรือวินาที)
   */
  void waitForDigits( uint8_t Digits, dtUnits_t Units);

  /**
   * @brief รอจนกว่าจะมีการเปลี่ยนแปลงของหน่วยเวลาที่กำหนด
   *
   * ฟังก์ชันนี้ใช้เพื่อรอจนกว่าหน่วยเวลาที่กำหนดจะเกิดการเปลี่ยนแปลง (เช่น การหมุนเวียนของนาทีหรือชั่วโมง)
   *
   * @param Units หน่วยเวลาที่ต้องการตรวจสอบ (เช่น ชั่วโมง นาที หรือวินาที)
   */
  void waitForRollover(dtUnits_t Units);

  // low level methods

  /**
   * @brief เปิดใช้งานการเตือนเพื่อให้เกิดการกระตุ้น
   *
   * ฟังก์ชันนี้ใช้เพื่อเปิดใช้งานการเตือนที่มี ID ที่กำหนด
   *
   * @param ID ID ของการเตือนที่ต้องการเปิดใช้งาน
   */
  void enable(AlarmID_t ID);

  /**
   * @brief ตรวจสอบว่าการเตือนถูกเปิดใช้งานหรือไม่
   *
   * ฟังก์ชันนี้จะตรวจสอบว่าการเตือนที่มี ID ที่กำหนดนั้นถูกเปิดใช้งานหรือไม่
   *
   * @param ID ID ของการเตือนที่ต้องการตรวจสอบ
   * @return true ถ้าการเตือนนั้นถูกเปิดใช้งาน
   * @return false ถ้าการเตือนนั้นยังไม่ถูกเปิดใช้งาน
   */
  bool is_enable(AlarmID_t ID);

  /**
   * @brief ป้องกันไม่ให้การเตือนเกิดการกระตุ้น
   *
   * ฟังก์ชันนี้ใช้เพื่อป้องกันไม่ให้การเตือนที่มี ID ที่กำหนดเกิดการกระตุ้น
   *
   * @param ID ID ของการเตือนที่ต้องการป้องกันไม่ให้กระตุ้น
   */      
  void disable(AlarmID_t ID); 

  /**
   * @brief คืนค่า ID ของการเตือนที่เกิดการกระตุ้นอยู่ในขณะนี้
   *
   * ฟังก์ชันนี้ใช้เพื่อดึง ID ของการเตือนที่เกิดการกระตุ้นอยู่ในขณะนี้
   *
   * @return ID ของการเตือนที่เกิดการกระตุ้นอยู่ในขณะนี้
   */          
  AlarmID_t getTriggeredAlarmId();          // returns the currently triggered  alarm id

  /**
   * @brief คืนค่าความเป็นจริงว่าอยู่ในสถานะการบริการหรือไม่
   *
   * ฟังก์ชันนี้ใช้เพื่อดึงค่าความเป็นจริงว่าในขณะนี้อยู่ในสถานะการบริการหรือไม่
   *
   * @return true ถ้าอยู่ในสถานะการบริการ, false ถ้าไม่
   */
  bool getIsServicing();                    // returns isServicing

  /**
   * @brief เขียนค่าและเปิดใช้งานการเตือนที่มี ID ที่กำหนด
   *
   * ฟังก์ชันนี้ใช้เพื่อเขียนค่าลงในการเตือนที่มี ID ที่กำหนดและเปิดใช้งานการเตือน
   *
   * @param ID ID ของการเตือนที่ต้องการเขียนค่าและเปิดใช้งาน
   * @param value ค่าที่ต้องการเขียนลงในการเตือน
   */
  void write(AlarmID_t ID, time_t value);   // write the value (and enable) the alarm with the given ID

  /**
   * @brief คืนค่าคาสำหรับการเตือนที่มี ID ที่กำหนด
   *
   * ฟังก์ชันนี้ใช้เพื่อดึงค่าของการเตือนที่มี ID ที่กำหนด
   *
   * @param ID ID ของการเตือนที่ต้องการดึงค่า
   * @return ค่าของการเตือนที่มี ID ที่กำหนด
   */
  time_t read(AlarmID_t ID);                // return the value for the given timer

  /**
   * @brief คืนค่าประเภทของการเตือนสำหรับ ID ที่กำหนด
   *
   * ฟังก์ชันนี้ใช้เพื่อดึงประเภทของการเตือนที่มี ID ที่กำหนด
   *
   * @param ID ID ของการเตือนที่ต้องการดึงประเภท
   * @return ประเภทของการเตือนที่มี ID ที่กำหนด
   */
  dtAlarmPeriod_t readType(AlarmID_t ID);   // return the alarm type for the given alarm ID

  /**
   * @brief ปลดปล่อย ID เพื่อให้สามารถใช้งานใหม่ได้
   *
   * ฟังก์ชันนี้ใช้เพื่อปลดปล่อย ID ของการเตือนที่กำหนดให้สามารถใช้งานใหม่ได้
   *
   * @param ID ID ของการเตือนที่ต้องการปลดปล่อย
   */
  void free(AlarmID_t ID);                  // free the id to allow its reuse

  /**
   * @brief ปลดปล่อยการเตือนทั้งหมดที่กำหนดไว้
   *
   * ฟังก์ชันนี้ใช้เพื่อปลดปล่อยการเตือนทั้งหมดที่ถูกตั้งค่าไว้ เพื่อให้ ID ทั้งหมดสามารถใช้งานใหม่ได้
   */
  void clear();

  /**
   * @brief รับข้อมูล user_data ที่ผูกไว้กับ AlarmID ที่กำหนด
   *
   * ฟังก์ชันนี้จะส่งคืนข้อมูล user_data ที่ผูกไว้กับ AlarmID ที่กำหนด
   *
   * @param ID AlarmID ของการเตือนที่ต้องการรับข้อมูลผู้ใช้
   * @return void* ตัวชี้ไปยังข้อมูลผู้ใช้ที่เชื่อมโยงกับการเตือนนั้น ถ้าไม่มีข้อมูลผู้ใช้ จะส่งคืน nullptr
   */
  void* user_data(AlarmID_t ID);

  /**
   * @brief กำหนดข้อมูล user_data สำหรับ AlarmID ที่กำหนด
   *
   * ฟังก์ชันนี้ใช้ในการผูกข้อมูลผู้ใช้ (user_data) กับการเตือนที่มี ID ที่กำหนด
   *
   * @param ID AlarmID ของการเตือนที่ต้องการกำหนดข้อมูลผู้ใช้
   * @param user_data ข้อมูลผู้ใช้ที่ต้องการผูกกับการเตือนนี้
   */
  void user_data(AlarmID_t ID, void* user_data);


#ifndef USE_SPECIALIST_METHODS
private:  // the following methods are for testing and are not documented as part of the standard library
#endif

  /**
   * @brief คืนค่าจำนวนของตัวจับเวลา (timers) ที่ถูกจัดสรร
   *
   * ฟังก์ชันนี้ใช้เพื่อดึงจำนวนของตัวจับเวลาที่ถูกจัดสรรอยู่ในขณะนี้
   *
   * @return จำนวนของตัวจับเวลาที่ถูกจัดสรร
   */
  uint8_t count();                          // returns the number of allocated timers

  /**
   * @brief คืนค่าช่วงเวลาของการเตือนถัดไปที่กำหนดไว้
   *
   * ฟังก์ชันนี้ใช้เพื่อดึงช่วงเวลาของการเตือนถัดไปที่ถูกกำหนดไว้ในระบบ
   *
   * @return ช่วงเวลาของการเตือนถัดไป
   */
  time_t getNextTrigger();                  // returns the time of the next scheduled alarm

 /**
   * @brief คืนค่าความเป็นจริงว่ามีการจองการใช้งาน Alarm ID นี้อยู่หรือไม่
   *
   * ฟังก์ชันนี้ใช้เพื่อตรวจสอบว่า Alarm ID ของการเตือนที่กำหนดถูกจองการใช้งานอยู่หรือไม่
   *
   * @param ID ID ของการเตือนที่ต้องการตรวจสอบ
   * @return true ถ้า ID นี้ถูกจัดสรร, false ถ้าไม่
   */
  bool isAllocated(AlarmID_t ID); 

  /**
   * @brief คืนค่าความเป็นจริงว่า ID นี้เป็น ID ของการเตือนตามเวลาหรือไม่
   *
   * ฟังก์ชันนี้ใช้เพื่อตรวจสอบว่า ID ของการเตือนที่กำหนดเป็น ID ของการเตือนตามเวลาหรือไม่
   *
   * @param ID ID ของการเตือนที่ต้องการตรวจสอบ
   * @return true ถ้า ID นี้เป็น ID ของการเตือนตามเวลา, false ถ้าเป็นตัวจับเวลา (timer) หรือไม่ถูกจัดสรร
   */
  bool isAlarm(AlarmID_t ID);               // returns true if id is for a time based alarm, false if its a timer or not allocated
};

extern TimeAlarmsClass Alarm;  // make an instance for the user

/*==============================================================================
 * MACROS
 *============================================================================*/

/* public */
#define waitUntilThisSecond(_val_) waitForDigits( _val_, dtSecond)
#define waitUntilThisMinute(_val_) waitForDigits( _val_, dtMinute)
#define waitUntilThisHour(_val_)   waitForDigits( _val_, dtHour)
#define waitUntilThisDay(_val_)    waitForDigits( _val_, dtDay)
#define waitMinuteRollover() waitForRollover(dtSecond)
#define waitHourRollover()   waitForRollover(dtMinute)
#define waitDayRollover()    waitForRollover(dtHour)


#endif /* TimeAlarms_h */
