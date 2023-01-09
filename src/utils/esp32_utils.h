#ifndef __ESP32_UTILS_H___
#define __ESP32_UTILS_H___

#include <Arduino.h>
#include "sdkconfig.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef map_f
#define map_f(v, v1, v2, a1, a2 ) ({ (1.0*(a2-a1)*(v-v1)/(v2-v1) + a1); })
#endif


#ifndef ANGLE360
//สำหรับปรับมุม a ให้อยู่ภายใน 0<= a < 360 องศา  // สูตรนี้ยังไม่ถูกนัก ได้เฉพาะ ที่ a ติดลบไม่มาก และ เกิน 360 ไม่มาก
#define ANGLE360(a)         ({ esp32_mod_f(a,360); })

//สำหรับปรับมุม a ให้อยู่ภายใน -180 =< a < 180 องศา
#define ANGLE180(a)         ({ float b =ANGLE360(a); if(b > 180) b -=360; b; })

//สำหรับแปลง วินาที ให้เป็น องศา (พิกัดมุมตามระนาบ xy ทางคณิตศาสตร์ หมุนมุมทวนเข็ม)
#define ANGLE_SEC(s)        ({ float a ; a = ANGLE360(90 - esp32_mod_f(s,60)*6); a; })
#define ANGLE_MINUTE(m)     ANGLE_SEC(m)
#define ANGLE_HOUR(h)       ({ float a; a = ANGLE360(90 - esp32_mod_f(h,12)*30); a; })
#endif // ANGLE360

#ifndef YIELD
#define YIELD()             taskYIELD()
#endif

#ifndef NO_INTERRUPTS
#define NO_INTERRUPTS()     taskENTER_CRITICAL_ISR( &mux )
#endif

#ifndef INTERRUPTS
#define INTERRUPTS()        taskEXIT_CRITICAL_ISR( &mux )
#endif

#ifndef MILLIS 
#define MILLIS()            xTaskGetTickCount()
#endif

#ifndef DELAY
#define DELAY(a)            vTaskDelay((a)/portTICK_PERIOD_MS)
#endif

#ifndef VOID
#define VOID
#endif

#ifndef SETUP
#define SETUP()
#endif

#ifndef LOOP
#define LOOP()              for(;;)
#endif


long  esp32_mod(long a,long b);
float esp32_mod_f( float a, long b);
// uint16_t rainbow_map(int val, int val_min, int val_max); // get rainbow color



#ifdef __cplusplus
}
#endif


#endif //__ESP32_UTILS_H__
