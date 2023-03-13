/***********************************************
 * ตัวอย่างเล่น โน้ต Melody แบบ None-Blocking 
 * เวลาจะไม่ถูกหยุดค้างด้วย delay(..)
 * อุปกรณ์รอบข้างอื่นๆ ยังทำงานต่อไปได้ระหว่างมีการเล่น melody
 * 
 ***********************************************/

#include <BlynkGO_Buzzer.h>     // ดาวโหลดได้จาก https://github.com/BlynkGO/BlynkGO_Buzzer

MELODY_DECLARE(melody_jingle_bells);

Buzzer buzzer(26);              // กำหนดขา pin ของ buzzer

void setup() {
  Serial.begin(115200); Serial.println();

  buzzer.addMelody(melody_jingle_bells);

  buzzer.playMelody();         // play แบบ วนไปเรื่อยๆ

  // หากจะ play แบบรอบเดียว
  // buzzer.playMelody(1);

  // หากจะ play แบบ 2 รอบ และ เมื่อเล่นเสร็จให้ทำอะไรต่อ
  // buzzer.playMelody(2, [](){
  //   /* เมื่อเล่นจบให้ทำอะไรวางตรงนี้ */
  //   Serial.println("[Melody] Done");
  // });

  // buzzer.pauseMelody();     // หากจะ pause หยุดชั่วคราว
  // buzzer.playMelody();      // หากจะ กลับมาเล่นต่อจากที่เคย pause ไว้
  // buzzer.stopMelody();      // หากจะ stop ยกเลิกการ play เลย

}

void loop(){

}