#include <BlynkGOv3.h>

GLabel label;
GChart chart;
serie_t serie1;
serie_t serie2;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  label = "กราฟเซ็นเซอร์";
  label.align(ALIGN_IN_TOP_MID, 0,20);
  
  chart.size(150,120);      // กำหนดขนาดของ chart แผนภูมิกราฟ
  chart.corner_radius(0);   // มุมรัศมีโค้งเป็น 0
  chart.color(TFT_COLOR_MIX(TFT_WHITE, TFT_BLACK, 50)); // กำหนดสีพื้นหลังของ chart ใช้การผสมสีขาว และ สีดำ ด้วยสัดส่วน 50:205 (บวกกัน=255)
  chart.opa(50);            // กำหนดค่าความโปร่งใสของสีพื้นหลัง chart
  chart.line_opa(128);      // กำหนดค่าความโปร่งใสของเส้นแกนกราฟ
  chart.line_color(TFT_GRAY);   // กำหนดสีของเส้นแกนกราฟ
  chart.line_width(1);          // กำหนดความหนาของเส้นแกนกราฟ
  chart.font(prasanmit_20, TFT_WHITE);  // กำหนด font และสี ที่จะใช้บน chart
  chart.align(ALIGN_CENTER, 0,10);      // วางไว้กลางจอ โดยเลื่อนลง 10pixel

  chart.point_count(7);         // จำนวน จุดข้อมูลบนเส้นกราฟนึงๆ ที่จะปรากฏใน chart
  chart.series_width(2);        // ความหนาเส้นกราฟ
  chart.margin(40);             // กำหนดระยะ margin เผื่อพื้นที่ให้แสดงตัวเลขแกนกราฟ
  chart.div_line_count(3,0);    // แบ่งส่วนออกเป็นตารางภายใน chart ออกเป็นกี่ส่วน ค่าพารามิเตอร์ คือ ส่วนของแนวนอน และ แนวตัั้ง ตามลำดับ

//  chart.type(CHART_TYPE_LINE | CHART_TYPE_POINT );  // ให้แสดงกราฟ แบบเส้นตรง และแสดงจุดด้วย
  chart.type(CHART_TYPE_SPLINE | CHART_TYPE_POINT );  // ให้แสดงกราฟ แบบเส้นโค้ง และแสดงจุดด้วย
//  chart.type(CHART_TYPE_AREA );                     // ให้แสดงกราฟแบบพื้นที่
  
  /*Add two data series*/
  serie1 = chart.createSerie( TFT_BLUE);    // เพิ่ม ชุดเส้นกราฟเข้าให้แก่ chart โดยเป็นเส้นสีน้ำเงิน
  serie2 = chart.createSerie( TFT_RED);     // เพิ่ม ชุดเส้นกราฟเข้าให้แก่ chart โดยเป็นเส้นสีแดง

  chart.x_tick_length(5,8);   // กำหนดระยะความยาวของเส้นขีดบนแกน x ; ค่าคือ คือ ระยะหลัก ค่าพารามิเตอร์ คือ ระยะความยาวขีดหลัก และรอง
  chart.y_tick_length(5,8);   // กำหนดระยะความยาวของเส้นขีดบนแกน y ; 

//  chart.x_tick_texts("1\n2\n3\n4\n5\n6\n7", 2, LV_CHART_AXIS_DRAW_LAST_TICK);
  chart.y_tick_texts("0\n25\n50\n75\n100",1);                 // กำหนดข้อความบนแกน y หลัก
  chart.secondary_y_tick_texts("20°\n35°\n50°\n65°\n80°", 3); // กำหนดข้อความบนแกน y รอง  

}

uint32_t timer;
void loop() {
  BlynkGO.update();
  if(millis() >= timer ) {
    timer = millis() + 100;
    chart.addPoint(serie1, random(100));    // เพิ่มข้อมูลเข้าไปใน กราฟแรก
    chart.addPoint(serie2, random(100));    // เพิ่มข้อมูลเข้าไปใน กราฟสอง
  }
}

