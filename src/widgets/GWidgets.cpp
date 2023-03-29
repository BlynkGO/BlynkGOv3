/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GWidgets.cpp นี้เป็นไลบรารี่ลิขลิทธิ์ 
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

#include "GWidgets.h"

void widget_del(GWidget* widget){
  if(widget == NULL) return;
  if(!widget->isCreated()) return;

  String    widget_type = widget->type();
  uint16_t  child_num   = widget->child_num();

  if(widget_type == "GAqi") {
    ((GAqi*) widget)->del();
  }else
  if(widget_type == "GArc") {
    ((GArc*) widget)->del();
  }else
  if(widget_type == "GBar") {
    ((GBar*) widget)->del();
  }else
  if(widget_type == "GButton") {
    ((GButton*) widget)->del();
  }else
  if(widget_type == "GButtonMatrix") {
    ((GButtonMatrix*) widget)->del();
  }else
  // if(widget_type == "GCamera") {
  //   ((GCamera*) widget)->del();
  // }else
  if(widget_type == "GCanvas") {
    ((GCanvas*) widget)->del();
  }else
  if(widget_type == "GChart") {
    ((GChart*) widget)->del();
  }else
  if(widget_type == "GCheckBox") {
    ((GCheckBox*) widget)->del();
  }else
  if(widget_type == "GCircle") {
    ((GCircle*) widget)->del();
  }else
  if(widget_type == "GColorPicker") {
    ((GColorPicker*) widget)->del();
  }else
  if(widget_type == "GContainer") {
    ((GContainer*) widget)->del();
  }else
  if(widget_type == "GDropdownList") {
    ((GDropdownList*) widget)->del();
  }else
  if(widget_type == "GGauge") {
    ((GGauge*) widget)->del();
  }else
  if(widget_type == "GGaugeLine") {
    ((GGaugeLine*) widget)->del();
  }else
  if(widget_type == "GGaugeRainbow") {
    ((GGaugeRainbow*) widget)->del();
  }else
  if(widget_type == "GIcon") {
    ((GIcon*) widget)->del();
  }else
  if(widget_type == "GImage") {
    ((GImage*) widget)->del();
  }else
  if(widget_type == "GImageButton") {
    ((GImageButton*) widget)->del();
  }else
  if(widget_type == "GJoystick") {
    ((GJoystick*) widget)->del();
  }else
  if(widget_type == "GKeyboard") {
    ((GKeyboard*) widget)->del();
  }else
  if(widget_type == "GKeyUnlocker") {
    ((GKeyUnlocker*) widget)->del();
  }else
  if(widget_type == "GKnob") {
    ((GKnob*) widget)->del();
  }else
  if(widget_type == "GLabel") {
    ((GLabel*) widget)->del();
  }else
  if(widget_type == "GLed") {
    ((GLed*) widget)->del();
  }else
  if(widget_type == "GLevel") {
    ((GLevel*) widget)->del();
  }else
  if(widget_type == "GLine") {
    ((GLine*) widget)->del();
  }else
  if(widget_type == "GList") {
    ((GList*) widget)->del();
  }else
  if(widget_type == "GMask") {
    ((GMask*) widget)->del();
  }else
  if(widget_type == "GMultiScreen") {
    ((GMultiScreen*) widget)->del();
  }else
  if(widget_type == "GObject") {
    ((GObject*) widget)->del();
  }else
  if(widget_type == "GPage") {
    ((GPage*) widget)->del();
  }else
  if(widget_type == "GPreload") {
    ((GPreload*) widget)->del();
  }else
  if(widget_type == "GQRCode") {
    ((GQRCode*) widget)->del();
  }else
  if(widget_type == "GRoller") {
    ((GRoller*) widget)->del();
  }else
  if(widget_type == "GSlider") {
    ((GSlider*) widget)->del();
  }else
  if(widget_type == "GSliderH") {
    ((GSliderH*) widget)->del();
  }else
  if(widget_type == "GSliderV") {
    ((GSliderV*) widget)->del();
  }else
  if(widget_type == "GStep") {
    ((GStep*) widget)->del();
  }else
  if(widget_type == "GSwitch") {
    ((GSwitch*) widget)->del();
  }else
  if(widget_type == "GSwitch3D") {
    ((GSwitch3D*) widget)->del();
  }else
  if(widget_type == "GTable") {
    ((GTable*) widget)->del();
  }else
  if(widget_type == "GTabview") {
    ((GTabview*) widget)->del();
  }else
  if(widget_type == "GTextArea") {
    ((GTextArea*) widget)->del();
  } else
  if(widget_type == "GThermo") {
    ((GThermo*) widget)->del();
  }else
  if(widget_type == "GTileview") {
    ((GTileview*) widget)->del();
  }else
  if(widget_type == "GTimeSelector") {
    ((GTimeSelector*) widget)->del();
  }else
  if(widget_type == "GTriangle") {
    ((GTriangle*) widget)->del();
  }
#if BLYNKGO_USE_WIFI
  else
  if(widget_type == "GWiFiManager") {
    ((GWiFiManager*) widget)->del();
  }else
  if(widget_type == "GWiFiSetting") {
    ((GWiFiSetting*) widget)->del();
  }
#endif
}

