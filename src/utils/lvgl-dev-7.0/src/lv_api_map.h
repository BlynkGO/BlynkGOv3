/**
 * @file lv_api_map.h
 *
 */

#ifndef LV_API_MAP_H
#define LV_API_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*---------------------
 * V6.0 COMPATIBILITY
 *--------------------*/


#if LV_USE_ARC


static inline void lv_arc_set_angles(lv_obj_t * arc, uint16_t start, uint16_t end)
{
    lv_arc_set_start_angle(arc, start);
    lv_arc_set_end_angle(arc, end);
}

#endif

#if LV_USE_CHART

#define lv_chart_get_point_cnt lv_chart_get_point_count

#endif


#if LV_USE_DDLIST

static inline void lv_ddlist_set_draw_arrow(lv_obj_t * ddlist, bool en)
{
    if(en) lv_ddlist_set_symbol(ddlist, LV_SYMBOL_DOWN);
    else lv_ddlist_set_symbol(ddlist, NULL);
}

static inline bool lv_ddlist_get_draw_arrow(lv_obj_t * ddlist)
{
    if(lv_ddlist_get_symbol(ddlist)) return true;
    else return false;
}

#endif

#if LV_USE_BAR

/**
 * Make the bar symmetric to zero. The indicator will grow from zero instead of the minimum
 * position.
 * @param bar pointer to a bar object
 * @param en true: enable disable symmetric behavior; false: disable
 * @deprecated As of v7.0, you should use `lv_bar_set_type` instead.
 */
static inline void lv_bar_set_sym(lv_obj_t * bar, bool en)
{
    if(en)
        lv_bar_set_type(bar, LV_BAR_TYPE_SYM);
    else
        lv_bar_set_type(bar, LV_BAR_TYPE_NORMAL);
}

/**
 * Get whether the bar is symmetric or not.
 * @param bar pointer to a bar object
 * @return true: symmetric is enabled; false: disable
 * @deprecated As of v7.0, you should use `lv_bar_get_type` instead.
 */
static inline bool lv_bar_get_sym(lv_obj_t * bar) {
    return lv_bar_get_type(bar) == LV_BAR_TYPE_SYM;
}

#endif

#if LV_USE_SLIDER

/**
 * Make the slider symmetric to zero. The indicator will grow from zero instead of the minimum
 * position.
 * @param slider pointer to a bar object
 * @param en true: enable disable symmetric behavior; false: disable
 * @deprecated As of v7.0, you should use `lv_slider_set_type` instead.
 */
static inline void lv_slider_set_sym(lv_obj_t * slider, bool en)
{
	lv_bar_set_sym(slider, en);
}

/**
 * Get whether the slider is symmetric or not.
 * @param slider pointer to a slider object
 * @return true: symmetric is enabled; false: disable
 * @deprecated As of v7.0, you should use `lv_slider_get_type` instead.
 */
static inline bool lv_slider_get_sym(lv_obj_t * slider) {
	return lv_bar_get_sym(slider);
}

#endif

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_API_MAP_H*/
