// เมื่อ MCU และ จอ BeeNeXT มีการต่อสายถึงกัน 
// มีหรือไม่ก็ได้
BEENEXT_CONNECTED(){
  Serial.println("[BeeNeXT] connected");
  GRAPHIC::BeeNeXT_Connected();
}

// เมื่อ MCU และ จอ BeeNeXT หลุดการต่อสายถึงกัน 
// มีหรือไม่ก็ได้
BEENEXT_DISCONNECTED(){
  Serial.println("[BeeNeXT] disconnected");
  GRAPHIC::BeeNeXT_Disconnected();
}

// เมื่อฝั่ง MCU ส่งข้อมูลมาให้ จอ BeeNeXT ด้วย key & value
// ให้เช็คค่า key ที่ส่งเข้ามาด้วย MCU.key()
// ส่วนค่า value สามารถแสดงเป็น ประเภทข้อมูลต่างได้ด้วยคำสั่ง
// เช่น MCU.toString(), MCU.toInt(), MCU.toFloat(), MCU.toBool() ได้ตามต้องการ
MCU_DATA(){
  if(BeeNeXT.key() == "TEMP"){            // เช็คฝั่ง MCU ส่งมาด้วย key "TEMP"
    GRAPHIC::setTemp( MCU.toFloat() );    // ค่า value จาก MCU ที่ส่งมา เปลี่ยนเป็น float แล้วใช้งานตามต้องการ
  }else
  if(BeeNeXT.key() == "HUMID"){           // เช็คฝั่ง MCU ส่งมาด้วย key "HUMID"
    GRAPHIC::setHumid( MCU.toFloat() );   // ค่า value จาก MCU ที่ส่งมา เปลี่ยนเป็น float แล้วใช้งานตามต้องการ
  }
}