// void child_del_recursive(GWidget* widget){
//   Serial.println(widget->child_num());
//   GWidget* child = widget->child(true);
//   while(child){
//     Serial.println(child->type());
//     String    widget_type = child->type();
//     uint16_t  child_num   = child->child_num();
//     if( ( widget_type=="GButton" && child_num > 1) || (widget_type != "GButton" && child_num >= 1 ) ) {
//       child_del_recursive(child); //recursive del
//     }else{
//       widget_del(child);
//       child = widget->child();
//     }
//   }
// }

void free_widget(GWidget*widget){
	if(widget == NULL) return;
  widget_del(widget);
  free(widget); widget=NULL;
}


/*********************************************
 * PSRAM Widget Alloc
 *********************************************/

GAqi* new_GAqi(GWidget& parent){
  GAqi* w = (GAqi*) esp32_malloc(sizeof(GAqi));
  new (w) GAqi(parent);
  return w;
}

GAqi* new_GAqi(GWidget* parent){
  GAqi* w = (GAqi*) esp32_malloc(sizeof(GAqi));
  new (w) GAqi(parent);
  return w;
}


GArc* new_GArc(GWidget& parent){
  GArc* w = (GArc*) esp32_malloc(sizeof(GArc));
  new (w) GArc(parent);
  return w;
}

GArc* new_GArc(GWidget* parent){
  GArc* w = (GArc*) esp32_malloc(sizeof(GArc));
  new (w) GArc(parent);
  return w;
}


GBar* new_GBar(GWidget& parent){
  GBar* w = (GBar*) esp32_malloc(sizeof(GBar));
  new (w) GBar(parent);
  return w;
}

GBar* new_GBar(GWidget *parent){
  GBar* w = (GBar*) esp32_malloc(sizeof(GBar));
  new (w) GBar(parent);
  return w;
}

GButton*  new_GButton(GWidget& parent){
  GButton* w = (GButton*) esp32_malloc(sizeof(GButton));
  new (w) GButton(parent);
  return w;
}

GButton* new_GButton(GWidget *parent){
  GButton* w = (GButton*) esp32_malloc(sizeof(GButton));
  new (w) GButton(parent);
  return w;
}

GButton* new_GButton(const char* label, GWidget& parent){
  GButton* w = (GButton*) esp32_malloc(sizeof(GButton));
  new (w) GButton(label, parent);
  return w;
}

GButton* new_GButton(const char* label, GWidget *parent){
  GButton* w = (GButton*) esp32_malloc(sizeof(GButton));
  new (w) GButton(label, parent);
  return w;
}

GButton* new_GButton(button_type_t type, const char* label, GWidget& parent){
  GButton* w = (GButton*) esp32_malloc(sizeof(GButton));
  new (w) GButton(type, label, parent);
  return w;
}

GButton* new_GButton(button_type_t type, const char* label, GWidget *parent){
  GButton* w = (GButton*) esp32_malloc(sizeof(GButton));
  new (w) GButton(type, label, parent);
  return w;
}

