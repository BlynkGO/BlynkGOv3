#ifndef __GAPP32_H__
#define __GAPP32_H__

#include "GObject.h"
#include "GIcon.h"
#include "GLabel.h"
#include "GImage.h"
#include "GContainer.h"

typedef struct {
  gicon_ext_t icon;
  GObject *app_main;
  bool alloc;
  // GWidget *pointer; here
} GApp32_ext_t;

class GApp32 : public GIcon {
  public:
    GApp32(GWidget& parent=GScreen);
    GApp32(GWidget* parent);
    GApp32(const img_t & src_img, String name, GWidget& parent=GScreen);
    GApp32(const img_t & src_img, String name, GWidget* parent);
    GApp32(String file_path, String name, GWidget& parent=GScreen);
    GApp32(String file_path, String name, GWidget* parent);
    GApp32(GWidget& app_link, const img_t & src_img, String name, GWidget& parent=GScreen);
    GApp32(GWidget& app_link, const img_t & src_img, String name, GWidget* parent);
    virtual ~GApp32();
    virtual void create();
            void create(GWidget& parent);
            void create(GWidget* parent);

    GRect&  app();   
  private:
    bool  _alloc=false;
    GObject *_app_link = NULL;
    String _name;
};



#endif //__GAPP32_H__
