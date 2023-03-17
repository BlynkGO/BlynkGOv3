# BlynkGOv3
 BlynkGOv3 for Arduino ESP32 core 2.0.x  (ESP32, ESP32S2, ESP32S3, ESP32C3)

## Version 3.0.0 beta5  
ให้ติดตั้งด้วย BlynGO SDK version3.0.5

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