GButtonMatrix*  new_GButtonMatrix(GWidget& parent){
  GButtonMatrix* w = (GButtonMatrix*) esp32_malloc(sizeof(GButtonMatrix));
  new (w) GButtonMatrix(parent);
  return w;
}

GButtonMatrix* new_GButtonMatrix(GWidget *parent){
  GButtonMatrix* w = (GButtonMatrix*) esp32_malloc(sizeof(GButtonMatrix));
  new (w) GButtonMatrix(parent);
  return w;
}

// GCamera*  new_GCamera(GWidget& parent){
//   GCamera* w = (GCamera*) esp32_malloc(sizeof(GCamera));
//   new (w) GCamera(parent);
//   return w;
// }

// GCamera*  new_GCamera(GWidget *parent){
//   GCamera* w = (GCamera*) esp32_malloc(sizeof(GCamera));
//   new (w) GCamera(parent);
//   return w;
// }

GCanvas*  new_GCanvas(GWidget& parent){
  GCanvas* w = (GCanvas*) esp32_malloc(sizeof(GCanvas));
  new (w) GCanvas(parent);
  return w;
}

GCanvas*  new_GCanvas(GWidget *parent){
  GCanvas* w = (GCanvas*) esp32_malloc(sizeof(GCanvas));
  new (w) GCanvas(parent);
  return w;
}

GChart*  new_GChart(GWidget& parent){
  GChart* w = (GChart*) esp32_malloc(sizeof(GChart));
  new (w) GChart(parent);
  return w;
}

GChart*  new_GChart(GWidget *parent){
  GChart* w = (GChart*) esp32_malloc(sizeof(GChart));
  new (w) GChart(parent);
  return w;
}

GCheckBox*  new_GCheckBox(GWidget& parent){
  GCheckBox* w = (GCheckBox*) esp32_malloc(sizeof(GCheckBox));
  new (w) GCheckBox(parent);
  return w;
}

GCheckBox*  new_GCheckBox(GWidget *parent){
  GCheckBox* w = (GCheckBox*) esp32_malloc(sizeof(GCheckBox));
  new (w) GCheckBox(parent);
  return w;
}

GCircle*  new_GCircle(GWidget& parent){
  GCircle* w = (GCircle*) esp32_malloc(sizeof(GCircle));
  new (w) GCircle(parent);
  return w;
}

GCircle*  new_GCircle(GWidget *parent){
  GCircle* w = (GCircle*) esp32_malloc(sizeof(GCircle));
  new (w) GCircle(parent);
  return w;
}

GColorPicker*  new_GColorPicker(GWidget& parent){
  GColorPicker* w = (GColorPicker*) esp32_malloc(sizeof(GColorPicker));
  new (w) GColorPicker(parent);
  return w;
}

GColorPicker*  new_GColorPicker(GWidget *parent){
  GColorPicker* w = (GColorPicker*) esp32_malloc(sizeof(GColorPicker));
  new (w) GColorPicker(parent);
  return w;
}

GContainer* new_GContainer(GWidget& parent){
  GContainer* w = (GContainer*) esp32_malloc(sizeof(GContainer));
  new (w) GContainer(parent);
  return w;
}

GContainer* new_GContainer(GWidget *parent){
  GContainer* w = (GContainer*) esp32_malloc(sizeof(GContainer));
  new (w) GContainer(parent);
  return w;
}

GDropdownList* new_GDropdownList(GWidget& parent){
  GDropdownList* w = (GDropdownList*) esp32_malloc(sizeof(GDropdownList));
  new (w) GDropdownList(parent);
  return w;
}

GDropdownList* new_GDropdownList(GWidget *parent){
  GDropdownList* w = (GDropdownList*) esp32_malloc(sizeof(GDropdownList));
  new (w) GDropdownList(parent);
  return w;
}

GGauge* new_GGauge(GWidget& parent){
  GGauge* w = (GGauge*) esp32_malloc(sizeof(GGauge));
  new (w) GGauge(parent);
  return w;
}

GGauge* new_GGauge(GWidget *parent){
  GGauge* w = (GGauge*) esp32_malloc(sizeof(GGauge));
  new (w) GGauge(parent);
  return w;
}

