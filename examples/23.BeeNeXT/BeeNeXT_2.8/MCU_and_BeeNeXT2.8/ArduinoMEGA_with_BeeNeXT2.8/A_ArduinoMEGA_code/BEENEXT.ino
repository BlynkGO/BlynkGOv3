SoftTimer timer_led_blinking;
bool last_led_state = LED_OFF;

// เมื่อ MCU และ จอ BeeNeXT มีการต่อสายถึงกัน สื่อสารถึงกันได้
// มีหรือไม่ก็ได้
BEENEXT_CONNECTED(){    // หากให้ตรงนี้ทำงาน ที่ฝั่ง จอ BeeNeXT เพิ่ม BeeNeXT.heartbeat(true); ด้วย
  Serial.println("[BeeNeXT] connected");

  // หยุดไฟกระพริบ
  timer_led_blinking.del();

  // คืนสถานะ led สุดท้าย
  digitalWrite(LED, last_led_state);
}

// เมื่อ MCU และ จอ BeeNeXT หลุดการต่อสายถึงกัน 
// มีหรือไม่ก็ได้
BEENEXT_DISCONNECTED(){ // หากให้ตรงนี้ทำงาน ที่ฝั่ง จอ BeeNeXT เพิ่ม BeeNeXT.heartbeat(true); ด้วย
  Serial.println("[BeeNeXT] disconnected");

  // จำสถานะ led สุดท้าย
  last_led_state = digitalRead(LED);
  
  // ทำไฟกระพริบ
  timer_led_blinking.setInterval(200,[](){
    digitalWrite(LED, !digitalRead(LED));
  });
}

// เมื่อ จอ BeeNeXT ส่งข้อมูลเข้ามา ให้เช็คค่า key ด้วย BeeNeXT.key()
// แล้วแปลง value ไปตามประเภทข้อมูลที่ต้องการ
// เช่น BeeNeXT.toString(), BeeNeXT.toInt(), BeeNeXT.toFloat(), BeeNeXT.toBool()
BEENEXT_DATA(){
  if(BeeNeXT.key() == "LED"){
    digitalWrite(LED, BeeNeXT.toBool()? LED_ON: LED_OFF);
  }
}

