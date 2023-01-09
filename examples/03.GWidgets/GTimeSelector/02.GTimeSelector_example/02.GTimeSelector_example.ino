#include <BlynkGOv3.h>

#define TIMESTAMP(h,m)          ((h%24)*3600 + (m%60)*60)
#define TIMESTAMP2Hour(ts)      ((ts%86400)/3600)
#define TIMESTAMP2Minute(ts)    (((int)(ts%86400)/60) %60)
#define TIMESTAMP2String(ts)    StringX::printf("%02d:%02d", TIMESTAMP2Hour(ts), TIMESTAMP2Minute(ts) )

// timestamp ภายใน 1 วัน ในหน่วยวินาที นับจาก 00:00 ไป  (ค่าตั้งแต่ 0 และ < 86400 )
// ค่าเวลานี้ จะใช้เก็บค่าที่เปลี่ยนแปลงด้วยวิตเจ็ต GTimeSelector
time_t timestamp_start[3] = { TIMESTAMP( 8,0), TIMESTAMP(12,30), TIMESTAMP(15,53) };
time_t timestamp_stop[3]  = { TIMESTAMP(10,0), TIMESTAMP(14,30), TIMESTAMP(20,15) };


GTimeSelector time_selector;    // วิตเจ็ต GTimeSelector สำหรับเลือกเวลาที่ต้องการ
GLabel lb_start("Start");
GLabel lb_stop("Stop");
GLabel lb_time_start[3];
GLabel lb_time_stop[3];

// เมื่อ GLabel ภูก clicked
void lb_time_onClicked(GWidget*widget){
  for(int i=0; i< 3;i++){
    if(widget->sameWith(lb_time_start[i]) ){   // เทียบว่า widget ที่ clicked เข้ามา คือวิตเจ็ตไหน เพื่อจะใช้ไปกำหนดให้ GTimeSelector
      time_selector.init(lb_time_start[i], timestamp_start[i]);
    }else
    if(widget->sameWith(lb_time_stop[i])){
      time_selector.init(lb_time_stop[i], timestamp_stop[i]);      
    }
  }
  time_selector.onApply([](GWidget*widget)  { time_selector.hidden(true); });
  time_selector.onCancel([](GWidget*widget) { time_selector.hidden(true); });
}

// เมื่อ GLabel มีการเปลี่ยนแปลงค่า (หลังจากผู้ใช้เปลี่ยนค่าจาก GTimerSelect )
void lb_time_onValueChanged(GWidget*widget){
  for(int i=0; i< 3;i++){
    if(widget->sameWith(lb_time_start[i]) ){ 
      Serial.printf("[TimeSeting %d] Start : %s (timestamp %d)\n", i, lb_time_start[i].c_str(), timestamp_start[i]);
    }else
    if(widget->sameWith(lb_time_stop[i])){
      Serial.printf("[TimeSeting %d] Stop  : %s (timestamp %d)\n", i, lb_time_stop[i].c_str(), timestamp_stop[i]);      
    }
  }
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

    // จัดตำแหน่ง วิตเจ็ตข้อความ
  lb_start.c_align( ALIGN_TOP, - lb_start.width() - 5 , GScreen.height()/4);
  lb_stop .c_align( ALIGN_TOP,   lb_start.width() + 5 , GScreen.height()/4 );

  for(int i=0; i< 3;i++){
    lb_time_start[i] = TIMESTAMP2String(timestamp_start[i]);
    lb_time_stop[i]  = TIMESTAMP2String(timestamp_stop[i]);

    // จัดตำแหน่ง วิตเจ็ตข้อความ
    if(i==0){
      lb_time_start[0].align(lb_start,  ALIGN_BOTTOM);
      lb_time_stop[0] .align(lb_stop,   ALIGN_BOTTOM);
    }else{
      lb_time_start[i].align(lb_time_start[i-1],  ALIGN_BOTTOM);
      lb_time_stop[i] .align(lb_time_stop[i-1],   ALIGN_BOTTOM);
    }

    lb_time_start[i].clickable(true);                             // เปิดให้ GLabel รับสัญญาณ สัมผัส ต่างๆไดด้วย
    lb_time_stop[i] .clickable(true);
    lb_time_start[i].onClicked( lb_time_onClicked );              // เมื่อ GLabel ภูก clicked
    lb_time_stop[i] .onClicked( lb_time_onClicked );
    lb_time_start[i].onValueChanged( lb_time_onValueChanged );    // เมื่อ GLabel มีการเปลี่ยนแปลงค่า (หลังจากผู้ใช้เปลี่ยนค่าจาก GTimerSelect )
    lb_time_stop[i] .onValueChanged( lb_time_onValueChanged );
  }
}

void loop() {
  BlynkGO.update();
}

