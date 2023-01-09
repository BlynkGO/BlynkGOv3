/*******************************************************************
 * 
 * ตัวอย่าง present ชุดข้อมูล sensor หลายๆชุด ภายในกรอบพื้นที่เดียวกัน
 * เพิ่มลูกเล่นในการแสดงค่า โดยใช้ Animation เข้าด้วย 
 * 
 *******************************************************************/

#include <BlynkGOv3.h>

String  room[5]         = { "ห้องนอน", "ห้องนั่งเล่น", "ห้องรับแขก", "ห้องครัว", "ห้องน้ำ" };
float   temperature[5]  = { 20.5, 24.3, 30.2, 18.8, 25.6 };


GRect rect;                                 // วิตเจ็ตสี่เหลี่ยม
GLabel lb_room(rect);                       // วิตเจ็ตข้อความ โดยให้เป็นเลเยอร์ลูกของ rect
GLabel lb_temp(rect);                       // วิตเจ็ตข้อความ โดยให้เป็นเลเยอร์ลูกของ rect

GAnimation anim_room;                       // ตัวสร้าง animation ให้วิตเจ็ต lb_room
GAnimation anim_temp;                       // ตัวสร้าง animation ให้วิตเจ็ต lb_temp

void setup() {
  BlynkGO.begin();
  
  rect.size(280, 80);                       // กำหนดขนาด
  rect.corner_radius(10);                   // กำหนดรัศมีมุมโค้ง
  rect.color(TFT_YELLOW, TFT_GREEN);        // กำหนดสีแบบไล่เฉดสี
  rect.border(3, TFT_SILVER);               // กำหนดระยะขอบและสี

  lb_room = room[0];                        // กำหนดชื่อห้องให้ วิตเจ็ตข้อความ
  lb_room.align(ALIGN_LEFT, 20);            // จัดวางไว้ห่างจากขอบด้านซ้ายของเลเยอร์แม่คือ rect ไปทางขวา 20 pixel
  lb_room.font(prasanmit_30, TFT_BLACK);    // กำหนดฟอนต์และสี

  lb_temp = String(temperature[0],1)+SYMBOL_DEGREE;  // กำหนดค่าอุณหภูมิ วิตเจ็ตข้อความ
  lb_temp.align(ALIGN_RIGHT,-20);           // จัดวางไว้ห่างจากขอบด้านขวาของเลเยอร์แม่คือ rect ไปทางซ้าย 20 pixel
  lb_temp.font(prasanmit_40, TFT_BLACK);    // กำหนดฟอนต์และสี

  // หลังจัดวางตำแหน่งและกำหนด style ต่างๆให้วิตเจ็ตแล้ว
  // ทำการสร้าง animation ให้วิตเจ็ตที่ต้องการ
  ANIM_ROOM_create(&anim_room);             // สร้าง animation ให้วิตเจ็ต lb_room
  ANIM_TEMP_create(&anim_temp);             // สร้าง animation ให้วิตเจ็ต lb_temp

}

void loop() {
  BlynkGO.update();
}

// สร้าง animation ให้วิตเจ็ต lb_room
void ANIM_ROOM_create(GAnimation* a){
  static int8_t id = -1;
  id = (id+1)%5;                            // เพิ่มค่า id ไปอีก 1 โดยไม่เกิน 5 ให้กลับมาเริ่ม 0 ใหม่
  
  lb_room = room[id];                       // กำหนดชื่อห้องให้ วิตเจ็ตข้อความ

  // ให้ anim_room สร้าง animation ให้ วิตเจ็ต lb_room
  // โดย ปรับพิกัด y ด้วย ANIM_Y จากค่า y = -lb_room.height() ไปถึง y= (rect.height()-lb_room.height())/2
  // ภายใน 2 วินาที
  anim_room.init(lb_room, ANIM_Y, -lb_room.height(), (rect.height()-lb_room.height())/2 , 2000);
  anim_room.path_type(ANIM_PATH_EASE_IN);   // ให้ค่าเปลี่ยนแปลงแบบค่อยๆเพิ่มเร็วมาช้า
  anim_room.playback(true, 3000);           // ให้เล่นค่าย้อนกลับด้วย โดยมีการหยุดคั่น 3 วินาที
  anim_room.ready_cb(ANIM_ROOM_create);     // เมื่อ animation เสร็จ ให้ไปเรียก ANIM_ROOM_create ขึ้นมาทำงาน
  anim_room.create();                       // ลงทะเบียนสร้างให้ anim_room เริ่มทำงาน
}

// สร้าง animation ให้วิตเจ็ต lb_temp
void ANIM_TEMP_create(GAnimation* a){
  static int8_t id = -1;
  id = (id+1)%5;                            // เพิ่มค่า id ไปอีก 1 โดยไม่เกิน 5 ให้กลับมาเริ่ม 0 ใหม่

  lb_temp = String(temperature[id],1)+SYMBOL_DEGREE;  // กำหนดค่าอุณหภูมิ วิตเจ็ตข้อความ
  
  // ให้ anim_temp สร้าง animation ให้ วิตเจ็ต lb_temp
  // โดย ปรับพิกัด y ด้วย ANIM_Y จากค่า y = rect.height() ไปถึง y= (rect.height()-lb_room.height())/2
  // ภายใน 2 วินาที
  anim_temp.init(lb_temp, ANIM_Y, rect.height(), (rect.height()-lb_temp.height())/2 , 2000);
  anim_temp.path_type(ANIM_PATH_EASE_IN);   // ให้ค่าเปลี่ยนแปลงแบบค่อยๆเพิ่มเร็วมาช้า
  anim_temp.playback(true, 3000);           // ให้เล่นค่าย้อนกลับด้วย โดยมีการหยุดคั่น 3 วินาที
  anim_temp.ready_cb(ANIM_TEMP_create);     // เมื่อ animation เสร็จ ให้ไปเรียก ANIM_TEMP_create ขึ้นมาทำงาน
  anim_temp.create();                       // ลงทะเบียนสร้างให้ anim_temp เริ่มทำงาน
}

