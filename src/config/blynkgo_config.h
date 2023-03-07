#ifndef __BLYNKGO_CONFIG_H__
#define __BLYNKGO_CONFIG_H__

/*********************************************************
 *
 *  User Config
 *
 * ---------------------------------
 *
 *  Compile Options
 *     เปิด/ปิด Options ในการ compile ร่วมกับ BlynkGO
 *
 *********************************************************/

//--------------------------------------------------------
//     หากไม่ได้ใช้ ให้ ปิด option นั้นออก เพื่อการ compile
//     ที่เร็วขึ้น และ ลดขนาด compile ลง
//--------------------------------------------------------

// เปิด/ปิด การใช้งาน WiFi
#define BLYNKGO_USE_WIFI              0       // 1 เลือกใช้ WiFi ด้วย                ; 0 ไม่เลือกใช้ WiFi ; 

// เปิด/ปิด การซิงค์เวลา NTP อัตโนมัติหรือไม่
#define BLYNKGO_USE_NTP               0       // 1 เลือกใช้ NTP Sync เวลาอัตโนมัติ ด้วย  ; 0 ไม่เลือกใช้ NTP ;   ( ถ้าใช้ NTP ให้เปิด WIFI ด้วย)

// เปิด/ปิด การใช้งาน Blynk หรือไม่ (ให้ติดตั้ง BlynkBlynkGO ก่อน)
#define BLYNKGO_USE_BLYNK             0       // 1 เลือกใช้ Blynk ด้วย               ; 0 ไม่เลือกใช้ Blynk ;  ( ถ้าใช้ Blynk ให้เปิด WIFI ด้วย)

// // เปิด/ปิด การใช้งาน Line Notify แบบ Async (ไม่สะดุดรอการเชื่อมต่อ) หรือไม่
// #define BLYNKGO_USE_LINENOTIFY        0       // 1 เลือกใช้ Line Notify ด้วย         ; 0 ไม่เลือกใช้ Line Notify ;  ( ถ้าใช้ Line Notify ให้เปิด WIFI ด้วย)

// เปิด/ปิด การใช้งาน MQTT แบบ Async (ไม่สะดุดรอการเชื่อมต่อ) หรือไม่
// สามารถเชื่อมต่อได้ทั้งแบบ TCP, SSL/TLS, WebSocket, WebSocket Secure และมี user/password ได้
#define BLYNKO_USE_ASYNC_MQTT32       0       // 1 เลือกใช้ MQTT ด้วย                ; 0 ไม่เลือกใช้ MQTT ;  ( ถ้าใช้ MQTT ให้เปิด WIFI ด้วย)

// เปิด/ปิด การใช้งาน BeeNeXT ร่วมด้วย
#define BLYNKGO_USE_BEENEXT           0

// เปิด/ปิด การใช้งาน SoftwareSerial
#define BLYNKGO_USE_SOFTWARESERIAL    0

// // เปิด/ปิด การใช้งาน QR Payment โดยผ่านสื่อกลาง Global PrimePay  (GB PrimePay)
// #define BLYNKGO_USE_QRPAYMENT         0      // 1 เลือกใช้ QR Payment ด้วย          ; 0 ไม่เลือกใช้ QR Payment ;   (ถ้าใช้ QR Payment  ให้เปิด WiFi, NTP, MQTT ด้วย )

// เปิด/ปิด การใช้งาน LORA ด้วยหรือไม่
#define BLYNKGO_USE_LORA              0       // 1 เลือกใช้ LORA ด้วย                ; 0 ไม่เลือกใช้ LORA ;

// // เปิด/ปิด การใช้งาน GDatabase หรือไม่
// #define BLYNKGO_USE_GDATABASE         0       // 1 เลือกใช้ GDatabase ด้วย           ; 0 ไม่เลือกใช้ GDatabase ; 

// // BlynkGO Board 1.2 + DAC Module , TTGO T-Watch2020 v1, v3
// #define BLYNKGO_USE_AUDIO             0       // 1 เลือกใช้ Audio  ด้วย              ; 0 ไม่เลือกใช้ Audio ;   สำหรับ   TTGO T-Watch2020 v1 และ v3

// // TTGO T-Watch2020 v3
// #define BLYNKGO_USE_MIC               0       // 1 เลือกใช้ Mic    ด้วย              ; 0 ไม่เลือกใช้ Mic ;     สำหรับ   TTGO T-Watch2020 v3

// เปิด/ปิด การใช้งาน ButtonISR (Button Interrupt) สำหรับ GPIO-0 (ปุ่ม EN)
#define BLYNKGO_USE_BUTTON_ISR        0       // 1 เลือกใช้ ButtonISR ด้วย GPIO-0 ด้วย; 0 ไม่เลือกใช้ ButtonISR GPIO-0 ; 

