/*******************************************************************
 * 
 * ตัวอย่างให้วิตเจ็ตรูปภาพ รูป football ทำ animation
 * มีการหมุนรูปภาพ และ เปลียนตำแหน่ง x ภายในระยะเวลาที่กำหนด
 * และให้เล่นซ้ำๆ ไปเรื่อยๆ
 * 
 * โดย GAnimation จะเป็นสิ่งที่ไว้สำหรับ เปลี่ยน ค่าใดๆ ของวิตเจ็ต 
 * ในช่วงค่านึงๆภายในเวลาที่กำหนด โดยไม่ทำให้ performance ของกราฟิกสะดุดลง
 * 
 *******************************************************************/

#include <BlynkGOv3.h>

IMG_DECLARE(pic_football);                        // ประกาศรูปแบบ c file ก่อนนำไปใช้ตามวิตเจ็ตต่างๆ

GImage football(pic_football);                    // วิตเจ็ตรูปภาพ football โดยโหลดรูป แบบ c file
//GImage football("SD://images/football.png");    // วิตเจ็ตรูปภาพ football โดยโหลดรูป football.png ที่วางอยู่บน SD card
GAnimation animation[2];                          // ประกาศตัวแปร GAnimation 2 ชุด

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_GREEN, TFT_DARKGREEN);   // กำหนดสีพื้นหลังแบบไล่เฉดสี
  
  football.create();                              // ให้วิตเจ็ตรูปภาพทำการสร้าง โดยมีการโหลดรูปขึ้นมาแสดง
  
  // กำหนดให้ animation ใช้สำหรับ วิตเจ็ตรูปภาพ football 
  // ด้วยการปรับค่ามุม ด้วยฟังกชั่น ANIM_ANGLE (ฟังกชั่นภายใน BlynkGO ที่เตรียมไว้ให้ )
  // โดย ให้ปรับ ค่ามุมหมุนจาก 0 ไป 360 (ตามเข็มจาก 12 นาฬิกา) ภายใน 5 วินาที 
  animation[0].init(football, ANIM_ANGLE, 0, 360, 5000);
  animation[0].repeat(true);                      // ให้มีการเล่นซ้ำ หลัง animation ครบรอบ
  animation[0].create();                          // ทำการลงทะเบียนให้ animation[0] สร้างขึ้นมาทำงาน

  // กำหนดให้ animation ใช้สำหรับ วิตเจ็ตรูปภาพ football 
  // ด้วยการเปลี่ยน x ของวิตเจ็ต ด้วยฟังกชั่น ANIM_X (ฟังกชั่นภายใน BlynkGO ที่เตรียมไว้ให้ )
  // เริ่มจาก x = -football.width() ไปจนถึง x = BlynkGO.width() ภายใน 10 วินาที
  animation[1].init(football, ANIM_X, -football.width(), BlynkGO.width(), 10000 );
  animation[1].repeat(true);                      // ให้มีการเล่นซ้ำ หลัง animation ครบรอบ
  animation[1].create();                          // ทำการลงทะเบียนให้ animation[1] สร้างขึ้นมาทำงาน

}

void loop() {
  BlynkGO.update();
}

/******************************************************************************
 * 
 * ฟังกชั่นสำเร็จ สำหรับทำ Animation ที่ภายใน BlynkGO ที่เตรียมไว้ให้ใช้ร่วมกับ GAnimation
 *   ANIM_X       : สำหรับ ปรับพิกัด x ของ วิตเจ็ตใดๆ
 *   ANIM_Y       : สำหรับ ปรับพิกัด y ของ วิตเจ็ตใดๆ
 *   ANIM_WIDTH   : สำหรับ ปรับค่าความกว้าง ของ วิตเจ็ตใดๆ
 *   ANIM_HEIGHT  : สำหรับ ปรับค่าความสูง   ของ วิตเจ็ตใดๆ
 *   ANIM_OPA     : สำหรับ ปรับค่าความโปร่งใส ของ วิตเจ็ตใดๆ
 *   ANIM_ANGLE   : สำหรับ ปรับค่า มุม (นับจาก12นาฬิกา) ให้วิตเจ็ตรูปภาพ
 *   ANIM_ZOOM    : สำหรับ ปรับค่า zoom ให้วิตเจ็ตรูปภาพ
 *   
 * หากจะสร้างฟังกชั่นทำ Animtaion เอง ให้มี prototype ดังนี้คือ
 *   void animation_function(GWidget* widget, int16_t value);
 * 
 * และ กำหนดให้ GAnimation ด้วยคำสั่ง 
 *   animation[0].init( football, animation_funtion, ค่าเริ่มต้น, ค่าสิ้นสุด, ช่วงเวลา_ms );
 *   animation[0].create();
 * 
 * 
 *****************************************************************************/
