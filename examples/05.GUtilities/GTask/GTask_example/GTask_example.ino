#include <BlynkGOv3.h>

GTask task;

int i=0;
void my_task(void*){
  Serial.println(i++);
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  task.setInterval(1000L, my_task);
  // task.setInterval(1000L, my_task, &i);  // แบบส่ง param ด้วย
  // task.setOnce(1000L, my_task);          // แบบให้เรียกทำงานครั้งเดียว
  // task.setOnce(1000L, my_task);          // แบบให้เรียกทำงานครั้งเดียว
  // task.priority(5);                      // กำหนด priority 0-5
  // task.del();                            // สำหรับลบ task ออก
}

void loop() {
  BlynkGO.update();
}
