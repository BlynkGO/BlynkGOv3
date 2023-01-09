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
#include "Touch_XPT2046_neo.hpp"

#include "../../internal/algorithm.h"
#include "../platforms/common.hpp"

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  bool Touch_XPT2046_neo::init(void)
  {
    if(_inited) return true;

    SPI.begin(_cfg.pin_sclk, _cfg.pin_miso, _cfg.pin_mosi, _cfg.pin_cs);
    _inited = _xpt2046.begin(SPI, _cfg.pin_cs, _cfg.pin_int);
    return _inited;
  }



  uint_fast8_t Touch_XPT2046_neo::getTouchRaw(touch_point_t* __restrict tp, uint_fast8_t count)
  {
    if (!_inited || count == 0) return 0;

    if(_xpt2046.touched()) {
      TS_Point ts_point = _xpt2046.getPoint();    

      tp->x    = ts_point.x;
      tp->y    = ts_point.y;
      tp->size = 1;
      return 1;
    }else{
      tp->x = 0;
      tp->y = 0;
      tp->size = 0;
      return 0;
    }
  }

//----------------------------------------------------------------------------
 }
}
