# BlynkGOv3
 BlynkGOv3 for Arduino ESP32 core 2.0.x  (ESP32, ESP32S2, ESP32S3, ESP32C3)  
 ( **ไลบรารี่ลิขสิทธิ์** สำหรับ ผลิตภัณฑ์สินค้า ของทาง BlynkGO)  
  
## [Version 3.1.9] @26/03/68   
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.3** (ติดต่อ Admin)    
 - เอา BeeNeXT.hearbeat(..) ออกจาก BlynkGO ตาม ไล BeeNeXT3.1.9 ล่าสุด 
  
## [Version 3.1.8] @24/03/68   
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.3** (ติดต่อ Admin)     
 - ปรับปรุง BlynkGO_AsyncMQTT32 เป็น v1.0.14  
    - ปรับ destroy() ที่มาจาก _stop + _destroy() ให้เช็คว่าเคย connected อยู่ ถึงค่อยมี event disconnected() แค่ครั้งเดียวออกไป  
    - client_id เปลี่ยนทุกครั้งที่ _init ใหม่  (ยกเว้น หากมีการ กำหนด client_id เองเช่นใน netpie)  
    - reconnect 5 รอบแล้วไม่ได้ ให้ _stop + _destroy แล้ว 10วิ ค่อย _init ใหม่ + _start  
    - reconnect 5 รอบ ใช้ของภายใน แล้ว บางทีกลับเชื่อมไม่ได้อีก อาจเป็นเพราะ client_id เดิมบน server ค้างอยู่  
      ทำการเปลี่ยนใหม่ ให้ _stop + _destroy ทิ้ง แล้ว  1 วินาทีค่อย _init client_id ใหม่ + _start แทน reconnect ภายใน  
          E (259013) MQTT_CLIENT: mqtt_message_receive: transport_read() error: errno=128  
          E (259014) MQTT_CLIENT: mqtt_process_receive: mqtt_message_receive() returned -1  
      ค่อยกลับมาเชื่อมต่อใหม่ได้!!  
    - WiFi ไม่หลุด Net หลุด Net มาสักพัก  
      หรือ WiFi หลุด แล้ว WiFi มา  
          E (998499) TRANSPORT_BASE: poll_read select error 104, errno = Connection reset by peer, fd = 48  
          E (998500) MQTT_CLIENT: Poll read error: 119, aborting connection  
      แล้วกลับมาเชื่อมต่อใหม่ได้!!  
    - WiFi & Net หลุด แล้ว WiFi มา แต่ Net ไม่มา  
          E (1144206) esp-tls: [sock=48] select() timeout  
          E (1144207) TRANSPORT_BASE: Failed to open a new connection: 32774  
          E (1144207) MQTT_CLIENT: Error transport connect  
      จะเข้าสู่ reconnect 5รอบ แล้ว หลุด 10 วิ ไปเรื่อยๆ  
      เมื่อ Net มา ก็กลับมาเชื่อมต่อได้!!  
   - subscribe มี ปิด ไม่ให้ save ลง subscribe topic list แต่เป็น direct subscribe ได้ (สำหรับใช้ สั่ง subscribe เองใน MQTT_CONNECTED)  
   
## [Version 3.1.7] @22/03/68   
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.3** (ติดต่อ Admin)     
 - ปรับปรุง BlynkGO_AsyncMQTT32 เป็น v1.0.13 การเชื่อมต่อเวลาหลุด ให้กลับมาเชื่อมต่อใหม่ได้  
   โดย event ย้ายมาทำงานใน loop (ป้องกันกราฟิกค้างเวลา event cb สลายตัว)   
   และ รองรับ NETPIE , รองรับ QoS, รองรับ clean session, รองรับ ตั้ง client_id ได้
 - ใช้กับ BeeNeXT3.1.6 ขึ้นไป (รองรับ BeeI2C, BeeMQTT ได้)  
  
## [Version 3.1.6] @20/12/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.3** (ติดต่อ Admin)    
 - BeeNeXT4.3IPS 800x480 (2025)  fq = 16MHz  
 - BeeNeXT5.0IPS 800x480 (2025)  fq = 16MHz  
  
## [Version 3.1.5] @20/12/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.3** (ติดต่อ Admin)   
 - แก้ typo พิมพ์ผิด BLYNKO_USE_ASYNC_MQTT32 (มีตกหล่น)  
  
## [Version 3.1.4] @19/12/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.3** (ติดต่อ Admin)   
 - วิตเจ็ต GGaugeLine สามารถกำหนด angle_offset เพื่อหมุนมุมเริ่มต้นได้ (แก้เข็มที่ยังผิด)
   และ มี direction ได้ ให้หมุนทวนเข็มได้
  
