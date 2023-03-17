SoftTimer timer_led_blinking;
bool last_led_state = LED_OFF;

// เมื่อ MCU และ จอ BeeNeXT มีการต่อสายถึงกัน 
// มีหรือไม่ก็ได้
BEENEXT_CONNECTED(){
  Serial.println("[BeeNeXT] connected");

  // หยุดไฟกระพริบ
  timer_led_blinking.del();

  // คืนสถานะ led สุดท้าย
  digitalWrite(LED, last_led_state);
}

// เมื่อ MCU และ จอ BeeNeXT หลุดการต่อสายถึงกัน 
// มีหรือไม่ก็ได้
BEENEXT_DISCONNECTED(){
  Serial.println("[BeeNeXT] disconnected");

  // จำสถานะ led สุดท้าย
  last_led_state = digitalRead(LED);
  
  // ทำไฟกระพริบ
  timer_led_blinking.setInterval(200,[](){
    digitalWrite(LED, !digitalRead(LED));
  });
}

// เมื่อฝั่ง MCU ส่งข้อมูลมาให้ จอ BeeNeXT ด้วย key & value
// ให้เช็คค่า key ที่ส่งเข้ามาด้วย BeeNeXT.key()
// ส่วนค่า value สามารถแสดงเป็น ประเภทข้อมูลต่างได้ด้วยคำสั่ง
// เช่น BeeNeXT.toString(), BeeNeXT.toInt(), BeeNeXT.toFloat(), BeeNeXT.toBool() ได้ตามต้องการ
BEENEXT_DATA(){
  if(BeeNeXT.key() == "LED"){
    digitalWrite(LED, BeeNeXT.toBool()? LED_ON: LED_OFF);
  }
}
