#include "melody_struct.h"

// -------------------------------------------
// ค่า Raw Duration ของ Note  ( Note Duration)
// -------------------------------------------
// whole note  (โนตตัวกลม)      = 800 durations
// haft note   (โนตตัวขาว)      = 400 durations
// quarter note (โนตตัวดำ)      = 200 durations
// eighth note (โนตเขบ็ต 1 ชั้น)  = 100 durations
// sixteenth note(โนตเขบ็ต 2 ชั้น)= 50 durations

// -------------------------------------------
// BPM :  ค่า Beat ต่อ นาที ของ Quarter Note  
// -------------------------------------------
// BPM 160   // คือ ค่า จน.ครั้ง beat ต่อนาทีของโน้ตตัวดำ

// -------------------------------------------
// สูตรค่า True Duration  (ms มิลลิวินาที)
// -------------------------------------------
// สูตร ระยะเวลาที่ Note ตัวนึงๆจะเล่นนานจริง 
// โดยคำนวณ จากค่า Raw Duration และ BPM ที่กำหนด
//
// True_Duration = Raw_Duration * 60000ms/ (200*BPM)  [ms]
//
//  - หาก BPM ยิ่ง มาก  โน้ตแต่ละตัวเวลา play ระยะเวลาจะสั้นลง
//  - หาก BPM ยิ่ง น้อย  โน้ตแต่ละตัวเวลา play ระยะเวลาจะสั้นยาวขึ้น
//

const PROGMEM melody_note_t jingle_bells_notes[52] = {
{ NOTE_E4, 200}, // โนต, Raw Duration
{ NOTE_E4, 200},
{ NOTE_E4, 400},
{ NOTE_E4, 200},
{ NOTE_E4, 200},
{ NOTE_E4, 400},
{ NOTE_E4, 200},
{ NOTE_G4, 200},
{ NOTE_C4, 300},
{ NOTE_D4, 100},
{ NOTE_E4, 800},
{ NOTE_F4, 200},
{ NOTE_F4, 200},
{ NOTE_F4, 300},
{ NOTE_F4, 100},
{ NOTE_F4, 200},
{ NOTE_E4, 200},
{ NOTE_E4, 200},
{ NOTE_E4, 100},
{ NOTE_E4, 100},
{ NOTE_E4, 200},
{ NOTE_D4, 200},
{ NOTE_D4, 200},
{ NOTE_E4, 200},
{ NOTE_D4, 400},
{ NOTE_G4, 400},
{ NOTE_E4, 200},
{ NOTE_E4, 200},
{ NOTE_E4, 400},
{ NOTE_E4, 200},
{ NOTE_E4, 200},
{ NOTE_E4, 400},
{ NOTE_E4, 200},
{ NOTE_G4, 200},
{ NOTE_C4, 300},
{ NOTE_D4, 100},
{ NOTE_E4, 800},
{ NOTE_F4, 200},
{ NOTE_F4, 200},
{ NOTE_F4, 200},
{ NOTE_F4, 200},
{ NOTE_F4, 200},
{ NOTE_E4, 200},
{ NOTE_E4, 200},
{ NOTE_E4, 100},
{ NOTE_E4, 100},
{ NOTE_G4, 200},
{ NOTE_G4, 200},
{ NOTE_F4, 200},
{ NOTE_D4, 200},
{ NOTE_C4, 800},
{ NOTE_NO, 1600},

};

const melody_t melody_jingle_bells = {
  .notes = jingle_bells_notes,
  .size  = 52,
  .bpm   = 160,
};

