#include <BeeNeXT.h>
#define LED_ON    true
#define LED_OFF   false

#define LED       13

SoftTimer timer_sensor;

void setup() {
  Serial.begin(9600); Serial.println();
  Serial.println("BeeNeXT : Arduino UNO");
  while(!Serial) delay(100);

//  BeeNeXT.begin(&Serial); // แบบใช้ Hardware Serial (RX=0, TX=1)
  BeeNeXT.begin(8,9);       // แบบใช้ Software Serial (RX=8, TX=9)
  delay(100);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LED_OFF);

  // ตั้งเวลา ส่งค่าไปยัง ฝั่งจอ BeeNeXT 
  // ส่งไป ด้วยคำสั่ง  BeeNeXT.send( key, value );
  timer_sensor.setInterval(2000,[](){
    float temp  = random(150,600)/10.0; // สุ่มเลขจาก 15.0 - 60.0
    float humid = random(600,950)/10.0; // สุ่มเลขจาก 60.0 - 95.0

    Serial.println(String("[Sensor] temp ")+ String(temp,2) + String(" ; humid ") + String(humid,2));

    if(!BeeNeXT.isConnected()) return;
    BeeNeXT.send("TEMP" , String(temp ,2) );   // ส่ง ทศนิยม 2 ตำแหน่งไปด้วย key "TEMP"
    BeeNeXT.send("HUMID", String(humid,2) );   // ส่ง ทศนิยม 2 ตำแหน่งไปด้วย key "HUMID"
  });
}

void loop() {
  BeeNeXT.update();
}
