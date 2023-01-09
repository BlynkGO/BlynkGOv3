/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1.โคดไฟล์ GChart.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
 *   สร้างโดย TridentTD
 *   
 *   2.ไม่อนุญาต ให้แจกจ่าย สำเนา หรือแก้ไข โดยไม่ได้รับอนุญาต 
 *   
 *   3.สำหรับผู้ได้รับ ลิขสิทธิ์ สามารถนำไปใช้สร้าง firmware/rom 
 *   สำหรับ บอร์ด ESP32 ที่ระบุเท่านั้น เพื่อใช้ทางการค้าต่างๆได้
 *   โดยห้ามแจกจ่าย จำหน่าย ดัดแปลง แก้ไขใดๆ ในตัว source ไลบรารี่ออกไป
 *
 *   4.หากมีการนำไปใช้คอมไพล์ถือว่าได้ยอมรับ ตามเงื่อนไขลิขสิทธิ์ดังกล่าว
 *   เป็นที่เรียบร้อยแล้ว
 * 
 *********************************************************************
 */

 #ifndef __GCHART_H__
#define __GCHART_H__

#include "GObject.h"

class GChart : public GObject {
  public:
    GChart(GWidget& parent=GScreen);
    GChart(GWidget* parent);
    virtual ~GChart();

    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    serie_t createSerie(uint16_t color);
    void    clearSerie(serie_t serie);
    /* chart_type_t :
       CHART_TYPE_NONE, CHART_TYPE_LINE, CHART_TYPE_COLUMN, CHART_TYPE_POINT,
       CHART_TYPE_VERTICAL_LINE, CHART_TYPE_AREA, CHART_TYPE_AREA_FADED, 
       CHART_TYPE_SPLINE */
    void    type(chart_type_t type); 
    void    series_width(uint16_t width);
    uint16_t series_width();
    void    series_opa(uint8_t opa);
    uint8_t series_opa();
    void    series_darking(uint8_t dark_eff);
    uint8_t series_darking();

    int16_t serie_get_y_min(serie_t serie);     
    int16_t serie_get_y_max(serie_t serie);     
    
    void    addPoint(serie_t serie, uint16_t value);
    void    point_count(uint16_t num);
    uint16_t point_count();

    void    range(int16_t y_min, int16_t y_max);

    int16_t axis_y_min();               // ค่า แกน y น้อยสุด
    int16_t axis_y_max();               // ค่า แกน y มากสุด
    int16_t serie_y_min(serie_t serie); // ค่า y ที่น้อยสุดใน series ที่ต้องการ
    int16_t serie_y_max(serie_t serie); // ค่า y ทีมากสุดใน series ที่ต้องการ

    inline int16_t y_min()                    { return axis_y_min(); }
    inline int16_t y_max()                    { return axis_y_max(); }
    inline int16_t y_min(serie_t serie)       { return serie_y_min(serie); }
    inline int16_t y_max(serie_t serie)       { return serie_y_max(serie); }

    void    margin(uint16_t margin);
    uint16_t margin();
    void    x_tick_length(uint8_t major_tick_len, uint8_t minor_tick_len);
    void    y_tick_length(uint8_t major_tick_len, uint8_t minor_tick_len);
    void    x_tick_texts(const char * list_of_values, uint8_t num_tick_marks,
                               lv_chart_axis_options_t options);
    void    y_tick_texts(const char * list_of_values, uint8_t num_tick_marks,
                               lv_chart_axis_options_t options = LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER);
    void    secondary_y_tick_texts(const char * list_of_values, uint8_t num_tick_marks,
                               lv_chart_axis_options_t options = LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER);



    // inline void x_tick_texts(String list_of_values, uint8_t num_tick_marks, lv_chart_axis_options_t options)                          { x_tick_texts(list_of_values.c_str(), num_tick_marks, options); }
    // inline void y_tick_texts(String list_of_values, uint8_t num_tick_marks,
    //                            lv_chart_axis_options_t options = LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER)   { y_tick_texts(list_of_values.c_str(), num_tick_marks, options); }
    // inline void secondary_y_tick_texts(String list_of_values, uint8_t num_tick_marks,
    //                            lv_chart_axis_options_t options = LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER)   { secondary_y_tick_texts( list_of_values.c_str(), num_tick_marks, options); }

    void    x_tick_texts(String list_of_values, uint8_t num_tick_marks, lv_chart_axis_options_t options)                              { _x_ticks = list_of_values; x_tick_texts(_x_ticks.c_str(), num_tick_marks, options); }
    void    y_tick_texts(String list_of_values, uint8_t num_tick_marks,
                               lv_chart_axis_options_t options = LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER)   { _y_ticks = list_of_values; y_tick_texts(_y_ticks.c_str(), num_tick_marks, options); }
    void    secondary_y_tick_texts(String list_of_values, uint8_t num_tick_marks,
                               lv_chart_axis_options_t options = LV_CHART_AXIS_DRAW_LAST_TICK | LV_CHART_AXIS_INVERSE_LABELS_ORDER)   { _y_secondary_ticks = list_of_values; secondary_y_tick_texts( _y_secondary_ticks.c_str(), num_tick_marks, options); }


    void    div_line_count(uint8_t hdiv, uint8_t vdiv);
    void    update_mode(lv_chart_update_mode_t update_mode);
    void    refresh();
  private:
    String  _x_ticks;
    String  _y_ticks;
    String  _y_secondary_ticks;
};

#endif //__GCHART_H__
