#include <BlynkGOv3.h>

#define TIMESTAMP(h,m)          ((h%24)*3600 + (m%60)*60)
#define TIMESTAMP2Hour(ts)      ((ts%86400)/3600)
#define TIMESTAMP2Minute(ts)    (((int)(ts%86400)/60) %60)
#define TIMESTAMP2String(ts)    StringX::printf("%02d:%02d", TIMESTAMP2Hour(ts), TIMESTAMP2Minute(ts) )

// timestamp ภายใน 1 วัน ในหน่วยวินาที นับจาก 00:00 ไป  (ค่าตั้งแต่ 0 และ < 86400 )
// ค่าเวลานี้ จะใช้เก็บค่าที่เปลี่ยนแปลงด้วยวิตเจ็ต GTimeSelector
time_t timestamp = TIMESTAMP( 9, 36); // กำหนดเวลาด้วย hour, minute แล้วเปลี่ยนเป็นค่า timestamp

GTimeSelector time_selector;    // วิตเจ็ต GTimeSelector สำหรับเลือกเวลาที่ต้องการ
GLabel lb_time;

// เมื่อ GLabel ภูก clicked
void lb_time_onClicked(GWidget*widget){
  time_selector.init(lb_time, timestamp);
  time_selector.onApply([](GWidget*widget)  { time_selector.hidden(true); });
  time_selector.onCancel([](GWidget*widget) { time_selector.hidden(true); });
}

// เมื่อ GLabel มีการเปลี่ยนแปลงค่า (หลังจากผู้ใช้เปลี่ยนค่าจาก GTimerSelect )
void lb_time_onValueChanged(GWidget*widget){
  Serial.printf("[TimeSeting] %s (timestamp %d)\n", lb_time.c_str(), timestamp);
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  lb_time = TIMESTAMP2String(timestamp);              // เปลี่ยน timastamp ของเวลาที่กำหนด เป็น String แล้วจับค่าใส่ให้วิตเจ็ต GLabel
  lb_time.clickable(true);                            // เปิดให้ GLabel รับสัญญาณ สัมผัส ต่างๆไดด้วย
  lb_time.onClicked( lb_time_onClicked );             // เมื่อ GLabel ภูก clicked
  lb_time.onValueChanged( lb_time_onValueChanged );   // เมื่อ GLabel มีการเปลี่ยนแปลงค่า (หลังจากผู้ใช้เปลี่ยนค่าจาก GTimerSelect )
}

void loop() {
  BlynkGO.update();
}


