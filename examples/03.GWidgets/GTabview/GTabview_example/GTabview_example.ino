#include <BlynkGOv3.h>

GTabview tabview;             // ประกาศตัวแปร tabview
GTab   tab1, tab2, tab3;      // ประกาศตัวแปร tab ย่อยๆ ใน tabview
GLabel label1(tab1), label2(tab2), label3(tab3);    // ประกาศตัวแปร label ที่ให้อยู่ภายใน tab แต่ละ tab

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  //tabview.tabbar_hidden(true);
  //tabview.tabbar_color(TFT_BLUE, TFT_WHITE, TFT_RED);   // เปลี่ยนสี แถบ : สีพื้นหลัง, สีฟอนต์ , สี indic ตัวระบุแทบปัจจุบัน ตามลำดับ

  tabview.addTab(tab1, "เซนเซอร์");   // เพิ่ม tab เข้าใน tabview
  tabview.addTab(tab2, "กราฟ");     // เพิ่ม tab เข้าใน tabview
  tabview.addTab(tab3, "สถิติ");      // เพิ่ม tab เข้าใน tabview
  tabview.current_tab_id(1);         // กำหนดให้ tabview มาแสดง tab ปัจจุบันที่ tab id = 1 

  tabview.onValueChanged([](GWidget*widget){
    uint8_t     tab_id    = tabview.current_tab_id();
    const char* tab_name  = tabview.tab_name(tab_id);
    Serial.printf("Current Tab ID : %d  %s\n", tab_id, tab_name );
  });


  label1 = "นี่คือ Tab แรก\n\n"
           "หาก เนื้อหาใน tab \n"
           "มีความยาวมากกว่าขนาด tab\n"
           "จะมี scrollbar\n"
           "เพิ่มให้เอง อัตโนมัติ\n"
           "------------------\n"
           " Test 1\n"
           " Test 2\n"
           " Test 3\n"
           "------------------";
  label2 = "นี่คือ Tab ที่สอง";
  label3 = "นี่คือ Tab ที่สาม";

}

  
void loop() {
  BlynkGO.update();
}

