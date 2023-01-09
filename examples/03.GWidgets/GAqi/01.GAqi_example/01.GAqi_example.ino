/*******************************************************************
 * วิตเจ็ต GAqi
 *  - กดคลิ๊กที่หน้าปัด จะสลับการแสดงค่าแบบ AQI และ PM2.5
 *  - กดค้างยาวที่หน้าปัด 
 *    จะเปลี่ยนมาตรฐานของ AQI โดย หมุนเปลี่ยน AQI_US ->  AQI_TH ->  AQI_CN วนไปเรื่อยๆ
 *
 *******************************************************************/
 

#include <BlynkGOv3.h>

GAqi myAqi;

void setup() {
  Serial.begin(115200); Serial.println();
  
  BlynkGO.begin();         		// เริ่มระบบ BlynkGO

  myAqi.aqi_type( AQI_US );         		  // กำหนดประเภท มาตรฐาน aqi แบบ US  ,  aqi_type: AQI_US, AQI_TH(default), AQI_CN
  myAqi.pm25(55);                    		  // กำหนดค่า pm2.5 ให้ แสดงออกจอ
  
  Serial.println( myAqi.getAQI(AQI_US));  // ดึงค่า aqi มาตรฐาน  US แสดงออก Serial Monitor (แปลงจากค่า pm2.5 ที่ได้กำหนด)
  Serial.println( myAqi.getAQI(AQI_TH));  // ดึงค่า aqi มาตรฐาน  TH แสดงออก Serial Monitor (แปลงจากค่า pm2.5 ที่ได้กำหนด)
  Serial.println( myAqi.getAQI(AQI_CN));  // ดึงค่า aqi มาตรฐาน  CN แสดงออก Serial Monitor (แปลงจากค่า pm2.5 ที่ได้กำหนด)

}

void loop() {
  BlynkGO.update();
}


