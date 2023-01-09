/*****************************************************************
 * 
 * ตัวอย่างนี้สำหรับ T-Watch รุ่น 2020 v3
 * ซึ่งจะมี Microphone แบบ I2S มาให้ด้วย
 * ตัวอย่างนี้ จะเป็นการอ่านค่า sound ที่เข้ามาทาง Microphone พื้นฐาน
 * ให้เปิด Arduino Serial-Plotter (Ctrl+Shift+L) เพื่อดูกราฟ 
 * ของเสียงที่เข้ามาทาง Microphone ของ TWatch
 * 
 * หมายเหตุ
 *    ให้เปิดใน config/blynkgo_config.h 
 *    
 *      #define BLYNKGO_USE_MIC    1
 * 
 *****************************************************************/

#include <BlynkGOv3.h>

int16_t sound_data = 0;

void setup()
{
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  // เมื่อมีการสัมผัสหน้าจอ ในขณะที่หน้าจอดับอยู่ ให้สั่งเปิดหน้าจอ 
  TWatch.onTouch_Interrupt([](){      // เมื่อมีการสัมผัสหน้าจอ จาก interrupt เข้ามา
    if(TWatch.display_isOFF()){       // หากหน้าจอปิดอยู่
      TWatch.display_ON();
    }
  });

}


void loop()
{
  BlynkGO.update();
  
  int16_t* sound_buffer = NULL;
  size_t sound_buffer_len = TWatch.MIC.read_buffer( sound_buffer);
  
  if( sound_buffer_len == 0 ){
    Serial.println("[MIC] read_buffer error");
  }else {  
    for (int i = 0; i < sound_buffer_len ; i++) {
      sound_data = sound_buffer[i];
      Serial.println(sound_data);   // ให้เปิด Arduino Serial-Plotter (Ctrl+Shift+L) เพื่อดูกราฟ ของเสียงที่เข้ามาทาง Microphone ของ TWatch
    }
  }
}
