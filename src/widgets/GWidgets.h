/*
 *********************************************************************
 * ลิขลิทธิ์ (Commercial License)
 * 
 *   1. โคดไฟล์ GWidgets.h นี้เป็นไลบรารี่ลิขลิทธิ์ 
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


#ifndef __GWIDGETS_H__
#define __GWIDGETS_H__




#include "../config/blynkgo_config.h"

#include "GAnimation.h"
#include "GApp32.h"
#include "GAqi.h"
#include "GArc.h"
#include "GBar.h"
#include "GButton.h"
#include "GButtonMatrix.h"
  // #include "GCalendar.h"
// #include "GCamera.h"  // ESP32core 2.0.3 ไม่พบ fd_forward.h ที่เป็นส่วน ESP32 Cam face detection
// #include "GCanvas.h"
#include "GChart.h"
#include "GCheckBox.h"
#include "GCircle.h"
#include "GColorPicker.h"
#include "GContainer.h"
#include "GDropdownList.h"
#include "GGauge.h"
#include "GGaugeLine.h"
#include "GGaugeRainbow.h"
#include "GIcon.h"
#include "GImage.h"
#include "GImageButton.h"
#include "GJoystick.h"
#include "GKeyboard.h"
#include "GKeyUnlocker.h"
#include "GKnob.h"
#include "GLabel.h"
#include "GLed.h"
#include "GLevel.h"
#include "GLine.h"
#include "GList.h"
#include "GMask.h"
#include "GMultiScreen.h"
#include "GObject.h"
#include "GPage.h"
#include "GPreload.h"
#include "GQRCode.h"
#include "GRoller.h"
#include "GSlider.h"
#include "GSliderH.h"
#include "GSliderV.h"
#include "GStep.h"
#include "GSwitch.h"
#include "GSwitch3D.h"
#include "GTable.h"
#include "GTabview.h"
#include "GTextArea.h"
#include "GThermo.h"
#include "GTileview.h"
#include "GTriangle.h"
#include "GTimer.h"
#include "GTimeSelector.h"
#include "GTask.h"

#if BLYNKGO_USE_WIFI
  #include "GWiFiManager.h"
#if BLYNKGO_USE_BLYNK
  #include "GBlynkManager.h"
#endif
#endif


// // // #include "GListWorkspace.h"
// // // #include "GContainerUI.h"
// // #include "GWiFiManager.h"
// // #include "GWinUI.h"
// // #include "GWorkspaceUI.h"
// // #include "GBaseUI.h"
// // #include "GMessageBox.h"
// // #include "GPreloader.h"
// // #include "GWindow.h"
// // #include "GLineAA.h"

#include "../objx/lv_objx_trident.h"

#if BLYNKGO_USE_KBD
extern "C" {
	#include "../kbd/lv_keyboard.h"
}
#endif //BLYNKGO_USE_KBD

#if BLYNKGO_USE_FS
#include "../fs/lv_fs_esp32.h"
#endif //BLYNKGO_USE_FS

extern GObject GScreen;

/**************************************
 * API สำหรับ จากคืนเม็มโมรี่ ที่จองบน PSRAM
 **************************************/
void free_widget(GWidget*widget);

/**************************************
 * API สำหรับ จากจอง (allocation) วิตเจ็ตบน PSRAM
 **************************************/
GApp32*         new_GApp32(GWidget& parent=GScreen);
GApp32*         new_GApp32(GWidget *parent);
GApp32*         new_GApp32(const img_t & src_img, String name, GWidget& parent=GScreen);
GApp32*         new_GApp32(const img_t & src_img, String name, GWidget *parent);

GAqi*           new_GAqi(GWidget& parent=GScreen);
GAqi*           new_GAqi(GWidget *parent);

GArc*           new_GArc(GWidget& parent=GScreen);
GArc*           new_GArc(GWidget *parent);

GBar*           new_GBar(GWidget& parent=GScreen);
GBar*           new_GBar(GWidget *parent);

GButton*        new_GButton(GWidget& parent=GScreen);
GButton*        new_GButton(GWidget *parent);
GButton*        new_GButton(const char* label, GWidget& parent=GScreen);
GButton*        new_GButton(const char* label, GWidget *parent);
GButton*        new_GButton(button_type_t type, const char* label, GWidget& parent=GScreen);
GButton*        new_GButton(button_type_t type, const char* label, GWidget *parent);

GButtonMatrix*  new_GButtonMatrix(GWidget& parent=GScreen);
GButtonMatrix*  new_GButtonMatrix(GWidget *parent);

// GCamera*        new_GCamera(GWidget& parent=GScreen);
// GCamera*        new_GCamera(GWidget *parent);

// GCanvas*        new_GCanvas(GWidget& parent=GScreen);
// GCanvas*        new_GCanvas(GWidget *parent);

GChart*         new_GChart(GWidget& parent=GScreen);
GChart*         new_GChart(GWidget *parent);

GCheckBox*      new_GCheckBox(GWidget& parent=GScreen);
GCheckBox*      new_GCheckBox(GWidget *parent);

GCircle*        new_GCircle(GWidget& parent=GScreen);
GCircle*        new_GCircle(GWidget *parent);

GColorPicker*   new_GColorPicker(GWidget& parent=GScreen);
GColorPicker*   new_GColorPicker(GWidget *parent);

GContainer*     new_GContainer(GWidget& parent=GScreen);
GContainer*     new_GContainer(GWidget *parent);

GDropdownList*  new_GDropdownList(GWidget& parent=GScreen);
GDropdownList*  new_GDropdownList(GWidget *parent);