## [Version 3.1.3] @18/12/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.2** (ติดต่อ Admin)   
 - เพิ่มคำสั่ง API ให้ GGaugeLine สามารถกำหนด angle_offset เพื่อหมุนมุมเริ่มต้นได้
  
## [Version 3.1.2] @17/12/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.1** (ติดต่อ Admin)   
 - แก้ typo (พิมพ์คำผิด) จาก BLYNKO_USE_ASYNC_MQTT32 เป็น BLYNKGO_USE_ASYNC_MQTT32
 - BeeNeXT5.0C รุ่น 12/2024 ล่าสุด ใช้ความถี่ 13000000 ไม่ได้ ปรับเป็น 16000000
  
## [Version 3.1.1] @23/11/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.1** (ติดต่อ Admin)   
 - BlynkGO_AsyncMQTT32 v1.0.5
 - GBar, GKnob, GLevel, GSlider, GSliderH, GSliderV, GThermo เพิ่ม API toInt()
 - GTextArea เพิ่ม padding_top(..), padding_left(..) เพื่อปรับ ให้ข้อความภายในช่อง textarea อยู่ในระดับที่ต้องการ
 - GAqi สำหรับ มาตรฐาน Thai AQI ปรับตามมาตรฐานใหม่ของกรมควบคุมมลพิษ 2566 ที่มีการเปลี่ยนการตัด Thai AQI level ใหม่
  
## [Version 3.1.0] @22/07/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.1.0** (ติดต่อ Admin)   
- เพิ่ม BlynkGO_HTTPClient32 v0.5.0 สำหรับเป็นพื้นฐานในการจัดการเชื่อมต่อ HTTPClient แบบ ไม่สะดุด  
  
## [Version 3.0.23] @16/07/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.21** (ติดต่อ Admin)   
- ปรับปรุง BlynkGO_AsyncMQTT32 เป็น version 1.0.4  
  subscribe(...)  
    หากยังไม่เชื่อมต่อ จะจำไว้ใน list พอเชื่อมต่อแล้ว subscribe จริงไล่ทั้งหมดให้อัตโนมัติ  
    แต่หากเชื่อมต่อแล้ว เกิดมีเพิ่ม subscribe ให้ เพิ่มใน list และ ให้ subscribe จริงทันทีเลย  
  
## [Version 3.0.22] @14/07/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.21** (ติดต่อ Admin)   
- Fixed ให้ PlatformIO รองรับได้ อัพขึ้นเป็น v3.0.22 แทน
  
## [Version 3.0.21] @14/07/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.21** (ติดต่อ Admin)   
- Fixed MQTT แบบ wss เชื่อมต่อได้แล้วแต่เข้า GWiFiManager แล้วมีเมื่อปิด SSID มีอาการตาย  
  หรือ ปิดแล้วเปิดใหม่ มีอาการหน่วง  (สำหรับจอที PSRAM เท่านั้น)
  
## [Version 3.0.20] @10/07/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.21** (ติดต่อ Admin)   
- เพิ่ม BlynkGO ESP32DevBoard สำหรับ MCU หลังบ้าน
- เพิ่ม BeeNeXT Esp32Cam สำหรับ Esp32Cam สามารถเพิ่ม ข้อความในรูปทีจับภาพได้

## [Version 3.0.19] @04/07/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.20** (ติดต่อ Admin)   
- fix สำหรับ library บน PlatformIO library manager

## [Version 3.0.18] @04/07/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.20** (ติดต่อ Admin)   
- fix ให้ default เป็นแป้นพิมพ์ไทย
  
## [Version 3.0.17] @30/06/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.20** (ติดต่อ Admin)   
- fix แป้นพิมพ์ลาว และฟอนต์ลาว ปรับสัญลักษณ์ให้แสดงที่แป้นพิมพ์ได้ถูกต้อง
  
## [Version 3.0.16] @23/06/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.20** (ติดต่อ Admin)   
- GWiFiManager เพิ่มคำสั่งเช็คสถานะ GSwitch ของ ssid และ  autoip ได้
- GImageButton fix   inactive(false); ให้กลับมาเป็นปุ่มปกติได้
  
## [Version 3.0.15] @17/06/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.20** (ติดต่อ Admin)   
- GTextArea แป้นพิมพ์ default เพิ่มให้กำหนด API numpad_first(true/false)  
  เพื่อให้เริ่มมาที่แป้นพิมพ์ numpad ก่อนได้  
  
