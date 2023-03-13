/********************************
 * ตัวอย่างจับภาพหน้าจอ ลง SD, SPIFFS
 ********************************/

#include <BlynkGOv3.h>

GGaugeRainbow gauge;

GRect rect_capture;  // สร้าง layer สำหรับจับภาพหน้าจอเมื่อมีการกด clicked

void setup(){
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  gauge.color(TFT_BLUE, TFT_RED);
  gauge.decimal(2);
  gauge = 79.34;
  
  rect_capture.opa(0);
  rect_capture.onClicked([](GWidget*widget){
//    BlynkGO.capture("SD://my_capture.png");     // สำหรับ รุ่นที่มี PSRAM สามารถบันทึกเป็น png ได้
//    BlynkGO.capture("SPIFFS://my_capture.bmp")  // สำหรับ จับภาพ ลง SPIFFS
    if(BlynkGO.capture("SD://my_capture.bmp")){
      Serial.println("[Capture] Done");
    }else{
      Serial.println("[Capture] Failed");
    }
  });

}

void loop(){
  BlynkGO.update();
}