GGaugeLine* new_GGaugeLine(GWidget& parent){
  GGaugeLine* w = (GGaugeLine*) esp32_malloc(sizeof(GGaugeLine));
  new (w) GGaugeLine(parent);
  return w;
}

GGaugeLine* new_GGaugeLine(GWidget *parent){
  GGaugeLine* w = (GGaugeLine*) esp32_malloc(sizeof(GGaugeLine));
  new (w) GGaugeLine(parent);
  return w;
}

GGaugeRainbow*  new_GGaugeRainbow(GWidget& parent){
  GGaugeRainbow* w = (GGaugeRainbow*) esp32_malloc(sizeof(GGaugeRainbow));
  new (w) GGaugeRainbow(parent);
  return w;
}

GGaugeRainbow*  new_GGaugeRainbow(GWidget *parent){
  GGaugeRainbow* w = (GGaugeRainbow*) esp32_malloc(sizeof(GGaugeRainbow));
  new (w) GGaugeRainbow(parent);
  return w;
}


GIcon* new_GIcon(GWidget& parent){
  GIcon* w = (GIcon*) esp32_malloc(sizeof(GIcon));
  new (w) GIcon(parent);
  return w;
}

GIcon* new_GIcon(GWidget *parent){
  GIcon* w = (GIcon*) esp32_malloc(sizeof(GIcon));
  new (w) GIcon(parent);
  return w;
}

GIcon* new_GIcon(const img_t & src_img, String name, GWidget& parent){
  GIcon* w = (GIcon*) esp32_malloc(sizeof(GIcon));
  new (w) GIcon(src_img, name, parent);
  return w;
}

GIcon* new_GIcon(const img_t & src_img, String name, GWidget *parent){
  GIcon* w = (GIcon*) esp32_malloc(sizeof(GIcon));
  new (w) GIcon(src_img, name, parent);
  return w;
}

GImage* new_GImage(GWidget& parent){
  GImage* w = (GImage*) esp32_malloc(sizeof(GImage));
  new (w) GImage(parent);
  return w;
}

GImage* new_GImage(GWidget *parent){
  GImage* w = (GImage*) esp32_malloc(sizeof(GImage));
  new (w) GImage(parent);
  return w;
}

GImage* new_GImage(const img_t & src_img, GWidget& parent){
  GImage* w = (GImage*) esp32_malloc(sizeof(GImage));
  new (w) GImage(src_img, parent);
  return w;
}

GImage* new_GImage(const img_t & src_img, GWidget *parent){
  GImage* w = (GImage*) esp32_malloc(sizeof(GImage));
  new (w) GImage(src_img, parent);
  return w;
}

GImage* new_GImage(String file_path, GWidget& parent){
  GImage* w = (GImage*) esp32_malloc(sizeof(GImage));
  new (w) GImage(file_path, parent);
  return w;
}

GImage* new_GImage(String file_path, GWidget *parent){
  GImage* w = (GImage*) esp32_malloc(sizeof(GImage));
  new (w) GImage(file_path, parent);
  return w;
}


GImageButton* new_GImageButton(GWidget& parent) {
  GImageButton* w = (GImageButton*) esp32_malloc(sizeof(GImageButton));
  new (w) GImageButton(parent);
  return w;
}

GImageButton* new_GImageButton(GWidget *parent){
  GImageButton* w = (GImageButton*) esp32_malloc(sizeof(GImageButton));
  new (w) GImageButton(parent);
  return w;
}

GJoystick* new_GJoystick(GWidget& parent){
  GJoystick* w = (GJoystick*) esp32_malloc(sizeof(GJoystick));
  new (w) GJoystick(parent);
  return w;
}

GJoystick* new_GJoystick(GWidget *parent){
  GJoystick* w = (GJoystick*) esp32_malloc(sizeof(GJoystick));
  new (w) GJoystick(parent);
  return w;
}

GKeyboard* new_GKeyboard(GWidget& parent){
  GKeyboard* w = (GKeyboard*) esp32_malloc(sizeof(GKeyboard));
  new (w) GKeyboard(parent);
  return w;
}

GKeyboard* new_GKeyboard(GWidget *parent){
  GKeyboard* w = (GKeyboard*) esp32_malloc(sizeof(GKeyboard));
  new (w) GKeyboard(parent);
  return w;
}

