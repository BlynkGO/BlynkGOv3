#include <BlynkGOv3.h>

typedef struct _smartfarm {
  GLabel label;
  GGauge gauge_temp;
  GGauge gauge_humid;
} smartfarm_t;

GMultiScreen multiscreen;
smartfarm_t smartfarm[15];
String name[] =   { "1. ผักกาดแก้ว", "2. เรดโอ๊ค",  "3. กรีนโอ๊ค", "4. ร็อคเก็ต", "5. ฟิลเลย์", 
                    "6. กรีนคอส", "7. บัตเตอร์เฮด", "8. ผักกาดหอม", "9. สตรอว์เบอร์รี่", "10. มะเขือเทศ",
                    "11. เรดคอรัล", "12. บัตตาเวีย", "13. กวางตุ้ง", "14. คะน้า", "15. ผักโขม" };

void setup() {
  Serial.begin(115200); Serial.println();     // เริ่มใช้งาน Serial Monitor
  BlynkGO.begin();                            // เริ่มใช้งาน BlynkGO
  BlynkGO.fillScreen(TFT_BLACK);              // เติมสีหน้าจอเป็นสี ดำ


  multiscreen.addScreen(15);                      // เพิ่ม screens ตามจน.ที่ต้องการ
  multiscreen.anim_time(500);                     // เวลาในการเลื่อนไปหน้า screen ถัดไป
  multiscreen.indic().align(ALIGN_BOTTOM,0,22);   // จัดวาง indic ของ multiscreen ได้ตามที่ต้องการ
  multiscreen.indic().padding_inner(1);           // ช่องไฟระหว่าง indic (ตัวจุดสำหรับบอกว่าปัจจุบันอยู่ screen ไหน)
  
  for(int i=0; i < multiscreen.num(); i++){
    smartfarm[i].label = name[i];
    smartfarm[i].label.parent(multiscreen[i]);     // ให้วิตเจ็ต มี แม่คือ multiscreen[i]
    smartfarm[i].label.font(prasanmit_25, TFT_WHITE);
    smartfarm[i].label.align(ALIGN_TOP,0,-1);
    
    smartfarm[i].gauge_temp.size(120,120); 
    smartfarm[i].gauge_temp.parent(multiscreen[i]);     // ให้วิตเจ็ต มี แม่คือ multiscreen[i]
    smartfarm[i].gauge_temp.title("อุณหภูมิ (°C)");
    smartfarm[i].gauge_temp.color( TFT_COLOR_HEX(0xF45757) );
    smartfarm[i].gauge_temp.decimal(1);
    smartfarm[i].gauge_temp.align(ALIGN_LEFT,0,12);
    smartfarm[i].gauge_temp = random(1000)/10.0;

    smartfarm[i].gauge_humid.size(120,120); 
    smartfarm[i].gauge_humid.parent(multiscreen[i]);     // ให้วิตเจ็ต มี แม่คือ multiscreen[i]
    smartfarm[i].gauge_humid.title("ความชื้น (%)");
    smartfarm[i].gauge_humid.color( TFT_COLOR_HEX(0x0455BF) );
    smartfarm[i].gauge_humid.decimal(1);
    smartfarm[i].gauge_humid.rounded(false);  // ปลายแบบไม่มน
    smartfarm[i].gauge_humid.align(ALIGN_RIGHT,0,12);
    smartfarm[i].gauge_humid = random(1000)/10.0;
  }

  // ควบคุมปุ่มกด เมื่อมีการกด Pressed เข้ามาให้เลื่อนหน้า multiscreen ถอยกลับ
  // เมื่อ btn1 มีการ click ให้ทำการเลื่อนหน้าจอ multiscreen ถอยย้อนหน้าก่อน
  BTN1.onPressed([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() > 0) {
      multiscreen.current_screen( multiscreen.current_screen_id()-1);
    }else{
      multiscreen.current_screen(multiscreen.num()-1, false);
    }
  });

  // ควบคุมปุ่มกด เมื่อมีการกด Pressed เข้ามาให้เลื่อนหน้า multiscreen ถัดไป
  // เมื่อ btn2 มีการ click ให้ทำการเลื่อนหน้าจอ multiscreen ไปหน้าถัดไป
  BTN2.onPressed([](ButtonISR* btn) {
    if( multiscreen.current_screen_id() < multiscreen.num()-1) {
      multiscreen.current_screen( multiscreen.current_screen_id()+1);
    }else{
      multiscreen.current_screen( 0, false);
    }
  });
}

void loop() {
  BlynkGO.update();                           // ให้ BlynkGO ทำการ update สถานะต่างๆ
}

