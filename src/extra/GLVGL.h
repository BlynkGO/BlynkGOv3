/********************************************
 *
 * include พื้นฐานให้ใช้กับ .c และ .cpp ได้ได้วย
 *
 ********************************************/

#ifndef _GVLGL_H__
#define _GVLGL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../config/blynkgo_config.h"
#include "../utils/esp32_utils.h"
#include "../utils/esp32_memory.h"
#include "../utils/BlynkGO_Type.h"

#include "../utils/lv_conf.h"
#include "../utils/lvgl-dev-7.0/lvgl.h"


#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_txt.h"
#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_circ.h"
#include "../utils/lvgl-dev-7.0/src/lv_draw/lv_draw.h"
#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_ll.h"
#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_gc.h"
#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_area.h"  // point_t;
#include "../utils/lvgl-dev-7.0/src/lv_font/lv_symbol_def.h"
// #include "../lvgl-dev-7.0/src/lv_draw/lv_draw_mask.h"
#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_task.h"
#include "../utils/lvgl-dev-7.0/src/lv_misc/lv_color.h"

#include "../utils/lvgl-dev-7.0/src/lv_core/lv_obj.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_cont.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_btnm.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_kb.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_ta.h"
#include "../utils/lvgl-dev-7.0/src/lv_objx/lv_label.h"


#include "../objx/lv_objx_trident.h"

#if BLYNKGO_USE_KBD
#include "../kbd/lv_keyboard.h"
#endif //BLYNKGO_USE_KBD

#if BLYNKGO_USE_FS
#include "../fs/lv_fs_esp32.h"
#endif //BLYNKGO_USE_FS

// #include "qrcode/qrcodegen.h"

#include <pgmspace.h>

// #define LV_ATTRIBUTE_MEM_ALIGN PROGMEM

#ifdef __cplusplus
}
#endif

#endif //_GVLGL_H__