GKeyUnlocker* new_GKeyUnlocker(GWidget& parent){
  GKeyUnlocker* w = (GKeyUnlocker*) esp32_malloc(sizeof(GKeyUnlocker));
  new (w) GKeyUnlocker(parent);
  return w;
}

GKeyUnlocker* new_GKeyUnlocker(GWidget *parent){
  GKeyUnlocker* w = (GKeyUnlocker*) esp32_malloc(sizeof(GKeyUnlocker));
  new (w) GKeyUnlocker(parent);
  return w;
}

GKnob* new_GKnob(GWidget& parent){
  GKnob* w = (GKnob*) esp32_malloc(sizeof(GKnob));
  new (w) GKnob(parent);
  return w;
}

GKnob* new_GKnob(GWidget *parent){
  GKnob* w = (GKnob*) esp32_malloc(sizeof(GKnob));
  new (w) GKnob(parent);
  return w;
}


GLabel * new_GLabel(GWidget& parent){
  GLabel* w = (GLabel*) esp32_malloc(sizeof(GLabel));
  new (w) GLabel(parent);
  return w;
}

GLabel * new_GLabel(GWidget *parent){
  GLabel* w = (GLabel*) esp32_malloc(sizeof(GLabel));
  new (w) GLabel(parent);
  return w;
}

GLabel* new_GLabel(const char* text, GWidget& parent){
  GLabel* w = (GLabel*) esp32_malloc(sizeof(GLabel));
  new (w) GLabel(text, parent);
  return w;
}

GLabel* new_GLabel(const char* text, GWidget *parent){
  GLabel* w = (GLabel*) esp32_malloc(sizeof(GLabel));
  new (w) GLabel(text, parent);
  return w; 
}


GLabel* new_GLabel(const String & text, GWidget& parent){
  GLabel* w = (GLabel*) esp32_malloc(sizeof(GLabel));
  new (w) GLabel(text, parent);
  return w;
}

GLabel* new_GLabel(const String & text, GWidget *parent){
  GLabel* w = (GLabel*) esp32_malloc(sizeof(GLabel));
  new (w) GLabel(text, parent);
  return w; 
}

GLed*   new_GLed(GWidget& parent){
  GLed* w = (GLed*) esp32_malloc(sizeof(GLed));
  new (w) GLed(parent);
  return w;
}

GLed*   new_GLed(GWidget *parent){
  GLed* w = (GLed*) esp32_malloc(sizeof(GLed));
  new (w) GLed(parent);
  return w;
}

GLevel*   new_GLevel(GWidget& parent){
  GLevel* w = (GLevel*) esp32_malloc(sizeof(GLevel));
  new (w) GLevel(parent);
  return w;
}

GLevel*   new_GLevel(GWidget *parent){
  GLevel* w = (GLevel*) esp32_malloc(sizeof(GLevel));
  new (w) GLevel(parent);
  return w;
}

GLine*   new_GLine(GWidget& parent){
  GLine* w = (GLine*) esp32_malloc(sizeof(GLine));
  new (w) GLine(parent);
  return w;
}

GLine*   new_GLine(GWidget *parent){
  GLine* w = (GLine*) esp32_malloc(sizeof(GLine));
  new (w) GLine(parent);
  return w;
}


GList * new_GList(GWidget& parent){
  GList* w = (GList*) esp32_malloc(sizeof(GList));
  new (w) GList(parent);
  return w;
}

GList * new_GList(GWidget *parent){
  GList* w = (GList*) esp32_malloc(sizeof(GList));
  new (w) GList(parent);
  return w;
}

GMask* new_GMask(GWidget& parent){
  GMask* w = (GMask*) esp32_malloc(sizeof(GMask));
  new (w) GMask(parent);
  return w;
}

GMask* new_GMask(GWidget *parent){
  GMask* w = (GMask*) esp32_malloc(sizeof(GMask));
  new (w) GMask(parent);
  return w;
}

GMask* new_GMask(uint16_t w, uint16_t h, GWidget& parent) {
  GMask* mask = (GMask*) esp32_malloc(sizeof(GMask));
  new (mask) GMask(w , h, parent);
  return mask;
}

