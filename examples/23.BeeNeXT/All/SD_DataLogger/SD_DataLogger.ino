#include <BlynkGOv3.h>

namespace SD_DataLogger{
  File _file;
  bool _isOpened;

  void close(){
    if(_isOpened){
      if(!_file) _file.close();
      _isOpened= false;
    }
  }  

  bool open(String path){
    SD_DataLogger::close();
    _file = SD.open(path, FILE_APPEND);
    if(_file) _isOpened = true;
    return _isOpened;
  }

  void println(String datalog){
    if(!_isOpened || !_file) return;
    _file.println(datalog);
  }
}

void setup(){
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();
//  BeeNeXT.enable(true);  // เปิดรับส่งข้อมูล ระหว่าง จอ BeeNeXT กับ MCU หลัก

  // ตัวอย่างใช้การตั้งเวลาเป็นระยะๆ ในการบันทึก datalog ลง SD
  static GTimer timer_datalogger;
  timer_datalogger.setInterval(10000,[](){
    static int c;
    if(SD_DataLogger::open("/mydata.txt")){
      SD_DataLogger::println(String(c++));
      SD_DataLogger::close();
    }
  });
}

void loop(){
  BlynkGO.update();  
}

BEENEXT_DATA(){
  String key = BeeNeXT.key();

  // ตัวอย่าง MCU หลัก ส่ง ข้อมูลมาให้บันทึกผ่านทาง key "SD_DATA"
  if(key == "SD_DATA"){
    if(SD_DataLogger::open("/mydata.txt")){
      SD_DataLogger::println(String(c++));
      SD_DataLogger::close();
    }
  }
}


