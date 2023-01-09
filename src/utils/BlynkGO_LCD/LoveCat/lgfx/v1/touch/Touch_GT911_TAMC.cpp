/*----------------------------------------------------------------------------/
  Lovyan GFX - Graphics library for embedded devices.

Original Source:
 https://github.com/lovyan03/LovyanGFX/

Licence:
 [FreeBSD](https://github.com/lovyan03/LovyanGFX/blob/master/license.txt)

Author:
 [lovyan03](https://twitter.com/lovyan03)

Contributors:
 [ciniml](https://github.com/ciniml)
 [mongonta0716](https://github.com/mongonta0716)
 [tobozo](https://github.com/tobozo)
/----------------------------------------------------------------------------*/
#include "Touch_GT911_TAMC.hpp"

#include "../../internal/algorithm.h"
#include "../platforms/common.hpp"

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  bool Touch_GT911_TAMC::init(void)
  {
    if (_inited) return true;
    _tamc_gt911.begin(_cfg.pin_sda, _cfg.pin_scl, _cfg.pin_int, _cfg.pin_rst, _cfg.x_max-_cfg.x_min+1, _cfg.y_max-_cfg.y_min+1,  _cfg.i2c_addr );
    _tamc_gt911.setRotation(ROTATION_INVERTED);
    _inited = true;
    return _inited;
  }

  void Touch_GT911_TAMC::wakeup(void)
  {
    if (!_inited) return;
    if (_cfg.pin_int < 0) return;
    lgfx::gpio_hi(_cfg.pin_int);
    lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::output);
    delay(5);
    lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::input);
  }

  void Touch_GT911_TAMC::sleep(void)
  {
    // if (!_inited) return;
    // static constexpr uint8_t writedata[] = { 0x80, 0x40, 0x05 };
    // _writeBytes(writedata, 3);
  }

  uint_fast8_t Touch_GT911_TAMC::getTouchRaw(touch_point_t* __restrict tp, uint_fast8_t count)
  {
    if (!_inited || count == 0) return 0;
    if (count > 5) { count = 5; }

    if( tp == nullptr) return 0;

    _tamc_gt911.read();
    if (!_tamc_gt911.isTouched) { return 0; }

    if( count >=1 && count <= _tamc_gt911.touches)
    tp->x = _tamc_gt911.points[count-1].x;
    tp->y = _tamc_gt911.points[count-1].y;

    return _tamc_gt911.touches;

    // for (int i=0; i<_tamc_gt911.touches; i++){
    //   if( count == i+1 ) {
    //     Serial.print("Touch ");Serial.print(i+1);Serial.print(": ");;
    //     Serial.print("  x: ");Serial.print(_tamc_gt911.points[i].x);
    //     Serial.print("  y: ");Serial.print(_tamc_gt911.points[i].y);
    //     Serial.print("  size: ");Serial.println(tp.points[i].size);
    //     Serial.println(' ');
    //     return 0;
    //   }
    // }
    // return points;
  }

  // void Touch_GT911_TAMC::setTouchNums(int_fast8_t nums)
  // {
  //   nums = std::max<int_fast8_t>(1, std::min<int_fast8_t>(5, nums));

  //   uint8_t buf[] = { 0x80, 0x4c, 0x00 };
  //   _writeReadBytes(buf, 2, &buf[2], 1);
  //   if (buf[2] != nums)
  //   {
  //     buf[2] = nums;
  //     _writeBytes(buf, 3);

  //     _freshConfig();
  //   }
  // }

  // void Touch_GT911_TAMC::_freshConfig(void)
  // {
  //   // 設定レジスタ全体を読取り;
  //   uint8_t writedata[188] = { 0x80, 0x47 };
  //   if (_writeReadBytes(writedata, 2, &writedata[2], 184))
  //   {
  //     // チェックサムを計算し、設定値の更新指示を行う;
  //     writedata[0xBA] = calcChecksum(&writedata[2], 184); // 0x80FF checksum
  //     writedata[0xBB] = 0x01;                             // 0x8100 config fresh
  //     _writeBytes(writedata, 188);
  //   }
  // }

//----------------------------------------------------------------------------
 }
}
