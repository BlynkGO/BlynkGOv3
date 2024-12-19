/* เพิ่มการ rotation ได้ */

#ifndef _CST820_H
#define _CST820_H

#include <Wire.h>

#define I2C_ADDR_CST820 0x15

//手势
enum GESTURE
{
  None = 0x00,       //无手势
  SlideDown = 0x01,  //向下滑动
  SlideUp = 0x02,    //向上滑动
  SlideLeft = 0x03,  //向左滑动
  SlideRight = 0x04, //向右滑动
  SingleTap = 0x05,  //单击
  DoubleTap = 0x0B,  //双击
  LongPress = 0x0C   //长按
};

/**************************************************************************/
/*!
    @brief  CST820 I2C CTP controller driver
*/
/**************************************************************************/
class CST820
{
  public:
    CST820(int8_t sda_pin = -1, int8_t scl_pin = -1, int8_t rst_pin = -1, int8_t int_pin = -1);

    void begin(void);

    // BlynkGO
    inline void begin(int8_t sda_pin, int8_t scl_pin, int8_t rst_pin, int8_t int_pin) {
      _sda = sda_pin; _scl = scl_pin; _rst = rst_pin; _int = int_pin;
      this->begin();
    }

    // BlynkGO
    inline bool getTouch(uint16_t *x, uint16_t *y, uint8_t *gesture){
      bool ret = _getTouch(x,y,gesture);
      uint16_t buf;
      if(ret){
        switch(_rotation){
          case 0:
            break;
          case 1:
            buf = *x;
            *x = *y;
            *y = 240- buf;
            break;
          case 2:
            buf = *x;
            *x = 240-*x;
            *y = 320-*y;
            break;
          case 3:
            buf = *x;
            *x = 320-*y;
            *y = buf;
            break;
        }
      }
      return ret;
    }
    // BlynkGO
    inline void rotation(uint8_t r) { _rotation = r%4;  }
    inline uint8_t rotation()       { return _rotation; } 

  private:
    bool _getTouch(uint16_t *x, uint16_t *y, uint8_t *gesture);

    int8_t _sda, _scl, _rst, _int;
    uint8_t _rotation = 3;

    uint8_t i2c_read(uint8_t addr);
    uint8_t i2c_read_continuous(uint8_t addr, uint8_t *data, uint32_t length);
    void i2c_write(uint8_t addr, uint8_t data);
    uint8_t i2c_write_continuous(uint8_t addr, const uint8_t *data, uint32_t length);
};
#endif