GMask* new_GMask(uint16_t w, uint16_t h, GWidget *parent){
  GMask* mask = (GMask*) esp32_malloc(sizeof(GMask));
  new (mask) GMask(w, h,  parent);
  return mask;
}

GMask* new_GMask(const img_t & src_img, GWidget& parent){
  GMask* w = (GMask*) esp32_malloc(sizeof(GMask));
  new (w) GMask(src_img, parent);
  return w;
}

GMask* new_GMask(const img_t & src_img, GWidget *parent){
  GMask* w = (GMask*) esp32_malloc(sizeof(GMask));
  new (w) GMask(src_img, parent);
  return w;
}

GMultiScreen* new_GMultiScreen(GWidget& parent){
  GMultiScreen* w = (GMultiScreen*) esp32_malloc(sizeof(GMultiScreen));
  new (w) GMultiScreen(parent);
  return w;
}

GMultiScreen* new_GMultiScreen(GWidget *parent){
  GMultiScreen* w = (GMultiScreen*) esp32_malloc(sizeof(GMultiScreen));
  new (w) GMultiScreen(parent);
  return w;
}

GObject* new_GObject(GWidget& parent){
  GObject* w = (GObject*) esp32_malloc(sizeof(GObject));
  new (w) GObject(parent);
  return w;
}

GObject* new_GObject(GWidget *parent){
  GObject* w = (GObject*) esp32_malloc(sizeof(GObject));
  new (w) GObject(parent);
  return w;
}

GPage*  new_GPage(GWidget& parent){
  GPage* w = (GPage*) esp32_malloc(sizeof(GPage));
  new (w) GPage(parent);
  return w;
}

GPage*  new_GPage(GWidget *parent){
  GPage* w = (GPage*) esp32_malloc(sizeof(GPage));
  new (w) GPage(parent);
  return w;
}

GPreload* new_GPreload(GWidget& parent){
  GPreload* w = (GPreload*) esp32_malloc(sizeof(GPreload));
  new (w) GPreload(parent);
  return w;
}

GPreload* new_GPreload(GWidget *parent){
  GPreload* w = (GPreload*) esp32_malloc(sizeof(GPreload));
  new (w) GPreload(parent);
  return w;
}


GQRCode*  new_GQRCode(GWidget& parent){
  GQRCode* w = (GQRCode*) esp32_malloc(sizeof(GQRCode));
  new (w) GQRCode(parent);
  return w;
}

GQRCode*  new_GQRCode(GWidget *parent){
  GQRCode* w = (GQRCode*) esp32_malloc(sizeof(GQRCode));
  new (w) GQRCode(parent);
  return w;
}



GRoller* new_GRoller(GWidget& parent){
  GRoller* w = (GRoller*) esp32_malloc(sizeof(GRoller));
  new (w) GRoller(parent);
  return w;
}

GRoller* new_GRoller(GWidget *parent){
  GRoller* w = (GRoller*) esp32_malloc(sizeof(GRoller));
  new (w) GRoller(parent);
  return w;
}

GSlider* new_GSlider(GWidget& parent){
  GSlider* w = (GSlider*) esp32_malloc(sizeof(GSlider));
  new (w) GSlider(parent);
  return w;
}

GSlider* new_GSlider(GWidget *parent){
  GSlider* w = (GSlider*) esp32_malloc(sizeof(GSlider));
  new (w) GSlider(parent);
  return w;
}

GSliderH* new_GSliderH(GWidget& parent){
  GSliderH* w = (GSliderH*) esp32_malloc(sizeof(GSliderH));
  new (w) GSliderH(parent);
  return w;
}

GSliderH* new_GSliderH(GWidget *parent){
  GSliderH* w = (GSliderH*) esp32_malloc(sizeof(GSliderH));
  new (w) GSliderH(parent);
  return w;
}


GSliderV* new_GSliderV(GWidget& parent){
  GSliderV* w = (GSliderV*) esp32_malloc(sizeof(GSliderV));
  new (w) GSliderV(parent);
  return w;
}

