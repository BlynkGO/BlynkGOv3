/*******************************************************
 * ตัวอย่าง สร้าง GSwitch3D  จำนวน 3 ชุดด้วย array
 *   โดยมีการโหลด และ บันทึกสถานะของปุ่มกด ลงบน flashMem
 *   หากมีการ restart ESP32 สถานะเดิมจะยังคงกลับมาเหมือนเดิม
 *******************************************************/

#include <BlynkGOv3.h>

GContainer  cont_sw3d;
  GSwitch3D sw3d[3];
  color_t   sw3d_color[3] = { TFT_RED, TFT_GREEN, TFT_BLUE };
GLabel      label[3]      = { "ห้องนอน", "ห้องครัว", "ห้องน้ำ" };

void setup() {
  Serial.begin(115200);
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);

  cont_sw3d.layout(LAYOUT_ROW_M); // จัด layout ให้ลูกๆ โดย เรียงตามแถว แนวกลางตรงกัน
  cont_sw3d.padding(0,0,0,0,20);  // จัด ช่องไฟ ให้ลูกๆ โดยช่องไฟ บน-ล่าง-ซ้าย-ขวา เป็น 0 ส่วนช่องไฟระหว่างลูกๆ 20px
  cont_sw3d.opa(0);               // ให้โปร่งใส
  
  for(int i=0; i < 3; i++){
    sw3d[i].parent(cont_sw3d);    // ให้มีแม่คือ cont_sw3d แล้วการจัดตำแหน่ง align ไม่ต้องจัดอีก ตำแหน่งจะถูกเรียงไปตาม layout ของ const_sw3d
    sw3d[i].color(sw3d_color[i]); 

    // ดึงสถานะจาก flashMem มากำหนดให้ sw3d[i] แต่ละตัว
    String flashmem_key = "SW3D_STATE" + String(i);
    if ( BlynkGO.flashMem_exists(flashmem_key)) { // เช็คว่ามี key  บน flashMem หรือไม่
      // ดึงค่าจาก flashMem
      bool state = (bool) BlynkGO.flashMem_Int(flashmem_key);
      (state) ? sw3d[i].ON() : sw3d[i].OFF();     // กำหนด สถานะให้ sw3d[i]
    } else {
      sw3d[i].OFF();
    }

    // หากมี user มากดเปลี่ยนสถานะปุ่มเข้ามา ให้บันทึกสถานะลง flashMem
    sw3d[i].onValueChanged([](GWidget * widget) {
      // ค่า i ไม่สามารถส่งเข้ามาใน callback function ได้โดยตรง
      // จึงต้องคำนวณว่า ปุ่มที่กดเข้ามาเป็นลำดับเท่าไหร่ของ array ของ sw3d[]
      uint8_t ii = (GSwitch3D*) widget - &sw3d[0];      // ค่า ii ที่ได้จะไปตรงกับ i ให้เอง
      
      String flashmem_key = "SW3D_STATE" + String(ii);  
      bool   state        = sw3d[ii].isON();
      
      // บันทึก state ของ sw3d ลง flashMem
      BlynkGO.flashMem(flashmem_key, state);
    });
  }

  for(int i=0;i < 3; i++){
    label[i].align(sw3d[i], ALIGN_BOTTOM);
    label[i].font(prasanmit_25, TFT_WHITE);
  }
}

void loop() {
  BlynkGO.update();
}

