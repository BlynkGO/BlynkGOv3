/*****************************************************************
 * 
 * ตัวอย่างนี้สำหรับ T-Watch รุ่น 2020 v3
 * ซึ่งจะมี Microphone แบบ I2S มาให้ด้วย
 * 
 * หมายเหตุ
 *    ให้เปิดใน config/blynkgo_config.h 
 *    
 *      #define BLYNKGO_USE_MIC    1
 * 
 *****************************************************************/

#include <BlynkGOv3.h>

float               sound_data_avg = 0;
int16_t             sound_data = 0;
uint32_t            j = 0;

GChart chart;
serie_t serie1;

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
  chart.size(220,200);      // กำหนดขนาดของ chart แผนภูมิกราฟ
  chart.corner_radius(0);   // มุมรัศมีโค้งเป็น 0
  chart.color(TFT_COLOR_MIX(TFT_WHITE, TFT_BLACK, 50)); // กำหนดสีพื้นหลังของ chart ใช้การผสมสีขาว และ สีดำ ด้วยสัดส่วน 50:205 (บวกกัน=255)
  chart.opa(50);            // กำหนดค่าความโปร่งใสของสีพื้นหลัง chart
  chart.line_opa(128);      // กำหนดค่าความโปร่งใสของเส้นแกนกราฟ
  chart.line_color(TFT_GRAY);   // กำหนดสีของเส้นแกนกราฟ
  chart.line_width(1);          // กำหนดความหนาของเส้นแกนกราฟ

  chart.point_count(10);         // จำนวน จุดข้อมูลบนเส้นกราฟนึงๆ ที่จะปรากฏใน chart
  chart.series_width(2);        // ความหนาเส้นกราฟ
  chart.margin(0);             // กำหนดระยะ margin เผื่อพื้นที่ให้แสดงตัวเลขแกนกราฟ
  chart.div_line_count(3,5);    // แบ่งส่วนออกเป็นตารางภายใน chart ออกเป็นกี่ส่วน ค่าพารามิเตอร์ คือ ส่วนของแนวนอน และ แนวตัั้ง ตามลำดับ
  chart.range(0,800);
  
  chart.type(CHART_TYPE_SPLINE | CHART_TYPE_POINT );
  serie1 = chart.createSerie( TFT_RED);
  
}

void loop()
{
  BlynkGO.update();
  
  j = j + 1;

  int16_t* sound_buffer = NULL;
  size_t sound_buffer_len = TWatch.MIC.read_buffer( sound_buffer);
  
  if( sound_buffer_len == 0 ){
    Serial.println("[MIC] read_buffer error");
  }else {  
    for (int i = 0; i < sound_buffer_len ; i++) {
      sound_data = sound_buffer[i];
      if (sound_data > 0) {
        sound_data_avg = sound_data_avg + sound_data;
      }
    }

    if (j % 2 == 0 && j > 0) {
      sound_data_avg = sound_data_avg / (2*sound_buffer_len) ;    
      Serial.println(sound_data_avg);
      chart.addPoint(serie1, sound_data_avg);
      sound_data_avg = 0;
    }
  }
}

