# BlynkGOv3
 BlynkGOv3 for Arduino ESP32 core 2.0.x  (ESP32, ESP32S2, ESP32S3, ESP32C3)

## [Version 3.0.0 beta11](https://github.com/BlynkGO/BlynkGOv3/tree/0d545130d2c1f7a6008611e8be602a42d16ffea4)  
ให้ติดตั้งด้วย BlynkGO SDK version3.0.11 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  
 - support BeeNeXT4.3C , BeeNeXT4.3IPS
 - เพิ่มคำสั่ง BlynkGO.setTimestamp(time_t t) สำหรับกำหนด timestamp ปัจจุบันให้ BlynkGO

## [Version 3.0.0 beta10]
ให้ติดตั้งด้วย BlynkGO SDK version3.0.10 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  
 - support BeeNeXT3.2C HMI Uart IPS  
 - support BeeNeXT7.0 HMI uart ให้สามารถทำงานสัมผัสไปได้ด้วย
 - เพิ่มคำสั่ง BlynkGO.centerpoint() จุดศูนย์กลางจอ 

## Version 3.0.0 beta9 
ให้ติดตั้งด้วย BlynkGO SDK version3.0.9 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  
 - fix BeeNeXT3.5C SDK 3.0.9  
 - fix BeeNeXT7.0IPS ให้ทำงานเฉพาะจออย่างเดียวไปก่อน สัมผัสยังไม่ได้ fix  
 - fix backlight ของ BlynkGO Emma-II ให้ on off ได้ถูกต้อง  
  
## Version 3.0.0 beta8  
ให้ติดตั้งด้วย BlynkGO SDK version3.0.8 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  
 - รองรับบอร์ด BeeNeXT 1.9" IPS (ESP32S3)
 - เพิ่ม Backlight PWM สำหรับ BeeNeXT 1.9",4.3",5.0"

## Version 3.0.0 beta7  
ให้ติดตั้งด้วย BlynkGO SDK version3.0.7 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  
 - RTC ทำงานร่วมกับ Touch I2C ได้ (Wire1 ร่วม) หรือจะแยก ด้วย Wire ได้  
 - fix img_decoder และ เพิ่ม jpg_data_to_img_dsc(..)  สำหรับแปลง jpg data -> img_t 
 
## Version 3.0.0 beta6  
ให้ติดตั้งด้วย BlynkGO SDK version3.0.6 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  

 - เพิ่ม GCanvas และมีเพิ่ม GCanvas สามารถ drawQRCode(..) ได้ด้วย
 - GCanvas มี API คำสั่ง getThermalPrinterBuffer(..) สำหรับสร้าง buffer ออก thermal printer ได้
 - เพิ่ม agfx core ด้วย
 - GObject เพิ่ม child_num_recursive() และ child_del_recursive() และ ระบบ widget_del()  
   ยังไม่ได้มีการปรับ del() ของแต่ละวิตเจ็ต

## Version 3.0.0 beta5  
ให้ติดตั้งด้วย BlynkGO SDK version3.0.5 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  

- ปรับส่วน driver lcd ให้แยกออกมาเป็น precompiled เพื่อให้ลดเวลา compile ส่วนนี้ลง
- additional libraries ปรับเป็น precompiled  
- BeeNeXT เป็น version precompiled และสามารถมี BEENEXT_CONNECTED(), BEENEXT_DISCONNECTED() ...

## Version 3.0.0 beta4  
ให้ติดตั้งด้วย BlynkGO SDK version3.0.4 FIX010

- รองรับ AsyncMQTT32 ที่สามารถเชื่อมต่อได้ครบ 4 แบบ  
คือ TCP, TCP Secure (SSL/TLS), WebSocket, WebSocket Secure  
พร้อมใส่ user/password ได้  
- รองรับ แสดงเวลาจาก RTC timestamp ที่ต่อบน MCU อื่น   
- เพิ่มคำสั่ง BlynkGO.autoNTP(true/false); ให้ compatable กับ BlynkGOv2  
- รองรับบอร์ด BlynkGO Enterprise 5" (old)
- ย้ายไลฯย่อย BeeNeXT ออกไปต่างหาก และเพิ่มตัวอย่าง NodeMCU <--> BeeNeXT3.5
  
## Version 3.0.0 beta3  
ให้ติดตั้งด้วย BlynkGO SDK version3.0.4 FIX001

- รองรับบอร์ด BlynkGO Board 3.5" 
- รองรับบอร์ด BlynkGO Emma-II 3.5"
- รองรับบอร์ด BlynkGO Enterprise 5" (jst)
- รองรับบอร์ด BlynkGO Enterprise 7" (่jst/old)
- รองรับบอร์ด BeeNeXT 2.8 Resistive/Capacitive
- รองรับบอร์ด BeeNeXT 3.5 Resistive/Capacitive
- รองรับบอร์ด BeeNeXT 4.3 Resistive/Capacitive/IPS
- รองรับบอร์ด BeeNeXT 5.0 IPS
- รองรับบอร์ด BeeNeXT 7.0 IPS

แสดงส่วนเฉพาะกราฟิก & วิตเจ็ต เป็นหลัก  
ส่วน component ต่างๆ ยังอยู่ระหว่าง upgrade
