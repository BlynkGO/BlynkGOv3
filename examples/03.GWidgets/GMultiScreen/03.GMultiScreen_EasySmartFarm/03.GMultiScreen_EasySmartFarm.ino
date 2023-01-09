/*********************************************************
 *
 * ตัวอย่าง Easy SmartFarm โดยใช้วิตเจ็ต GMultiScreen ในการเพิ่มจำนวนหน้า
 * แต่ละหน้าให้แสดงค่า DHT 5 ชุด ด้วยวิตเจ็ต GGauge
 * และ สามารถ เปิด ปิดสวิตซ์ ด้วย GSwitch 
 * โดยให้มีการจดจำสถานะของสวิตว์ลงบน flashMem ด้วย
 *
 * --------------------------------------------------
 * Project Size : 1.44MB
 * 
 *********************************************************/

#include <BlynkGOv3.h>
#include <DHTesp.h>

#include <BlynkGOv3.h>

DHTesp  dht[5];                                   // สำหรับอ่านค่า sensor DHT11 5 ชุด
uint8_t dhtPin[5] = {21, 22, 25, 5, 15 };         // ต่อ sensor DHT11 ที่ขาต่างๆ ตาม array นี้

GMultiScreen screens;
GRect  rect;
GLabel lb_room[5] = {"กรีนโอ๊ค", "เรดโอ๊ค", "กรีนคอส", "บัตเตอร์เฮด", "เรดคอรัล" };
GSwitch sw[5];
String SW_FlashMem_KEY[5] = {"GREEN_OAK", "RED_OAK", "GREEN_COS", "BUTTERHEAD", "RED_CORAL"}; // ชื่อ Key สำหรับเก็บสถานะของ sw บน flashMem
GGauge gauge_temp[5];
GGauge gauge_humid[5];
GTimer dht_timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();
  BlynkGO.fillScreen(TFT_WHITE);
  
  rect.size( BlynkGO.width(), 70);
  rect.color(TFT_BLUE);
  rect.align(ALIGN_TOP);
  
  screens.addScreen(5);                               // เพิ่ม 5 screen
  screens.indic_color(TFT_BLUE);                      // กำหนดสีของ indic

  for(int i=0; i < 5; i++){
    dht[i].setup(dhtPin[i], DHTesp::DHT22);           // เริ่มต้น dht[i] แต่ละตัว ด้วย pin ที่กำหนด

    lb_room[i]    .parent(screens[i]);
    sw[i]         .parent(screens[i]);
    gauge_temp[i] .parent(screens[i]);                // ให้วิตเจ็ตต่างๆ ไปอยู่ screens[i] 
    gauge_humid[i].parent(screens[i]);

    lb_room[i]    .align(ALIGN_TOP, 0, 10);
    lb_room[i]    .font(prasanmit_40);

    sw[i].align(ALIGN_TOP_RIGHT, -10, 15);
    if(BlynkGO.flashMem_exists(SW_FlashMem_KEY[i])){
      Serial.println("found flashMem " + SW_FlashMem_KEY[i]);
      BlynkGO.flashMem_Int(SW_FlashMem_KEY[i]) ? sw[i].ON() : sw[i].OFF();  // เรียกสถานะจาก flashMem มาสั่งเปิดหรือปิด  sw[i]
    }
    sw[i].onValueChanged([](GWidget*widget){
      for(int i =0; i < 5; i++){
        if( sw[i] == widget ) {  // ใช้เทียบเช็คว่า วิตเจ็ตไหน ตรงกับ widget ที่มีการเปลี่ยนแปลงเข้ามา
          // code ให้ควบคุมอะไร สำหรับ sw[i] ใส่ตรงนี้ ...
          Serial.printf("สวิตซ์ %s : %s\n", lb_room[i].c_str(), sw[i].isON()? "ON" : "OFF");
          BlynkGO.flashMem(SW_FlashMem_KEY[i], sw[i].isON());              // บันทึกสถานะ sw[i] ที่เปลี่ยนแปลงลง flashMem
          return;
        }
      }
    });

    gauge_temp[i].decimal(1);                         // ให้แสดงทศนิยม 1 ตำแหน่ง
    gauge_temp[i] = random(200,250)/10.0;             // สุ่มตัวอย่างใส่ค่าให้เ gauge
    gauge_temp[i] .title("อุณหภูมิ");
    gauge_temp[i] .title_color(TFT_BLACK);
    gauge_temp[i] .color(TFT_RED);                    // กำหนดสีแดง สำหรับ gauge อุณภูมิ
    gauge_temp[i] .font(prasanmit_40, TFT_BLACK);     // กำหนดฟอนต์แสดงเลขตรงกลาง gauge
    gauge_temp[i] .align(ALIGN_CENTER,-gauge_temp[i].width()/2, 35);   // จัดตำแหน่ง
    gauge_temp[i] .clickable(false);                  // ไม่ให้รับสัญญาณการกดใดๆ

    gauge_humid[i].decimal(1);                        // ให้แสดงทศนิยม 1 ตำแหน่ง
    gauge_humid[i] = random(400,800)/10.0;            // สุ่มตัวอย่างใส่ค่าให้เ gauge
    gauge_humid[i].title("ความชื้น");
    gauge_humid[i].title_color(TFT_BLACK);
    gauge_humid[i].color(TFT_BLUE);                   // กำหนดสีฟ้า สำหรับ gauge อุณภูมิ
    gauge_humid[i].font(prasanmit_40, TFT_BLACK);     // กำหนดฟอนต์แสดงเลขตรงกลาง gauge
    gauge_humid[i].align(gauge_temp[i],ALIGN_RIGHT);  // จัดตำแหน่งไว้ ด้านขวาของ gauge_temp[i]
    gauge_humid[i].clickable(false);                  // ไม่ให้รับสัญญาณการกดใดๆ
  }

  // ให้วนให้ timer ทำงานอ่านค่าจาก sensor DHT ทุกๆตัว ในทุกๆ 2 วินาที
  // พร้อมกำหนดค่าให้ gauge
  dht_timer.setInterval(2000L, [](){
    for(int i=0 ; i < 5; i++){
      TempAndHumidity dht_value = dht[i].getTempAndHumidity();
      if( dht[i].getStatus() != 0) continue;          // แสดงว่าอ่านไม่ได้ ให้ข้ามไปอ่าน sensor dht ตัวถัดไป

      gauge_temp[i]  = dht_value.temperature;         // กำหนดค่าให้ วิตเจ็ต gauge_temp   จากที่ sensor dht[i] อ่านออกมาได้
      gauge_humid[i] = dht_value.humidity;            // กำหนดค่าให้ วิตเจ็ต gauge_humid  จากที่ sensor dht[i] อ่านออกมาได้
    }
  });
}

void loop() {
  BlynkGO.update();
}

