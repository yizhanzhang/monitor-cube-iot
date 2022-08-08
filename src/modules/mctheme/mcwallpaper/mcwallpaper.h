#ifndef MC_WALLPAPER_H
#define MC_WALLPAPER_H

#include <TJpg_Decoder.h>
#include "../base.h"
#include "../../../img/wallpaper/windows-xp-x64.h"

class McWallpaper: public McThemeBase {
  private:
    bool hasDraw;
  public:
    McWallpaper();
    void init();
    void update();
};

#endif