#include "BlynkGO_BMA423.h"

static TwoWire *pWire = NULL;
BlynkGO_BMA423 BMA423;

static float lsb_to_ms2(int16_t val, float g_range, uint8_t bit_width);

bool BlynkGO_BMA423::init(TwoWire &wire, uint8_t bma423_interrupt_pin){
  pWire = &wire;
  this->bma423_int_pin = bma423_int_pin;

  bma423_device.dev_addr        = BMA4_I2C_ADDR_SECONDARY;  // = adr 0x19
  bma423_device.interface       = BMA4_I2C_INTERFACE;
  bma423_device.bus_read        = bma423_read;
  bma423_device.bus_write       = bma423_write;
  bma423_device.delay           = delay;
  bma423_device.read_write_len  = 8;
  bma423_device.resolution      = 12;
  bma423_device.feature_len     = BMA423_FEATURE_SIZE;

  if (bma423_init(&bma423_device) == BMA4_OK) {
//    Serial.println("[BMA423] init ok");
  }else{
    Serial.println("[BMA423] init fail");
    return false;
  }

  if( bma423_write_config_file(&bma423_device) == BMA4_OK ){
//    Serial.println("[BMA423] write_config_file ok");
  }else{
    Serial.println("[BMA423] write_config_file fail");
    return false;
  }

  
  accel_conf.odr = BMA4_OUTPUT_DATA_RATE_100HZ;

  /* Gravity range of the sensor (+/- 2G, 4G, 8G, 16G) */
  accel_conf.range = BMA4_ACCEL_RANGE_2G;

  /* Bandwidth configure number of sensor samples required to average
   * if value = 2, then 4 samples are averaged
   * averaged samples = 2^(val(accel bandwidth))
   * Note1 : More info refer datasheets
   * Note2 : A higher number of averaged samples will result in a lower noise level of the signal, but
   * since the performance power mode phase is increased, the power consumption will also rise.
   */
  accel_conf.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

  /* Enable the filter performance mode where averaging of samples
   * will be done based on above set bandwidth and ODR.
   * There are two modes
   *  0 -> Averaging samples (Default)
   *  1 -> No averaging
   * For more info on No Averaging mode refer datasheets.
   */
  accel_conf.perf_mode = BMA4_CIC_AVG_MODE;

  /* Set the accel configurations */
  if( bma4_set_accel_config(&accel_conf, &bma423_device) == BMA4_OK ){
//    Serial.println("[BMA423] accel_config ok");
  }else{
    Serial.println("[BMA423] accel_config fail");
    return false;
  }
  
//   if( this->enableAcceleration(true) ){
// //    Serial.println("[bma423] accel enable ok");
//   }else{
//     Serial.println("[BMA423] accel enable fail");
//     return false;
//   }

  this->resetStepCounter();

  Serial.println("[BMA423] init done");
  return true;
}

bool  BlynkGO_BMA423::enableStepCounter(bool enable){

}

bool  BlynkGO_BMA423::enableDirection(bool enable){

}

bool  BlynkGO_BMA423::enableAcceleration(bool enable){
  return (bma4_set_accel_enable(enable, &bma423_device)==BMA4_OK);
}

acceleration_t BlynkGO_BMA423::getAcceleration(){
  acceleration_t accel;
  struct bma4_accel _accel;

  bma4_read_accel_xyz( &_accel, &bma423_device);

  //low pass filter;
  static float fx=0;
  static float fy=0;
  static float fz=0;
  static float alpha = 0.5;
    
  fx = _accel.x * alpha + (fx*(1.0-alpha));
  fy = _accel.y * alpha + (fy*(1.0-alpha));
  fz = _accel.z * alpha + (fz*(1.0-alpha));

  accel.x = _accel.x;
  accel.y = _accel.y;
  accel.z = _accel.z;

  /* Converting lsb to meters per seconds square for 12 bit accelerometer at 2G range */
  accel.x_ms2 = lsb_to_ms2(accel.x, 2, bma423_device.resolution);
  accel.y_ms2 = lsb_to_ms2(accel.y, 2, bma423_device.resolution);
  accel.z_ms2 = lsb_to_ms2(accel.z, 2, bma423_device.resolution);

  // accel.roll = (atan2(-fy,fz)*180.0)/PI;
  // accel.pitch = (atan2(fx,sqrt(fy*fy+fz*fz))*180.0)/PI;
  accel.pitch_z = (atan2(-fy,fz)*180.0)/PI;
  accel.roll_z  = (atan2(fx,sqrt(fy*fy+fz*fz))*180.0)/PI;

  accel.pitch_y = (atan2(-fz,-fy)*180.0)/PI;
  accel.roll_y  = (atan2(fx,sqrt(fy*fy+fz*fz))*180.0)/PI;

  accel.pitch_x = -(atan2(fz,-fx)*180.0)/PI;
  accel.roll_x  = (atan2(-fy,sqrt(fx*fx+fz*fz))*180.0)/PI;

  return accel;
}

void   BlynkGO_BMA423::resetStepCounter(){
  bma423_reset_step_counter(&bma423_device);
}



void BlynkGO_BMA423::onStepCounter(BMA423_StepCounterCB_t fn_step_cb){
  this->fn_step_cb = fn_step_cb;
}

void BlynkGO_BMA423::onDirection(BMA423_DirectionCB_t fn_direction_cb){
  this->fn_direction_cb = fn_direction_cb;
}

void BlynkGO_BMA423::onAcceleration(BMA423_AccelerationCB_t fn_accel_cb){
  this->fn_accel_cb = fn_accel_cb;
}

void  BlynkGO_BMA423::onDoubleClick(BMA423_DoubleClickCB_t fn_doubleclick_cb){
  this->fn_doubleclick_cb = fn_doubleclick_cb;
}

void  BlynkGO_BMA423::onTilt(BMA423_TiltCB_t fn_tilt_cb){
  this->fn_tilt_cb = fn_tilt_cb; 
}

void  BlynkGO_BMA423::onActivity(BMA423_Activity_t fn_activity_cb){
  this->fn_activity_cb = fn_activity_cb;
}

void  BlynkGO_BMA423::onAnyNoMotion(BMA423_AnyNoMotionCB_t  fn_anynomotion_cb) {
  this->fn_anynomotion_cb = fn_anynomotion_cb;
}


uint16_t BlynkGO_BMA423::bma423_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *write_data, uint16_t len)
{
  uint16_t ret = 0;
  char *err = NULL;
  pWire->beginTransmission(dev_addr);
  pWire->write(reg_addr);
  pWire->write(write_data, len );
  ret =  pWire->endTransmission();
  return ret ? 1 << 12 : ret;
}

uint16_t BlynkGO_BMA423::bma423_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *read_data, uint16_t len)
{
  uint16_t ret = 0;
  pWire->beginTransmission(dev_addr);
  pWire->write(reg_addr);
  pWire->endTransmission(false);
  uint8_t cnt = pWire->requestFrom(dev_addr, (uint8_t)len, (uint8_t)1);
  if (!cnt) { ret =  1 << 13; }
  while (pWire->available()) {
    pWire->readBytes(read_data, len);
  }
  return ret;
}

/* Earth's gravity in m/s^2 */
#define GRAVITY_EARTH       (9.80665f)
float lsb_to_ms2(int16_t val, float g_range, uint8_t bit_width)
{
 float half_scale = (float)(1 << bit_width) / 2.0f;
 return GRAVITY_EARTH * val * g_range / half_scale;
}