## [Version 3.0.14] @14/06/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.19** (ติดต่อ Admin)   
- BeeNeXT7" เปิดให้ SD card ทำงานได้   
    
## [Version 3.0.13] @06/06/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.19** (ติดต่อ Admin)   
- GWiFiManager v1.0.13 เพิ่ม onBack(..) สำหรับดักเมื่อผู้ใช้กดตอนออกได้  
  
## [Version 3.0.12] @25/05/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.19** (ติดต่อ Admin)   
- fix touch ของ BeeNeXT4.3IPS 800x480  
  
## [Version 3.0.11] @01/03/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.19** (ติดต่อ Admin)   
- fix&tune BeeNeXT4.3R/C 480x272 
  
## [Version 3.0.10] @11/02/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.19** (ติดต่อ Admin)   
- รองรับ LittleFS โดยให้กำหนด BLYNKGO_USE_LITTLEFS ใน blynkgo_config.h 
  
## [Version 3.0.9] @25/01/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.18** (ติดต่อ Admin)  
- ปรับ version ให้ตรงกับ version บน PlatformIO  
  
## [Version 3.0.8] @24/01/67  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.18** (ติดต่อ Admin)  
- เพิ่ม BeeWave-S3 RGB8048 Capactive สำหรับบอร์ด PCB 
  
## [Version 3.0.7] @09/11/66  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.17** (ติดต่อ Admin)  
- เพิ่ม sd_mount(), sd_unmount()
- PlatformIO BlynkGO SDK ปรับให้บอร์ดมี default partition ให้เลย โดยไม่ต้องกำหนด platformio.ini ก็ได้  
  
## [Version 3.0.6] @13/11/23
 - จูน BeeNeXT5.0IPS ให้รองรับ จอ 5.0IPS 1000cd/m2 4.3IPS 1600cd/m2 by Mr.Beem  
  
## [Version 3.0.5] @09/11/66  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.14 Fix001** (ติดต่อ Admin)  
- รองรับ ทำงานบน PlatformIO ได้  
- จูน BeeNeXT4.3IPS, BeeNeXT5.0IPS, BeeNeXT7.0 ลด flickering  

## [Version 3.0.0 released] @18/09/66
ดาวโหลดที่ https://github.com/BlynkGO/BlynkGOv3/releases/tag/3.0.0  
ทำงานบน **ESP32 core 2.0.6 ; ใช้ BlynkGO SDK 3.0.12** (ติดต่อ Admin)  
ทำงานบน **ESP32 core 2.0.13; ใช้ BlynkGO SDK 3.0.14** (ติดต่อ Admin)   
ทำงานบน ArduinoIDE และ Platform IO ได้  
  
รองรับบอร์ด  
- BlynkGO Board v1.3
- BlynkGO Board S3 Parallel
- BlynkGO Emma-II (4MB/4MB & 16MB/8MB)
- BlynkGO Enterprise 5.0
- BlynkGO Enterprise 7.0
- BeeNeXT1.9IPS
- BeeNeXT2.4R/C
- BeeNeXT2.8R
- BeeNeXT3.2IPS
- BeeNeXT4.3R/C/IPS
- BeeNeXT5.0IPS
- BeeNeXT7.0

## [Version 3.0.0 beta12] 
ให้ติดตั้งด้วย BlynkGO SDK version3.0.12 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  
 - fix ให้ BeeNeXT1.9IPS, BeeNeXT5.0IPS, BlynkGO Emma-II กลับมาแสดงร่วมกันได้หมดใน version เดียว
 - เพิ่มบอร์ด BeeNeXT2.4R, BeeNeXT2.4C, BlynkGO Board 3.5 S3 Parallel
 - BlynkGO LCD อัพขึ้นเป็น 1.0.3 ให้เหลือเพียง lcd พอ โดยเลือกบอร์ดจาก ArduinoIDE ที่เมนูเลือก Board แทน
 - fix GLabel ให้ rolling ข้อความยาวๆได้ และ false ได้

## [Version 3.0.0 beta11]
ให้ติดตั้งด้วย BlynkGO SDK version3.0.11 for ESP32core2.0.6 (based on ESP-IDF4.4.3)  
 - support BeeNeXT4.3C , BeeNeXT4.3IPS
 - เพิ่มคำสั่ง BlynkGO.setTimestamp(time_t t) สำหรับกำหนด timestamp ปัจจุบันให้ BlynkGO
 - เพิ่ม GTimer ให้ใช้คำสั่ง delay(...), setInterval(...) แบบ ส่ง parameter เข้าไปใน timer และทำงานแบบ lambda function ได้ด้วย

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
