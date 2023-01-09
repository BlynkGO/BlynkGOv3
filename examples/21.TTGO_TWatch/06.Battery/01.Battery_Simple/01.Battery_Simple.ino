/****************************************************************
 * ตัวอย่าง สำหรับเกี่ยวกับ Batt Li-ON ของ T-Watch
 * 
 ****************************************************************/

#include <BlynkGOv3.h>

GLabel lb_battery;
GLabel lb_charging(SYMBOL_CHARGE);
GTimer batt_timer;

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  TWatch.BATT.enableChargeing(true);
  
  lb_charging.align(ALIGN_TOP_LEFT,5);
  lb_battery.align(ALIGN_TOP_RIGHT, -5);

  // ตั้งเวลาอ่านค่า battery ของ T-Watch ทุกๆ 5 วินาที
  batt_timer.setInterval(5000,[](){
    lb_charging.hidden( TWatch.BATT.isChargeing() == false ); // หากมีการชาร์จไฟอยู่ ให้แสดงสัญลักษณ์ชาร์จไฟด้วย
   
    int batt_percentage = TWatch.BATT.getBattPercentage();    // ดึงค่า battery ระดับ percentageว

    if( batt_percentage >=95){
       lb_battery = String(batt_percentage)+"% " + SYMBOL_BATTERY_FULL; 
       lb_battery.color(TFT_WHITE);
    }else
    if( batt_percentage >=75){
      lb_battery = String(batt_percentage)+"% " + SYMBOL_BATTERY_3; 
      lb_battery.color(TFT_WHITE);
    }else
    if( batt_percentage >=50){
      lb_battery = String(batt_percentage)+"% " + SYMBOL_BATTERY_2; 
      lb_battery.color(TFT_WHITE);
    }else
    if( batt_percentage >=25){
      lb_battery = String(batt_percentage)+"% " + SYMBOL_BATTERY_1; 
      lb_battery.color(TFT_WHITE);
    }else
    if( batt_percentage > 5){
      lb_battery = String(batt_percentage)+"% " + SYMBOL_BATTERY_1; 
      lb_battery.color(TFT_RED);
    }else{
      lb_battery = String(batt_percentage)+"% " + SYMBOL_BATTERY_EMPTY;
      lb_battery.color(TFT_RED);
    }

      
    Serial.print("[Batt] isChargeing ? : ");
    Serial.print(TWatch.BATT.isChargeing());
    if(TWatch.BATT.isChargeing()) {
      Serial.print(" getBattChargeCurrent : ");
      Serial.print( TWatch.BATT.getBattChargeCurrent());
    } else {
      Serial.print(" getBattDischargeCurrent : ");
      Serial.print( TWatch.BATT.getBattDischargeCurrent());
    }
   
    Serial.print(" getVbusVoltage : ");
    Serial.print( TWatch.BATT.getVbusVoltage());
    Serial.print(" getVbusCurrent : ");
    Serial.print( TWatch.BATT.getVbusCurrent());
    Serial.print(" getBattInpower : ");
    Serial.print( TWatch.BATT.getBattInpower());
    Serial.print(" getTemp : ");
    Serial.print( TWatch.BATT.getTemp());
    Serial.print("\n");
  });

}

void loop() {
  BlynkGO.update();
}


