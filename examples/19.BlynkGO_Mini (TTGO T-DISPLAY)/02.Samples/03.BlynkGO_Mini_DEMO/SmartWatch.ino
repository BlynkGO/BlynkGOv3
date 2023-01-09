GCircle cir_smartwatch;
GLabel  lb_num[12];

GLine    pin_hour(cir_smartwatch);
GLine    pin_minute(cir_smartwatch);
GLine    pin_second(cir_smartwatch);
GCircle  pin_dot(cir_smartwatch);

void SmartWatch_create(GWidget& parent){
  parent.color(TFT_BLUE, TFT_CYAN);
  cir_smartwatch.parent(parent);
  cir_smartwatch.radius(62);
  cir_smartwatch.border(5, TFT_BLACK);
  cir_smartwatch.color(TFT_WHITE);
  cir_smartwatch.font(prasanmit_25, TFT_BLACK);

  Point c = cir_smartwatch.centerpoint();   // จุดศูนย์กลางจอ TFT
  for(int i=0; i < 12; i++){
    //จุด p สร้างจาก แขน 90pixels หมุนไปมุม i*30 โดยเริ่มต้นที่มุม 270 รอบจุด c นับมุมแบบตามเข็ม
    Point p = Point(45, i*30, 270, c, CLOCKWISE);  
    lb_num[i].parent(cir_smartwatch);
    lb_num[i].center(p);               // กำหนดตำแหน่ง
    lb_num[i] = String((i>0)? i : 12); // ใส่ข้อความให้ วิตเจ็ตข้อความ
  }
  
  pin_hour.color(TFT_BLACK);
  pin_hour.thickness(8);
  pin_hour.rounded(false);
  pin_minute.color(TFT_BLACK);
  pin_minute.thickness(6);
  pin_minute.rounded(false);
  pin_second.color(TFT_RED);
  pin_second.thickness(2);
  pin_second.rounded(false);
  pin_dot.size(15,15);
  pin_dot.color(TFT_BLACK);
  pin_dot.clickable(false);

  SmartWatch_setTime(0,0,0);
}

void SmartWatch_setTime(uint8_t h, uint8_t m, uint8_t s) {
  static Point c = cir_smartwatch.centerpoint();
  
  float angle_h = ((h * 1.0 + m / 60.0 + s / 3600.0) * 5 * 6 );
//  Point p_h1 = Point(12, angle_h ,270,c, CLOCKWISE);
  Point p_h2 = Point(30, angle_h ,270,c, CLOCKWISE);
  pin_hour.setPoints(c, p_h2);
  
  float angle_m = (m * 1.0 + s / 60.0) * 6;
//    Point p_m1 = Point(0, angle_m ,270,c, CLOCKWISE);
  Point p_m2 = Point(50, angle_m ,270,c, CLOCKWISE);
  pin_minute.setPoints(c, p_m2);

  float angle_s = (s) * 6;
//  Point p_s1 = Point(88, angle_s ,270,c, CLOCKWISE);
  Point p_s2 = Point( 55, angle_s ,270,c, CLOCKWISE);
  pin_second.setPoints(c, p_s2);
}