GSliderV* new_GSliderV(GWidget *parent){
  GSliderV* w = (GSliderV*) esp32_malloc(sizeof(GSliderV));
  new (w) GSliderV(parent);
  return w;
}

GStep* new_GStep(GWidget& parent){
  GStep* w = (GStep*) esp32_malloc(sizeof(GStep));
  new (w) GStep(parent);
  return w;
}

GStep* new_GStep(GWidget *parent){
  GStep* w = (GStep*) esp32_malloc(sizeof(GStep));
  new (w) GStep(parent);
  return w;
}

GSwitch*  new_GSwitch(GWidget& parent){
  GSwitch* w = (GSwitch*) esp32_malloc(sizeof(GSwitch));
  new (w) GSwitch(parent);
  return w;
}

GSwitch*  new_GSwitch(GWidget *parent){
  GSwitch* w = (GSwitch*) esp32_malloc(sizeof(GSwitch));
  new (w) GSwitch(parent);
  return w;
}

GSwitch3D*  new_GSwitch3D(GWidget& parent){
  GSwitch3D* w = (GSwitch3D*) esp32_malloc(sizeof(GSwitch3D));
  new (w) GSwitch3D(parent);
  return w;
}

GSwitch3D*  new_GSwitch3D(GWidget *parent){
  GSwitch3D* w = (GSwitch3D*) esp32_malloc(sizeof(GSwitch3D));
  new (w) GSwitch3D(parent);
  return w;
}

GTable* new_GTable(GWidget& parent){
  GTable* w = (GTable*) esp32_malloc(sizeof(GTable));
  new (w) GTable(parent);
  return w;
}

GTable* new_GTable(GWidget *parent){
  GTable* w = (GTable*) esp32_malloc(sizeof(GTable));
  new (w) GTable(parent);
  return w;
}

GTabview* new_GTabview(GWidget& parent){
  GTabview* w = (GTabview*) esp32_malloc(sizeof(GTabview));
  new (w) GTabview(parent);
  return w;
}

GTabview* new_GTabview(GWidget *parent){
  GTabview* w = (GTabview*) esp32_malloc(sizeof(GTabview));
  new (w) GTabview(parent);
  return w;
}

GTextArea* new_GTextArea(GWidget& parent){
  GTextArea* w = (GTextArea*) esp32_malloc(sizeof(GTextArea));
  new (w) GTextArea(parent);
  return w;
}

GTextArea* new_GTextArea(GWidget *parent){
  GTextArea* w = (GTextArea*) esp32_malloc(sizeof(GTextArea));
  new (w) GTextArea(parent);
  return w;
}


GThermo* new_GThermo(GWidget& parent){
  GThermo* w = (GThermo*) esp32_malloc(sizeof(GThermo));
  new (w) GThermo(parent);
  return w;
}

GThermo* new_GThermo(GWidget *parent){
  GThermo* w = (GThermo*) esp32_malloc(sizeof(GThermo));
  new (w) GThermo(parent);
  return w;
}

GTileview* new_GTileview(GWidget& parent){
  GTileview* w = (GTileview*) esp32_malloc(sizeof(GTileview));
  new (w) GTileview(parent);
  return w;
}

GTileview* new_GTileview(GWidget *parent){
  GTileview* w = (GTileview*) esp32_malloc(sizeof(GTileview));
  new (w) GTileview(parent);
  return w;
}


GTimeSelector*  new_GTimeSelector(GWidget& parent){
  GTimeSelector* w = (GTimeSelector*) esp32_malloc(sizeof(GTimeSelector));
  new (w) GTimeSelector(parent);
  return w;
}

GTimeSelector*  new_GTimeSelector(GWidget *parent){
  GTimeSelector* w = (GTimeSelector*) esp32_malloc(sizeof(GTimeSelector));
  new (w) GTimeSelector(parent);
  return w;
}


GTriangle* new_GTriangle(GWidget& parent){
  GTriangle* w = (GTriangle*) esp32_malloc(sizeof(GTriangle));
  new (w) GTriangle(parent);
  return w;
}

GTriangle* new_GTriangle(GWidget *parent){
  GTriangle* w = (GTriangle*) esp32_malloc(sizeof(GTriangle));
  new (w) GTriangle(parent);
  return w;
}