GGauge*         new_GGauge(GWidget& parent=GScreen);
GGauge*         new_GGauge(GWidget *parent);

GGaugeLine*     new_GGaugeLine(GWidget& parent=GScreen);
GGaugeLine*     new_GGaugeLine(GWidget *parent);

GGaugeRainbow*  new_GGaugeRainbow(GWidget& parent=GScreen);
GGaugeRainbow*  new_GGaugeRainbow(GWidget *parent);

GIcon*          new_GIcon(GWidget& parent=GScreen);
GIcon*          new_GIcon(GWidget *parent);
GIcon*          new_GIcon(const img_t & src_img, String name, GWidget& parent=GScreen);
GIcon*          new_GIcon(const img_t & src_img, String name, GWidget *parent);

GImage*         new_GImage(GWidget& parent=GScreen);
GImage*         new_GImage(GWidget *parent);
GImage*         new_GImage(const img_t & src_img, GWidget& parent=GScreen);
GImage*         new_GImage(const img_t & src_img, GWidget *parent);
GImage*         new_GImage(String file_path, GWidget& parent=GScreen);
GImage*         new_GImage(String file_path, GWidget *parent);

GImageButton*   new_GImageButton(GWidget& parent=GScreen);
GImageButton*   new_GImageButton(GWidget *parent);

GJoystick*      new_GJoystick(GWidget& parent=GScreen);
GJoystick*      new_GJoystick(GWidget *parent);

GKeyboard*      new_GKeyboard(GWidget& parent=GScreen);
GKeyboard*      new_GKeyboard(GWidget *parent);

GKeyUnlocker*   new_GKeyUnlocker(GWidget& parent=GScreen);
GKeyUnlocker*   new_GKeyUnlocker(GWidget *parent);

GKnob*          new_GKnob(GWidget& parent=GScreen);
GKnob*          new_GKnob(GWidget *parent);

GLabel*         new_GLabel(GWidget& parent=GScreen);
GLabel*         new_GLabel(GWidget *parent);
GLabel*         new_GLabel(const char* text, GWidget& parent=GScreen);
GLabel*         new_GLabel(const char* text, GWidget *parent);
GLabel*         new_GLabel(const String & text, GWidget& parent=GScreen);
GLabel*         new_GLabel(const String & text, GWidget *parent);

GLed*           new_GLed(GWidget& parent=GScreen);
GLed*           new_GLed(GWidget *parent);

GLevel*         new_GLevel(GWidget& parent=GScreen);
GLevel*         new_GLevel(GWidget *parent);

GLine*          new_GLine(GWidget& parent=GScreen);
GLine*          new_GLine(GWidget *parent);

GList*          new_GList(GWidget& parent=GScreen);
GList*          new_GList(GWidget *parent);

GMask*          new_GMask(GWidget& parent=GScreen);
GMask*          new_GMask(GWidget *parent);
GMask*          new_GMask(uint16_t w, uint16_t h, GWidget& parent=GScreen);
GMask*          new_GMask(uint16_t w, uint16_t h, GWidget *parent);
GMask*          new_GMask(const img_t & src_img, GWidget& parent=GScreen);
GMask*          new_GMask(const img_t & src_img, GWidget *parent);

GMultiScreen*   new_GMultiScreen(GWidget& parent=GScreen);
GMultiScreen*   new_GMultiScreen(GWidget *parent);

GObject*        new_GObject(GWidget& parent=GScreen);
GObject*        new_GObject(GWidget *parent);

GPage*          new_GPage(GWidget& parent=GScreen);
GPage*          new_GPage(GWidget *parent);

GPreload*       new_GPreload(GWidget& parent=GScreen);
GPreload*       new_GPreload(GWidget *parent);

GQRCode*        new_GQRCode(GWidget& parent=GScreen);
GQRCode*        new_GQRCode(GWidget *parent);

GRoller*        new_GRoller(GWidget& parent=GScreen);
GRoller*        new_GRoller(GWidget *parent);

GSlider*        new_GSlider(GWidget& parent=GScreen);
GSlider*        new_GSlider(GWidget *parent);

GSliderH*       new_GSliderH(GWidget& parent=GScreen);
GSliderH*       new_GSliderH(GWidget *parent);

GSliderV*       new_GSliderV(GWidget& parent=GScreen);
GSliderV*       new_GSliderV(GWidget *parent);

GStep*          new_GStep(GWidget& parent=GScreen);
GStep*          new_GStep(GWidget *parent);

GSwitch*        new_GSwitch(GWidget& parent=GScreen);
GSwitch*        new_GSwitch(GWidget *parent);

GSwitch3D*      new_GSwitch3D(GWidget& parent=GScreen);
GSwitch3D*      new_GSwitch3D(GWidget *parent);

GTable*         new_GTable(GWidget& parent=GScreen);
GTable*         new_GTable(GWidget *parent);

GTabview*       new_GTabview(GWidget& parent=GScreen);
GTabview*       new_GTabview(GWidget *parent);

GTextArea*      new_GTextArea(GWidget& parent=GScreen);
GTextArea*      new_GTextArea(GWidget *parent);

GThermo*        new_GThermo(GWidget& parent=GScreen);
GThermo*        new_GThermo(GWidget *parent);

GTileview*      new_GTileview(GWidget& parent=GScreen);
GTileview*      new_GTileview(GWidget *parent);

GTimeSelector*  new_GTimeSelector(GWidget& parent=GScreen);
GTimeSelector*  new_GTimeSelector(GWidget *parent);

GTriangle*      new_GTriangle(GWidget& parent=GScreen);
GTriangle*      new_GTriangle(GWidget *parent);



#endif //__GWIDGETS_H__