// เปิด/ปิด การใช้งาน GPS
#define BLYNKGO_USE_GPS               0       // 1 เลือกใช้ GPS ด้วย                 ; 0 ไม่เลือกใช้  GPS

// เปิด/ปิด การใช้งาน RTC DS3231 , DS3232
#define BLYNKGO_USE_RTC_DS323X        0       // 1 เลือกใช้ RTC DS323X ด้วย          ; 0 ไม่เลือกใช้  RTC DS323X

// เปิด/ปิด การใช้งาน RTC DS1307
#define BLYNKGO_USE_RTC_DS1307        0       // 1 เลือกใช้ RTC DS1307 ด้วย          ; 0 ไม่เลือกใช้  RTC DS1307

// เปิด/ปิด การใช้งาน RTC PCF8523
#define BLYNKGO_USE_RTC_PCF8523       0       // 1 เลือกใช้ RTC PCF8523 ด้วย         ; 0 ไม่เลือกใช้  RTC PCF8523

// เปิด/ปิด การใช้งาน RTC PCF8563
#define BLYNKGO_USE_RTC_PCF8563       0       // 1 เลือกใช้ RTC PCF8563 ด้วย         ; 0 ไม่เลือกใช้  RTC PCF8563

// แบบกำหนดขา RTC I2C ร่วมกับ Touch I2C โดยใช้ Wire1 ร่วม Touch I2C  (BlynkGOv3 จะใช้ I2C Port1 (Wire1) เป็น ตัวทำสัมผัส )
// BlynkGO Board v1.1-1.3 ต่อ DS3231 ที่ขา 26 (I2C SDA),27 (I2C SCL)  ---> Wire1 แบบใช้ I2C ร่วม กับ Touch I2C
// BlynkGO Emma-II        ต่อ DS3231 ที่ขา 18 (I2C SDA),19 (I2C SCL)  ---> Wire1 แบบใช้ I2C ร่วม กับ Touch I2C

// แบบกำหนดขา RTC I2C แยกจาก Touch I2C โดยใช้ Wire แยกต่างหาก
// #define RTC_I2C_SDA               25      // RTC I2C SDA        ---> Wire แบบกำหนดขาเอง ใช้ I2C แยกจาก Touch I2C
// #define RTC_I2C_SCL               4       // RTC I2C SCL        ---> Wire แบบกำหนดขาเอง ใช้ I2C แยกจาก Touch I2C


// เปิด/ปิด การใช้ DEBUG
#define BLYNKGO_DEBUG                 0       // 1 เลือกใช้ Debug ด้วย               ; 0 ไม่เลือกใช้ Debug ; 

// สำหรับปรับค่า offset ของการหมุนหน้าจอ เริ่มต้น 
// สำหรับ จอ TFT ขนาด 3.2 นิ้ว บางจอ ที่อาจมีปัญหาระบบสัมผัสสลับ
// หากมีปัญหาระบบสัมผัสสลับกันด้าน ให้ทดสอบด้วย 0 หรือ 2
// For TFT 3.2 Inch
// sometime rotation is inverted direction problems. Try value  0 or 2.
// #define TFT_ROTATION_OFFSET       0  // ใส่ 0 หรือ 2

// สำหรับปรับรอบในการอ่านค่าสัมผัส TOUCH หากไวไปให้ปรับให้มากขึ้น
// #define TOUCH_PERIOD              100     // ค่า default = 30ms

// เปิด/ปิด โหมดให้ NTP ซิงค์เวลาเป็นระยะๆไหม
// #define BLYNKGO_USE_NTP_RESYNC		0				//  1 เลือกใช้ NTP ReSync เวลาเป็นระยะด้วย  ; 0 ไม่เลือกให้ NTP ReSync เป็นระยะๆ ;   ( ถ้าใช้ ให้เปิด WIFI และ NTP ด้วย)


/*********************************************************
 *
 * เลือกภาษาอย่างใดอย่างหนึ่ง 
 * Language Option
 *
 *  *****   Select ONLY ONE Language *****
 *********************************************************/
// #define ENG_TEXT
#define THAI_TEXT
// #define LAOS_TEXT			  // For Laos Language    ; this option can use font : saysettha_15 and saysettha_25 by default
// #define VIETNAM_TEXT			// For Vietnam Language ; this option can use font : arial_vietnam_20 and arial_vietnam_30 by default


#include "blynkgo_system_config.h"
#endif //__BLYNKGO_CONFIG_H__

