#include <BlynkGOv3.h>

GDropdownList ddlist_day;
GDropdownList ddlist_month;
GDropdownList ddlist_year;

String th_month[] = { "มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", 
                      "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", 
                      "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม" };

void ddlist_onValueChange(GWidget* widget){
  String date_selected = ddlist_day.selected() +  " " +  ddlist_month.selected() + " " + ddlist_year.selected();
  Serial.println( date_selected );

  int day = ddlist.toInt();     // ดึงค่า ตัวเลือก ปัจจุบันของ ddist ออกมาและแปลงเป็น int ให้
  int year = ddlist.toInt();    // ดึงค่า ตัวเลือก ปัจจุบันของ ddist ออกมาและแปลงเป็น int ให้
  int month = ddlist.selected_id() + 1;  // ดึงค่า ลำดับ ของ ตัวเลือกปัจจุบัน  (ค่าจะเป็น 0 หรือ 1, 2, 3... ) เมื่อ +1 คือเลขเดือนปัจจุบันที่เลือกอยู่
  Serial.printf("%02d/%02d/%04d\n", day, month, year);
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  ddlist_day  .options(1,31);           // กำหนด options ให้ ddlist_day ด้วยตัวเลข ตั้งแต่ 1 ถึง 31
  ddlist_month.options(th_month, 12);   // กำหนด options ให้ ddlist_month ด้วย String Array
  ddlist_year .options(2510,2563);      // กำหนด options ให้ ddlist_year ด้วยตัวเลข ตั้งแต่ 2510 ถึง 2563
  
  ddlist_day  .onValueChanged(ddlist_onValueChange);    // เมื่อ user มีการกดเลือกแล้วค่าเปลี่ยนแปลงเข้ามา
  ddlist_month.onValueChanged(ddlist_onValueChange);
  ddlist_year .onValueChanged(ddlist_onValueChange);

  ddlist_day  .size( 60,60);            // กำหนดความกว้าง x ความสูง ขณะย่อลง
  ddlist_month.size(140,60);
  ddlist_year .size( 90,60);

  ddlist_day  .height_open(200);        // กำหนดความสูงขณะถูกขยายออกมา
  ddlist_month.height_open(200);
  ddlist_year .height_open(200);

  ddlist_day  .font(prasanmit_30);      // กำหนด font  
  ddlist_month.font(prasanmit_30);
  ddlist_year .font(prasanmit_30);

  // จัดเรียงตำแหน่ง
  uint16_t w = 10 + ddlist_month.width() + 10 + ddlist_year.width();
  ddlist_day  .align( ALIGN_TOP  , -w/2, 10  );
  ddlist_month.align( ddlist_day  , ALIGN_RIGHT   , 10  , 0   ); 
  ddlist_year .align( ddlist_month, ALIGN_RIGHT   , 10  , 0   );
  
}

void loop() {
  BlynkGO.update();
}


