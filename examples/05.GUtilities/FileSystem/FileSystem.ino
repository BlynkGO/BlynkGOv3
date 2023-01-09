/***************************************************
 * BlynkGO File System
 * 
 * ตัวอย่างพิมพ์คำสั่ง จาก Serial Monitor
 * เพื่อ ดูข้อมูล file บน SD หรือ SPIFFS ด้วย BlynkGO File System
 *  โดย drive SD ใช้ S:/path/filename
 *  และ drive SPFFS ใช้ F:/filename     ( SPIFFS ไม่สามารถมี folder ได้ )
 * 
 * เช่นที่ Serial Monitor ใส่คำสั่ง 
 *   S:       ---> สำหรับเปลี่ยน Drive เป็น SD
 *   F:       ---> สำหรับเปลีย่น Drive เป็น SPIFFS
 *   dir S:/     ---> สำหรับดูไฟล์ใน SD
 *   copy S:/filename F:/filename   --> copy file จาก SD ไปยัง SPIFFS
 *   move  S:/filename F:/filename   --> move file จาก SD ไปยัง SPIFFS
 *   ren  S:/filename1 S:/filename2  --> rename file
 *   cd  S:/BlynkGO    --> change directory
 *   del S:/filename    --> delete file
 *
 ***************************************************/

#include <BlynkGOv3.h>

String cur_drive="F:";
String cur_path="/";

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
}

void loop() {
  BlynkGO.update();
  if(Serial.available()){
    String command = Serial.readStringUntil('\n'); command.trim();
    if(command.startsWith("dir")){
      String path = command.substring(4); path.trim();
      listDir(path.c_str());
    }else
    if(command.startsWith("ren ")){
      String src, dsc;
      command_parse_to_src_dsc(command, src, dsc);       
      renameFile(src.c_str(),dsc.c_str());
    }else
    if(command.startsWith("copy ")){
      String src, dsc;
      command_parse_to_src_dsc(command, src, dsc);       
      copyFile(src.c_str(),dsc.c_str());
    }else
    if(command.startsWith("move ")){
      String src, dsc;
      command_parse_to_src_dsc(command, src, dsc);       
      moveFile(src.c_str(),dsc.c_str());
    }else
    if(command.startsWith("del ")){
      String path = command.substring(4); path.trim();  
      deleteFile(path.c_str());
    }else
    if(command.startsWith("cd ")){
      String path = command.substring(3); path.trim();
      currentDir();
      changeDir(path.c_str());
    }else
    if(command.startsWith("exist ")){
      String path = command.substring(6); path.trim();
      Serial.println(existPath(path.c_str()));
    }else
    if(command.startsWith("S:") || command.startsWith("s:") ||
       command.startsWith("F:") || command.startsWith("f:") ){
      command.toUpperCase();
      cur_drive = command;
      changeDrive(command.c_str());
      cur_path = "/";
    }
  }
}

void command_parse_to_src_dsc(String& command, String& src, String & dsc){
  uint8_t start = command.indexOf(" ")+1;
  uint8_t end   = command.indexOf(" ", start);
  src    = command.substring(start, end);
  dsc    = command.substring(end+1);

  if( !src.startsWith("S:") && !src.startsWith("s:") &&
      !src.startsWith("F:") && !src.startsWith("f:") ) {
        src = cur_drive + "/" + src;
        src.replace("//", "/");
      }

  if( !dsc.startsWith("S:") && !dsc.startsWith("s:") &&
      !dsc.startsWith("F:") && !dsc.startsWith("f:") ) {
        dsc = cur_drive + "/" + dsc;
        dsc.replace("//", "/");
      }
}

