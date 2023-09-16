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

 *** เพิ่ม Touch_CST820.hpp ใน v1_init.hpp ด้วย ***
 by BlynkGO
/----------------------------------------------------------------------------*/
#pragma once

#include "../Touch.hpp"
#include "CST820/CST820.h"

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  struct Touch_CST820 : public ITouch
  {
    Touch_CST820(void)
    {
      _cfg.x_min = 0;
      _cfg.x_max = 320;
      _cfg.y_min = 0;
      _cfg.y_max = 240;
    }

    bool init(void) override;

    void wakeup(void) override;

    void sleep(void) override;

    uint_fast8_t getTouchRaw(touch_point_t *tp, uint_fast8_t count) override;

    // void setTouchNums(int_fast8_t nums);

  private:
    CST820 _cst820;
    // void _freshConfig(void);
  };

//----------------------------------------------------------------------------
 }
}
